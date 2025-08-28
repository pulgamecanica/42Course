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
static void _render_tree(SDL_Renderer* r, const GuiWindow* w);
static int  _event_tree(GuiWindow* w, const SDL_Event* ev); // returns 1 if handled
static void _get_view_size(SDL_Renderer* r, float* out_w, float* out_h);

// ── helpers (put near top of file) ───────────────────────────────────────────
static inline bool _is_zero_rect(GuiRect r){ return r.x==0 && r.y==0 && r.w==0 && r.h==0; }
static inline float _clampf(float v, float a, float b){ return v < a ? a : (v > b ? b : v); }

// Treat windows with AUTO_POS or AUTO_SIZE as "managed" by the tiler.
// Are we auto-managed by tiler?
static inline bool _is_managed(const GuiWindow* w) {
    return (w->flags & (GUI_WINDOW_AUTO_POS | GUI_WINDOW_AUTO_SIZE)) != 0;
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

static void _auto_place_grid(GuiManager* gm, GuiRect* r) {
    float vw, vh; _get_view_size(gm->renderer, &vw, &vh);
    const uint32_t n = gui_window_count(gm);
    const uint32_t cols = (uint32_t)ceilf(sqrtf((float)(n + 1)));
    const uint32_t rows = (uint32_t)ceilf((float)(n + 1) / (float)cols);

    const float cell_w = vw / (float)cols;
    const float cell_h = vh / (float)rows;

    const uint32_t idx = n; // place next in sequence
    const uint32_t cx = idx % cols;
    const uint32_t cy = idx / cols;

    r->x = cx * cell_w + (cell_w - r->w) * 0.5f;
    r->y = cy * cell_h + (cell_h - r->h) * 0.5f;

    // clamp inside cell
    r->x = _clampf(r->x, cx * cell_w, (cx+1)*cell_w - r->w);
    r->y = _clampf(r->y, cy * cell_h, (cy+1)*cell_h - r->h);
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

// ensure the rect fits the viewport
static void _clamp_to_view(SDL_Renderer* r, GuiRect* rect) {
    float vw, vh; _get_view_size(r, &vw, &vh);
    rect->w = _clampf(rect->w, 1.0f, vw);
    rect->h = _clampf(rect->h, 1.0f, vh);
    rect->x = _clampf(rect->x, 0.0f, vw - rect->w);
    rect->y = _clampf(rect->y, 0.0f, vh - rect->h);
}

// ---- API --------------------------------------------------------------------
GuiManager* gui_create(void* sdl_renderer)
{
    GuiManager* gm = (GuiManager*)calloc(1, sizeof(GuiManager));
    if (!gm) return NULL;

    gm->renderer = (SDL_Renderer*)sdl_renderer;
    gm->d.rect   = _rect_zero();
    gm->d.flags  = _default_window_flags();
    gm->d.style  = gui_style_default_dark(); // default

    gm->max_window_depth = 4;
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

// NULL or opt_desc zero rect -> automatic placement
// When either flag is set (Auto pos, or auto size) it follows the flags and ignores the desc. If flags are not set 
// the zero_rect will also be auto, any other rect,, respects the given values
GuiWindow* gui_window_create(GuiManager* gm, GuiWindow* parent,
                                     const char* title, const GuiWindowDesc* opt_desc)
{
    if (!gm) return NULL;

    GuiWindowDesc desc = {0};
    desc.rect  = (opt_desc && (opt_desc->rect.w || opt_desc->rect.h)) ? opt_desc->rect : gm->d.rect;
    desc.flags = (opt_desc && opt_desc->flags) ? opt_desc->flags : gm->d.flags;
    desc.title = (opt_desc && opt_desc->title) ? opt_desc->title : title;
    desc.user_level = (opt_desc ? opt_desc->user_level : 0);

    GuiWindow* w = window_create(&desc);
    if (!w) return NULL;

    window_set_style(w, &gm->d.style);
    w->level = parent ? (parent->level + 1) : 0;
    gm->window_count++;

    // compute initial rect if AUTO flags or zero rect pieces are present
    GuiRect r = w->base.rect; // initialized by factory to desc.rect
    const bool is_managed = _is_managed(w);
    const bool auto_pos  = (w->flags & GUI_WINDOW_AUTO_POS) || _is_zero_rect(gm->d.rect));
    const bool auto_size = (w->flags & GUI_WINDOW_AUTO_SIZE) || (r.w == 0.0f && r.h == 0.0f);

    // Find the correct size for the newly created window.
    // Depends if it has parents
    if (auto_size) {
        float vw, vh;
        _get_view_size(gm->renderer, &vw, &vh);
        GuiRect def = _default_content_rect(&gm->d.style, vw, vh);
        r.w = def.w;
        r.h = def.h;
    }

    if (parent) {
        // link as last child first
        w->base.parent = &parent->base;
        GuiObject** pnext = &parent->base.first_child;
        while (*pnext) pnext = &(*pnext)->next_sibling;
        *pnext = &w->base; // Linked sibling after last most recent one

        if (gm->tiling == GUI_TILING_GRID) {
            _relayout_grid_children(gm, parent);   // always grid children
            return w;
        }

        // non-grid fallback (single auto placement)
        _auto_place_child(gm, parent, &r);
        if (w->base.vtbl && w->base.vtbl->arrange) w->base.vtbl->arrange(&w->base, r);
        else w->base.rect = r;
        return w;
    } else {
        // top-level
        if (gm->tiling == GUI_TILING_GRID && _is_managed(w)) {
            // link first, then relayout ALL managed top-level windows to fill the grid
            _link_front(gm, w);
            _relayout_grid(gm);
            return w;
        } else {
            // cascade (or explicit pos) for non-grid or unmanaged
            if ((w->flags & GUI_WINDOW_AUTO_POS) || (r.x == 0.0f && r.y == 0.0f))
                _auto_place_top_level(gm, &r);
            _clamp_to_view(gm->renderer, &r);

            if (w->base.vtbl && w->base.vtbl->arrange) w->base.vtbl->arrange(&w->base, r);
            else w->base.rect = r;

            _link_front(gm, w);
        }
    }

    return w;
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

    // TODO: recursively destroy children if any (minimal for now)
    // destroy this window object via its vtable
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
        _render_tree(gm->renderer, w);
    }
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

static void _render_tree(SDL_Renderer* r, const GuiWindow* w)
{
    if (w->base.vtbl && w->base.vtbl->render) w->base.vtbl->render(&w->base, r);
    for (GuiObject* c = w->base.first_child; c; c = c->next_sibling) {
        const GuiWindow* cw = (const GuiWindow*)c; // if children are windows; otherwise branch by type
        if (cw->base.vtbl && cw->base.vtbl->render) cw->base.vtbl->render(&cw->base, r);
    }
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
