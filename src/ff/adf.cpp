#include "../basedef.hpp"
#include "../i_base.hpp"
#include "../i_data_stack.hpp"
#include <emscripten.h>
#include <iostream>

// for PYTHON
// emcc adf.cpp -o deca_adf.wasm -O3 -v --js-library i_data_stack.js -s INITIAL_MEMORY=256MB -s MAXIMUM_MEMORY=1GB -s EXPORT_ALL=0 -s EXPORTED_FUNCTIONS="[]"

// for JavaScript
// emcc adf.cpp -o deca_adf.js -O3 -v --js-library i_data_stack.js -s INITIAL_MEMORY=256MB -s MAXIMUM_MEMORY=1GB -s EXPORT_ALL=0 -s EXPORTED_FUNCTIONS="[]"


// adf functions
u32 const typedef_s8 = 0x580D0A62;
u32 const typedef_u8 = 0x0ca2821d;
u32 const typedef_s16 = 0xD13FCF93;
u32 const typedef_u16 = 0x86d152bd;
u32 const typedef_s32 = 0x192fe633;
u32 const typedef_u32 = 0x075e4e4f;
u32 const typedef_s64 = 0xAF41354F;
u32 const typedef_u64 = 0xA139E01F;
u32 const typedef_f32 = 0x7515a207;
u32 const typedef_f64 = 0xC609F663;
u32 const typedef_str = 0x8955583e;


enum MetaType : u32
{ Primitive = 0
, Structure = 1
, Pointer = 2
, Array = 3
, InlineArray = 4
, String = 5
, MetaType6 = 6
, BitField = 7
, Enumeration = 8
, StringHash = 9
};


class MemberDef
{
public:
    MemberDef()
    : name_()
    , type_hash_()
    , size_()
    , offset_()
    , bit_offset_()
    , default_type_()
    , default_value_()
    {}

    StringRef name_;
    u32 type_hash_;
    u32 size_;
    u32 offset_;
    u32 bit_offset_;
    u32 default_type_;
    u64 default_value_;

    void deserialize(DecaBufferFile & f, std::vector<StringRef> const & nt)
    {
        name_ = nt.at(f.read<u64>());
        type_hash_ = f.read<u32>();
        size_ = f.read<u32>();
        u32 const offset = f.read<u32>();
        bit_offset_ = (offset >> 24) & 0xff;
        offset_ = offset & 0x00ffffff;
        default_type_ = f.read<u32>();
        default_value_ = f.read<u64>();
    }

};


class EnumDef
{
public:
    EnumDef()
    : name_()
    , value_()
    {}

    StringRef name_;
    u32 value_;

    void deserialize(DecaBufferFile & f, std::vector<StringRef> const & nt)
    {
        name_ = nt.at(f.read<u64>());
        value_ = f.read<u32>();
    }
};


class TypeDef
{
public:
    TypeDef()
    : meta_type_()
    , size_()
    , alignment_()
    , type_hash_()
    , name_()
    , flags_()
    , element_type_hash_()
    , element_length_()
    , members_()
    , enums_()
    {}

    MetaType meta_type_;
    u32 size_;
    u32 alignment_;
    u32 type_hash_;
    StringRef name_;
    u32 flags_;
    u32 element_type_hash_;
    u32 element_length_;
    std::vector<MemberDef> members_;
    std::vector<EnumDef> enums_;

