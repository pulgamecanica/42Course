#include "gui_draw.h"
#include <math.h>

static inline Uint8 gui__to8(float f) {
    if (f <= 0.0f) return 0;
    if (f >= 1.0f) return 255;
    return (Uint8)(f * 255.0f + 0.5f);
}

static inline void draw_hspan(SDL_Renderer* r, float x1, float x2, float y_center) {
    if (x2 <= x1) return;
    SDL_FRect seg = (SDL_FRect){ x1, y_center - 0.5f, x2 - x1, 1.0f };
    SDL_RenderFillRect(r, &seg);
}

void gui_set_rgba(SDL_Renderer* r, GuiColor c, float mul_alpha) {
    SDL_SetRenderDrawColor(r, gui__to8(c.r), gui__to8(c.g), gui__to8(c.b), gui__to8(c.a * mul_alpha));
}

/* ---------- rectircle helpers (|x/a|^(p_x) + |y/b|^(p_y) = 1) ---------- */

static inline float clampf_(float v, float lo, float hi){ return v < lo ? lo : (v > hi ? hi : v); }

/* normalize per-corner radii like CSS: scale all if sums exceed box size */
static void norm_radii4(const SDL_FRect* fr, float* rtl, float* rtr, float* rbr, float* rbl)
{
    float tl = fmaxf(*rtl, 0.0f), tr = fmaxf(*rtr, 0.0f), br = fmaxf(*rbr, 0.0f), bl = fmaxf(*rbl, 0.0f);
    float w = fmaxf(fr->w, 0.0f), h = fmaxf(fr->h, 0.0f);

    float sx1 = (tl + tr) > 0.0f ? w / (tl + tr) : 1.0f;
    float sx2 = (bl + br) > 0.0f ? w / (bl + br) : 1.0f;
    float sy1 = (tl + bl) > 0.0f ? h / (tl + bl) : 1.0f;
    float sy2 = (tr + br) > 0.0f ? h / (tr + br) : 1.0f;
    float k = fminf(fminf(sx1, sx2), fminf(sy1, sy2));
    if (k < 1.0f) { tl *= k; tr *= k; br *= k; bl *= k; }

    // clamp to half-min for numerical safety
    float rmax = 0.5f * fminf(w, h);
    *rtl = fminf(tl, rmax);
    *rtr = fminf(tr, rmax);
    *rbr = fminf(br, rmax);
    *rbl = fminf(bl, rmax);
}

static inline float left_boundary_for_y(const SDL_FRect* fr, float rtl, float rbl, float y)
{
    // centers
    float y_tl = fr->y + rtl, x_tl = fr->x + rtl;
    float y_bl = fr->y + fr->h - rbl, x_bl = fr->x + rbl;

    if (rtl > 0.0f && y < y_tl) {
        float dy = y - y_tl;
        float dx = sqrtf(fmaxf(rtl*rtl - dy*dy, 0.0f));
        return x_tl - dx;
    }
    if (rbl > 0.0f && y > y_bl) {
        float dy = y - y_bl;
        float dx = sqrtf(fmaxf(rbl*rbl - dy*dy, 0.0f));
        return x_bl - dx;
    }
    return fr->x;
}

static inline float right_boundary_for_y(const SDL_FRect* fr, float rtr, float rbr, float y)
{
    float y_tr = fr->y + rtr, x_tr = fr->x + fr->w - rtr;
    float y_br = fr->y + fr->h - rbr, x_br = fr->x + fr->w - rbr;

    if (rtr > 0.0f && y < y_tr) {
        float dy = y - y_tr;
        float dx = sqrtf(fmaxf(rtr*rtr - dy*dy, 0.0f));
        return x_tr + dx;
    }
    if (rbr > 0.0f && y > y_br) {
        float dy = y - y_br;
        float dx = sqrtf(fmaxf(rbr*rbr - dy*dy, 0.0f));
        return x_br + dx;
    }
    return fr->x + fr->w;
}

