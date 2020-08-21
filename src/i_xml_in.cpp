#include "i_xml_in.hpp"
#include <emscripten.h>
#include <iostream>
#include <cstdio>

extern "C" {

EMSCRIPTEN_KEEPALIVE
void xml_process(FileHnd hnd_input, FileHnd hnd_output)
{

}

EMSCRIPTEN_KEEPALIVE
FileHnd xml_r_open() {
    wasm_out << "xml_r_open" << std::endl;
    return XmlProcessor::instance().file_open();
}

EMSCRIPTEN_KEEPALIVE
void xml_r_close(FileHnd const hnd) {
    XmlProcessor::instance().file_close(hnd);
}

EMSCRIPTEN_KEEPALIVE
void xml_r_element_begin(FileHnd const hnd, StringRef const &name) {
    XmlProcessor::instance().element_begin(hnd, name);
}

EMSCRIPTEN_KEEPALIVE
void xml_r_element_end(FileHnd const hnd, StringRef const &name) {
    XmlProcessor::instance().element_end(hnd, name);
}

EMSCRIPTEN_KEEPALIVE
void xml_r_element_close(FileHnd const hnd, StringRef const &name) {
    XmlProcessor::instance().element_close(hnd, name);
}

EMSCRIPTEN_KEEPALIVE
void xml_r_attr(FileHnd const hnd, StringRef const &id, StringRef const &value) {
    XmlProcessor::instance().attr(hnd, id, value);
}

EMSCRIPTEN_KEEPALIVE
void xml_r_value(FileHnd const hnd, StringRef const &value) {
    XmlProcessor::instance().value(hnd, value);
}

EMSCRIPTEN_KEEPALIVE
void xml_r_comment(FileHnd const hnd, StringRef const &value) {
    XmlProcessor::instance().comment(hnd, value);
}

}