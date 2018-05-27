/**
 * file_io.hpp
 */

#include <cstdint>
#include <cstdlib>

char* loadFile( const char *fname, uint64_t &fsize );

uint64_t getFilesize( const char *fname );

