#pragma once
#include "defs.hpp"
#include <vector>
#include <cassert>


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

extern std::ostream wasm_out;
extern std::ostream wasm_err;
extern std::istream wasm_in;


template<typename T_>
void db_print(T_ const& str) { wasm_out.write(str_ptr(str), str_sz(str)); }

template<typename T_>
void db_warn(T_ const& str) { wasm_err.write(str_ptr(str), str_sz(str)); }

template<typename T_>
void db_error(T_ const& str) { wasm_err.write(str_ptr(str), str_sz(str)); }

template<typename T_>
[[noreturn]] void db_exception(T_ const& str) {
    db_error("DecaException: ");
    db_error(str);
    db_error("\n");
    throw DecaException(str);
}

template<typename N_, typename M_>
FileHnd file_open(N_ const& path, M_ const& mode)
{
    return file_open(str_ptr(path), str_sz(path), str_ptr(mode), str_sz(mode));
}


template<typename T_>
struct FileArrayRef {
    typedef T_ ElementType;
    FileHnd hnd_;
    s64 offset_;
    s64 cnt_;

    std::vector<ElementType> buffer_;

    void prep_buffer()
    {
        if(cnt_ != buffer_.size())
        {
            buffer_.resize(cnt_);
            //make sure we read expected amount of data
            //TODO may be too strict
            s32 const sz = file_read(hnd_, offset_, (c8*)buffer_.data(), cnt_ * sizeof(ElementType));
            assert(cnt_ * sizeof(ElementType) == sz);
        }
    }

    ElementType const * arr_ptr()
    {
        prep_buffer();
        return buffer_.data();
    }

    size_t arr_cnt()
    {
        prep_buffer();
        return buffer_.size();
    }

};

struct FileStringRef {
    typedef c8 ElementType;
    FileHnd hnd_;
    s64 offset_;
    s64 cnt_;

    std::string buffer_;

    void prep_buffer()
    {
        if(cnt_ != buffer_.size())
        {
            buffer_.resize(cnt_);
            //make sure we read expected amount of data
            //TODO may be too strict
            s32 const sz = file_read(hnd_, offset_, (c8*)buffer_.data(), cnt_ * sizeof(ElementType));
            assert(cnt_ * sizeof(ElementType) == sz);
        }
    }

    ElementType const * str_ptr()
    {
        prep_buffer();
        return buffer_.data();
    }

    size_t str_sz()
    {
        prep_buffer();
        return buffer_.size();
    }

};

c8 const* str_ptr(FileStringRef & str);
size_t str_sz(FileStringRef & str);

std::string to_string(FileStringRef & str);

