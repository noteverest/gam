/*
 * render.h
 * handles rendering with OpenGL
 */

bool initResources( void );

void render( SDL_Window* window );

void freeResources();

int renderMain(int argc, char* argv[]);

