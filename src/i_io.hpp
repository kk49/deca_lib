#pragma once
#include "basedef.hpp"


typedef s32 FileHnd;


// exports
extern "C" {
    c8 *alloc_bin(u64 sz);
}


// imports
extern "C" {
    FileHnd file_open(c8 const* path_ptr, size_t path_sz, c8 const* mode_ptr, size_t mode_sz);
    void file_close(FileHnd hnd);
    s64 file_size(FileHnd hnd);
    s64 file_write(FileHnd hnd, s64 pos, c8 const* ptr, size_t sz);
    s64 file_read(FileHnd hnd, s64 pos, c8* ptr, size_t sz);
}

template<typename N_, typename M_>
FileHnd file_open(N_ const& path, M_ const& mode)
{
    return file_open(str_ptr(path), str_sz(path), str_ptr(mode), str_sz(mode));
}

struct WasmStreamBuf
: public std::streambuf
{
    WasmStreamBuf(std::string const& path)
    : hnd_(-1)
    , read_pos_(0)
    , write_pos_(0)
    {
        hnd_ = file_open(path, "rwb");
        file_size_ = file_size(hnd_);
    }

    ~WasmStreamBuf()
    {

    }

    WasmStreamBuf(WasmStreamBuf const&) = delete;
    WasmStreamBuf& operator=(WasmStreamBuf const&) = delete;

//protected:
public:
    //write
    std::streamsize xsputn(const char_type* s, std::streamsize n) override
    {
        s64 cnt = file_write(hnd_, write_pos_, s, n);
        write_pos_ += cnt;
        file_size_ = std::max(file_size_, write_pos_);
        return cnt;
    };

    int_type overflow(int_type ch) override
    {
        c8 const c =  traits_type::to_char_type(ch);
        return  xsputn(&c, 1);
    }

    //read
    std::streamsize xsgetn (char* s, std::streamsize n) override
    {
        s64 cnt = file_read(hnd_, read_pos_, s, n);
        read_pos_ += cnt;
        return cnt;
    }

    int_type underflow() override
    {
        c8 c;
        s64 cnt = file_read(hnd_, read_pos_, &c, 1);

        if(0 == cnt)
        {
            return traits_type::eof();
        }
        else
        {
            return traits_type::to_int_type(c);
        }
    }

    int_type uflow() override
    {
        c8 c;
        s64 cnt = file_read(hnd_, read_pos_, &c, 1);

        if(0 == cnt)
        {
            return traits_type::eof();
        }
        else
        {
            read_pos_ += cnt;
            return traits_type::to_int_type(c);
        }
    }

    int_type pbackfail(int_type ch) override
    {
        if(read_pos_ > 0)
        {
            c8 c;
            s64 cnt = file_read(hnd_, read_pos_ - 1, &c, 1);
            if(ch != traits_type::eof() && ch != traits_type::to_int_type(c))
                return traits_type::eof();
            read_pos_ -= 1;
            return traits_type::to_int_type(c);
        }
        else
        {
            return traits_type::eof();
        }
    }

    std::streamsize showmanyc() override
    {
        return file_size_ - read_pos_;
    }

private:
    FileHnd hnd_;
    s64 file_size_;
    s64 read_pos_;
    s64 write_pos_;
};

extern std::ostream wasm_out;
extern std::ostream wasm_err;
extern std::istream wasm_in;


