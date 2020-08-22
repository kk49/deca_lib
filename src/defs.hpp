#pragma once
#include <cstdint>
#include <string>
#include <sstream>
#include <exception>

typedef char c8;

typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;
typedef int64_t s64;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

class DecaException
: public std::exception
{
public:
    DecaException(std::string const& s = std::string())
    : std::exception()
    , msg_(s)
    {
    }

    DecaException(std::stringstream const & ss)
    : std::exception()
    , msg_(ss.str())
    {}

    std::string msg_;
};


struct StringRef
{
    typedef c8 ElementType;

    StringRef()
    : ptr_(nullptr)
    , sz_(0)
    {
    }

    StringRef(c8 const* ptr, s64 sz)
    : ptr_(ptr)
    , sz_(sz)
    {
    }

    ElementType const* str_ptr() const {
        return ptr_;
    }

    s64 str_sz() const {
        return sz_;
    }

    ElementType const * ptr_;
    s64 sz_;
};

template<typename T_>
class ArrayRef
{
public:
    typedef T_ ElementType;

    ArrayRef()
    : ptr_(nullptr)
    , cnt_(0)
    {
    }

    ArrayRef(T_ const* ptr, s64 cnt)
    : ptr_(ptr)
    , cnt_(cnt)
    {
    }

    ElementType const* arr_ptr() const {
        return ptr_;
    }

    s64 arr_cnt() const {
        return cnt_;
    }

    T_ const * ptr_;
    s64 cnt_;
};


std::string to_string(StringRef const & ref);

template<typename T_>
std::string to_hex(T_ const & value, u32 width)
{
    std::stringstream ss;
    ss.width(width);
    ss.fill('0');
    ss << std::hex << value;

    return ss.str();
}

template<typename T_> struct DetElementType {
    typedef typename T_::ElementType ElementType;
};



c8 const* str_ptr(c8 const *  v);
u32 str_sz(c8 const *  v);

c8 const* str_ptr(std::string const&  v);
u32 str_sz(std::string const&  v);

c8 const* str_ptr(std::stringstream const&  v);
u32 str_sz(std::stringstream const&  v);

c8 const* str_ptr(StringRef const& str);
size_t str_sz(StringRef const& str);

template <typename TE_> TE_ const* arr_ptr(ArrayRef<TE_> const& arr) { return arr.ptr_; }
template <typename TE_> size_t arr_cnt(ArrayRef<TE_> const& arr) { return arr.cnt_; }

