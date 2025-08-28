#include <stdlib.h>
#include <string.h>
#include <SDL3/SDL.h>

#include "gui_manager.h"
#include "gui_window.h"
#include "gui_style.h"

// ---- internal ---------------------------------------------------------------
struct GuiManager {
    SDL_Renderer* renderer;

    // Defaults
    struct {
        GuiRect rect;              // {0,0,0,0} => auto
        GuiWindowFlags flags;      // defaults applied on creation
        GuiStyle style;            // copied; windows point to this
    } d;

    struct {
        bool        active;
        GuiWindow*  target;         // dragged window (the real one)
        GuiWindow*  origin_parent;  // original parent (NULL if top-level)
        GuiWindow*  hover_parent;   // current candidate parent under mouse (may be NULL for root)
        float       dx, dy;         // mouse offset inside window at grab time
        GuiRect     proxy;          // floating preview rect following the cursor
    } drag;

    // Soft config
    uint32_t max_window_depth;
    uint32_t max_objects;
    GuiTilingMode tiling;

    // basic lists
    GuiWindow* root_head;         // z-order: head = back, tail = front
    GuiWindow* root_tail;
    GuiWindow* focused;

    uint32_t window_count;
};

// tiny helpers
static GuiWindowFlags _default_window_flags(void) {
    return (GuiWindowFlags)(GUI_WINDOW_VISIBLE | GUI_WINDOW_INPUT |
                            GUI_WINDOW_AUTO_POS | GUI_WINDOW_AUTO_SIZE |
                            GUI_WINDOW_MOVABLE);
}
static GuiRect _rect_zero(void){ GuiRect r = {0,0,0,0}; return r; }
static void _link_front(struct GuiManager* gm, GuiWindow* w);
static void _unlink(struct GuiManager* gm, GuiWindow* w);
static int  _event_tree(GuiWindow* w, const SDL_Event* ev); // returns 1 if handled
static void _get_view_size(SDL_Renderer* r, float* out_w, float* out_h);
static void _render_subtree(GuiManager* gm, const GuiObject* o, uint32_t depth);

// ── helpers (put near top of file) ───────────────────────────────────────────
static inline float _clampf(float v, float a, float b){ return v < a ? a : (v > b ? b : v); }

// Treat windows with AUTO_POS or AUTO_SIZE as "managed" by the tiler.
// Are we auto-managed by tiler?
static inline bool _is_managed(const GuiWindow* w) {
    return (w->flags & (GUI_WINDOW_AUTO_POS | GUI_WINDOW_AUTO_SIZE)) != 0;
}

static inline void _gui_manager_viewport_resized(GuiManager* gm) {
    gui_manager_invalidate_layout(gm, NULL);
}

// Parent client rect (inside border + padding; below title if present)
static GuiRect _parent_client_rect(const GuiManager* gm, const GuiWindow* parent)
{
    const GuiStyle* s = &gm->d.style;
    const float dp = s->dp ? s->dp : 1.0f;
    const float pad = s->padding * dp;
    const float bw  = s->border_width;
    const float th  = parent->title ? (24.0f * dp) : 0.0f; // keep in sync with title bar height

    const GuiRect pr = parent->base.rect;
    GuiRect cr;
    cr.x = pr.x + bw + pad;
    cr.y = pr.y + bw + pad + th;
    cr.w = pr.w - 2.0f*(bw + pad);
    cr.h = pr.h - (th + 2.0f*(bw + pad));
    if (cr.w < 0) cr.w = 0;
    if (cr.h < 0) cr.h = 0;
    return cr;
}

