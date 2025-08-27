#ifndef GUI_DRAW_H
#define GUI_DRAW_H

#ifdef __cplusplus
extern "C" {
#endif

#include <SDL3/SDL.h>
#include "gui_types.h"   // GuiColor

// Sets renderer color from normalized RGBA (0..1), multiplied by mul_alpha.
void gui_set_rgba(SDL_Renderer* r, GuiColor c, float mul_alpha);

// Fills a rectangle with color (no border).
void gui_box_fill(SDL_Renderer* r, const SDL_FRect* fr, GuiColor fill, float alpha);

// Draws only the border of a rectangle. 'thickness' in pixels (>= 0).
void gui_box_border(SDL_Renderer* r, const SDL_FRect* fr, GuiColor border, float thickness);

// Convenience: fill + (optional) border.
void gui_box(SDL_Renderer* r, const SDL_FRect* fr, GuiColor fill, float alpha,
             GuiColor border, float thickness);

// Radius is in pixels; clamped to half the min(width,height).
void gui_box_fill_rounded(SDL_Renderer* r, const SDL_FRect* fr, GuiColor fill,
                          float alpha, float radius);

void gui_box_border_rounded(SDL_Renderer* r, const SDL_FRect* fr, GuiColor border,
                            float thickness, float radius);

void gui_box_rounded(SDL_Renderer* r, const SDL_FRect* fr, GuiColor fill, float alpha,
                     GuiColor border, float thickness, float radius);

void gui_box_fill_rounded4(SDL_Renderer* r, const SDL_FRect* fr, GuiColor fill,
                           float alpha, float rtl, float rtr, float rbr, float rbl);

void gui_box_border_rounded4(SDL_Renderer* r, const SDL_FRect* fr, GuiColor border,
                             float thickness, float rtl, float rtr, float rbr, float rbl);

void gui_box_rounded4(SDL_Renderer* r, const SDL_FRect* fr, GuiColor fill, float alpha,
                      GuiColor border, float thickness,
                      float rtl, float rtr, float rbr, float rbl);

// Fill only a vertical slice [y0, y1) of a per-corner rounded box (matches the outer shape).
void gui_box_fill_rounded4_clipY(SDL_Renderer* r, const SDL_FRect* fr, GuiColor fill,
                                 float alpha, float rtl, float rtr, float rbr, float rbl,
                                 float y0, float y1);


#ifdef __cplusplus
} // extern "C"
#endif

#endif // GUI_DRAW_H
