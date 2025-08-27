#ifndef GUI_WINDOW_H
#define GUI_WINDOW_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gui_base.h"
#include "gui_types.h"

typedef struct GuiWindow GuiWindow;

typedef struct {
    GuiRect rect;           // if {0,0,0,0} → manager auto-places/sizes
    GuiWindowFlags flags;   // defaults come from manager; these override
    const char* title;      // nullable
    uint32_t user_level;    // logical nesting level hint (optional)
} GuiWindowDesc;

struct GuiWindow {
    GuiObject base;         // must be first
    GuiWindowFlags flags;
    const char* title;
    uint32_t level;         // resolved nesting level
    // internal: hit-test caches, z-index, etc.
    int32_t z;
};

extern const GuiTypeId GUI_TYPE_WINDOW;

// minimal constructor for a lone window
// caller owns the returned pointer and must call base.vtbl->destroy(&win->base)
GuiWindow* gui_window_create(const GuiWindowDesc* desc);

// a convenience if you want to change rect later (optional)
static inline void gui_window_set_rect(GuiWindow* w, GuiRect r) { w->base.vtbl->arrange(&w->base, r); }


#ifdef __cplusplus
} // extern "C" {
#endif

#endif // GUI_WINDOW_H
