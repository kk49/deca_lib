#include "../basedef.hpp"
#include "../hashes.hpp"
#include "../i_base.hpp"
#include "../i_data_stack.hpp"
#include "../i_xml.hpp"
#include <emscripten.h>
#include <sstream>
#include <iostream>

namespace Rtpc {
    enum PropType: u8 {
        k_type_none = 0
        , k_type_u32 = 1
        , k_type_f32 = 2
        , k_type_str = 3
        , k_type_vec2 = 4
        , k_type_vec3 = 5
        , k_type_vec4 = 6
        , k_type_mat3x3 = 7  // DEPRECIATED?
        , k_type_mat4x4 = 8
        , k_type_array_u32 = 9
        , k_type_array_f32 = 10
        , k_type_array_u8 = 11
        , k_type_depreciated_12 = 12
        , k_type_objid = 13
        , k_type_array_event = 14
    };

    c8 const* to_string(PropType pt) {
        switch(pt)
        {
            case k_type_none: return "none";
            case k_type_u32: return "u32";
            case k_type_f32: return "f32";
            case k_type_str: return "str";
            case k_type_vec2: return "vec2";
            case k_type_vec3: return "vec3";
            case k_type_vec4: return "vec4";
            case k_type_mat3x3: return "mat3x3";
            case k_type_mat4x4: return "mat4x4";
            case k_type_array_u32: return "u32s";
            case k_type_array_f32: return "f32s";
            case k_type_array_u8: return "u8s";
            case k_type_depreciated_12: return "d12";
            case k_type_objid: return "objid";
            case k_type_array_event: return "events";
        }
    }


    u32 static const h_prop_class = hash32_func("_class");
    u32 static const h_prop_class_hash = hash32_func("_class_hash");
    u32 static const h_prop_name = hash32_func("name");
    u32 static const h_prop_world = hash32_func("world");
    u32 static const h_prop_script = hash32_func("script");
    u32 static const h_prop_border = hash32_func("border");
    u32 static const h_prop_object_id = hash32_func("_object_id");
    u32 static const h_prop_label_key = hash32_func("label_key");
    u32 static const h_prop_note = hash32_func("note");
    u32 static const h_prop_spline = hash32_func("spline");
    u32 static const h_prop_spawn_tags = hash32_func("spawn_tags");
    u32 static const h_prop_model_skeleton = hash32_func("model_skeleton");
    u32 static const h_prop_skeleton = hash32_func("skeleton");
    u32 static const h_prop_need_type = hash32_func("need_type");
    u32 static const h_prop_start_time = hash32_func("start_time");

    u32 static const h_prop_item_item_id = hash32_func("[Item]  Item ID");
    u32 static const h_prop_ref_apex_identifier = hash32_func("[ref] apex identifier");

// guess at naming these fields
    u32 static const h_prop_deca_crafting_type = 0xa949bc65;
    u32 static const h_prop_deca_cpoi_desc = 0xe6b6b3f9;


    struct Property {
        u32 META_pos_;
        u32 name_hash_;
        u32 data_raw_;
        PropType type_;

        c8 const * META_data_pointer(c8 const * beg) const
        {
            switch(this->type_) {
                case k_type_none:
                case k_type_u32:
                case k_type_f32:
                case k_type_depreciated_12:
                    return beg + this->META_pos_ + 4;
                case k_type_str:
                case k_type_vec2:
                case k_type_vec3:
                case k_type_vec4:
                case k_type_mat3x3:
                case k_type_mat4x4:
                case k_type_objid:
                    return beg + this->data_raw_;
                case k_type_array_u32:
                case k_type_array_f32:
                case k_type_array_u8:
                case k_type_array_event:
                    return beg + this->data_raw_ + 4;
            }
        }

        u32 META_data_count(c8 const * beg) const
        {
            switch(this->type_) {
                case k_type_none:
                case k_type_depreciated_12:
                case k_type_str:
                    return 0;
                case k_type_u32:
                case k_type_f32:
                case k_type_objid:
                    return 1;
                case k_type_vec2:
                    return 2;
                case k_type_vec3:
                    return 3;
                case k_type_vec4:
                    return 4;
                case k_type_mat3x3:
                    return 9;
                case k_type_mat4x4:
                    return 16;
                case k_type_array_u32:
                case k_type_array_f32:
                case k_type_array_u8:
                case k_type_array_event:
                    return *(u32 const *)(beg + this->data_raw_);
            }
        }
    };

