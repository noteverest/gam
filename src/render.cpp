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

// Attributes
GLint attribute_coord2d;
GLint attribute_v_color;

// Vertex buffer objects
GLuint vbo_triangle;
GLuint vbo_triangle_colors;


static void printLog( GLuint object );

/*
 * Compile a shader given a filename
 */
GLuint createShader( const char *filename, GLenum type )
{
    GLint compile_ok = GL_FALSE;
    GLint link_ok = GL_FALSE;
    uint64_t fsize = 0;

    const GLchar *source = loadFile( filename, fsize );
    if( NULL == source )
    {
        fprintf( stderr, "Error opening %s\n", filename );
        return 0;
    }
    GLuint shader = glCreateShader( type );
    glShaderSource( shader, 1, &source, NULL );
    delete source;

    glCompileShader( shader );
    glGetShaderiv( shader, GL_COMPILE_STATUS, &compile_ok );
    if( GL_FALSE == compile_ok )
    {
        fprintf( stderr, "Error in shader %s\n", filename );
        printLog( shader );
        glDeleteShader( shader );
        return 0;
    }

    return shader;
}


void display()
{
    // Enable alpha
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    // Clear background to white
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
    glClear( GL_COLOR_BUFFER_BIT );

    glUseProgram( program );

    // describe our vertices array to OpenGL
    glBindBuffer( GL_ARRAY_BUFFER, vbo_triangle );
    glEnableVertexAttribArray( attribute_coord2d );
    glVertexAttribPointer(
        attribute_coord2d,  // attribute
        2,                  // number of elements per vertex, here (x,y)
        GL_FLOAT,           // the type of each element
        GL_FALSE,           // take our values as-is
        0,                  // no extra data between each position
        0 );                // offset of first element

    // describe our colors array to OpenGL
    glEnableVertexAttribArray( attribute_v_color );
    glBindBuffer( GL_ARRAY_BUFFER, vbo_triangle_colors );
    glVertexAttribPointer(
        attribute_v_color,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        0 );

    // push each element in buffer_vertices to the vertex shader
    glDrawArrays( GL_TRIANGLES, 0, 3 );

    glDisableVertexAttribArray( attribute_coord2d );
    glDisableVertexAttribArray( attribute_v_color );

    // Display the result
    glutSwapBuffers();
}


bool initResources( void )
{
    GLint compile_ok = GL_FALSE;
    GLint link_ok = GL_FALSE;
    uint64_t fsize = 0;

    // Compile vertex and fragment shaders
    // TODO: paths not multiplatform
    GLuint vs = createShader( "..\\src\\minimal.vert", GL_VERTEX_SHADER );
    GLuint fs = createShader( "..\\src\\minimal.frag", GL_FRAGMENT_SHADER );
    if( 0 == vs || 0 == fs )
    {
        return false;
    }

    // Compile GLSL program
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

    // Store triangle vertices in VBO
    GLfloat triangle_vertices[] =
    {
        0.0f,    0.8f,
        -0.8f,   -0.8f,
        0.8f,    -0.8f,
    };
    glGenBuffers( 1, &vbo_triangle );
    glBindBuffer( GL_ARRAY_BUFFER, vbo_triangle );
    glBufferData( GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW );

    // Pass triangle vertices to vertex shader
    char* attribute_name = "coord2d";
    attribute_coord2d = glGetAttribLocation( program, attribute_name );
    if( attribute_coord2d == -1 )
    {
        fprintf( stderr, "Could not bind attribute %s\n", attribute_name );
        return false;
    }

    // Store triangle colors in VBO
    GLfloat triangle_colors[] =
    {
        1.0f,   1.0f,   0.0f,
        0.0f,   0.0f,   1.0f,
        1.0f,   0.0f,   0.0f,
    };
    glGenBuffers( 1, &vbo_triangle_colors );
    glBindBuffer( GL_ARRAY_BUFFER, vbo_triangle_colors );
    glBufferData( GL_ARRAY_BUFFER, sizeof(triangle_colors), triangle_colors, GL_STATIC_DRAW );

    // Pass triangle colors to vertex shader
    attribute_name = "v_color";
    attribute_v_color = glGetAttribLocation( program, attribute_name );
    if( -1 == attribute_v_color )
    {
        fprintf( stderr, "Could not bind attribute %s\n", attribute_name );
        return false;
    }

    return true;
}


int renderMain(int argc, char* argv[])
{
    // init freeglut
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );
    glutInitWindowSize( 800, 600 ); // TODO: resolution
    glutCreateWindow( "gam" );

    // init glew
    glewInit();
    GLenum err = glewInit();
    if( GLEW_OK != err )
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf( stderr, "glewInit failed, aborting.\n" );
        exit( 1 );
    }
    printf( "Status: Using GLEW %s\n", glewGetString( GLEW_VERSION ) );
    printf( "OpenGL version %s supported\n", glGetString( GL_VERSION ) );
    if( !GLEW_VERSION_2_0 )
    {
        fprintf( stderr, "Your graphic card does not support OpenGL 2.0\n" );
        exit( 1 );
    }

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


void reshape( int width, int height )
{
    // TODO
}


/*
 * Display compilation errors from the OpenGL shader compiler
 */
static void printLog( GLuint object )
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
        fprintf( stderr, "%s: Not a shader or program\n", __func__ );
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
