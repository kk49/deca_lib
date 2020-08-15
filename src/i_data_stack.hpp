#pragma once
#include "basedef.hpp"
#include <cstring>
#include <vector>
#include <map>

extern "C"
{
    // external functions used to build adf structure in parent environment
    // file_begin ?
    // file_end ?
    // obj_begin ?

    extern void dict_push();           // ... -> ... dict
    extern void dict_field_set();      // ... dict str value -> dict

    extern void list_push();
    extern void list_append();

    extern void hash_register(u64 hash, char const *ptr, u32 sz);
    extern void hash32_push(u32 value);
    extern void hash48_push(u64 value);
    extern void hash64_push(u64 value);

    extern void bool_push(bool value);  // ... -> ... value
    extern void s8_push(s8 value);      // ... -> ... value
    extern void u8_push(u8 value);      // ... -> ... value
    extern void s16_push(s16 value);   // ... -> ... value
    extern void u16_push(u16 value);   // ... -> ... value
    extern void s32_push(s32 value);   // ... -> ... value
    extern void u32_push(u32 value);   // ... -> ... value
    extern void s64_push(s64 value);   // ... -> ... value
    extern void u64_push(u64 value);   // ... -> ... value
    extern void f32_push(f32 value);   // ... -> ... value
    extern void f64_push(f64 value);   // ... -> ... value

    extern void str_push(c8 const *ptr, u32 cnt);   // ... -> ... array[]

    extern void enum_push(u32 value, c8 const *ptr, u32 cnt);

    extern void s8s_push(s8 const * ptr, u32 cnt);   // ... -> ... array[]
    extern void u8s_push(u8 const * ptr, u32 cnt);   // ... -> ... array[]
    extern void s16s_push(s16 const * ptr, u32 cnt);   // ... -> ... array[]
    extern void u16s_push(u16 const * ptr, u32 cnt);   // ... -> ... array[]
    extern void s32s_push(s32 const * ptr, u32 cnt);   // ... -> ... array[]
    extern void u32s_push(u32 const * ptr, u32 cnt);   // ... -> ... array[]
    extern void s64s_push(s64 const * ptr, u32 cnt);   // ... -> ... array[]
    extern void u64s_push(u64 const * ptr, u32 cnt);   // ... -> ... array[]
    extern void f32s_push(f32 const * ptr, u32 cnt);   // ... -> ... array[]
    extern void f64s_push(f64 const * ptr, u32 cnt);   // ... -> ... array[]
}

void array_push(ArrayRef<s8> const & ref);
void array_push(ArrayRef<u8> const & ref);
void array_push(ArrayRef<s16> const & ref);
void array_push(ArrayRef<u16> const & ref);
void array_push(ArrayRef<s32> const & ref);
void array_push(ArrayRef<u32> const & ref);
void array_push(ArrayRef<s64> const & ref);
void array_push(ArrayRef<u64> const & ref);
void array_push(ArrayRef<f32> const & ref);
void array_push(ArrayRef<f64> const & ref);

void hash_register(u64 hash, StringRef const & ref);
void str_push(c8 const * ptr);
void str_push(std::string const & str);
void str_push(StringRef const & str);
void enum_push(u64 value, StringRef const & str);