    struct Node {
        u32 META_pos_;
        u32 name_hash_;
        u32 data_offset_;
        u16 prop_count_;
        u16 child_count_;
        std::vector<Property> prop_table_;
        std::vector<Node> child_table_;
    };

    struct Container {
        u32 magic_;
        u32 version_;
        Node root_node_;
    };
    
    class Visitor {
    public:
        virtual void visit_prop_data_strz(Property const & prop, c8 const* ptr) {}
        virtual void visit_prop_data_u8(Property const & prop, u32 n, u8 const* ptr) {}
        virtual void visit_prop_data_u32(Property const & prop, u32 n, u32 const* ptr) {}
        virtual void visit_prop_data_u64(Property const & prop, u32 n, u64 const* ptr) {}
        virtual void visit_prop_data_f32(Property const & prop, u32 n, f32 const* ptr) {}

        virtual void visit_node_begin(Node const & node) {}
        virtual void visit_node_end(Node const & node) {}
        virtual void visit_node_prop_begin(Node const & node) {}
        virtual void visit_node_prop_end(Node const & node) {}
        virtual void visit_node_children_begin(Node const & node) {}
        virtual void visit_node_children_end(Node const & node) {}

        virtual void visit_container_begin(Container const & container) {}
        virtual void visit_container_end(Container const & container) {}
    };

    void prop_deserialize(Property & prop, DecaBufferFile & f) {
        prop.META_pos_ = f.pos_peek();
        prop.name_hash_ = f.read<u32>();
        prop.data_raw_ = f.read<u32>();
        prop.type_ = (PropType) f.read<u8>();
    }

    void node_deserialize(Node & node, DecaBufferFile & f) {
        node.META_pos_ = f.pos_peek();
        node.name_hash_ = f.read<u32>();
        node.data_offset_ = f.read<u32>();
        node.prop_count_ = f.read<u16>();
        node.child_count_ = f.read<u16>();

        StorePos old_p(f);

        f.pos_seek(node.data_offset_);

        // read properties
        node.prop_table_.resize(node.prop_count_);
        for(auto && i : node.prop_table_)
            prop_deserialize(i, f);

        //  children 4-byte aligned
        size_t pos = f.pos_peek();
        f.pos_seek(pos + (4 - (pos % 4)) % 4);

        // read children
        node.child_table_.resize(node.child_count_);
        for(auto && i : node.child_table_)
            node_deserialize(i, f);
    }

    bool file_deserialize(Container & rtpc, DecaBufferFile & f)
    {
        rtpc.magic_ = f.read<u32>();

        if(rtpc.magic_ != 'CPTR')
            db_exception(std::stringstream() << "Bad MAGIC " << to_hex(rtpc.magic_, 8));

        rtpc.version_ = f.read<u32>();

        node_deserialize(rtpc.root_node_, f);

        return true;
    }

    void visitor_prop_process(Property const & prop, DecaBufferFile & f, Visitor & visitor)
    {
        c8 const * const data_ptr = prop.META_data_pointer(f.beg_);
        u32 const data_cnt = prop.META_data_count(f.beg_);

        switch(prop.type_)
        {
            case k_type_array_u8:
                visitor.visit_prop_data_u8(prop, data_cnt, (u8 const*)data_ptr);
                break;
            case k_type_none:
            case k_type_u32:
            case k_type_array_u32:
                visitor.visit_prop_data_u32(prop, data_cnt, (u32 const*)data_ptr);
                break;
            case k_type_objid:
            case k_type_array_event:
                visitor.visit_prop_data_u64(prop, data_cnt, (u64 const*)data_ptr);
                break;
            case k_type_f32:
            case k_type_vec2:
            case k_type_vec3:
            case k_type_vec4:
            case k_type_mat3x3:
            case k_type_mat4x4:
            case k_type_array_f32:
                visitor.visit_prop_data_f32(prop, data_cnt, (f32 const*)data_ptr);
                break;
            case k_type_str:
                visitor.visit_prop_data_strz(prop, data_ptr);
                break;
            default:
               db_exception(std::stringstream() << "NOT HANDLED " << prop.type_);
        }
    }

