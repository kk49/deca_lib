#include "i_data_stack.hpp"

void array_push(ArrayRef<s8> const & ref) { s8s_push(ref.ptr_, ref.cnt_); }
void array_push(ArrayRef<u8> const & ref) { u8s_push(ref.ptr_, ref.cnt_); }
void array_push(ArrayRef<s16> const & ref) { s16s_push(ref.ptr_, ref.cnt_); }
void array_push(ArrayRef<u16> const & ref) { u16s_push(ref.ptr_, ref.cnt_); }
void array_push(ArrayRef<s32> const & ref) { s32s_push(ref.ptr_, ref.cnt_); }
void array_push(ArrayRef<u32> const & ref) { u32s_push(ref.ptr_, ref.cnt_); }
void array_push(ArrayRef<s64> const & ref) { s64s_push(ref.ptr_, ref.cnt_); }
void array_push(ArrayRef<u64> const & ref) { u64s_push(ref.ptr_, ref.cnt_); }
void array_push(ArrayRef<f32> const & ref) { f32s_push(ref.ptr_, ref.cnt_); }
void array_push(ArrayRef<f64> const & ref) { f64s_push(ref.ptr_, ref.cnt_); }

void hash_register(u64 hash, StringRef const & ref)
{
    hash_register(hash, ref.ptr_, ref.sz_);
}

void str_push(c8 const * ptr)
{
    str_push(ptr, strlen(ptr));
}

void str_push(std::string const & str)
{
    str_push(str.data(), str.size());
}

void str_push(StringRef const & str)
{
    str_push(str.ptr_, str.sz_);
}

void enum_push(u64 value, StringRef const & str)
{
    enum_push(value, str.ptr_, str.sz_);
}