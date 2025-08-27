#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "gui_window.h"  // <- include our tiny API
#include "gui_style.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static GuiWindow* g_window = NULL;  // <- our one GUI window

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("GUI main", 640, 480, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // create a simple GUI window taking a portion of the SDL window
    GuiWindowDesc desc = {
        .rect  = (GuiRect){ 50, 50, 300, 220 },
        .flags = GUI_WINDOW_VISIBLE|GUI_WINDOW_CLOSABLE|GUI_WINDOW_RESIZABLE,
        .title = "Hello GUI",
        .user_level = 0,
    };
    g_window = gui_window_create(&desc);
    if (!g_window) {
        SDL_Log("Couldn't create GuiWindow");
        return SDL_APP_FAILURE;
    }

    static GuiStyle style = {0};
    style = gui_style_default_dark();        // or gui_style_default_light()
    g_window->base.style = &style;

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    // (optional) forward events to the window’s event handler
    if (g_window && g_window->base.vtbl->event) {
        g_window->base.vtbl->event(&g_window->base, (const void*)event);
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    // clear background
    SDL_SetRenderDrawColor(renderer, 25, 25, 30, 255);
    SDL_RenderClear(renderer);

    // render our one GUI window
    if (g_window && g_window->base.vtbl->render) {
        g_window->base.vtbl->render(&g_window->base, renderer);
    }

    // show it
    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    if (g_window) {
        g_window->base.vtbl->destroy(&g_window->base);
        g_window = NULL;
    }
    // SDL cleans up window/renderer automatically
}