// Grid relayout for ALL immediate children of `parent` (fills parent client area).
static void _relayout_grid_children(GuiManager* gm, GuiWindow* parent)
{
    if (!gm || !parent) return;

    // count immediate children
    uint32_t n = 0;
    for (GuiObject* c = parent->base.first_child; c; c = c->next_sibling) ++n;
    if (n == 0) return;

    const GuiRect cr = _parent_client_rect(gm, parent);
    if (cr.w <= 0.0f || cr.h <= 0.0f) return;

    const float dp  = gm->d.style.dp ? gm->d.style.dp : 1.0f;
    const float gut = gm->d.style.margin * dp;   // optional gutter

    const uint32_t cols = (uint32_t)ceilf(sqrtf((float)n));
    const uint32_t rows = (uint32_t)ceilf((float)n / (float)cols);

    const float cell_w = (cols ? cr.w / (float)cols : cr.w);
    const float cell_h = (rows ? cr.h / (float)rows : cr.h);

    uint32_t idx = 0;
    for (GuiObject* c = parent->base.first_child; c; c = c->next_sibling) {
        GuiWindow* w = (GuiWindow*)c;

        const uint32_t cx = idx % cols;
        const uint32_t cy = idx / cols;

        GuiRect r = {
            .x = cr.x + cx * cell_w + gut,
            .y = cr.y + cy * cell_h + gut,
            .w = fmaxf(1.0f, cell_w - 2.0f*gut),
            .h = fmaxf(1.0f, cell_h - 2.0f*gut),
        };

        if (w->base.vtbl && w->base.vtbl->arrange) w->base.vtbl->arrange(&w->base, r);
        else w->base.rect = r;

        if (++idx >= n) break;
    }
}

// Re-grid this parent's immediate children, then recurse into each child.
static void _relayout_grid_subtree(GuiManager* gm, GuiWindow* parent)
{
    if (!gm || !parent) return;
    _relayout_grid_children(gm, parent);  // uses parent's current rect

    for (GuiObject* c = parent->base.first_child; c; c = c->next_sibling) {
        _relayout_grid_subtree(gm, (GuiWindow*)c);
    }
}

// Recompute a full-viewport grid for all top-level managed windows.
static void _relayout_grid(GuiManager* gm)
{
    if (!gm || !gm->renderer) return;

    // collect managed top-level windows in z-order
    uint32_t n = 0;
    for (GuiWindow* w = gm->root_head; w; w = GUI_CAST(GuiWindow, w->base.next_sibling))
        if (!w->base.parent && _is_managed(w)) ++n;
    if (n == 0) return;

    float vw, vh; _get_view_size(gm->renderer, &vw, &vh);

    // decide cols/rows to roughly square the grid; we fill row-major (horizontal first)
    const uint32_t cols = (uint32_t)ceilf(sqrtf((float)n));
    const uint32_t rows = (uint32_t)ceilf((float)n / (float)cols);

    // full-bleed cells; use margin as an optional gutter (still uses all space visually)
    const float dp   = gm->d.style.dp ? gm->d.style.dp : 1.0f;
    const float gut  = gm->d.style.margin * dp;             // gutter on each side
    const float cell_w = vw / (float)cols;
    const float cell_h = vh / (float)rows;

    uint32_t idx = 0;
    for (GuiWindow* w = gm->root_head; w; w = GUI_CAST(GuiWindow, w->base.next_sibling)) {
        if (w->base.parent || !_is_managed(w)) continue;

        const uint32_t cx = idx % cols;
        const uint32_t cy = idx / cols;

        GuiRect r = {
            .x = cx * cell_w + gut,
            .y = cy * cell_h + gut,
            .w = fmaxf(1.0f, cell_w - 2*gut),
            .h = fmaxf(1.0f, cell_h - 2*gut),
        };

        if (w->base.vtbl && w->base.vtbl->arrange) w->base.vtbl->arrange(&w->base, r);
        else w->base.rect = r;

        ++idx;
        if (idx >= n) break;
    }
    for (GuiWindow* w = gm->root_head; w; w = GUI_CAST(GuiWindow, w->base.next_sibling)) {
        _relayout_grid_subtree(gm, w);
    }
}

// renderer viewport size (fallback if API not available)
static void _get_view_size(SDL_Renderer* r, float* out_w, float* out_h) {
    int w=800, h=600;
    if (r) SDL_GetRenderOutputSize(r, &w, &h);
    *out_w = (float)w; *out_h = (float)h;
}

static GuiRect _default_content_rect(const GuiStyle* s, float vw, float vh) {
    const float dp = s ? s->dp : 1.0f;
    GuiRect r = { 64*dp, 64*dp, 360*dp, 240*dp };     // reasonable default size
    // keep inside viewport margins
    r.w = _clampf(r.w, 120*dp, vw - 2*64*dp);
    r.h = _clampf(r.h, 100*dp, vh - 2*64*dp);
    return r;
}

