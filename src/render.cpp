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


// GLSL program
GLuint program;

// triangle vertices or something
GLint attribute_coord2d;

bool initResources( void )
{
    GLint compile_ok = GL_FALSE;
    GLint link_ok = GL_FALSE;

    // compile vertex shader
    GLuint vs = glCreateShader( GL_VERTEX_SHADER );
    const char *vs_source =
        "#version 120\n"
        "attribute vec2 coord2d;"
        "void main( void ) {"
        "   gl_Position = vec4( coord2d, 0.0, 1.0 );"
        "}"; // TODO this is horrible
    glShaderSource( vs, 1, &vs_source, NULL );
    glCompileShader( vs );
    glGetShaderiv( vs, GL_COMPILE_STATUS, &compile_ok );
    if( !compile_ok )
    {
        std::cerr << "Error in vertex shader" << std::endl;
        return false;
    }

    // compile fragment shader
    GLuint fs = glCreateShader( GL_FRAGMENT_SHADER );
    const char *fs_source = 
        "#version 120\n"
        "void main( void ) {"
        "   gl_FragColor[0] = 0.0;"
        "   gl_FragColor[1] = 0.0;"
        "   gl_FragColor[2] = 1.0;"
        "}"; // TODO this is horrible
    glShaderSource( fs, 1, &fs_source, NULL );
    glCompileShader( fs );
    glGetShaderiv( fs, GL_COMPILE_STATUS, &compile_ok );
    if( !compile_ok )
    {
        std::cerr << "Error in fragment shader" << std::endl;
        return false;
    }

    // compile GLSL program
    program = glCreateProgram();
    glAttachShader( program, vs );
    glAttachShader( program, fs );
    glLinkProgram( program );
    glGetProgramiv( program, GL_LINK_STATUS, &link_ok );
    if( !link_ok )
    {
        std::cerr << "Error in glLinkProgram" << std::endl;
        return false;
    }

    // pass triangle vertices to vertex shader
    const char* attribute_name = "coord2d";
    attribute_coord2d = glGetAttribLocation( program, attribute_name );
    if( attribute_coord2d == -1 )
    {
        std::cerr << "Could not bind attribute " << attribute_name << std::endl;
        return false;
    }

    return true;
}


void display()
{
    // clear background to white
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
    glClear( GL_COLOR_BUFFER_BIT );

    glUseProgram( program );
    glEnableVertexAttribArray( attribute_coord2d );
    GLfloat triangle_vertices[] =
    {
        0.0,    0.8,
        -0.8,   -0.8,
        0.8,    -0.8,
    };

    // describe our vertices array to OpenGL
    glVertexAttribPointer(
        attribute_coord2d,  // attribute
        2,                  // number of elements per vertex, here (x,y)
        GL_FLOAT,           // the type of each element
        GL_FALSE,           // take our values as-is
        0,                  // no extra data between each position
        triangle_vertices   // pointer to the C array
    );

    // push each element in buffer_vertices to the vertex shader
    glDrawArrays( GL_TRIANGLES, 0, 3 );

    glDisableVertexAttribArray( attribute_coord2d );

    // Display the result
    glutSwapBuffers();
}


void reshape( int width, int height )
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
        std::cout << "glewInit failed, aborting." << std::endl;
        exit( 1 );
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "OpenGL version " << glGetString(GL_VERSION) << " supported" << std::endl;

    // general init
    initResources();
    //init();
    //initShaders();

    // setup freeglut callbacks
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    
    // start loop   
    glutMainLoop();

    return 0;
}

