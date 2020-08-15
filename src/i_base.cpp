#include "i_base.hpp"

void db_print(c8 const* str)
{
    db_print(str, strlen(str));
}

void db_print(StringRef const & str)
{
    db_print(str.ptr_, str.sz_);
}

void db_print(std::string const& str)
{
    db_print(str.data(), str.size());
}

void db_print(std::stringstream const& ss)
{
    db_print(ss.str());
}

void db_warn(std::string const& str)
{
    db_warn(str.data(), str.size());
}

void db_warn(std::stringstream const& ss)
{
    db_warn(ss.str());
}

void db_exception(std::string const& str)
{
    db_error(str.data(), str.size());
    throw DecaException(str);
}

void db_exception(std::stringstream const& ss)
{
    db_exception(ss.str());
}
