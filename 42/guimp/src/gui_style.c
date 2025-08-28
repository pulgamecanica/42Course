#include "gui_style.h"

// helpers
static inline GuiColor RGBAf(float r, float g, float b, float a) {
    GuiColor c = { r, g, b, a }; return c;
}

GuiStyle gui_style_default_light(void)
{
    GuiStyle s;
    s.dp = 1.0f;

    s.padding = 4.0f;
    s.margin  = 0.0f;
    s.border_radius = 4.0f;
    s.border_width  = 1.0f;

    s.fg     = RGBAf(0.10f, 0.10f, 0.12f, 1.0f);
    s.bg     = RGBAf(0.95f, 0.95f, 0.96f, 1.0f);
    s.border = RGBAf(0.70f, 0.70f, 0.72f, 1.0f);

    s.win_bg        = RGBAf(0.93f, 0.93f, 0.95f, 1.0f);
    s.win_title_bg  = RGBAf(0.86f, 0.86f, 0.90f, 1.0f);
    s.win_title_fg  = RGBAf(0.10f, 0.10f, 0.12f, 1.0f);
    s.win_border    = RGBAf(0.55f, 0.55f, 0.60f, 1.0f);

    s.state_hover_tint = RGBAf(0.10f, 0.10f, 0.10f, 0.06f);
    s.state_focus_tint = RGBAf(0.10f, 0.30f, 1.00f, 0.12f);

    return s;
}

GuiStyle gui_style_default_dark(void)
{
    GuiStyle s;
    s.dp = 1.0f;

    s.padding = 4.0f;
    s.margin  = 0.0f;
    s.border_radius = 4.0f;
    s.border_width  = 1.0f;

    s.fg     = RGBAf(0.90f, 0.92f, 0.96f, 1.0f);
    s.bg     = RGBAf(0.08f, 0.09f, 0.11f, 1.0f);
    s.border = RGBAf(0.25f, 0.27f, 0.30f, 1.0f);

    s.win_bg        = RGBAf(0.13f, 0.14f, 0.17f, 1.0f);
    s.win_title_bg  = RGBAf(0.18f, 0.19f, 0.23f, 1.0f);
    s.win_title_fg  = RGBAf(0.92f, 0.94f, 0.97f, 1.0f);
    s.win_border    = RGBAf(0.35f, 0.37f, 0.40f, 1.0f);

    s.state_hover_tint = RGBAf(1.00f, 1.00f, 1.00f, 0.06f);
    s.state_focus_tint = RGBAf(0.20f, 0.50f, 1.00f, 0.16f);

    return s;
}
