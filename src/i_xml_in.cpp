#include "i_xml_in.hpp"
#include <emscripten.h>
#include <iostream>
#include <cstdio>

extern "C" {

EMSCRIPTEN_KEEPALIVE
void xml_process(FileHnd hnd_input, FileHnd hnd_output)
{
    wasm_out << "xml_process(" << hnd_input << ", " << hnd_output << ")" << std::endl;
}

}