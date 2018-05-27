/**
 * file_io.hpp
 */

#include <cstdint>
#include <cstdlib>

char* loadFile( const char *fname, size_t &fsize );

uint64_t getFilesize( const char *fname );

