/*
Core types

ui_context_t, ui_window_t, ui_widget_t, ui_rect_t, ui_color_t, ui_vec2_t.

ui_status_t error codes (OK, ERR_INIT, ERR_BAD_ARG, ERR_OOM, …).

ui_string_view_t (ptr+len) for zero‑copy params.

Initialization

ui_init(ui_context_t** out_ctx, const ui_init_params_t* p)

ui_shutdown(ui_context_t* ctx)

Run loop (integrates with your app’s loop)

ui_pump_events(ctx) → processes SDL events; updates focus/hover/capture.

ui_render(ctx) → draws all windows & widgets.

Strategy: one clear ownership model:

lib owns widgets/windows; callers get opaque handles.

RAII‑like “create → attach → destroy” pairs (explicit ui_destroy_*).
*/
