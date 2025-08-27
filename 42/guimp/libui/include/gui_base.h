#ifndef GUI_BASE_H
#define GUI_BASE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gui_types.h"

typedef struct GuiObject GuiObject;

typedef struct {
    void (*destroy)(GuiObject*);
    void (*set_visible)(GuiObject*, bool);
    bool (*is_visible)(const GuiObject*);
    // layout
    void (*measure)(GuiObject*);              // compute preferred size (later: constraints)
    void (*arrange)(GuiObject*, GuiRect rect);
    // input
    bool (*event)(GuiObject*, const void* sdl_event);
    // render
    void (*render)(const GuiObject*, void* renderer);
} GuiVTable;

struct GuiObject {
    GuiTypeId   type;
    GuiId       id;
    GuiObject*  parent;
    GuiObject*  first_child;
    GuiObject*  next_sibling;

    GuiRect     rect;        // arranged (x,y,w,h)
    float       opacity;     // 0..1
    bool        visible;
    void*       style;       // GuiStyle* (opaque here)

    const GuiVTable* vtbl;
};

#define GUI_CAST(T, obj) ((T*)(obj))

#ifdef __cplusplus
} // extern "C" {
#endif

#endif // GUI_BASE_H
