#include "../basedef.hpp"
#include "../i_base.hpp"
#include "../i_data_stack.hpp"
#include <emscripten.h>

/*


*/

class RtpcContainer {

};

bool read_file(RtpcContainer & rtpc, DecaBufferFile & f)
{
    return false;
}

extern "C" {

EMSCRIPTEN_KEEPALIVE
bool process_rtpc(c8 const* buffer, u64 buffer_sz)
{
    RtpcContainer rtpc;
    DecaBufferFile f(buffer, buffer + buffer_sz);
    return read_file(rtpc, f);
}

}