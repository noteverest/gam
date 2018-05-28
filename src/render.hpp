/*
 * render.h
 * handles rendering with OpenGL
 */

#include <GL/glew.h>
#include <GL/freeglut.h>

GLuint createShader( const char *filename, GLenum type );

void display();

bool initResources( void );

int renderMain( int argc, char* argv[] );

void reshape( int width, int height );
