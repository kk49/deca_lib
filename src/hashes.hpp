#pragma once
#include "basedef.hpp"

u32 hash32_func(u8 const * data, u32 len, u32 init_val=0) noexcept;
u32 hash32_func(c8 const * data) noexcept;


//u64 hash48_func(u8 const * data, u32 len){
////    v = mmh3.hash128(key=data, x64arch=True)
//    u64 v = mmh3.hash128(data, true);
//    return (v >> 16) & 0x0000FFFFFFFFFFFF;
//}
//
//u64 hash64_func(u8 const * data, u32 len){
////    v = mmh3.hash128(key=data, x64arch=True)
//    u64 v = mmh3.hash128(data, true);
//    return v & 0xFFFFFFFFFFFFFFFF;
//}

//def hash_all_func(data):
//if isinstance(data, str):
//data = data.encode('ascii')
//
//c, b = hashlittle2(data, 0, 0)
//
//v = mmh3.hash128(key=data, x64arch=True)
//
//return c, (v >> 16) & 0x0000FFFFFFFFFFFF, int(np.int64(np.uint64(v & 0xFFFFFFFFFFFFFFFF)))