// simple cascade placement for top-level windows
static void _auto_place_top_level(GuiManager* gm, GuiRect* r) {
    float vw, vh; _get_view_size(gm->renderer, &vw, &vh);
    const float dp = gm->d.style.dp ? gm->d.style.dp : 1.0f;
    const float step = 24.0f * dp;
    const uint32_t n = gui_window_count(gm);

    r->x = 40.0f * dp + (n % 10) * step;
    r->y = 40.0f * dp + (n % 10) * step;

    // clamp so it stays on-screen
    r->x = _clampf(r->x, 0.0f, vw - r->w);
    r->y = _clampf(r->y, 0.0f, vh - r->h);
}

static void _auto_place_child(const GuiManager* gm, const GuiWindow* parent, GuiRect* r) {
    const GuiStyle* s = &gm->d.style;
    const float dp = s->dp ? s->dp : 1.0f;
    const float pad = s->padding * dp;
    const float title_h = 24.0f * dp; // TITLE_FONT_SIZE equivalent

    const GuiRect pr = parent->base.rect;
    const float inset_x = pad;
    const float inset_y = pad + title_h; // keep below title
    const float avail_w = fmaxf(0.0f, pr.w - 2*pad);
    const float avail_h = fmaxf(0.0f, pr.h - title_h - 2*pad);

    // size first if still zero (or flagged auto)
    if (r->w <= 0.0f) r->w = fmaxf(120.0f * dp, avail_w * 0.75f);
    if (r->h <= 0.0f) r->h = fmaxf(100.0f * dp,  avail_h * 0.60f);

    // place within client area
    r->x = pr.x + inset_x;
    r->y = pr.y + inset_y;

    // clamp
    if (r->w > avail_w) r->w = avail_w;
    if (r->h > avail_h) r->h = avail_h;
}

// ---------- helpers & hit-testing ----------
static inline bool _pt_in_rect(float x, float y, const GuiRect* r) {
    return (x >= r->x && y >= r->y && x < (r->x + r->w) && y < (r->y + r->h));
}

static bool _is_under(const GuiObject* maybe_ancestor, const GuiObject* node) {
    for (const GuiObject* p = node; p; p = p->parent) if (p == maybe_ancestor) return true;
    return false;
}

// walk children back-to-front (O(n^2) but trees are small; fine for now)
static const GuiObject* _hit_deep_obj(const GuiObject* o, float x, float y, const GuiObject* exclude_root)
{
    if (!o || !o->visible || o->opacity <= 0.0f) return NULL;
    if (!_pt_in_rect(x, y, &o->rect)) return NULL;
    if (exclude_root && _is_under(exclude_root, o)) return NULL;

    // find last child
    const GuiObject* last = o->first_child;
    if (last) { while (last->next_sibling) last = last->next_sibling; }

    // iterate children in reverse z (last → first)
    for (const GuiObject* tail = last; tail; ) {
        const GuiObject* prev = NULL;
        for (const GuiObject* it = o->first_child; it && it != tail; it = it->next_sibling) prev = it;
        const GuiObject* hit = _hit_deep_obj(tail, x, y, exclude_root);
        if (hit) return hit;
        tail = prev;
    }
    return o;
}

static GuiWindow* _hit_window_at(GuiManager* gm, float x, float y, const GuiWindow* exclude_subtree)
{
    const GuiObject* exclude = exclude_subtree ? &exclude_subtree->base : NULL;
    // top-level, front-to-back
    for (GuiWindow* w = gm->root_tail; w; ) {
        // find previous (singly linked)
        GuiWindow* prev = NULL;
        for (GuiWindow* it = gm->root_head; it && it != w; it = GUI_CAST(GuiWindow, it->base.next_sibling)) prev = it;

        const GuiObject* hit = _hit_deep_obj(&w->base, x, y, exclude);
        if (hit) {
            // return nearest window ancestor of the hit object
            const GuiObject* o = hit;
            while (o && o->type != GUI_TYPE_WINDOW) o = o->parent;
            return o ? (GuiWindow*)o : NULL;
        }
        w = prev;
    }
    return NULL;
}

static inline bool _rect_all_zero(GuiRect r){ return r.x==0 && r.y==0 && r.w==0 && r.h==0; }

static GuiRect _viewport_rect(GuiManager* gm) {
    float vw=800, vh=600; _get_view_size(gm->renderer, &vw, &vh);
    return (GuiRect){0,0,vw,vh};
}

static GuiRect _context_rect(GuiManager* gm, const GuiWindow* parent) {
    if (!parent) return _viewport_rect(gm);
    return _parent_client_rect(gm, parent);  // inside border+padding+title
}