    void deserialize(DecaBufferFile & f, std::vector<StringRef> const & nt)
    {
        size_t const pos = f.pos_peek();
        meta_type_ = static_cast<MetaType>(f.read<u32>());
        size_ = f.read<u32>();
        alignment_ = f.read<u32>();
        type_hash_ = f.read<u32>();
        name_ = nt.at(f.read<u64>());
        flags_ = f.read<u32>();
        element_type_hash_ = f.read<u32>();
        element_length_ = f.read<u32>();

//        db_print(std::stringstream()
//            << "pos = " << pos
//            << ", meta_type_ = " << meta_type_
//            << ", size_ = " << size_
//            << ", alignment_ = " << alignment_
//            << ", type_hash_ = " << type_hash_
//            << ", name_ = " << to_string(name_)
//            << ", flags_ = " << flags_
//            << ", element_type_hash_ = " << element_type_hash_
//            << ", element_length_ = " << element_length_
//        );

        switch(meta_type_)
        {
        case Primitive:
            break;
        case Structure:
            {
                u32 count = f.read<u32>();
                members_.resize(count);
                for(auto && i : members_) i.deserialize(f, nt);
            }
            break;
        case Pointer:
            {
                u32 count = f.read<u32>();
                if(count != 0) db_warn((std::stringstream() << "Pointer: Not Implemented: count == " << count).str());
            }
            break;
        case Array:
            {
                u32 count = f.read<u32>();
                if(count != 0) db_warn((std::stringstream() << "Array: Not Implemented: count == " << count).str());
            }
            break;
        case InlineArray:
            {
                u32 count = f.read<u32>();
                if(count != 0) db_exception((std::stringstream() << "InlineArray: Not Implemented: count == " << count).str());
            }
            break;
        case BitField:
            {
                u32 count = f.read<u32>();
                if(count != 0) db_exception((std::stringstream() << "BitField: Not Implemented: count == " << count).str());
            }
            break;
        case Enumeration:
            {
                u32 count = f.read<u32>();
                enums_.resize(count);
                for(auto && i : enums_) i.deserialize(f, nt);
            }
            break;
        case StringHash:
            {
                u32 count = f.read<u32>();
                if(count != 0) db_exception((std::stringstream() << "StringHash: Not Implemented: count == " << count).str());
            }
            break;
        case String:
        case MetaType6:
        default:
            db_exception((std::stringstream() << "Unknown Typedef Type " << meta_type_).str());
        }
    }

    void read_instance(DecaBufferFile & f, std::map<u32, TypeDef> const & map_typedef)
    {
    }
};


void read_instance(DecaBufferFile & f, std::map<u32, TypeDef> const & map_typedef, u32 type_hash, s32 bit_offset = -1);


void read_instance_array(DecaBufferFile & f, std::map<u32, TypeDef> const & map_typedef, u32 type_hash, u32 length)
{
    switch(type_hash)
    {
        case typedef_s8: array_push(f.reads<s8>(length)); break;
        case typedef_u8: array_push(f.reads<u8>(length)); break;
        case typedef_s16: array_push(f.reads<s16>(length)); break;
        case typedef_u16: array_push(f.reads<u16>(length)); break;
        case typedef_s32: array_push(f.reads<s32>(length)); break;
        case typedef_u32: array_push(f.reads<u32>(length)); break;
        case typedef_s64: array_push(f.reads<s64>(length)); break;
        case typedef_u64: array_push(f.reads<u64>(length)); break;
        case typedef_f32: array_push(f.reads<f32>(length)); break;
        case typedef_f64: array_push(f.reads<f64>(length)); break;
        default:
            {
                list_push();
                for(size_t i = 0; i < length; ++i)
                {
                    read_instance(f, map_typedef, type_hash);
                    list_append();
                }
            }
            break;
    }
}


