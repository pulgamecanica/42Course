#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "gui_manager.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
// static GuiWindow* g_window = NULL;  // <- our one GUI window
static GuiManager* gm = NULL;

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
    GuiStyle gs =  gui_style_default_dark();
    gs.margin = 3.0f;
    gs.border_radius = 0.0f;
    gui_set_style(gm, &gs);

    // gui_defaults_add_window_flags(gm, GUI_WINDOW_MOVABLE | GUI_WINDOW_RESIZABLE);
    // gui_defaults_clear_window_flags(gm, GUI_WINDOW_AUTO_POS | GUI_WINDOW_AUTO_SIZE);
    // gui_set_style(gm, &(GuiStyle){ .dp=1, .border_width=1, .border_radius=6, /* ... or use gui_style_default_dark() */ });

    // create
    // GuiWindow* root  = gui_window_create(gm, NULL, "Root", NULL);
    // GuiWindow* root2  = gui_window_create(gm, NULL, "Root", NULL);
    for (int i = 0; i < 4; ++i) {
        GuiWindow* tmp1 = gui_window_create(gm, NULL, "Root", NULL);
        GuiWindow* tmp2 = gui_window_create(gm, tmp1, "Root", NULL);
        GuiWindow* tmp3 = gui_window_create(gm, tmp1, "Root", NULL);
        GuiWindow* tmp4 = gui_window_create(gm, tmp1, "Root", NULL);
        GuiWindow* tmp5 = gui_window_create(gm, tmp1, "Root", NULL);
        GuiWindow* tmp6 = gui_window_create(gm, tmp5, "Root", NULL);
        GuiWindow* tmp7 = gui_window_create(gm, tmp5, "Root", NULL);
        tmp1->flags = tmp1->flags | GUI_WINDOW_CLOSABLE;
        // { // Style tmp1
        //     GuiStyle* style = (GuiStyle*)tmp1->base.style;
        //     style->win_bg = (GuiColor){ 0.9f, 0.2f, 0.4f, 1.0f };
        //     style->margin = 5.0f;
        // }
        { // Style tmp4
        tmp4->flags = tmp4->flags | GUI_WINDOW_CLOSABLE | GUI_WINDOW_RESIZABLE;
        }
        { // Style tmp2
            GuiStyle* style = (GuiStyle*)tmp2->base.style;
            style->border_width = 3.0f;
            style->border_radius = 15.0f;
            style->margin = 5.0f;
        }
    }
    // GuiWindow* root3  = gui_window_create(gm, NULL, "Root", NULL);
    // root3->flags = root3->flags | GUI_WINDOW_CLOSABLE;

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
