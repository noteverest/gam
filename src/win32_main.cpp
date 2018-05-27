/*
 * win32_main.cpp
 */

#include <cstdlib>
#include <cstdio>

#include "Windows.h"
#include "Shellapi.h"

#include "render.hpp"

int WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow )
{
    LPWSTR cmd_line_w;
    LPWSTR* argv_w;
    int argc;
    char** argv;
    int argv_sz;

    // Need to procure argc and argv from the Windows API
    cmd_line_w = GetCommandLineW();
    argv_w = CommandLineToArgvW( cmd_line_w, &argc );

    argv = new char*[argc];
    for( int i = 0; i < argc; i++ )
    {
        // Get length of converted string
        argv_sz = WideCharToMultiByte(
            CP_UTF8,
            0,
            argv_w[i],
            -1,
            NULL,
            0,
            NULL,
            NULL );
        // Convert
        argv[i] = new char[argv_sz];
        WideCharToMultiByte(
            CP_UTF8,
            0,
            argv_w[i],
            -1,
            argv[i],
            argv_sz,
            NULL,
            NULL );
    }

    // Start rendering
    renderMain( argc, argv );

    return 0;
}

