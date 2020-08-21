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

std::ostream wasm_out{new WasmStreamBuf("wasm_out")};
std::ostream wasm_err{new WasmStreamBuf("wasm_err")};
std::istream wasm_in{new WasmStreamBuf("wasm_in")};

