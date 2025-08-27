#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <stdlib.h>
#include <string.h>

#include "gui_window.h"
#include "gui_style.h"
#include "gui_draw.h"

#ifndef TITLE_FONT_SIZE
#define TITLE_FONT_SIZE 24.0f
#endif

// simple monotonic id generator
static GuiId s_next_id = 1;

// choose any non-zero type tag you like for windows
const GuiTypeId GUI_TYPE_WINDOW = 0x00000001u;

// ----------------- helpers ------------------
static inline Uint8 to8(float f) {
    if (f <= 0.0f) return 0; if (f >= 1.0f) return 255;
    return (Uint8)(f * 255.0f + 0.5f);
}
static void set_draw_rgba(SDL_Renderer* r, GuiColor c, float mul_alpha) {
    SDL_SetRenderDrawColor(r, to8(c.r), to8(c.g), to8(c.b), to8(c.a * mul_alpha));
}

// ----- forward decls of vtable funcs -----
static void gw_destroy(GuiObject* obj);
static void gw_set_visible(GuiObject* obj, bool v);
static bool gw_is_visible(const GuiObject* obj);
static void gw_measure(GuiObject* obj);
static void gw_arrange(GuiObject* obj, GuiRect rect);
static bool gw_event(GuiObject* obj, const void* sdl_event);
static void gw_render(const GuiObject* obj, void* renderer);

// a single static vtable instance
static const GuiVTable GW_VTABLE = {
    .destroy     = gw_destroy,
    .set_visible = gw_set_visible,
    .is_visible  = gw_is_visible,
    .measure     = gw_measure,
    .arrange     = gw_arrange,
    .event       = gw_event,
    .render      = gw_render,
};

GuiWindow* gui_window_create(const GuiWindowDesc* desc)
{
    GuiWindow* win = (GuiWindow*)calloc(1, sizeof(GuiWindow));
    if (!win) return NULL;

    // base fields
    win->base.type         = GUI_TYPE_WINDOW;
    win->base.id           = s_next_id++;
    win->base.parent       = NULL;
    win->base.first_child  = NULL;
    win->base.next_sibling = NULL;
    win->base.rect         = (GuiRect){0,0,42,42};
    win->base.opacity      = 1.0f;
    win->base.visible      = true;
    win->base.style        = NULL;
    win->base.vtbl         = &GW_VTABLE;

    // window fields
    win->flags = desc ? desc->flags : 0;
    win->title = desc ? desc->title : NULL;
    win->level = desc ? desc->user_level : 0;
    win->z     = 0;

    if (desc) {
        // if rect is all zeros and AUTO flags aren’t set, we’ll still treat it as no-op;
        // caller can arrange later or depend on SDL window size.
        if (!(desc->rect.w <= 0 && desc->rect.h <= 0 && desc->rect.x <= 0 && desc->rect.y <= 0)) {
            win->base.rect = desc->rect;
        }
    }

    return win;
}

// ----- vtable impls -----

static void gw_destroy(GuiObject* obj)
{
    free(obj);
}

static void gw_set_visible(GuiObject* obj, bool v)
{
    obj->visible = v;
}

static bool gw_is_visible(const GuiObject* obj)
{
    return obj->visible;
}

static void gw_measure(GuiObject* obj)
{
    (void)obj; // nothing: we don’t do preferred size yet
}

static void gw_arrange(GuiObject* obj, GuiRect rect)
{
    obj->rect = rect;
}

static bool gw_event(GuiObject* obj, const void* sdl_event)
{
    (void)obj;
    (void)sdl_event;
    // no interaction yet
    return false;
}

static void _gw_render_close_btn(SDL_Renderer* r, SDL_FRect *fr, const GuiStyle* style, float opacity) {
    float dp         = style ? style->dp : 1.0f;
    const float title_h = TITLE_FONT_SIZE * dp;
    const float dp_local = dp > 0 ? dp : 1.0f;
    const float pad   = (style ? style->padding : 4.0f) * dp_local;
    const float btn   = 16.0f * dp_local;
    
    SDL_FRect close_r = {
        fr->x + fr->w - pad - btn,
        fr->y + (title_h - btn) * 0.5f,
        btn,
        btn
    };

    // button background (use title bg a bit darker)
    GuiColor btn_bg = style ? style->win_title_bg : (GuiColor){0.7f,0.7f,0.74f,1.0f};
    btn_bg.r *= 0.95f; btn_bg.g *= 0.95f; btn_bg.b *= 0.95f;
    set_draw_rgba(r, btn_bg, opacity);
    SDL_RenderFillRect(r, &close_r);

    // button border
    set_draw_rgba(r, style ? style->win_border : (GuiColor){0.24f,0.24f,0.28f,1.0f}, 1.0f);
    SDL_RenderRect(r, &close_r);

    // draw the 'X'
    const float inset = 4.0f * dp_local;
    const float x1 = close_r.x + inset,               y1 = close_r.y + inset;
    const float x2 = close_r.x + close_r.w - inset,   y2 = close_r.y + close_r.h - inset;
    set_draw_rgba(r, style ? style->win_title_fg : (GuiColor){0.1f,0.1f,0.12f,1.0f}, 1.0f);
    SDL_RenderLine(r, x1, y1, x2, y2);
    SDL_RenderLine(r, x1, y2, x2, y1);
}

