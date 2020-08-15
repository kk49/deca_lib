#include "basedef.hpp"

std::string to_string(StringRef const & ref)
{
    return std::string(ref.ptr_, ref.ptr_ + ref.sz_);
}


