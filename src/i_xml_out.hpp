#pragma once
#include "defs.hpp"
#include "i_io.hpp"

// imports
extern "C" {

void xml_w_element_begin(FileHnd const hnd, c8 const * name_ptr, u32 name_sz);
void xml_w_element_end(FileHnd const hnd, c8 const * name_ptr, u32 name_sz);
void xml_w_element_close(FileHnd const hnd, c8 const * name_ptr, u32 name_sz);
void xml_w_attr_set(FileHnd const hnd, c8 const * id_ptr, u32 id_sz, c8 const * value_ptr, u32 value_sz);
void xml_w_value_set(FileHnd const hnd, c8 const * value_ptr, u32 value_sz);
void xml_w_comment(FileHnd const hdn, c8 const * value_ptr, u32 value_sz);

//void xml_r_element_begin(FileHnd const hnd,c8 const * name_ptr, u32 name_sz);
//void xml_r_element_close(FileHnd const hnd, c8 const * name_ptr, u32 name_sz);
//void xml_r_attr(FileHnd const hnd, c8 const * id_ptr, u32 id_sz, c8 const * value_ptr, u32 value_sz);
//void xml_r_value(FileHnd const hnd, c8 const * value_ptr, u32 value_sz);
//void xml_r_comment(FileHnd const hdn, c8 const * value_ptr, u32 value_sz);

}

class XmlWriterExternal
{
public:
    XmlWriterExternal(FileHnd hnd)
    : hnd_(hnd)
    {
    }

    FileHnd hnd_;

    template<typename T_>
    void w_element_begin(T_ const & name)
    {
        xml_w_element_begin(hnd_, str_ptr(name), str_sz(name));
    }

    template<typename T_>
    void w_element_end(T_ const & name)
    {
        xml_w_element_end(hnd_, str_ptr(name), str_sz(name));
    }

    template<typename T_>
    void w_element_close(T_ const & name)
    {
        xml_w_element_close(hnd_, str_ptr(name), str_sz(name));
    }

    template<typename T0_, typename T1_>
    void w_attr_set(T0_ const & id, T1_ const & value)
    {
        xml_w_attr_set(hnd_, str_ptr(id), str_sz(id), str_ptr(value), str_sz(value));
    }

    template<typename T_>
    void w_value_set(T_ & value)
    {
        xml_w_value_set(hnd_, str_ptr(value), str_sz(value));
    }

    template<typename T_>
    void w_comment(FileHnd const hnd, T_ & value)
    {
        xml_w_comment(hnd_, str_ptr(value), str_sz(value));
    }
};

class XmlWriterFile
{
public:
    XmlWriterFile(std::ostream * file)
    : file_(*file)
    {
    }

    std::ostream & file_;

    template<typename T_>
    void w_element_begin(T_ const & name)
    {
        file_ << "<";
        file_.write(str_ptr(name), str_sz(name));
//        xml_w_element_begin(str_ptr(name), str_sz(name));
    }

    template<typename T_>
    void w_element_end(T_ const & name)
    {
        file_ << ">";
    }

    template<typename T_>
    void w_element_close(T_ const & name)
    {
        file_ << "</";
        file_.write(str_ptr(name), str_sz(name));
        file_ << ">";
    }

    template<typename T0_, typename T1_>
    void w_attr_set(T0_ const & id, T1_ const & value)
    {
        file_ << " " << to_string(id) <<  "=\"" << to_string(value) << "\"";
    }

    template<typename T_>
    void w_value_set(T_ & value)
    {
        file_.write(str_ptr(value), str_sz(value));
    }

    template<typename T_>
    void w_comment(FileHnd const hnd, T_ & value)
    {
//        xml_w_comment(hnd_, str_ptr(value), str_sz(value));
    }
};


//void xml_r_element_begin(FileHnd const hnd, StringRef const & name);
//void xml_r_element_close(FileHnd const hnd, StringRef const & name);
//void xml_r_attr(FileHnd const hnd, StringRef const& id, StringRef const& value);
//void xml_r_value(FileHnd const hnd, StringRef const& value);
//void xml_r_comment(FileHnd const hdn, StringRef const& value);


