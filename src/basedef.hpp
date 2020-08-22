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
    StringRef()
    : ptr_(nullptr)
    , sz_(0)
    {
    }

    StringRef(c8 const* ptr, size_t sz)
    : ptr_(ptr)
    , sz_(sz)
    {
    }

    c8 const * ptr_;
    size_t sz_;
};



template<typename T_>
class ArrayRef
{
public:
    ArrayRef()
    : ptr_(nullptr)
    , cnt_(0)
    {
    }

    ArrayRef(T_ const* ptr, size_t cnt)
    : ptr_(ptr)
    , cnt_(cnt)
    {
    }

    T_ const * ptr_;
    size_t cnt_;
};

class DecaBufferFile
{
public:
    DecaBufferFile(c8 const* buffer_beg, c8 const* buffer_end)
    : beg_(buffer_beg)
    , end_(buffer_end)
    , ptr_(buffer_beg)
    {
    }

    c8 const * beg_;
    c8 const * end_;
    c8 const * ptr_;

    size_t size() const { return end_ - beg_; }

    size_t pos_peek() const
    {
        return ptr_ - beg_;
    }

    void pos_seek(size_t offset)
    {
        ptr_ = beg_ + offset;
    }

    template<typename T_>
    T_ read(s64 offset = -1)
    {
        c8 const ** pptr = &ptr_;
        c8 const * tptr = beg_ + offset;
        if(offset >= 0)  //abs read do not update pointer
            pptr = &tptr;
        c8 const * & ptr = *pptr;

        if(ptr + sizeof(T_) > end_)
            throw DecaException("EOL REACHED");

        T_ const value = *((T_ *)ptr);
        ptr += sizeof(T_);
        return value;
    }



    template<typename T_>
    ArrayRef<T_> reads(size_t count, s64 offset=-1)
    {
        c8 const ** pptr = &ptr_;
        c8 const * tptr = beg_ + offset;
        if(offset >= 0)  //abs read do not update pointer
            pptr = &tptr;
        c8 const * & ptr = *pptr;

        if(ptr + sizeof(T_) * count > end_)
            throw DecaException("EOL REACHED");

        T_ * const ptr2 = (T_ *)ptr;
        ptr += sizeof(T_) * count;

        return ArrayRef<T_>{ptr2,count};
    }

    StringRef read_strz(s64 offset = -1)
    {
        c8 const ** pptr = &ptr_;
        c8 const * tptr = beg_ + offset;
        if(offset >= 0)  //abs read do not update pointer
            pptr = &tptr;
        c8 const * & ptr = *pptr;

        c8 const* start = ptr;

        for(;(ptr < end_) && (*ptr != 0); ++ptr);

        StringRef result(start, ptr - start);

        if(ptr < end_)
            ++ptr;  //consume 0 at end of string

        return result;
    }

    StringRef read_strn(u32 size, bool const trim_last = false)
    {
        c8 const * start = ptr_;
        ptr_ += size;
        if(trim_last) --size;

        return StringRef(start, size);
    }

    template<typename OFFSET_, typename LENGTH_>
    StringRef read_strol()
    {
        //TODO Add buffer end check
        u32 const offset = read<OFFSET_>();
        u32 const length = read<LENGTH_>();  // TODO Size may be needed for byte codes?
        return StringRef(beg_ + offset, length);
    }

};

template<typename F_>
class StorePos
{
public:
    StorePos(F_ & f)
    : f_(f)
    , offset_(f.pos_peek())
    {}

    F_ & f_;
    size_t offset_;

    ~StorePos()
    {
        f_.pos_seek(offset_);
    }
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

c8 const* str_ptr(c8 const *  v);
u32 str_sz(c8 const *  v);

c8 const* str_ptr(std::string const&  v);
u32 str_sz(std::string const&  v);

c8 const* str_ptr(std::stringstream const&  v);
u32 str_sz(std::stringstream const&  v);

c8 const* str_ptr(StringRef const& v);
u32 str_sz(StringRef const&  v);

