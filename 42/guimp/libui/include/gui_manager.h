#ifndef GUI_MANAGER_H
#define GUI_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "gui_types.h"
#include "gui_style.h"
#include "gui_window.h"

typedef struct GuiManager GuiManager;

typedef struct {
    uint32_t max_window_depth;   // nesting limit (default 4)
    uint32_t max_objects;        // total object capacity (for handle pool)
} GuiConfig;

typedef struct {
    // Defaults applied when fields are unspecified in desc
    struct {
        GuiRect rect;            // default 0→auto
        GuiWindowFlags flags;    // default: VISIBLE|INPUT|AUTO_POS|AUTO_SIZE|MOVABLE
    } window_default;
    // global style
    GuiStyle style; // default dark mode
} GuiDefaults;

// Simple tiling hint for auto-placement
typedef enum { GUI_TILING_CASCADE, GUI_TILING_GRID } GuiTilingMode;

// Create with built-in defaults (no config arg).
GuiManager* gui_create(void* sdl_renderer);
void        gui_destroy(GuiManager* gm);

// --- Global style & defaults (per-manager, no globals) ---
void        gui_set_style(GuiManager* gm, const GuiStyle* s); // copies
const GuiStyle* gui_style(const GuiManager* gm);              // view

// Window default rect/flags used when desc fields are zero/unspecified
void        gui_defaults_set_window_rect(GuiManager* gm, GuiRect rect);
void        gui_defaults_add_window_flags(GuiManager* gm, GuiWindowFlags flags);
void        gui_defaults_clear_window_flags(GuiManager* gm, GuiWindowFlags flags);

// Soft “config” setters (tunable anytime)
void        gui_set_max_window_depth(GuiManager* gm, uint32_t depth);
void        gui_set_max_objects(GuiManager* gm, uint32_t max_objects);
void        gui_set_tiling(GuiManager* gm, GuiTilingMode mode);

// Create/destroy windows managed by GM (parent may be NULL for top-level)
GuiWindow*  gui_window_create(GuiManager* gm, GuiWindow* parent,
                                      const char* title, const GuiWindowDesc* desc);
void        gui_window_destroy(GuiManager* gm, GuiWindow* win);

// Z-order & focus
void        gui_window_bring_to_front(GuiManager* gm, GuiWindow* win);
void        gui_window_focus(GuiManager* gm, GuiWindow* win);

// Frame loop
void        gui_handle_sdl_event(GuiManager* gm, const void* sdl_event);
void        gui_update(GuiManager* gm, float delta_ms);
void        gui_render(GuiManager* gm);

// Introspection
uint32_t    gui_window_count(const GuiManager* gm);
uint32_t    gui_window_child_count(const GuiWindow* win);
uint32_t    gui_window_level(const GuiWindow* win);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // GUI_MANAGER_H
