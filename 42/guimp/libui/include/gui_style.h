// https://m2.material.io/design/layout/pixel-density.html#pixel-density-on-android

#ifndef GUI_STYLE_H
#define GUI_STYLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gui_types.h"

typedef struct {
    // global metrics
    float dp;             // device-independent unit base (pixel density)
    float padding, margin, border_radius, border_width;
    GuiColor fg, bg, border;

    // window palette
    GuiColor win_bg, win_title_bg, win_title_fg, win_border;

    // state variants (simple v1; can expand)
    GuiColor state_hover_tint;
    GuiColor state_focus_tint;

    bool hovered;

    // future: fonts, shadows, etc.
} GuiStyle;

GuiStyle gui_style_default_light(void);
GuiStyle gui_style_default_dark(void);

#ifdef __cplusplus
} // extern "C" {
#endif

#endif // GUI_STYLE_H
