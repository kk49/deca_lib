#pragma once
#include "basedef.hpp"

typedef u32 FileHnd;


// exports
extern "C" {
    c8 *alloc_bin(u64 sz);
}

// imports
extern "C" {
    FileHnd file_open(StringRef const& name, StringRef const& mode);
    void file_close(FileHnd const hnd);
}
