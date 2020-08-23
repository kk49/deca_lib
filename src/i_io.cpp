#include "i_io.hpp"

#include <emscripten.h>
#include <vector>


// input buffer management
std::vector<c8> buffer_in;


extern "C" {

EMSCRIPTEN_KEEPALIVE
c8 *alloc_bin(u64 sz) {
    if (sz > buffer_in.size()) {
        buffer_in.resize(sz);
    }
    return buffer_in.data();
}

}

c8 const* str_ptr(FileStringRef & str) { return str.str_ptr(); }
size_t str_sz(FileStringRef & str) { return str.str_sz(); }
std::string to_string(FileStringRef & str) { str.prep_buffer(); return str.buffer_; };

std::ostream wasm_out{new WasmStreamBuf("stdout", false)};
std::ostream wasm_err{new WasmStreamBuf("stderr", false)};
std::istream wasm_in{new WasmStreamBuf("stdin", false)};

