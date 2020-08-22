#pragma once
#include "defs.hpp"
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

template<typename ET_> struct Func;
template<> struct Func<s8> { static auto constexpr push_func = s8s_push; };
template<> struct Func<u8> { static auto constexpr push_func = u8s_push; };
template<> struct Func<s16> { static auto constexpr push_func = s16s_push; };
template<> struct Func<u16> { static auto constexpr push_func = u16s_push; };
template<> struct Func<s32> { static auto constexpr push_func = s32s_push; };
template<> struct Func<u32> { static auto constexpr push_func = u32s_push; };
template<> struct Func<s64> { static auto constexpr push_func = s64s_push; };
template<> struct Func<u64> { static auto constexpr push_func = u64s_push; };
template<> struct Func<f32> { static auto constexpr push_func = f32s_push; };
template<> struct Func<f64> { static auto constexpr push_func = f64s_push; };


template<typename T_> void str_push(T_ const & str) { str_push(str_ptr(str), str_sz(str)); }
template<typename T_> void str_push(T_ & str) { str_push(str_ptr(str), str_sz(str)); }
template<typename T_> void enum_push(u64 value, T_ const & str) { enum_push(value, str_ptr(str), str_sz(str)); }
template<typename T_> void enum_push(u64 value, T_ & str) { enum_push(value, str_ptr(str), str_sz(str)); }
template<typename T_> void hash_register(u64 value, T_ const & str) { hash_register(value, str_ptr(str), str_sz(str)); }
template<typename T_> void hash_register(u64 value, T_ & str) { hash_register(value, str_ptr(str), str_sz(str)); }

template<typename T_> void array_push(T_ & arr) { Func<typename DetElementType<T_>::ElementType >::push_func(arr_ptr(arr), arr_cnt(arr)); }
