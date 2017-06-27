/*
 * render.h
 * handles OpenGL rendering
 */

#include <GL/glew.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>

#include "render.h"

#include <cstdlib>
#include <iostream>
#include <string>


bool initResources( void )
{
    // TODO
    return true;
}


void render( SDL_Window* window )
{
    // TODO
}


void freeResources()
{
    // TODO
}

int renderMain(int argc, char* argv[])
{
    // init freeglut
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );
    glutInitWindowSize( 800, 600 ); // TODO
    glutCreateWindow( "Triangle Test" );

    // init glew
    glewInit();
    GLenum err = glewInit();
    if( GLEW_OK != err )
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        cout << "glewInit failed, aborting." << endl;
        exit( 1 );
    }
    cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << endl;
    cout << "OpenGL version " << glGetString(GL_VERSION) << " supported" << endl;

    // general init
    init();
    initShaders();

    // setup freeglut callbacks
    glutDisplayFunc( render );
    glutReshapeFunc( reshape );
    
    // start loop   
    glutMainLoop();

    return 0;
}

