#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>

#include "gui_manager.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
// static GuiWindow* g_window = NULL;  // <- our one GUI window
static GuiManager* gm = NULL;

static const char *event_type_to_string(Uint32 type);
void log_sdl_event(const SDL_Event *e);

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    if (!SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear"))
        return SDL_APP_FAILURE;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("GUI main", 640, 480, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_SetWindowResizable(window, true)) {
        SDL_Log("Couldn't make window resizable: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    gm = gui_create(renderer);
    gui_set_tiling(gm, GUI_TILING_GRID);
    // GuiStyle gs =  gui_style_default_dark();
    // gs.border_radius = 0.0f;
    // gs.padding = 0.0f;
    // gs.border_width = 0.0f;
    // gui_set_style(gm, &gs);

    // gui_defaults_add_window_flags(gm, GUI_WINDOW_MOVABLE | GUI_WINDOW_RESIZABLE);
    // gui_defaults_clear_window_flags(gm, GUI_WINDOW_AUTO_POS | GUI_WINDOW_AUTO_SIZE);
    // gui_set_style(gm, &(GuiStyle){ .dp=1, .border_width=1, .border_radius=6, /* ... or use gui_style_default_dark() */ });

    // create
    GuiStyle custom_style =  gui_style_default_dark();
    custom_style.border_radius = 0.0f;
    custom_style.padding = 0.0f;
    custom_style.margin = 1.0f;
    custom_style.border_width = 0.0f;
    // gui_set_style(gm, &gs);
    GuiWindow* main_window  = gui_window_create(gm, NULL, NULL, NULL, &custom_style);
    custom_style.border_width = 1.0f;
    custom_style.win_bg = (GuiColor){ 0.9f, 0.9f, 0.9f, 1.0f };
    GuiWindow* canvas  = gui_window_create(gm, main_window, NULL, NULL, &custom_style);
    GuiWindow* tools  = gui_window_create(gm, main_window, "Tools", NULL, NULL);
    GuiWindow* brushes  = gui_window_create(gm, tools, "Brushes", NULL, NULL);
    GuiWindow* layers  = gui_window_create(gm, tools, "Layers", NULL, NULL);
    GuiWindow* palette  = gui_window_create(gm, tools, "Palette", NULL, NULL);
    // for (int i = 0; i < 6; ++i) {
    //     GuiWindow* tmp1 = gui_window_create(gm, NULL, "Root", NULL);
    //     GuiWindow* tmp2 = gui_window_create(gm, tmp1, "[Root].child 1", NULL);
    //     GuiWindow* tmp3 = gui_window_create(gm, tmp1, NULL, NULL);
    //     GuiWindow* tmp4 = gui_window_create(gm, tmp1, "[Root].child 3", NULL);
    //     GuiWindow* tmp5 = gui_window_create(gm, tmp1, "[Root].child 4", NULL);
    //     GuiWindow* tmp6 = gui_window_create(gm, tmp2, NULL, NULL);
    //     GuiWindow* tmp7 = gui_window_create(gm, tmp5, "[child 4]child 2", NULL);
    //     tmp1->flags = tmp1->flags | GUI_WINDOW_CLOSABLE;
    //     tmp6->flags = tmp6->flags | GUI_WINDOW_CLOSABLE;
    //     // { // Style tmp1
    //     //     GuiStyle* style = (GuiStyle*)tmp1->base.style;
    //     //     style->win_bg = (GuiColor){ 0.9f, 0.2f, 0.4f, 1.0f };
    //     //     style->margin = 5.0f;
    //     // }
    //     { // Style tmp4
    //     tmp4->flags = tmp4->flags | GUI_WINDOW_CLOSABLE | GUI_WINDOW_RESIZABLE;
    //     }
    //     { // Style tmp2
    //         GuiStyle* style = (GuiStyle*)tmp2->base.style;
    //         style->border_width = 3.0f;
    //         style->border_radius = 15.0f;
    //         style->margin = 5.0f;
    //     }
    //     // gui_window_destroy(gm, tmp5);
    // }

    // GuiWindow* tools = gui_window_create(gm, root, "Tools", NULL);
    // GuiWindow* tools2 = gui_window_create(gm, root, "Tools", NULL);
    // GuiWindow* tools3 = gui_window_create(gm, root, "Tools", NULL);
    // GuiWindow* tools4 = gui_window_create(gm, root, "Tools", NULL);

    // in SDL callbacks
    // SDL_AppEvent:    gui_handle_sdl_event(gm, &event);
    // SDL_AppIterate:  gui_update(gm, dt_ms); gui_render(gm);

    // // create a simple GUI window taking a portion of the SDL window
    // GuiWindowDesc desc = {
    //     .rect  = (GuiRect){ 50, 50, 300, 220 },
    //     .flags = GUI_WINDOW_VISIBLE|GUI_WINDOW_CLOSABLE|GUI_WINDOW_RESIZABLE,
    //     .title = "Hello GUI",
    //     .user_level = 0,
    // };
    // g_window = window_create(&desc);
    // if (!g_window) {
    //     SDL_Log("Couldn't create GuiWindow");
    //     return SDL_APP_FAILURE;
    // }

    // static GuiStyle style = {0};
    // style = gui_style_default_dark();        // or gui_style_default_light()
    // g_window->base.style = &style;

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    // // (optional) forward events to the window’s event handler
    // if (g_window && g_window->base.vtbl->event) {
    //     g_window->base.vtbl->event(&g_window->base, (const void*)event);
    // }
    log_sdl_event(event);
    gui_handle_sdl_event(gm, event);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    // clear background
    SDL_SetRenderDrawColor(renderer, 25, 25, 30, 255);
    SDL_RenderClear(renderer);
    if (gm) {
        gui_update(gm, 0.0);

        gui_render(gm);

        // render our one GUI window
        // if (g_window && g_window->base.vtbl->render) {
        //     g_window->base.vtbl->render(&g_window->base, renderer);
        // }

        // show it
    }
    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    // if (g_window) {
    //     g_window->base.vtbl->destroy(&g_window->base);
    //     g_window = NULL;
    // }
    // SDL cleans up window/renderer automatically
}

/* ---------- LOG UTILS ----------- */

static const char *event_type_to_string(Uint32 type) {
    switch (type) {
        case SDL_EVENT_QUIT: return "QUIT";
        case SDL_EVENT_KEY_DOWN: return "KEY_DOWN";
        case SDL_EVENT_KEY_UP: return "KEY_UP";
        case SDL_EVENT_TEXT_INPUT: return "TEXT_INPUT";
        case SDL_EVENT_MOUSE_MOTION: return "MOUSE_MOTION";
        case SDL_EVENT_MOUSE_BUTTON_DOWN: return "MOUSE_BUTTON_DOWN";
        case SDL_EVENT_MOUSE_BUTTON_UP: return "MOUSE_BUTTON_UP";
        case SDL_EVENT_MOUSE_WHEEL: return "MOUSE_WHEEL";
        case SDL_EVENT_WINDOW_SHOWN: return "WINDOW_SHOWN";
        case SDL_EVENT_WINDOW_HIDDEN: return "WINDOW_HIDDEN";
        case SDL_EVENT_WINDOW_EXPOSED: return "WINDOW_EXPOSED";
        case SDL_EVENT_WINDOW_MOVED: return "WINDOW_MOVED";
        case SDL_EVENT_WINDOW_RESIZED: return "WINDOW_RESIZED";
        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: return "WINDOW_PIXEL_SIZE_CHANGED";
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED: return "WINDOW_CLOSE_REQUESTED";
        case SDL_EVENT_WINDOW_FOCUS_GAINED: return "WINDOW_FOCUS_GAINED";
        case SDL_EVENT_WINDOW_FOCUS_LOST: return "WINDOW_FOCUS_LOST";
        case SDL_EVENT_WINDOW_HIT_TEST: return "WINDOW_HIT_TEST";
        case SDL_EVENT_GAMEPAD_ADDED: return "GAMEPAD_ADDED";
        case SDL_EVENT_GAMEPAD_REMOVED: return "GAMEPAD_REMOVED";
        case SDL_EVENT_GAMEPAD_BUTTON_DOWN: return "GAMEPAD_BUTTON_DOWN";
        case SDL_EVENT_GAMEPAD_BUTTON_UP: return "GAMEPAD_BUTTON_UP";
        case SDL_EVENT_GAMEPAD_AXIS_MOTION: return "GAMEPAD_AXIS_MOTION";
        default: return "UNKNOWN";
    }
}

void log_sdl_event(const SDL_Event *e) {
    printf("Event %-20s (type=%u)\n", event_type_to_string(e->type), e->type);

    switch (e->type) {
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
        printf("  key.sym = %s (%d), scancode=%d, state=%s, repeat=%d\n",
               SDL_GetKeyName(e->key.key),
               e->key.raw,
               e->key.scancode,
               (e->type == SDL_EVENT_KEY_DOWN ? "DOWN" : "UP"),
               e->key.repeat);
        break;

    case SDL_EVENT_TEXT_INPUT:
        printf("  text: \"%s\"\n", e->text.text);
        break;

    case SDL_EVENT_MOUSE_MOTION:
        printf("  mouse motion: x=%d y=%d (rel %d,%d), state=0x%x\n",
               (int)e->motion.x, (int)e->motion.y,
               (int)e->motion.xrel, (int)e->motion.yrel,
               e->motion.state);
        break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
        printf("  mouse button %d %s at (%d,%d), clicks=%d\n",
               e->button.button,
               (e->type == SDL_EVENT_MOUSE_BUTTON_DOWN ? "DOWN" : "UP"),
               e->button.x, e->button.y,
               e->button.clicks);
        break;

    case SDL_EVENT_MOUSE_WHEEL:
        printf("  mouse wheel: x=%d y=%d, precise=(%.2f, %.2f), direction=%d\n",
               e->wheel.x, e->wheel.y,
               e->wheel.mouse_x, e->wheel.mouse_y,
               e->wheel.direction);
        break;

    case SDL_EVENT_WINDOW_MOVED:
        printf("  window moved: (%d,%d)\n", e->window.data1, e->window.data2);
        break;

    case SDL_EVENT_WINDOW_RESIZED:
    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
        printf("  window size: (%d,%d)\n", e->window.data1, e->window.data2);
        break;

    case SDL_EVENT_GAMEPAD_AXIS_MOTION:
        printf("  gamepad %d axis %d value=%d\n",
               e->gaxis.which, e->gaxis.axis, e->gaxis.value);
        break;

    case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
    case SDL_EVENT_GAMEPAD_BUTTON_UP:
        printf("  gamepad %d button %d %s\n",
               e->gbutton.which,
               e->gbutton.button,
               (e->type == SDL_EVENT_GAMEPAD_BUTTON_DOWN ? "DOWN" : "UP"));
        break;

    default:
        // Print raw memory for unknown/unhandled events
        printf("  [no detailed printer for this type]\n");
        break;
    }
}
