#pragma once
#include "basedef.hpp"
#include <sstream>

extern "C"
{
    extern void db_print(c8 const * ptr, u32 sz);
    extern void db_warn(c8 const * ptr, u32 sz);
    extern void db_error(c8 const * ptr, u32 sz);
};

template<typename T_>
void db_print(T_ const& str) { db_print(str_ptr(str), str_sz(str)); }

template<typename T_>
void db_warn(T_ const& str) { db_warn(str_ptr(str), str_sz(str)); }

template<typename T_>
void db_exception(T_ const& str) { db_error(str_ptr(str), str_sz(str)); throw DecaException(str); }