static void _limit_size_to_context(GuiRect* r, const GuiRect* cx) {
    // Don’t move (respect position), just cap width/height so rect stays inside cx.
    float maxw = (cx->x + cx->w) - r->x;
    float maxh = (cx->y + cx->h) - r->y;
    if (maxw < 1) maxw = 1;
    if (maxh < 1) maxh = 1;
    if (r->w > maxw) r->w = maxw;
    if (r->h > maxh) r->h = maxh;
}

static GuiRect _default_size_in_context(GuiManager* gm, const GuiRect* cx) {
    float vw, vh; _get_view_size(gm->renderer, &vw, &vh);
    GuiRect def = _default_content_rect(&gm->d.style, vw, vh);
    if (def.w > cx->w) def.w = cx->w;
    if (def.h > cx->h) def.h = cx->h;
    // Position is not used here; only w/h matter.
    return (GuiRect){0,0,def.w,def.h};
}

static uint32_t _sibling_index(const GuiWindow* parent) {
    uint32_t n=0; if (!parent) return 0;
    for (GuiObject* c = parent->base.first_child; c; c = c->next_sibling) ++n;
    return n;
}

// ---------- drag helpers ----------
static inline GuiWindow* _parent_of(GuiWindow* w) {
    return w && w->base.parent ? (GuiWindow*)w->base.parent : NULL;
}

static inline bool _rect_contains_pt(const GuiRect* r, float x, float y) {
    return x >= r->x && y >= r->y && x < r->x + r->w && y < r->y + r->h;
}

static void _drag_begin(GuiManager* gm, GuiWindow* w, float mx, float my)
{
    gm->drag.active        = true;
    gm->drag.target        = w;
    gm->drag.origin_parent = _parent_of(w);
    gm->drag.hover_parent  = gm->drag.origin_parent;
    gm->drag.proxy         = w->base.rect;
    gm->drag.dx            = mx - w->base.rect.x;
    gm->drag.dy            = my - w->base.rect.y;

    // bring to front if top-level (cosmetic)
    if (!w->base.parent) gui_window_bring_to_front(gm, w);
}

static void _drag_update(GuiManager* gm, float mx, float my)
{
    if (!gm->drag.active || !gm->drag.target) return;

    if (gm->drag.hover_parent)
        set_hover(gm->drag.hover_parent, false);
    // move ghost
    gm->drag.proxy.x = mx - gm->drag.dx;
    gm->drag.proxy.y = my - gm->drag.dy;

    // choose candidate parent (GRID rules), otherwise keep origin
    if (gm->tiling == GUI_TILING_GRID) {
        // hysteresis: only leave origin if pointer exits its client area by some margin
        const float dp = gm->d.style.dp ? gm->d.style.dp : 1.0f;
        const float hysteresis = 12.0f * dp;

        GuiWindow* origin = gm->drag.origin_parent;
        GuiRect keep = origin ? _parent_client_rect(gm, origin)
                              : (GuiRect){0,0,0,0};
        if (!origin) {
            float vw, vh; _get_view_size(gm->renderer, &vw, &vh);
            keep = (GuiRect){0,0,vw,vh};
        }
        GuiRect inflated = keep;
        inflated.x -= hysteresis; inflated.y -= hysteresis;
        inflated.w += 2*hysteresis; inflated.h += 2*hysteresis;

        GuiWindow* candidate = _hit_window_at(gm, mx, my, gm->drag.target);

        // stay with origin unless we truly left its (inflated) client area or we're over a different child explicitly
        if (!_rect_contains_pt(&inflated, mx, my)) {
            gm->drag.hover_parent = candidate;  // may be NULL for root
        } else if (candidate && candidate != origin && !_is_under(&gm->drag.target->base, &candidate->base)) {
            // inside origin, but over some sibling/descendant container → allow reparent
            gm->drag.hover_parent = candidate;
        } else {
            gm->drag.hover_parent = origin;
        }
    }
    if (gm->drag.hover_parent)
        set_hover(gm->drag.hover_parent, true);
    // CASCADE: nothing else to do during drag; we just move the ghost.
}

static void _link_as_last_child(GuiWindow* parent, GuiWindow* w)
{
    w->base.parent = &parent->base;
    GuiObject** pnext = &parent->base.first_child;
    while (*pnext) pnext = &(*pnext)->next_sibling;
    *pnext = &w->base;
}

