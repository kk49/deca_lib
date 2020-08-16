#include "hashes.hpp"
#include "i_base.hpp"

// Need to constrain U32 to only 32 bits using the & 0xffffffff
// since Python has no native notion of integers limited to 32 bit
// http://docs.python.org/library/stdtypes.html#numeric-types-int-float-long-complex

// KK FOUND HERE: https://stackoverflow.com/questions/3279615/python-implementation-of-jenkins-hash


// Original copyright notice:
// By Bob Jenkins, 1996.  bob_jenkins@burtleburtle.net.  You may use this
// code any way you wish, private, educational, or commercial.  Its free.

//TODO remove 0xFFFFFF anding
u32 rot(u32 const x, u32 const k) {
    return (x << k) | (x >> (32 - k));
}

void mix(u32 & a, u32 & b, u32 & c) {
    a &= 0xffffffff; b &= 0xffffffff; c &= 0xffffffff;
    a -= c; a &= 0xffffffff; a ^= rot(c,4);  a &= 0xffffffff; c += b; c &= 0xffffffff;
    b -= a; b &= 0xffffffff; b ^= rot(a,6);  b &= 0xffffffff; a += c; a &= 0xffffffff;
    c -= b; c &= 0xffffffff; c ^= rot(b,8);  c &= 0xffffffff; b += a; b &= 0xffffffff;
    a -= c; a &= 0xffffffff; a ^= rot(c,16); a &= 0xffffffff; c += b; c &= 0xffffffff;
    b -= a; b &= 0xffffffff; b ^= rot(a,19); b &= 0xffffffff; a += c; a &= 0xffffffff;
    c -= b; c &= 0xffffffff; c ^= rot(b,4);  c &= 0xffffffff; b += a; b &= 0xffffffff;
}

void final(u32 & a, u32 & b, u32 & c) {
    a &= 0xffffffff; b &= 0xffffffff; c &= 0xffffffff;
    c ^= b; c &= 0xffffffff; c -= rot(b,14); c &= 0xffffffff;
    a ^= c; a &= 0xffffffff; a -= rot(c,11); a &= 0xffffffff;
    b ^= a; b &= 0xffffffff; b -= rot(a,25); b &= 0xffffffff;
    c ^= b; c &= 0xffffffff; c -= rot(b,16); c &= 0xffffffff;
    a ^= c; a &= 0xffffffff; a -= rot(c,4);  a &= 0xffffffff;
    b ^= a; b &= 0xffffffff; b -= rot(a,14); b &= 0xffffffff;
    c ^= b; c &= 0xffffffff; c -= rot(b,24); c &= 0xffffffff;
}


void hashlittle2(
        u32 & a, u32 & b, u32 & c,
        u8 const * data, u32 len, u32 initval, u32 initval2) {
    u32 lenpos = len;
    u32 length = len;

    a = b = c = (0xdeadbeef + length + initval);

    c += initval2;
    c &= 0xffffffff;

    u32 p = 0;  // string offset
    while(lenpos > 12) {
        a += ((data[p+0]) + ((data[p+1])<<8u) + ((data[p+2])<<16u) + ((data[p+3])<<24u)); a &= 0xffffffff;
        b += ((data[p+4]) + ((data[p+5])<<8u) + ((data[p+6])<<16u) + ((data[p+7])<<24u)); b &= 0xffffffff;
        c += ((data[p+8]) + ((data[p+9])<<8u) + ((data[p+10])<<16u) + ((data[p+11])<<24u)); c &= 0xffffffff;
        mix(a, b, c);
        p += 12;
        lenpos -= 12;
    }

    switch(lenpos)
    {
        case 12: c += ((data[p+8]) + ((data[p+9])<<8u) + ((data[p+10])<<16u) + ((data[p+11])<<24u)); b += ((data[p+4]) + ((data[p+5])<<8u) + ((data[p+6])<<16u) + ((data[p+7])<<24u)); a += ((data[p+0]) + ((data[p+1])<<8u) + ((data[p+2])<<16u) + ((data[p+3])<<24u)); break;
        case 11: c += ((data[p+8]) + ((data[p+9])<<8u) + ((data[p+10])<<16u)); b += ((data[p+4]) + ((data[p+5])<<8u) + ((data[p+6])<<16u) + ((data[p+7])<<24u)); a += ((data[p+0]) + ((data[p+1])<<8u) + ((data[p+2])<<16u) + ((data[p+3])<<24u)); break;
        case 10: c += ((data[p+8]) + ((data[p+9])<<8u)); b += ((data[p+4]) + ((data[p+5])<<8u) + ((data[p+6])<<16u) + ((data[p+7])<<24u)); a += ((data[p+0]) + ((data[p+1])<<8u) + ((data[p+2])<<16u) + ((data[p+3])<<24u)); break;
        case 9:  c += ((data[p+8])); b += ((data[p+4]) + ((data[p+5])<<8u) + ((data[p+6])<<16u) + ((data[p+7])<<24u)); a += ((data[p+0]) + ((data[p+1])<<8u) + ((data[p+2])<<16u) + ((data[p+3])<<24u)); break;
        case 8:  b += ((data[p+4]) + ((data[p+5])<<8u) + ((data[p+6])<<16u) + ((data[p+7])<<24u)); a += ((data[p+0]) + ((data[p+1])<<8u) + ((data[p+2])<<16u) + ((data[p+3])<<24u)); break;
        case 7:  b += ((data[p+4]) + ((data[p+5])<<8u) + ((data[p+6])<<16u)); a += ((data[p+0]) + ((data[p+1])<<8u) + ((data[p+2])<<16u) + ((data[p+3])<<24u)); break;
        case 6:  b += (((data[p+5])<<8u) + (data[p+4])); a += ((data[p+0]) + ((data[p+1])<<8u) + ((data[p+2])<<16u) + ((data[p+3])<<24u)); break;
        case 5:  b += ((data[p+4])); a += ((data[p+0]) + ((data[p+1])<<8u) + ((data[p+2])<<16u) + ((data[p+3])<<24u)); break;
        case 4:  a += ((data[p+0]) + ((data[p+1])<<8u) + ((data[p+2])<<16u) + ((data[p+3])<<24u)); break;
        case 3:  a += ((data[p+0]) + ((data[p+1])<<8u) + ((data[p+2])<<16u)); break;
        case 2:  a += ((data[p+0]) + ((data[p+1])<<8u)); break;
        case 1:  a += (data[p+0]); break;
        case 0: break;
        default: db_exception("Should not occur, while loop handles other cases");
    }

    a &= 0xffffffff; b &= 0xffffffff; c &= 0xffffffff;

    if(lenpos != 0)
        final(a, b, c);
}

u32 hash32_func(u8 const * data, u32 len, u32 init_val) noexcept {
    u32 a, b, c;
    hashlittle2(a, b, c, data, len, init_val, 0);
    return c;
}

u32 hash32_func(c8 const * data) noexcept {
    return hash32_func((u8 const*)data, strlen(data));
}


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

