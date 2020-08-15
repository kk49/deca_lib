#pragma once
#include "basedef.hpp"

FileHnd file_open(StringRef const& name, StringRef const& mode);
void file_close(FileHnd const hnd);

void xml_w_element_begin(FileHnd const hnd, StringRef const & name);
void xml_w_element_end(FileHnd const hnd, StringRef const & name);
void xml_w_attr_set(FileHnd const hnd, StringRef const& id, StringRef const& value);
void xml_w_value_set(FileHnd const hnd, StringRef const& value);
void xml_w_comment(FileHnd const hdn, StringRef const& value);

void xml_r_element_begin(FileHnd const hnd, StringRef const & name);
void xml_r_element_end(FileHnd const hnd, StringRef const & name);
void xml_r_attr(FileHnd const hnd, StringRef const& id, StringRef const& value);
void xml_r_value(FileHnd const hnd, StringRef const& value);
void xml_r_comment(FileHnd const hdn, StringRef const& value);

