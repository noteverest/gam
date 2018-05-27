/*
 * win32_file_io.cpp
 */

#include "file_io.hpp"

#include <cstdint>
#include <cstdio>
#include <cstring>

#include <Windows.h>


/*
 * Load a file into a buffer.
 * Free memory after use.
 */
char* loadFile( const char *fname, size_t &fsize )
{
    FILE *fp;
    char *fbuf;
    size_t amtRead;

    fsize = getFilesize( fname );
    fbuf = new char[fsize + 1];
    memset( fbuf, 0, fsize + 1 );

    fp = fopen( fname, "r" );
    if( !fp )
    {
        fprintf( stderr, "loadFile: failed to open %s\n", fname );
        return NULL;
    }
    amtRead = fread( fbuf, 1, fsize, fp );
    if( amtRead != fsize )
    {
        fprintf( stderr, "loadFile: expected %lu bytes, read %lu bytes\n",
             (unsigned long)fsize, (unsigned long)amtRead );
    }
    fclose( fp );

    return fbuf;
}


/*
 * Return filesize in bytes.
 */
uint64_t getFilesize( const char *fname )
{
    HANDLE file;
    PLARGE_INTEGER file_size;
    LPWSTR fname_w;
    int fname_w_sz;
    int fname_sz;

    // Convert filename to UTF-16
    fname_sz = strlen( fname ) + 1;
    fname_w_sz = MultiByteToWideChar(
        CP_UTF8,
        0,
        fname,
        fname_sz,
        NULL,
        0 );
    fname_w = new WCHAR[fname_sz];
    MultiByteToWideChar(
        CP_UTF8,
        0,
        fname,
        fname_sz,
        fname_w,
        fname_w_sz );

    // Open file
    file = CreateFileW(
        fname_w,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL );

    GetFileSizeEx( file, file_size );

    return file_size->QuadPart;
}