static void _detach_from_parent_or_root(GuiManager* gm, GuiWindow* w)
{
    if (w->base.parent) {
        GuiObject** it = &w->base.parent->first_child;
        while (*it) {
            if (*it == &w->base) { *it = (*it)->next_sibling; break; }
            it = &(*it)->next_sibling;
        }
        w->base.parent = NULL;
        w->base.next_sibling = NULL;
    } else {
        _unlink(gm, w);
    }
}

static void _drag_end(GuiManager* gm)
{
    if (!gm->drag.active || !gm->drag.target) { gm->drag.active=false; return; }

    if (gm->drag.hover_parent)
        set_hover(gm->drag.hover_parent, false);
    GuiWindow* w       = gm->drag.target;
    GuiWindow* old_par = gm->drag.origin_parent;
    GuiWindow* new_par = gm->drag.hover_parent;

    if (gm->tiling == GUI_TILING_GRID) {
        // reparent if changed
        if (new_par != old_par) {
            _detach_from_parent_or_root(gm, w);
            if (new_par) _link_as_last_child(new_par, w);
            else         _link_front(gm, w);
        }
        // snap to grid
        if (new_par) _relayout_grid_subtree(gm, new_par);
        if (old_par && old_par != new_par) _relayout_grid_subtree(gm, old_par);
        if (!new_par) _relayout_grid(gm); // top-level grid refresh
    } else {
        // CASCADE → commit the proxy rect (clamped to context)
        const GuiRect cx = _context_rect(gm, _parent_of(w));
        GuiRect r = gm->drag.proxy;
        _limit_size_to_context(&r, &cx);
        if (w->base.vtbl && w->base.vtbl->arrange) w->base.vtbl->arrange(&w->base, r);
        else w->base.rect = r;
    }

    // clear drag
    gm->drag.active = false;
    gm->drag.target = gm->drag.origin_parent = gm->drag.hover_parent = NULL;
    
}


// ---------- overlay ----------
static void _render_drag_overlay(GuiManager* gm)
{
    if (!gm->drag.active) return;

    SDL_Renderer* r = gm->renderer;
    const GuiStyle* s = &gm->d.style;

    // Make sure overlay is not clipped by previous node
    SDL_SetRenderClipRect(r, NULL);

    // ghost rectangle
    GuiColor fill   = (GuiColor){0.20f, 0.55f, 1.0f, 0.25f};
    GuiColor border = (GuiColor){0.20f, 0.55f, 1.0f, 0.9f};
    float br        = s->border_radius > 0 ? s->border_radius : 6.0f;
    float bw        = s->border_width  > 0 ? s->border_width  : 1.0f;

    SDL_FRect fr = (SDL_FRect){ gm->drag.proxy.x, gm->drag.proxy.y, gm->drag.proxy.w, gm->drag.proxy.h };
    gui_box_rounded(gm->renderer, &fr, fill, 1.0f, border, bw, br);

    // highlight candidate parent’s client rect (if any, in GRID)
    if (gm->tiling == GUI_TILING_GRID && gm->drag.hover_parent) {
        // ((GuiStyle*)(gm->drag.hover_parent->base.style))->win_bg = (GuiColor){0.0f, 1.0f, 0.15f, 0.9f};

        // GuiRect cr = _parent_client_rect(gm, gm->drag.hover_parent);
        // SDL_FRect h = (SDL_FRect){ cr.x, cr.y, cr.w, cr.h };
        // GuiColor hb = (GuiColor){0.0f, 1.0f, 0.15f, 0.9f};
        // gui_box_border_rounded(gm->renderer, &h, hb, bw, br);
    }
}

// ---- Public API ------------------------------------------------------------
GuiManager* gui_create(void* sdl_renderer)
{
    GuiManager* gm = (GuiManager*)calloc(1, sizeof(GuiManager));
    if (!gm) return NULL;

    gm->renderer = (SDL_Renderer*)sdl_renderer;
    gm->d.rect   = _rect_zero();
    gm->d.flags  = _default_window_flags();
    gm->d.style  = gui_style_default_dark(); // default

    gm->max_window_depth = 10;
    gm->max_objects      = 1024;
    gm->tiling           = GUI_TILING_CASCADE;

    return gm;
}