void read_instance(DecaBufferFile & f, std::map<u32, TypeDef> const & map_typedef, u32 type_hash, s32 bit_offset)
{
    switch(type_hash)
    {
        case typedef_s8: s8_push(f.read<s8>()); break;
        case typedef_u8: u8_push(f.read<u8>()); break;
        case typedef_s16: s16_push(f.read<s16>()); break;
        case typedef_u16: u16_push(f.read<u16>()); break;
        case typedef_s32: s32_push(f.read<s32>()); break;
        case typedef_u32: u32_push(f.read<u32>()); break;
        case typedef_s64: s64_push(f.read<s64>()); break;
        case typedef_u64: u64_push(f.read<u64>()); break;
        case typedef_f32: f32_push(f.read<f32>()); break;
        case typedef_f64: f64_push(f.read<f64>()); break;
        case typedef_str: str_push(f.read_strol<u32,u32>()); break;
        case 0xdefe88ed:
            {
                u32 offset = f.read<u32>();
                u32 v01 = f.read<u32>();
                u32 sub_type_hash = f.read<u32>();
                u32 v03 = f.read<u32>();

                if(offset == 0 || sub_type_hash == 0)
                {
                }
                else
                {
                    StorePos sp(f);
                    f.pos_seek(offset);
                    read_instance(f, map_typedef, sub_type_hash);
                }
            }
            break;
        default:
            {
                auto it = map_typedef.find(type_hash);
                if(it == map_typedef.end())
                {
                    db_exception(std::stringstream() << "ERROR: Unknown type_hash = 0x" << std::hex << type_hash);
                }
                else
                {
                    TypeDef const & type_def = it->second;

                    switch(type_def.meta_type_)
                    {
                    case Primitive:
                        db_exception(std::stringstream() << "ERROR: Encountered Primitive");
                        break;
                    case Structure:
                        {
                            dict_push();
                            size_t p0 = f.pos_peek();
                            for(auto && m : type_def.members_)
                            {
                                str_push(m.name_);

                                f.pos_seek(p0 + m.offset_);
                                read_instance(f, map_typedef, m.type_hash_, m.bit_offset_);

                                dict_field_set();
                            }
                            f.pos_seek(p0 + type_def.size_);
                        }
                        break;
                    case Pointer:
                        {
                            //TODO not sure how this is used yet, but it's used by effects so lower priority
                            u64 const pointer = f.read<u64>();
                            str_push(
                                (std::stringstream() << "NOTE: " << to_string(type_def.name_) << ": "
                                    << std::hex << pointer << " to " << type_def.element_type_hash_).str()
                            );
                        }
                        break;
                    case Array:
                        {
                            u32 const offset = f.read<u32>();
                            u32 const flags = f.read<u32>();
                            u32 const length = f.read<u32>();
                            StorePos sp(f);

                            f.pos_seek(offset);

                            read_instance_array(f, map_typedef, type_def.element_type_hash_, length);
                        }
                        break;
                    case String:
                        db_exception((std::stringstream() << "ERROR: Unhandled meta type String"));
                        break;
                    case MetaType6:
                        db_exception((std::stringstream() << "ERROR: Unhandled meta type MetaType6"));
                        break;
                    case InlineArray:
                        {
                            read_instance_array(f, map_typedef, type_def.element_type_hash_, type_def.element_length_);
                        }
                        break;
                    case BitField:
                        {
                            u64 v = 0;
                            switch(type_def.size_)
                            {
                                case 1: v = f.read<u8>(); break;
                                case 2: v = f.read<u16>(); break;
                                case 4: v = f.read<u32>(); break;
                                case 8: v = f.read<u64>(); break;
                                default: db_exception("Unknown bitfield size"); break;
                            }

                            if(bit_offset < 0)
                            {
                                bit_offset = 0;
                                db_print("Missing bit offset");
                            }

                            v = (v >> bit_offset) & 1;
                            bool_push(v != 0);
                        }
                        break;
                    case Enumeration:
                        {
                            if(type_def.size_ != 4)
                            {
                                db_exception("Unknown Enum Size");
                            }
                            else
                            {
                                u32 v = f.read<u32>();
                                StringRef vs;

                                if(v < type_def.enums_.size())
                                    vs = type_def.enums_[v].name_;

                                enum_push(v, vs);
                            }
                        }
                        break;
                    case StringHash:
                        {
                            switch(type_def.size_)
                            {
                            case 4: hash32_push(f.read<u32>()); break;
                            case 8: hash64_push(f.read<u64>()); break;
                            case 6:
                                {
                                    u64 const v0 = f.read<u16>();
                                    u64 const v1 = f.read<u16>();
                                    u64 const v2 = f.read<u16>();
                                    hash48_push(v0 << 32 | v1 << 16 | v2);
                                }
                                break;
                            default: db_exception(std::stringstream() << "Unknown hash size " << type_def.size_); break;
                            }
                        }
                        break;
                    default:
                        db_exception((std::stringstream() << "ERROR: Unhandled meta type = " << type_def.meta_type_));
                        break;

                    }
                }
            }
            break;

    }
}


class InstanceEntry
{
public:
    InstanceEntry()
    : name_hash_()
    , type_hash_()
    , offset_()
    , size_()
    , name_()
    {
    }

    u32 name_hash_;
    u32 type_hash_;
    u32 offset_;
    u32 size_;
    StringRef name_;