template <typename TE_> TE_ const* arr_ptr(FileArrayRef<TE_> & arr) { return arr.arr_ptr(); }
template <typename TE_> size_t arr_cnt(FileArrayRef<TE_> & arr) { return arr.arr_cnt(); }


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

    s64 size() const { return end_ - beg_; }

    s64 pos_peek() const
    {
        return ptr_ - beg_;
    }

    void pos_seek(s64 offset)
    {
        ptr_ = beg_ + offset;
    }

    template<typename T_>
    T_ read(s64 offset = -1)
    {
//        db_print(std::stringstream()
//            << "DecaBufferFile::read"
//            << ", " << (u64)ptr_ - (u64)beg_ << "/" << (u64)end_ - (u64)beg_
//            << ", " << (u64)ptr_ << "/(" << (u64)beg_ << ","<< (u64)end_ << ")"
//            << ", " << offset
//            << ", " << sizeof(T_)
//            << "\n");

        c8 const ** pptr = &ptr_;
        c8 const * tptr = beg_ + offset;
        if(offset >= 0)  //abs read do not update pointer
            pptr = &tptr;
        c8 const * & ptr = *pptr;

        if(ptr + sizeof(T_) > end_)
            db_exception(std::stringstream() << "DecaBufferFile::read: EOL REACHED: " << (u64)ptr << "/(" << (u64)beg_ << ","<< (u64)end_ << "), " << sizeof(T_));

        T_ const value = *((T_ *)ptr);
        ptr += sizeof(T_);
        return value;
    }

    template<typename T_>
    ArrayRef<T_> reads(s64 count, s64 offset=-1)
    {
        c8 const ** pptr = &ptr_;
        c8 const * tptr = beg_ + offset;
        if(offset >= 0)  //abs read do not update pointer
            pptr = &tptr;
        c8 const * & ptr = *pptr;

//        db_print(std::stringstream()
//            << "DecaBufferFile::reads"
//            << ", " << (u64)ptr_ - (u64)beg_ << "/" << (u64)end_ - (u64)beg_
//            << ", " << (u64)ptr_ << "/(" << (u64)beg_ << ","<< (u64)end_ << ")"
//            << ", " << offset
//            << ", " << count
//            << ", " << sizeof(T_)
//            << "\n");

        if(count < 0)
        {
            count = read<u32>(ptr - beg_);
            ptr += 4;
//            db_print(std::stringstream() << "DecaBufferFile::reads: count = " << count << "\n");
        }

        if(ptr + sizeof(T_) * count > end_)
            db_exception(std::stringstream() << "DecaBufferFile::reads: EOL REACHED: " << (u64)ptr << "/(" << (u64)beg_ << ","<< (u64)end_ << "), " << count);

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

    s64 size() const { return file_size_; }

    s64 pos_peek() const
    {
        return file_pos_;
    }

    void pos_seek(s64 offset)
    {
        file_pos_ = offset;
    }

    template<typename T_>
    T_ read(s64 offset = -1)
    {
//        db_print(std::stringstream()
//            << "DecaBufferFile2::read"
//            << ", " << file_pos_ << "/" << file_size_
//            << ", " << offset
//            << ", " << sizeof(T_)
//            << "\n");

        s64 * pfp = &file_pos_;
        if(offset >= 0)
            pfp = &offset;
        s64 & fp = *pfp;

        if(fp + sizeof(T_) > file_size_)
            db_exception(std::stringstream() << "DecaBufferFile2::read: EOL REACHED: " << fp << "/" << file_size_ << ", " << sizeof(T_));

        T_ value;
        fp += file_read(hnd_, fp, (c8*)&value, sizeof(T_));
        return value;
    }



    template<typename T_>
    FileArrayRef<T_> reads(s64 count, s64 offset=-1)
    {
//        db_print(std::stringstream()
//            << "DecaBufferFile2::reads"
//            << ", " << file_pos_ << "/" << file_size_
//            << ", " << offset
//            << ", " << count
//            << ", " << sizeof(T_)
//            << "\n");

        s64 * pfp = &file_pos_;
        if(offset >= 0)
            pfp = &offset;
        s64 & fp = *pfp;

        if(count < 0)
        {
            count = read<u32>(fp);
            fp += 4;
        }

        if(fp + sizeof(T_) * count > file_size_)
            db_exception(std::stringstream() << "DecaBufferFile2::reads: EOL REACHED: " << fp << "/" << file_size_ << ", " << count);

        s64 const old_pos = fp;
        fp += sizeof(T_) * count;

        return FileArrayRef<T_>{
            .hnd_ = hnd_,
            .offset_ = old_pos,
            .cnt_ = count
        };
    }

    FileStringRef read_strz(s64 offset = -1)
    {
//        db_print(std::stringstream()
//            << "DecaBufferFile2::read_strz"
//            << ", " << file_pos_ << "/" << file_size_
//            << ", " << offset
//            << ", " << sizeof(c8)
//            << "\n");

        s64 * pfp = &file_pos_;
        if(offset >= 0)
            pfp = &offset;
        s64 & fp = *pfp;

        s64 const start_pos = fp;
        s64 const buffer_size = 64;
        c8 buffer[buffer_size];

        s64 count = 0;
        bool run = true;
        while(run)
        {
            s64 rcnt = file_read(hnd_, fp, buffer, buffer_size);
            if(rcnt <= 0)
                break;

            c8 const * const buffer_end = buffer + rcnt;

            c8* pos;
            for(pos = buffer; pos < buffer_end; ++pos)
            {
                if(*pos == 0)
                {
                    run = false;
                    fp += 1;
                    break;
                }
            }
            count += pos - buffer;
            fp += pos - buffer;
        }

        return  FileStringRef{
            .hnd_ = hnd_,
            .offset_ = start_pos,
            .cnt_ = count
        };
    }

    FileStringRef read_strn(u32 size, bool const trim_last = false, s64 offset=-1)
    {
        s64 * pfp = &file_pos_;
        if(offset >= 0)
            pfp = &offset;
        s64 & fp = *pfp;

        if(fp + sizeof(c8) * size > file_size_)
            db_exception("EOL REACHED");

        s64 const start_pos = fp;

        fp += size;
        if(trim_last) --fp;

        return FileStringRef{
            .hnd_ = hnd_,
            .offset_ = start_pos,
            .cnt_ = size
        };
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

struct WasmStreamBuf
: public std::streambuf
{
public:
    WasmStreamBuf(std::string const& path, bool auto_close = true)
    : hnd_(-1)
    , auto_close_(auto_close)
    , read_pos_(0)
    , write_pos_(0)
    {
        hnd_ = file_open(path, "rwb");
        file_size_ = file_size(hnd_);
    }

    WasmStreamBuf(FileHnd hnd)
    : hnd_(hnd)
    , auto_close_(false)
    , read_pos_(0)
    , write_pos_(0)
    {
        file_size_ = file_size(hnd_);
    }

    ~WasmStreamBuf()
    {
        if(auto_close_ && hnd_ >= 0)
            file_close(hnd_);
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
    bool auto_close_;
    s64 file_size_;
    s64 read_pos_;
    s64 write_pos_;
};
