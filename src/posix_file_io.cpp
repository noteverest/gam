/*
 * posix_file_io.cpp
 */

#include "file_io.hpp"

#include <cstdint>
#include <cstdio>
#include <cstring>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


/*
 * loadFile with C-style file IO
 * free memory after use
 */
char* loadFile( const char *fname, uint64_t &fsize )
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
 * Returns the filesize in bytes
 */
uint64_t getFilesize( const char *fname )
{
    struct stat statbuf;
    memset( &statbuf, 0, sizeof( statbuf ) );
    if( !stat( fname, &statbuf ) )
    {
        // TODO: handle failure
    }
    return (uint64_t)statbuf.st_size;
}

