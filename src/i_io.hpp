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

template<typename T_>
struct FileArrayRef {
    typedef T_ ElementType;
    FileHnd hnd_;
    s64 offset_;
    s64 cnt_;
};

struct FileStringRef {
    FileHnd hnd_;
    s64 offset_;
    s64 cnt_;
};

class DecaBufferFile2
{
public:
    DecaBufferFile2(FileHnd hnd)
    : hnd_(hnd)
    , file_size_(0)
    , file_pos_(0)
    {
        file_size_ = file_size(hnd_);
    }

    FileHnd hnd_;
    s64 file_size_;
    s64 file_pos_;

    size_t size() const { return file_size_; }

    size_t pos_peek() const
    {
        return file_pos_;
    }

    void pos_seek(size_t offset)
    {
        file_pos_ = offset;
    }

    template<typename T_>
    T_ read()
    {
        if(file_pos_ + sizeof(T_) > file_size_)
            throw DecaException("EOL REACHED");

        T_ value;
        file_pos_ += file_read(hnd_, file_pos_, (c8*)&value, sizeof(T_));
        return value;
    }



    template<typename T_>
    FileArrayRef<T_> reads(size_t count)
    {
        if(file_pos_ + sizeof(T_) * count > file_size_)
            throw DecaException("EOL REACHED");

        s64 const old_pos = file_pos_;
        file_pos_ += sizeof(T_) * count;

        return FileArrayRef<T_>{
            .hnd_ = hnd_,
            .offset_ = old_pos,
            .cnt_ = count
        };
    }

    FileStringRef read_strz()
    {
        s64 const start_pos = file_pos_;
        s64 const buffer_size = 64;
        c8 buffer[buffer_size];

        s32 count = 0;
        bool run = true;
        while(run)
        {
            s64 rcnt = file_read(hnd_, file_pos_, buffer, buffer_size);
            if(rcnt <= 0)
                break;

            c8 const * const buffer_end = buffer + rcnt;

            c8* pos;
            for(pos = buffer; pos < buffer_end; ++pos)
            {
                if(*pos == 0)
                {
                    run = false;
                    count += 1;
                    break;
                }
            }
            count += pos - buffer;
        }

        file_pos_ += count;

        FileStringRef result{
            .hnd_ = hnd_,
            .offset_ = start_pos,
            .cnt_ = count
        };

        return result;
    }

    FileStringRef read_strn(u32 size, bool const trim_last = false)
    {
        if(file_pos_ + sizeof(c8) * size > file_size_)
            throw DecaException("EOL REACHED");

        FileStringRef result{
            .hnd_ = hnd_,
            .offset_ = file_pos_,
            .cnt_ = size
        };

        file_pos_ += size;
        if(trim_last) --file_pos_;

        return result;
    }

    template<typename OFFSET_, typename LENGTH_>
    FileStringRef read_strol()
    {
        //TODO Add buffer end check
        u32 const offset = read<OFFSET_>();
        u32 const length = read<LENGTH_>();  // TODO Size may be needed for byte codes?

        return FileStringRef{
            .hnd_ = hnd_,
            .offset_ = offset,
            .cnt_ = length
        };
    }
};

extern std::ostream wasm_out;
extern std::ostream wasm_err;
extern std::istream wasm_in;


