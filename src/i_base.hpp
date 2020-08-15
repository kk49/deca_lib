#pragma once
#include "basedef.hpp"
#include <sstream>

extern "C"
{
    extern void db_print(c8 const * ptr, u32 sz);
    extern void db_warn(c8 const * ptr, u32 sz);
    extern void db_error(c8 const * ptr, u32 sz);
};

void db_print(c8 const* str);
void db_print(StringRef const & str);
void db_print(std::string const& str);
void db_print(std::stringstream const& ss);
void db_warn(std::string const& str);
void db_warn(std::stringstream const& ss);
void db_exception(std::string const& str);
void db_exception(std::stringstream const& ss);
