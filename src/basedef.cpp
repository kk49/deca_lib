#include "basedef.hpp"

std::string to_string(StringRef const & ref)
{
    return std::string(ref.ptr_, ref.ptr_ + ref.sz_);
}


c8 const* str_ptr(c8 const *  v) { return v; }
u32 str_sz(c8 const *  v) { return strlen(v); }

c8 const* str_ptr(std::string const&  v) { return v.data(); }
u32 str_sz(std::string const&  v) { return v.size(); }

c8 const* str_ptr(std::stringstream const&  v) { return str_ptr(v.str()); }
u32 str_sz(std::stringstream const&  v) { return str_sz(v.str()); }