//***************************//
//*Template by pulgamecanica*//
//***************************//

#include "BSP_TESTER.hpp"
#include "bsp.hpp"
#include "SDL.h"   /* All SDL App's need this */
#include <stdio.h>
#include <cmath>

#define WINDOW_W 800
#define WINDOW_H 600

void	draw_triangle(SDL_Renderer *renderer, Point a, Point b, Point c) {
	SDL_SetRenderDrawColor(renderer, rand(), rand(), rand(), rand());
	for (int i = 0; i < WINDOW_W; ++i) {
		for (int j = 0; j < WINDOW_H; ++j) {
			Point p(i, j);
			if (bsp(a, b, c, p))
        		SDL_RenderDrawPoint(renderer, i, j);
		}
	}
}

void	draw_point(Point p, int size, SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, rand(), rand(), rand());
	int x, y;
	x = p.get_x().toInt();
	y = p.get_y().toInt();
	for (int i = x - size; i < x + size; ++i)
		for (int j = y - size; j < y + size; ++j)
			if ((pow((i - x), 2) + pow((j - y), 2)) <= pow(size, 2))
        		SDL_RenderDrawPoint(renderer, i, j);
}

int	main(void)
{
	srand(time(0));
	int xMouse, yMouse;
	Point a(110, 20);
	Point b(200.3, 22.4);
	Point c(40.4, 134);
	Point d(rand() % WINDOW_W, rand() % WINDOW_H);
	Point e_(rand() % WINDOW_W, rand() % WINDOW_H);
	Point f(rand() % WINDOW_W, rand() % WINDOW_H);
	Point g(rand() % WINDOW_W, rand() % WINDOW_H);
	Point h(rand() % WINDOW_W, rand() % WINDOW_H);
	Point i(rand() % WINDOW_W, rand() % WINDOW_H);
	std::cout << a << std::endl;
	std::cout << b << std::endl;
	std::cout << c << std::endl;
	{	
	    SDL_Window *window;
	    SDL_Window *window_stats;

	    SDL_Init(SDL_INIT_EVERYTHING);              // Initialize SDL2

	    // Create an application window with the following settings:
	    window = SDL_CreateWindow(
	        "BSP Tester",                  // window title
	        SDL_WINDOWPOS_CENTERED,           // initial x position
	        SDL_WINDOWPOS_CENTERED,           // initial y position
	        WINDOW_W,								// width, in pixels
	        WINDOW_H,								// height, in pixels
	        SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN                  // flags - see below
	    );
		window_stats = SDL_CreateWindow(
	        "Stats",                  // window title
	        SDL_WINDOWPOS_CENTERED,           // initial x position
	        SDL_WINDOWPOS_CENTERED,           // initial y position
	        150,								// width, in pixels
	        150,								// height, in pixels
	        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN                  // flags - see below
	    );
	    // Check that the window was successfully created
	    if (window == NULL) {
	        // In the case that the window could not be made...
	        printf("Could not create window: %s\n", SDL_GetError());
	        return 1;
	    }

	    SDL_Renderer *renderer = SDL_CreateRenderer(window, -100, 0);
	    SDL_Renderer *mouse_target = SDL_CreateRenderer(window_stats, -1, 0);

	    SDL_SetRenderDrawColor(renderer, 50, 97, 200, 144);
	    SDL_RenderClear(renderer);

		draw_triangle(renderer, a, b, c);
		draw_triangle(renderer, d, e_, f);
		draw_triangle(renderer, g, h, i);

    	draw_point(a, 2, renderer);
    	draw_point(b, 2, renderer);
    	draw_point(c, 2, renderer);
    	draw_point(d, 2, renderer);
    	draw_point(e_, 2, renderer);
    	draw_point(f, 2, renderer);
    	draw_point(g, 2, renderer);
    	draw_point(h, 2, renderer);
    	draw_point(i, 2, renderer);

    	SDL_RenderPresent(renderer);

	    // The window is open: could enter program loop here (see SDL_PollEvent())

	    SDL_Event e;
		bool quit = false;
		while (!quit){
		    while (SDL_PollEvent(&e)){
		        if (e.type == SDL_QUIT){
		            quit = true;
		        }
		        if (e.type == SDL_KEYDOWN){
		        	 /* Code to set a yellow pixel at the center of the screen */
		        	std::cout << "key pressed!" << std::endl;
		        	// for (int i = 0; i < WINDOW_H; ++i)
        			// 	SDL_RenderDrawPoint(renderer, i, i);
		        }
		        if (e.type == SDL_MOUSEMOTION){
		            SDL_GetMouseState(&xMouse,&yMouse);
		            Point mouse_on(xMouse, yMouse);
		            if (bsp(a, b, c, mouse_on))
		            	SDL_SetRenderDrawColor(mouse_target, 0, 255, 0, 0);
		            else if (bsp(g, h, i, mouse_on))
		            	SDL_SetRenderDrawColor(mouse_target, 0, 255, 0, 0);
		            else if (bsp(d, e_, f, mouse_on))
		            	SDL_SetRenderDrawColor(mouse_target, 0, 255, 0, 0);
		            else
		            	SDL_SetRenderDrawColor(mouse_target, 255, 0, 0, 0);
		            SDL_RenderClear(mouse_target);
		        }
		        SDL_RenderPresent(mouse_target);
		    }
		}

	    // Close and destroy the window & Quit
	    SDL_DestroyRenderer(renderer);
	    SDL_DestroyRenderer(mouse_target);
    	SDL_DestroyWindow(window);
    	SDL_DestroyWindow(window_stats);
	    SDL_Quit();
	}
	return (0);
}
