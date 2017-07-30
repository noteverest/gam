/*
 * file_io.cpp
 * very basic file io
 */

#include "file_io.hpp"

#include <cstdio>
#include <cstring>

// TODO: POSIX only - stat
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


// loadFile with C-style file IO
// allocates memory - so need to free after use
char* loadFile( const char *fname, size_t &fsize )
{
    FILE *fp;
    char *fbuf;
    size_t amtRead;

    fsize = getFilesize( fname );
    fbuf = (char *)malloc( fsize + 1 );
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


// Returns the filesize in bytes
// TODO: POSIX only - stat
size_t getFilesize( const char *fname )
{
    struct stat statbuf;
    memset( &statbuf, 0, sizeof( statbuf ) );
    if( !stat( fname, &statbuf ) )
    {
        // TODO: handle failure
    }
    return (size_t)statbuf.st_size;
}