    void deserialize(DecaBufferFile & f, std::vector<StringRef> const & nt)
    {
        name_hash_ = f.read<u32>();
        type_hash_ = f.read<u32>();
        offset_ = f.read<u32>();
        size_ = f.read<u32>();
        name_ = nt.at(f.read<u64>());
    }

    void read_instance(DecaBufferFile & f, std::map<u32, TypeDef> const & map_typedef)
    {
        f.pos_seek(offset_);
        DecaBufferFile f_instance(f.beg_ + offset_, f.beg_ + offset_ + size_);
        ::read_instance(f_instance, map_typedef, type_hash_);
    }

};

class AdfContainer {
public:
    AdfContainer()
    {
    }

    u32 magic_;
    u32 version_;

    u32 instance_count_;
    u32 instance_offset_;

    u32 typedef_count_;
    u32 typedef_offset_;

    u32 string_hash_count_;
    u32 string_hash_offset_;

    u32 name_table_count_;
    u32 name_table_offset_;

    u32 total_size_;

    u32 unknown_000_;
    u32 unknown_001_;
    u32 unknown_002_;
    u32 unknown_003_;
    u32 unknown_004_;

    StringRef comment_;

    std::vector<StringRef> name_table_;
    std::map<u64, StringRef> map_string_hash_;
    std::map<u32, TypeDef> map_typedef_;
    std::vector<InstanceEntry> table_instance_;

};


bool read_file(AdfContainer & adf, DecaBufferFile & f)
{
    if(f.size() < 0x40)
    {
        db_warn("Error: FileTooShort");
        return false; // raise FileTooShort
    }

    adf.magic_ = f.read<u32>();

    if (adf.magic_ != 'ADF ')
    {
        db_warn("Error: Magic does not match");
        return false;
    }

    adf.version_ = f.read<u32>();

    adf.instance_count_ = f.read<u32>();
    adf.instance_offset_ = f.read<u32>();

    adf.typedef_count_ = f.read<u32>();
    adf.typedef_offset_ = f.read<u32>();

    adf.string_hash_count_ = f.read<u32>();
    adf.string_hash_offset_ = f.read<u32>();

    adf.name_table_count_ = f.read<u32>();
    adf.name_table_offset_ = f.read<u32>();

    adf.total_size_ = f.read<u32>();

    adf.unknown_000_ = f.read<u32>();
    adf.unknown_001_ = f.read<u32>();
    adf.unknown_002_ = f.read<u32>();
    adf.unknown_003_ = f.read<u32>();
    adf.unknown_004_ = f.read<u32>();

    adf.comment_ = f.read_strz();  // comment is a zero delimited string

    // name table
    f.pos_seek(adf.name_table_offset_);
    std::vector<u8> name_table_sz(adf.name_table_count_);
    for(size_t i = 0; i < adf.name_table_count_; ++i)
    {
        name_table_sz[i] = f.read<u8>();
    }

    adf.name_table_.resize(adf.name_table_count_);
    for(size_t i = 0; i < adf.name_table_count_; ++i)
    {
        adf.name_table_[i] = f.read_strn(name_table_sz[i] + 1, true);
    }

    // string hash
    f.pos_seek(adf.string_hash_offset_);
    for(size_t i = 0; i < adf.string_hash_count_; ++i)
    {
        StringRef const s{f.read_strz()};
        u64 const h = f.read<u64>();
        adf.map_string_hash_[h] = s;
        hash_register(h, s);
    }

    // typedef
    f.pos_seek(adf.typedef_offset_);
    for(size_t i = 0; i < adf.typedef_count_; ++i)
    {
        TypeDef td;
        td.deserialize(f, adf.name_table_);
        adf.map_typedef_[td.type_hash_] = td;
    }

    // instance
    f.pos_seek(adf.instance_offset_);
    adf.table_instance_.resize(adf.instance_count_);

    for(auto && i : adf.table_instance_) i.deserialize(f, adf.name_table_);

    for(auto && i : adf.table_instance_) i.read_instance(f, adf.map_typedef_);

    return true;
}


extern "C" {
    EMSCRIPTEN_KEEPALIVE
    bool process_adf(c8 const* buffer, u64 buffer_sz)
    {
        AdfContainer adf;
        DecaBufferFile f(buffer, buffer + buffer_sz);
        return read_file(adf, f);
    }
}
