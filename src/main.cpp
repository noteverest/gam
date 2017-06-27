/*
 * triangle
 */

#include <GL/glew.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>

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

void mainLoop( SDL_Window* window )
{
        
}

int main(int argc, char* argv[])
{
    // do glut init
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );
    glutInitWindowSize( 800, 600 ); // TODO
    glutCreateWindow( "Triangle Test" );

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

    init();
    initShaders();

    // finish glut setup
    glutDisplayFunc( render );
    glutReshapeFunc( reshape );   
    glutMainLoop();
    return 0;

}

