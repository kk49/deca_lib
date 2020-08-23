#include "defs.hpp"
#include <emscripten.h>


extern "C" {
    s64 timing_test(s64 v);
}

extern "C" {
EMSCRIPTEN_KEEPALIVE
    void timing_test_run() {
        for(size_t i = 0; i < 2048; ++i)
            timing_test(i);
    }
}