static void rr4_fill(SDL_Renderer* r, const SDL_FRect* fr, float rtl, float rtr, float rbr, float rbl)
{
    if (fr->w <= 0.0f || fr->h <= 0.0f) return;
    norm_radii4(fr, &rtl, &rtr, &rbr, &rbl);

    const int rows = (int)ceilf(fr->h);
    for (int i = 0; i < rows; ++i) {
        float y = fr->y + i + 0.5f;
        float x1 = left_boundary_for_y(fr, rtl, rbl, y);
        float x2 = right_boundary_for_y(fr, rtr, rbr, y);
        if (x2 > x1) SDL_RenderLine(r, x1, y, x2, y);
    }
}

static void rr4_stroke(SDL_Renderer* r, const SDL_FRect* fr,
                       float rtl, float rtr, float rbr, float rbl, float bw)
{
    if (bw <= 0.0f) return;

    // outer radii
    float o_tl = rtl, o_tr = rtr, o_br = rbr, o_bl = rbl;
    norm_radii4(fr, &o_tl, &o_tr, &o_br, &o_bl);

    // inner box
    SDL_FRect in = (SDL_FRect){ fr->x + bw, fr->y + bw, fr->w - 2*bw, fr->h - 2*bw };
    if (in.w <= 0.0f || in.h <= 0.0f) { rr4_fill(r, fr, o_tl, o_tr, o_br, o_bl); return; }

    // inner radii
    float i_tl = fmaxf(o_tl - bw, 0.0f);
    float i_tr = fmaxf(o_tr - bw, 0.0f);
    float i_br = fmaxf(o_br - bw, 0.0f);
    float i_bl = fmaxf(o_bl - bw, 0.0f);
    norm_radii4(&in, &i_tl, &i_tr, &i_br, &i_bl);

    const float inner_top    = in.y;
    const float inner_bottom = in.y + in.h;

    const int rows = (int)ceilf(fr->h);
    for (int i = 0; i < rows; ++i) {
        float y  = fr->y + i + 0.5f;

        // outer boundaries at this row
        float x_lo = left_boundary_for_y (fr,  o_tl, o_bl, y);
        float x_ro = right_boundary_for_y(fr,  o_tr, o_br, y);

        // if no inner cross-section intersects this row → draw the full strip
        if (y < inner_top || y > inner_bottom) {
            draw_hspan(r, x_lo, x_ro, y);
            continue;
        }

        // inner boundaries at this row
        float x_li = left_boundary_for_y (&in, i_tl, i_bl, y);
        float x_ri = right_boundary_for_y(&in, i_tr, i_br, y);

        // if inner collapses or inverts at this row → draw full strip
        if (x_ri <= x_li) {
            draw_hspan(r, x_lo, x_ro, y);
            continue;
        }

        // left and right ring segments (exclusive end → exact bw on screen)
        draw_hspan(r, x_lo, x_li, y);
        draw_hspan(r, x_ri, x_ro, y);
    }
}

// internal: same scanline as rr4_fill but clipped to [y0, y1)
static void rr4_fill_slice(SDL_Renderer* r, const SDL_FRect* fr,
                           float rtl, float rtr, float rbr, float rbl,
                           float y0, float y1)
{
    if (fr->w <= 0.0f || fr->h <= 0.0f || y1 <= y0) return;

    norm_radii4(fr, &rtl, &rtr, &rbr, &rbl);

    const float y_start = fmaxf(y0, fr->y);
    const float y_end   = fminf(y1, fr->y + fr->h);
    if (y_end <= y_start) return;

    const int i0 = (int)floorf(y_start - fr->y);
    const int i1 = (int)ceilf (y_end   - fr->y);

    for (int i = i0; i < i1; ++i) {
        float y = fr->y + i + 0.5f;
        if (y < y_start || y >= y_end) continue;
        float x1 = left_boundary_for_y (fr, rtl, rbl, y);
        float x2 = right_boundary_for_y(fr, rtr, rbr, y);
        if (x2 > x1) SDL_RenderLine(r, x1, y, x2, y);
    }
}

// ===== Public API ============================================================

