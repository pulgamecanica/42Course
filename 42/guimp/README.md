Goals

C first, C++ friendly: headers guard with extern "C", no C++ keywords in API.

Object model in C: one base “object” struct + vtables; every element derives from it.

Windowing system: multiple windows, nesting with a configurable max depth.

Automatic UX: sensible defaults (position, size, z-order, focus) that you can override per instance.

Styles: swap themes at runtime; styles cascade but can be overridden at any node.

SDL3: no extra render libs required; SDL_Renderer or your GL path under SDL is fine.

```c
// 1) init once
GuiConfig cfg = gui_config_default();
cfg.max_window_depth = 4;            // default nesting limit
GuiManager *gm = gui_manager_create(&cfg, renderer);

// 2) set global defaults (optional)
GuiDefaults d = gui_defaults_default();
d.window_default.flags |= GUI_WINDOW_RESIZABLE | GUI_WINDOW_MOVABLE;
gui_manager_set_defaults(gm, &d);

// 3) create windows (automatic placement unless overridden)
GuiWindow *root = gui_window_create(gm, NULL, "Root", NULL);   // parent=NULL → top-level
GuiWindow *tools = gui_window_create(gm, root, "Tools", NULL); // nested

// 4) per-instance override
GuiWindowDesc desc = {0};
desc.flags = GUI_WINDOW_MOVABLE | GUI_WINDOW_RESIZABLE;
desc.rect = (GuiRect){ 40,40, 320,240 };
GuiWindow *dock = gui_window_create(gm, root, "Dock", &desc);

// 5) in your SDL callbacks
// SDL_AppEvent → gui_manager_handle_sdl_event(gm, &event)
// SDL_AppIterate → gui_manager_update(gm, delta_ms); gui_manager_render(gm);

```

---

```c
// 1) init once
GuiConfig cfg = gui_config_default();
cfg.max_window_depth = 4;            // default nesting limit
GuiManager *gm = gui_manager_create(&cfg, renderer);

// 2) set global defaults (optional)
GuiDefaults d = gui_defaults_default();
d.window_default.flags |= GUI_WINDOW_RESIZABLE | GUI_WINDOW_MOVABLE;
gui_manager_set_defaults(gm, &d);

// 3) create windows (automatic placement unless overridden)
GuiWindow *root = gui_window_create(gm, NULL, "Root", NULL);   // parent=NULL → top-level
GuiWindow *tools = gui_window_create(gm, root, "Tools", NULL); // nested

// 4) per-instance override
GuiWindowDesc desc = {0};
desc.flags = GUI_WINDOW_MOVABLE | GUI_WINDOW_RESIZABLE;
desc.rect = (GuiRect){ 40,40, 320,240 };
GuiWindow *dock = gui_window_create(gm, root, "Dock", &desc);

// 5) in your SDL callbacks
// SDL_AppEvent → gui_manager_handle_sdl_event(gm, &event)
// SDL_AppIterate → gui_manager_update(gm, delta_ms); gui_manager_render(gm);
```
---

### Object model (Ruby-ish in C)

Every visible/interactive thing is a GuiObject.

Each concrete type (Window, Button later, etc.) has its own vtable implementing common operations: measure, arrange, paint, event.

Objects form a tree (parent/child/next_sibling). The window manager owns the top-level roots.

All objects have properties (visible, enabled, opacity, rect, z, style, id, name).

---

### Windowing model

Window is just another GuiObject, but:

participates in z-order and focus stacks,

has bounds, flags, children, style,

can be nested inside other windows up to max_window_depth.

Automatic placement & sizing if rect is zero/unspecified: the manager computes a grid/tiling or cascading positions per level.

Overrides: any per-instance GuiWindowDesc property beats defaults.

---

### Styles

A GuiStyle is a collection of numeric/color/text metrics with variants for states (normal/hover/focus/disabled).

The active style is a pointer stored in the manager; swap at runtime to restyle everything.

Objects can hold override pointers (or specific fields) to shadow manager style.

---

### Event & frame flow

Input: SDL events → gui_manager_handle_sdl_event.

Update: timers/animations → gui_manager_update.

Layout: dirty/layout passes run lazily as needed.

Render: batched draw commands to SDL (or your GL backend).

