// #ifndef GUI_MANAGER_H
// #define GUI_MANAGER_H

// #ifdef __cplusplus
// extern "C" {
// #endif

// #include "gui_types.h"
// #include "gui_style.h"

// typedef struct GuiManager GuiManager;
// typedef struct GuiWindow GuiWindow;

// typedef struct {
//     uint32_t max_window_depth;   // nesting limit (default 4)
//     uint32_t max_objects;        // total object capacity (for handle pool)
// } GuiConfig;

// typedef struct {
//     // Defaults applied when fields are unspecified in desc
//     struct {
//         GuiRect rect;            // default 0→auto
//         GuiWindowFlags flags;    // default: VISIBLE|INPUT|AUTO_POS|AUTO_SIZE|MOVABLE
//     } window_default;
//     // global style
//     GuiStyle style; // default dark mode
// } GuiDefaults;

// GuiConfig   gui_config_default(void);
// GuiDefaults gui_defaults_default(void);

// GuiManager* gui_manager_create(const GuiConfig* cfg, void* sdl_renderer);
// void        gui_manager_destroy(GuiManager* gm);

// void        gui_manager_set_defaults(GuiManager* gm, const GuiDefaults* d);
// void        gui_manager_set_style(GuiManager* gm, const GuiStyle* s);

// GuiWindow*  gui_window_create(GuiManager* gm, GuiWindow* parent,
//                               const char* title, const GuiWindowDesc* desc);
// void        gui_window_destroy(GuiManager* gm, GuiWindow* win);

// // Frame loop
// void        gui_manager_handle_sdl_event(GuiManager* gm, const void* sdl_event);
// void        gui_manager_update(GuiManager* gm, float delta_ms);
// void        gui_manager_render(GuiManager* gm);

// // Z-order & focus
// void        gui_window_bring_to_front(GuiManager* gm, GuiWindow* win);
// void        gui_window_focus(GuiManager* gm, GuiWindow* win);

// // Introspection
// uint32_t    gui_manager_window_count(const GuiManager* gm);
// uint32_t    gui_window_child_count(const GuiWindow* win);
// uint32_t    gui_window_level(const GuiWindow* win);

// // Automatic layout policy (simple v1)
// typedef enum { GUI_TILING_CASCADE, GUI_TILING_GRID } GuiTilingMode;
// void        gui_manager_set_tiling(GuiManager* gm, GuiTilingMode mode);

// #ifdef __cplusplus
// } // extern "C" {
// #endif

// #endif // GUI_MANAGER_H
