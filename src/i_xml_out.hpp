#pragma once
#include "defs.hpp"
#include "i_io.hpp"

// imports
extern "C" {

void xml_w_element_begin(FileHnd const hnd, c8 const * name_ptr, u32 name_sz);
void xml_w_element_end(FileHnd const hnd, c8 const * name_ptr, u32 name_sz);
void xml_w_attr_set(FileHnd const hnd, c8 const * id_ptr, u32 id_sz, c8 const * value_ptr, u32 value_sz);
void xml_w_value_set(FileHnd const hnd, c8 const * value_ptr, u32 value_sz);
void xml_w_comment(FileHnd const hdn, c8 const * value_ptr, u32 value_sz);

//void xml_r_element_begin(FileHnd const hnd,c8 const * name_ptr, u32 name_sz);
//void xml_r_element_close(FileHnd const hnd, c8 const * name_ptr, u32 name_sz);
//void xml_r_attr(FileHnd const hnd, c8 const * id_ptr, u32 id_sz, c8 const * value_ptr, u32 value_sz);
//void xml_r_value(FileHnd const hnd, c8 const * value_ptr, u32 value_sz);
//void xml_r_comment(FileHnd const hdn, c8 const * value_ptr, u32 value_sz);

}

template<typename T_>
void xml_w_element_begin(FileHnd const hnd, T_ const & name)
{
    xml_w_element_begin(hnd, str_ptr(name), str_sz(name));
}

template<typename T_>
void xml_w_element_end(FileHnd const hnd, T_ const & name)
{
    xml_w_element_end(hnd, str_ptr(name), str_sz(name));
}

template<typename T0_, typename T1_>
void xml_w_attr_set(FileHnd const hnd, T0_ const & id, T1_ const & value)
{
    xml_w_attr_set(hnd, str_ptr(id), str_sz(id), str_ptr(value), str_sz(value));
}

template<typename T_>
void xml_w_value_set(FileHnd const hnd, T_ & value)
{
    xml_w_value_set(hnd, str_ptr(value), str_sz(value));
}


template<typename T_>
void xml_w_comment(FileHnd const hnd, T_ & value)
{
    xml_w_comment(hnd, str_ptr(value), str_sz(value));
}

//void xml_r_element_begin(FileHnd const hnd, StringRef const & name);
//void xml_r_element_close(FileHnd const hnd, StringRef const & name);
//void xml_r_attr(FileHnd const hnd, StringRef const& id, StringRef const& value);
//void xml_r_value(FileHnd const hnd, StringRef const& value);
//void xml_r_comment(FileHnd const hdn, StringRef const& value);