void gui_destroy(GuiManager* gm)
{
    if (!gm) return;
    // destroy all top-level windows recursively (naive)
    GuiWindow* w = gm->root_head;
    while (w) {
        GuiWindow* next = GUI_CAST(GuiWindow, w->base.next_sibling);
        // let user-owned windows be freed by our window destroy:
        gui_window_destroy(gm, w);
        w = next;
    }
    free(gm);
}

void gui_set_style(GuiManager* gm, const GuiStyle* s) {
    if (!gm || !s) return;
    gm->d.style = *s; // copy
}
const GuiStyle* gui_style(const GuiManager* gm) {
    return gm ? &gm->d.style : NULL;
}

void gui_defaults_set_window_rect(GuiManager* gm, GuiRect rect) {
    if (!gm) return; gm->d.rect = rect;
}
void gui_defaults_add_window_flags(GuiManager* gm, GuiWindowFlags flags) {
    if (!gm) return; gm->d.flags |= flags;
}
void gui_defaults_clear_window_flags(GuiManager* gm, GuiWindowFlags flags) {
    if (!gm) return; gm->d.flags &= ~flags;
}

void gui_set_max_window_depth(GuiManager* gm, uint32_t depth) { if (gm) gm->max_window_depth = depth; }
void gui_set_max_objects(GuiManager* gm, uint32_t max_objects){ if (gm) gm->max_objects = max_objects; }
void gui_set_tiling(GuiManager* gm, GuiTilingMode mode) { if (gm) gm->tiling = mode; }

// ───────────────────────── the create function (drop-in) ────────────────────
// NULL or opt_desc zero rect -> automatic placement
// When either flag is set (AUTO_POS/AUTO_SIZE) we ignore the desc rect fields for that concern.
// If flags are not set and the desc rect is all zero -> treat as auto for both.
GuiWindow* gui_window_create(GuiManager* gm, GuiWindow* parent,
                                     const char* title, const GuiWindowDesc* opt_desc, const GuiStyle* opt_style)
{
    if (!gm) return NULL;

    // 1) resolve desc from manager defaults + override
    GuiWindowDesc desc = (GuiWindowDesc){0};
    desc.rect   = (opt_desc ? opt_desc->rect  : gm->d.rect);
    desc.flags  = (opt_desc && opt_desc->flags) ? opt_desc->flags : gm->d.flags;
    desc.title  = (opt_desc && opt_desc->title) ? opt_desc->title : title;
    desc.user_level = (opt_desc ? opt_desc->user_level : 0);

    const bool rect_zero = _rect_all_zero(desc.rect);
    bool want_auto_pos   = (desc.flags & GUI_WINDOW_AUTO_POS)  || rect_zero;
    bool want_auto_size  = (desc.flags & GUI_WINDOW_AUTO_SIZE) || rect_zero;

    // 2) build the object
    GuiWindow* w = window_create(&desc);
    if (!w) return NULL;

    if (opt_style) window_set_style(w, opt_style);
    else window_set_style(w, &gm->d.style);
    w->level = parent ? (parent->level + 1) : 0;

    // 3) link first (needed for grid relayout to see it)
    if (parent) {
        w->base.parent = &parent->base;
        GuiObject** pnext = &parent->base.first_child;
        while (*pnext) pnext = &(*pnext)->next_sibling;
        *pnext = &w->base;
    } else {
        _link_front(gm, w);
    }
    gm->window_count++;

    // 4) compute context
    const GuiRect cx = _context_rect(gm, parent);

    // 5) choose behavior
    GuiRect r = w->base.rect; // initialized from desc.rect by factory

    // GRID: if auto (pos or size), relayout the whole context and we’re done.
    if (gm->tiling == GUI_TILING_GRID && (want_auto_pos || want_auto_size)) {
        if (parent) _relayout_grid_children(gm, parent);
        else        _relayout_grid(gm);
        return w;
    }

    // CASCADE / non-grid policies:
    if (want_auto_pos && want_auto_size) {
        // Both auto: pick default size for the context + compute position.
        GuiRect dsz = _default_size_in_context(gm, &cx);
        r.w = dsz.w; r.h = dsz.h;

        if (parent) {
            // place in parent client area (simple child cascade start)
            _auto_place_child(gm, parent, &r);
        } else {
            // top-level cascade uses sibling count for offset
            _auto_place_top_level(gm, &r);
        }
        _limit_size_to_context(&r, &cx);
    }
    else if (want_auto_pos && !want_auto_size) {
        // Auto position only: keep size from desc, just place it.
        if (parent) _auto_place_child(gm, parent, &r);
        else        _auto_place_top_level(gm, &r);
        _limit_size_to_context(&r, &cx);
        // NOTE: In GRID this would ideally pair with AUTO_SIZE; for cascade this is fine.
    }
    else if (!want_auto_pos && want_auto_size) {
        // Auto size only: keep given position, fit size inside context.
        GuiRect dsz = _default_size_in_context(gm, &cx);
        // fit but don’t exceed available space from current (given) position
        r.w = fminf(dsz.w, (cx.x + cx.w) - r.x);
        r.h = fminf(dsz.h, (cx.y + cx.h) - r.y);
        if (r.w < 1) r.w = 1;
        if (r.h < 1) r.h = 1;
    }
    else {
        // Neither auto: respect desc; just clamp to be safe (viewport for top-level, client for child)
        _limit_size_to_context(&r, &cx);
    }

    // 6) apply
    if (w->base.vtbl && w->base.vtbl->arrange) w->base.vtbl->arrange(&w->base, r);
    else w->base.rect = r;

    return w;
}

