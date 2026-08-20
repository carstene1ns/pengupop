
#include <SDL3/SDL.h>

#include "gfx.h"
#include "error.h"

bool fullscreen = true; /* XXX: Change to true before release */
SDL_Surface *screen = NULL;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;

void GFX_init() {
	screen = GFX_CreateRGBSurface(SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE;
	if(fullscreen)
	{
		flags |= SDL_WINDOW_FULLSCREEN;
	}
	window = SDL_CreateWindow("Pengupop", SCREEN_WIDTH, SCREEN_HEIGHT, flags);
	renderer = SDL_CreateRenderer(window, NULL);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
		SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

	if(!window || !screen)
		fatal_error("Failed to create window: %s", SDL_GetError());

	if(!renderer || !texture)
		fatal_error("Failed to create renderer: %s", SDL_GetError());

	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_LINEAR);
	SDL_SetRenderLogicalPresentation(renderer, SCREEN_WIDTH, SCREEN_HEIGHT,
		SDL_LOGICAL_PRESENTATION_LETTERBOX);

	SDL_HideCursor();
}

void GFX_deinit() {
	SDL_ShowCursor();

	SDL_DestroySurface(screen);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

SDL_Surface * GFX_CreateRGBSurface(int width, int height) {
	return SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32);
}

void GFX_ToggleFullScreen() {
	fullscreen = !fullscreen;
	SDL_SetWindowFullscreen(window, fullscreen);
}

void GFX_UpdateScreen() {
	SDL_RenderClear(renderer);
	SDL_UpdateTexture(texture, NULL, screen->pixels, screen->pitch);
	SDL_RenderTexture(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}