static void _gw_render_resizable_grip(SDL_Renderer* r, SDL_FRect *fr, const GuiStyle* style, float opacity) {
    float dp         = style ? style->dp : 1.0f;
    const float dp_local = dp > 0 ? dp : 1.0f;
    const float grip   = 6.0f * dp_local;
    const float step   = 3.0f  * dp_local;
    const float right  = fr->x + fr->w - 2.0f * dp_local;
    const float bottom = fr->y + fr->h - 2.0f * dp_local;

    set_draw_rgba(r, style ? style->win_border : (GuiColor){0.35f,0.37f,0.40f,1.0f}, 1.0f);
    // three diagonal lines forming a "grip"
    SDL_RenderLine(r, right - grip, bottom,       right, bottom - grip);
    SDL_RenderLine(r, right - grip + step, bottom,       right, bottom - grip + step);
    SDL_RenderLine(r, right - grip + step*2, bottom,     right, bottom - grip + step*2);
}

static void _gw_render_body(SDL_Renderer* r, SDL_FRect *fr, const GuiStyle* style, float opacity) {
    const GuiColor bg = style ? style->win_bg : (GuiColor){0.86f, 0.86f, 0.86f, 1.0f};
    const float border_radius = style ? style->border_radius : 5.0f;
    gui_box_fill_rounded(r, fr, bg, opacity, border_radius);
}

static void _gw_render_title_bar(SDL_Renderer* r, SDL_FRect *fr, const GuiStyle* style, float opacity) {
    const float dp = style ? style->dp : 1.0f;
    const float th = TITLE_FONT_SIZE * dp;
    if (th <= 0.0f || th >= fr->h) return;

    const GuiColor titlebg = style ? style->win_title_bg : (GuiColor){0.70f, 0.70f, 0.74f, 1.0f};
    const float br = style ? style->border_radius : 5.0f;  // same radii as body
    gui_box_fill_rounded4_clipY(r, fr, titlebg, opacity, br, br, br, br, fr->y, fr->y + th);
}


static void _gw_render_border(SDL_Renderer* r, SDL_FRect *fr, const GuiStyle* style, float opacity) {
    (void)opacity; // keep for future alpha-able borders if desired
    const float bw = style ? style->border_width : 1.0f;
    float border_radius = style ? style->border_radius : 5.0f;
    if (bw <= 0.0f) return;

    const GuiColor border = style ? style->win_border : (GuiColor){0.24f, 0.24f, 0.24f, 1.0f};
    gui_box_border_rounded(r, fr, border, bw, border_radius);
}

static void gw_render(const GuiObject* obj, void* renderer)
{
    if (!obj->visible || obj->opacity <= 0.0f) return;

    SDL_Renderer* r      = (SDL_Renderer*)renderer;
    const GuiWindow* win = GUI_CAST(GuiWindow, obj);
    const GuiStyle* style= (const GuiStyle*)obj->style;

    SDL_FRect fr = (SDL_FRect){ obj->rect.x, obj->rect.y, obj->rect.w, obj->rect.h };

    // 1) body
    _gw_render_body(r, &fr, style, obj->opacity);

    // 2) title bar (if any)
    if (win->title) {
        _gw_render_title_bar(r, &fr, style, obj->opacity);
        // TODO: draw title text here
    }

    // 3) border LAST so it’s not covered by the title bar
    _gw_render_border(r, &fr, style, obj->opacity);

    // 4) adornments
    if (win->flags & GUI_WINDOW_CLOSABLE)
        _gw_render_close_btn(r, &fr, style, obj->opacity);

    if (win->flags & GUI_WINDOW_RESIZABLE)
        _gw_render_resizable_grip(r, &fr, style, obj->opacity);
}

