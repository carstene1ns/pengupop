
#ifndef GFX_H
#define GFX_H

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

extern SDL_Surface* screen;
extern SDL_Surface* logo;
extern SDL_Surface* background;

extern bool fullscreen;

extern void GFX_init();
extern void GFX_deinit();

extern SDL_Surface * GFX_CreateRGBSurface(int width, int height);

extern void GFX_ToggleFullScreen();

extern void GFX_UpdateScreen();

#endif
