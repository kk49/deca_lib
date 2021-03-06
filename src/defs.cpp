#include "defs.hpp"

std::string to_string(StringRef const & ref)
{
    return std::string(ref.ptr_, ref.ptr_ + ref.sz_);
}

c8 const * to_string(c8 const * str) { return str; }
std::string const & to_string(std::string const & str) { return str; }
std::string to_string(std::stringstream const & ss) { return ss.str(); }


c8 const* str_ptr(c8 const *  v) { return v; }
u32 str_sz(c8 const *  v) { return strlen(v); }

c8 const* str_ptr(std::string const&  v) { return v.data(); }
u32 str_sz(std::string const&  v) { return v.size(); }

c8 const* str_ptr(std::stringstream const&  v) { return str_ptr(v.str()); }
u32 str_sz(std::stringstream const&  v) { return str_sz(v.str()); }

c8 const* str_ptr(StringRef const& str) { return str.ptr_; }
size_t str_sz(StringRef const& str) { return str.sz_; }