void gui_manager_invalidate_layout(GuiManager* gm, GuiWindow* container)
{
    if (!gm) return;

    if (gm->tiling == GUI_TILING_GRID) {
        if (container) {
            _relayout_grid_subtree(gm, container); // container changed → fix its subtree
        } else {
            _relayout_grid(gm);                    // viewport changed → fix all roots + descendants
        }
        return;
    }
}

void gui_window_destroy(GuiManager* gm, GuiWindow* win)
{
    if (!gm || !win) return;

    // detach from parent list or root list
    if (win->base.parent) {
        GuiObject* p = GUI_CAST(GuiObject, ((GuiWindow*)win->base.parent));
        GuiObject** it = &p->first_child;
        while (*it) {
            if (*it == &win->base) { *it = (*it)->next_sibling; break; }
            it = &(*it)->next_sibling;
        }
    } else {
        _unlink(gm, win);
    }
    
    // Destroying a window should recursively destroy all it's children
    if (win->base.vtbl && win->base.vtbl->destroy) win->base.vtbl->destroy(&win->base);

    if (gm->window_count) gm->window_count--;
}

void gui_window_bring_to_front(GuiManager* gm, GuiWindow* win)
{
    if (!gm || !win || win->base.parent) return; // only top-level now
    _unlink(gm, win);
    _link_front(gm, win);
}

void gui_window_focus(GuiManager* gm, GuiWindow* win)
{
    if (!gm || !win) return;
    gm->focused = win;
}

void gui_handle_sdl_event(GuiManager* gm, const void* sdl_event)
{
    if (!gm || !sdl_event) return;
    const SDL_Event* ev = (const SDL_Event*)sdl_event;

    if (ev->type == SDL_EVENT_WINDOW_RESIZED ||
        ev->type == SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED) {
        _gui_manager_viewport_resized(gm);
        return ;
    }

    // DRAG state machine (manager-level)
    switch (ev->type) {
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        if (ev->button.button == SDL_BUTTON_LEFT) {
            GuiWindow* w = _hit_window_at(gm, (float)ev->button.x, (float)ev->button.y, NULL);
            if (w && (w->flags & GUI_WINDOW_MOVABLE)) {
                _drag_begin(gm, w, (float)ev->button.x, (float)ev->button.y);
                return; // consume
            }
        }
        break;
    case SDL_EVENT_MOUSE_MOTION:
        if (gm->drag.active) {
            _drag_update(gm, (float)ev->motion.x, (float)ev->motion.y);
            return; // consume
        }
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        if (ev->button.button == SDL_BUTTON_LEFT && gm->drag.active) {
            _drag_end(gm);
            return; // consume
        }
        break;
    default: break;
    }

    // walk front-to-back (so top-most handles first)
    for (GuiWindow* w = gm->root_tail; w; ) {
        // find previous by walking (list is singly-linked; keep it simple)
        GuiWindow* prev = NULL;
        for (GuiWindow* cur = gm->root_head; cur && cur != w; cur = GUI_CAST(GuiWindow, cur->base.next_sibling)) prev = cur;

        if (_event_tree(w, ev)) {
            // if clicked, bring to front 
            if (ev->type == SDL_EVENT_MOUSE_BUTTON_DOWN) gui_window_bring_to_front(gm, w);
            return;
        }
        w = prev;
    }
}