void gui_box_fill(SDL_Renderer* r, const SDL_FRect* fr, GuiColor fill, float alpha) {
    gui_set_rgba(r, fill, alpha);
    SDL_RenderFillRect(r, fr);
}

void gui_box_border(SDL_Renderer* r, const SDL_FRect* fr, GuiColor border, float thickness) {
    if (thickness <= 0.0f) return;
    gui_set_rgba(r, border, 1.0f);

    // simple, crisp border by drawing inset rects
    const int loops = (int)(thickness + 0.5f);
    for (int i = 0; i < loops; ++i) {
        SDL_FRect br = { fr->x + i, fr->y + i, fr->w - i*2.0f, fr->h - i*2.0f };
        if (br.w <= 0.0f || br.h <= 0.0f) break;
        SDL_RenderRect(r, &br);
    }
}

void gui_box(SDL_Renderer* r, const SDL_FRect* fr, GuiColor fill, float alpha,
             GuiColor border, float thickness) {
    gui_box_fill(r, fr, fill, alpha);
    gui_box_border(r, fr, border, thickness);
}

// rounded variants
void gui_box_fill_rounded(SDL_Renderer* r, const SDL_FRect* fr, GuiColor fill,
                          float alpha, float radius)
{
    gui_set_rgba(r, fill, alpha);
    if (radius <= 0)
        gui_box_fill(r, fr, fill, alpha);
    else {
        float clamped_radius = clampf_(radius, 0.0f, fminf(fr->w, fr->h) * 0.5f);
        rr4_fill(r, fr, clamped_radius, clamped_radius, clamped_radius, clamped_radius);
    }
}

void gui_box_border_rounded(SDL_Renderer* r, const SDL_FRect* fr, GuiColor border,
                            float thickness, float radius)
{
    if (thickness <= 0.0f) return;
    gui_set_rgba(r, border, 1.0f);
    if (radius <= 0)
        gui_box_border(r, fr, border, thickness);
    else {
        float clamped_radius = clampf_(radius, 0.0f, fminf(fr->w, fr->h) * 0.5f);
        rr4_stroke(r, fr, clamped_radius, clamped_radius, clamped_radius, clamped_radius, thickness);
    }
}

void gui_box_rounded(SDL_Renderer* r, const SDL_FRect* fr, GuiColor fill, float alpha,
                     GuiColor border, float thickness, float radius)
{
    gui_box_fill_rounded(r, fr, fill, alpha, radius);
    gui_box_border_rounded(r, fr, border, thickness, radius);
}

void gui_box_fill_rounded4(SDL_Renderer* r, const SDL_FRect* fr, GuiColor fill,
                           float alpha, float rtl, float rtr, float rbr, float rbl)
{
    gui_set_rgba(r, fill, alpha);
    rr4_fill(r, fr, rtl, rtr, rbr, rbl);
}

void gui_box_border_rounded4(SDL_Renderer* r, const SDL_FRect* fr, GuiColor border,
                             float thickness, float rtl, float rtr, float rbr, float rbl)
{
    if (thickness <= 0.0f) return;
    gui_set_rgba(r, border, 1.0f);
    rr4_stroke(r, fr, rtl, rtr, rbr, rbl, thickness);
}

void gui_box_rounded4(SDL_Renderer* r, const SDL_FRect* fr, GuiColor fill, float alpha,
                      GuiColor border, float thickness,
                      float rtl, float rtr, float rbr, float rbl)
{
    gui_box_fill_rounded4(r, fr, fill, alpha, rtl, rtr, rbr, rbl);
    gui_box_border_rounded4(r, fr, border, thickness, rtl, rtr, rbr, rbl);
}

void gui_box_fill_rounded4_clipY(SDL_Renderer* r, const SDL_FRect* fr, GuiColor fill,
                                 float alpha, float rtl, float rtr, float rbr, float rbl,
                                 float y0, float y1)
{
    gui_set_rgba(r, fill, alpha);
    rr4_fill_slice(r, fr, rtl, rtr, rbr, rbl, y0, y1);
}
