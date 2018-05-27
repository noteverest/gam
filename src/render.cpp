/*
 * render.h
 * handles OpenGL rendering
 */

#include <GL/glew.h>
//#define FREEGLUT_STATIC
#include <GL/freeglut.h>

#include "render.hpp"
#include "file_io.hpp"

#include <cstdlib>
#include <string>


// GLSL program
GLuint program;

// Triangle vertices or something
GLint attribute_coord2d;

/**
 * Display compilation errors from the OpenGL shader compiler
 */
void printLog( GLuint object )
{
    GLint log_length = 0;
    if( glIsShader( object ) )
    {
        glGetShaderiv( object, GL_INFO_LOG_LENGTH, &log_length );
    }
    else if( glIsProgram( object ) )
    {
        glGetProgramiv( object, GL_INFO_LOG_LENGTH, &log_length );
    }
    else
    {
        fprintf( stderr, "printLog: Not a shader or program\n" );
    }

    char* log = new char[log_length];

    if( glIsShader( object ) )
    {
        glGetShaderInfoLog( object, log_length, NULL, log );
    }
    else if( glIsProgram( object ) )
    {
        glGetProgramInfoLog( object, log_length, NULL, log );
    }

    fprintf( stderr, log );
    delete log;
}

bool initResources( void )
{
    GLint compile_ok = GL_FALSE;
    GLint link_ok = GL_FALSE;
    uint64_t fsize = 0;

    // Compile vertex shader
    GLuint vs = glCreateShader( GL_VERTEX_SHADER );
    // TODO: paths not multiplatform
    char *vs_source = loadFile( "..\\src\\minimal.vert", fsize );
    //printf( "minimal.vert:\n%s", vs_source );
    glShaderSource( vs, 1, &vs_source, NULL );
    glCompileShader( vs );
    glGetShaderiv( vs, GL_COMPILE_STATUS, &compile_ok );
    delete vs_source;
    if( !compile_ok )
    {
        fprintf( stderr, "Error in vertex shader\n" );
        printLog( vs );
        return false;
    }

    // compile fragment shader
    GLuint fs = glCreateShader( GL_FRAGMENT_SHADER );
    // TODO: paths not multiplatform
    char *fs_source = loadFile( "..\\src\\minimal.frag", fsize );
    //printf( "minimal.frag:\n%s", fs_source );
    glShaderSource( fs, 1, &fs_source, NULL );
    glCompileShader( fs );
    glGetShaderiv( fs, GL_COMPILE_STATUS, &compile_ok );
    delete fs_source;
    if( !compile_ok )
    {
        fprintf( stderr, "Error in fragment shader\n" );
        printLog( fs );
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
        fprintf( stderr, "Error in glLinkProgram\n" );
        printLog( program );
        return false;
    }

    // pass triangle vertices to vertex shader
    const char* attribute_name = "coord2d";
    attribute_coord2d = glGetAttribLocation( program, attribute_name );
    if( attribute_coord2d == -1 )
    {
        fprintf( stderr, "Could not bind attribute %s\n", attribute_name );
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
        0.0f,    0.8f,
        -0.8f,   -0.8f,
        0.8f,    -0.8f,
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
        printf( "glewInit failed, aborting.\n" );
        exit( 1 );
    }
    printf( "Status: Using GLEW %s\n", glewGetString( GLEW_VERSION ) );
    printf( "OpenGL version %s supported\n", glGetString( GL_VERSION ) );

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