void gui_update(GuiManager* gm, float delta_ms)
{
    (void)gm; (void)delta_ms;
    // later: animations, layout invalidation, etc.
}

void gui_render(GuiManager* gm)
{
    if (!gm || !gm->renderer) return;
    for (GuiWindow* w = gm->root_head; w != NULL; w = GUI_CAST(GuiWindow, w->base.next_sibling)) {
        _render_subtree(gm, &w->base, 0);
    }
    _render_drag_overlay(gm);
}

uint32_t gui_window_count(const GuiManager* gm) { return gm ? gm->window_count : 0; }
uint32_t gui_window_child_count(const GuiWindow* win) {
    uint32_t n=0; for (GuiObject* c = win ? win->base.first_child : NULL; c; c = c->next_sibling) ++n; return n;
}
uint32_t gui_window_level(const GuiWindow* win) { return win ? win->level : 0; }

// ---- internals --------------------------------------------------------------
static void _link_front(struct GuiManager* gm, GuiWindow* w)
{
    w->base.next_sibling = NULL;
    if (!gm->root_head) {
        gm->root_head = gm->root_tail = w;
    } else {
        gm->root_tail->base.next_sibling = &w->base;
        gm->root_tail = w;
    }
    w->z = (gm->root_tail ? gm->root_tail->z + 1 : 0);
}
static void _unlink(struct GuiManager* gm, GuiWindow* w)
{
    if (!gm->root_head) return;
    if (gm->root_head == w) {
        gm->root_head = GUI_CAST(GuiWindow, w->base.next_sibling);
        if (gm->root_tail == w) gm->root_tail = NULL;
        return;
    }
    GuiWindow* prev = gm->root_head;
    while (prev && prev->base.next_sibling != &w->base) {
        prev = GUI_CAST(GuiWindow, prev->base.next_sibling);
    }
    if (prev) {
        prev->base.next_sibling = w->base.next_sibling;
        if (gm->root_tail == w) gm->root_tail = prev;
    }
    w->base.next_sibling = NULL;
}

static void _render_subtree(GuiManager* gm, const GuiObject* o, uint32_t depth)
{
    if (!gm || !o || !o->visible || o->opacity <= 0.0f) return;

    const uint32_t maxd = gm->max_window_depth; // 0 = unlimited
    if (maxd && depth >= maxd) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "GUI: render depth limit reached (id=%u type=%u depth=%u >= max=%u). "
                    "Children will not be rendered.",
                    (unsigned)o->id, (unsigned)o->type, (unsigned)depth, (unsigned)maxd);
        return;
    }

    SDL_Renderer* r = gm->renderer;

    // Clip to this object’s rect (rounded to ints).
    SDL_Rect prevClip; bool hadPrev = SDL_GetRenderClipRect(r, &prevClip);
    SDL_Rect clip = {
        (int)floorf(o->rect.x),
        (int)floorf(o->rect.y),
        (int)ceilf (o->rect.w),
        (int)ceilf (o->rect.h)
    };
    // Only set if positive size; otherwise keep previous clip.
    bool setClip = (clip.w > 0 && clip.h > 0);
    if (setClip) SDL_SetRenderClipRect(r, &clip);

    // 1) self (background/body)
    if (o->vtbl && o->vtbl->render) o->vtbl->render(o, r);

    // 2) children
    for (const GuiObject* c = o->first_child; c; c = c->next_sibling) {
        _render_subtree(gm, c, depth + 1);
    }

    // 3) overlay (chrome on top of children)
    if (o->vtbl && o->vtbl->render_overlay) o->vtbl->render_overlay(o, r);

    // restore parent’s clip
    if (setClip) SDL_SetRenderClipRect(r, hadPrev ? &prevClip : NULL);
}

static int _event_tree(GuiWindow* w, const SDL_Event* ev)
{
    // try children first (front-most last sibling first would be nicer; minimal here)
    for (GuiObject* c = w->base.first_child; c; c = c->next_sibling) {
        GuiWindow* cw = (GuiWindow*)c;
        if (cw->base.vtbl && cw->base.vtbl->event && cw->base.vtbl->event(&cw->base, ev))
            return 1;
    }
    if (w->base.vtbl && w->base.vtbl->event) return w->base.vtbl->event(&w->base, ev) ? 1 : 0;
    return 0;
}