    void visitor_node_process(Node const & node, DecaBufferFile & f, Visitor & visitor)
    {
        visitor.visit_node_begin(node);

        visitor.visit_node_prop_begin(node);
        for(auto && i : node.prop_table_)
            visitor_prop_process(i, f, visitor);
        visitor.visit_node_prop_end(node);

        visitor.visit_node_children_begin(node);
        for(auto && i : node.child_table_)
            visitor_node_process(i, f, visitor);
        visitor.visit_node_children_end(node);

        visitor.visit_node_end(node);
    }

    void visitor_file_process(Container const & container, DecaBufferFile & f, Visitor & visitor)
    {
        visitor.visit_container_begin(container);

        visitor_node_process(container.root_node_, f, visitor);

        visitor.visit_container_end(container);
    }

    template<typename T_> struct XmlSerializeType { typedef T_ Type; };
    template<> struct XmlSerializeType<u8> { typedef u32 Type; };

    class VisitorXml
    : public Visitor
    {
    public:
        VisitorXml(FileHnd file_hnd)
        : file_hnd_(file_hnd)
        {}

        FileHnd file_hnd_;

        template<typename T_>
        void visit_prop_data(Property const & prop, u32 n, T_ const* ptr)
        {
            xml_w_element_begin(file_hnd_, "value");
            xml_w_attr_set(file_hnd_, "id", std::stringstream() << "0x" << to_hex(prop.name_hash_, 8));
            xml_w_attr_set(file_hnd_, "type", to_string(prop.type_));

            std::stringstream ss;
            auto ptr_beg = ptr;
            auto ptr_end = ptr + n;

            for(;ptr < ptr_end; ++ptr)
            {
                if(ptr_beg != ptr)
                    ss << ",";
                ss << (typename XmlSerializeType<T_>::Type)*ptr;
            }
            xml_w_value_set(file_hnd_, ss.str());
            xml_w_element_end(file_hnd_, "value");
        }

        void visit_prop_data_strz(Property const & prop, c8 const* ptr) override {
            xml_w_element_begin(file_hnd_, "value");
            xml_w_attr_set(file_hnd_, "id", "0x" + to_hex(prop.name_hash_, 8));
            xml_w_attr_set(file_hnd_, "type", to_string(prop.type_));
            xml_w_value_set(file_hnd_, ptr);
            xml_w_element_end(file_hnd_, "value");
        }

        void visit_prop_data_u8(Property const & prop, u32 n, u8 const* ptr) override {
            this->visit_prop_data(prop, n, ptr);
        }

        void visit_prop_data_u32(Property const & prop, u32 n, u32 const* ptr) override {
            this->visit_prop_data(prop, n, ptr);
        }

        void visit_prop_data_u64(Property const & prop, u32 n, u64 const* ptr) override {
            this->visit_prop_data(prop, n, ptr);
        }

        void visit_prop_data_f32(Property const & prop, u32 n, f32 const* ptr) override {
            this->visit_prop_data(prop, n, ptr);
        }

        void visit_node_begin(Node const & node) override {
            xml_w_element_begin(file_hnd_, "object");
            xml_w_attr_set(file_hnd_, "id", "0x" + to_hex(node.name_hash_, 8));
        }
        void visit_node_end(Node const & node) override {
            xml_w_element_end(file_hnd_, "object");
        }

        void visit_node_prop_begin(Node const & node) override {}
        void visit_node_prop_end(Node const & node) override {}
        void visit_node_children_begin(Node const & node) override {}
        void visit_node_children_end(Node const & node) override {}

        void visit_container_begin(Container const & container) override {
            xml_w_element_begin(file_hnd_, "container");
            xml_w_attr_set(file_hnd_, "format", "deca.rtpc");
        }

        void visit_container_end(Container const & container) override {
            xml_w_element_end(file_hnd_, "container");
        }
    };
}

extern "C" {

EMSCRIPTEN_KEEPALIVE
bool process_rtpc(FileHnd file_hnd, c8 const* buffer, u64 buffer_sz)
{
    Rtpc::Container rtpc;
    DecaBufferFile f(buffer, buffer + buffer_sz);

    if(!Rtpc::file_deserialize(rtpc, f))
        return false;

    Rtpc::VisitorXml visitor(file_hnd);

    Rtpc::visitor_file_process(rtpc, f, visitor);

    return true;
}

}