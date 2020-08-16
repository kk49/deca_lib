#include "../basedef.hpp"
#include "../hashes.hpp"
#include "../i_base.hpp"
#include "../i_data_stack.hpp"
#include "../i_xml.hpp"
#include <emscripten.h>
#include <sstream>
#include <iostream>
#include <list>

namespace Rtpc {
    enum PropType : u8 {
        k_type_none = 0,
        k_type_u32 = 1,
        k_type_f32 = 2,
        k_type_str = 3,
        k_type_vec2 = 4,
        k_type_vec3 = 5,
        k_type_vec4 = 6,
        k_type_mat3x3 = 7  // DEPRECIATED?
        ,
        k_type_mat4x4 = 8,
        k_type_array_u32 = 9,
        k_type_array_f32 = 10,
        k_type_array_u8 = 11,
        k_type_depreciated_12 = 12,
        k_type_objid = 13,
        k_type_array_event = 14
    };

    c8 const *to_string(PropType pt) {
        switch (pt) {
            case k_type_none:
                return "none";
            case k_type_u32:
                return "u32";
            case k_type_f32:
                return "f32";
            case k_type_str:
                return "str";
            case k_type_vec2:
                return "vec2";
            case k_type_vec3:
                return "vec3";
            case k_type_vec4:
                return "vec4";
            case k_type_mat3x3:
                return "mat3x3";
            case k_type_mat4x4:
                return "mat4x4";
            case k_type_array_u32:
                return "u32s";
            case k_type_array_f32:
                return "f32s";
            case k_type_array_u8:
                return "u8s";
            case k_type_depreciated_12:
                return "d12";
            case k_type_objid:
                return "objid";
            case k_type_array_event:
                return "events";
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

        c8 const *META_data_pointer(c8 const *beg) const {
            switch (this->type_) {
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

        u32 META_data_count(c8 const *beg) const {
            switch (this->type_) {
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
                    return *(u32 const *) (beg + this->data_raw_);
            }
        }
    };

    struct Node {
        u32 META_pos_;
        u32 name_hash_;
        u32 data_offset_;
        u16 prop_count_;
        u16 child_count_;
    };

    struct Container {
        u32 magic_;
        u32 version_;
    };

    class Visitor {
    public:
        virtual void visit_prop(DecaBufferFile &f, Property const &prop) {
            c8 const *const data_ptr = prop.META_data_pointer(f.beg_);
            u32 const data_cnt = prop.META_data_count(f.beg_);

            switch (prop.type_) {
                case k_type_array_u8:
                    this->visit_prop_data_u8(prop, data_cnt, (u8 const *) data_ptr);
                    break;
                case k_type_none:
                case k_type_u32:
                case k_type_array_u32:
                    this->visit_prop_data_u32(prop, data_cnt, (u32 const *) data_ptr);
                    break;
                case k_type_objid:
                case k_type_array_event:
                    this->visit_prop_data_u64(prop, data_cnt, (u64 const *) data_ptr);
                    break;
                case k_type_f32:
                case k_type_vec2:
                case k_type_vec3:
                case k_type_vec4:
                case k_type_mat3x3:
                case k_type_mat4x4:
                case k_type_array_f32:
                    this->visit_prop_data_f32(prop, data_cnt, (f32 const *) data_ptr);
                    break;
                case k_type_str:
                    this->visit_prop_data_strz(prop, data_ptr);
                    break;
                default:
                    db_exception(std::stringstream() << "NOT HANDLED " << prop.type_);
            }
        }

        virtual void visit_prop_data_strz(Property const &prop, c8 const *ptr) = 0;

        virtual void visit_prop_data_u8(Property const &prop, u32 n, u8 const *ptr) = 0;

        virtual void visit_prop_data_u32(Property const &prop, u32 n, u32 const *ptr) = 0;

        virtual void visit_prop_data_u64(Property const &prop, u32 n, u64 const *ptr) = 0;

        virtual void visit_prop_data_f32(Property const &prop, u32 n, f32 const *ptr) = 0;

        virtual void visit_node_begin(DecaBufferFile &f, Node const &node) = 0;

        virtual void visit_node_end(DecaBufferFile &f, Node const &node) = 0;

        virtual void visit_node_prop_begin(DecaBufferFile &f, Node const &node) = 0;

        virtual void visit_node_prop_end(DecaBufferFile &f, Node const &node) = 0;

        virtual void visit_node_children_begin(DecaBufferFile &f, Node const &node) = 0;

        virtual void visit_node_children_end(DecaBufferFile &f, Node const &node) = 0;

        virtual void visit_container_begin(DecaBufferFile &f, Container const &container) = 0;

        virtual void visit_container_end(DecaBufferFile &f, Container const &container) = 0;

        virtual Property &prop_table_ref(u32 index) = 0;

        virtual Node &child_table_ref(u32 index) = 0;

        virtual Node &root_ref() = 0;
    };

    void prop_deserialize(DecaBufferFile &f, Property &prop, Visitor &visitor) {
        prop.META_pos_ = f.pos_peek();
        prop.name_hash_ = f.read<u32>();
        prop.data_raw_ = f.read<u32>();
        prop.type_ = (PropType) f.read<u8>();
        visitor.visit_prop(f, prop);
    }

    void node_deserialize(DecaBufferFile &f, Node &node, Visitor &visitor) {
        node.META_pos_ = f.pos_peek();
        node.name_hash_ = f.read<u32>();
        node.data_offset_ = f.read<u32>();
        node.prop_count_ = f.read<u16>();
        node.child_count_ = f.read<u16>();

        StorePos old_p(f);

        visitor.visit_node_begin(f, node);

        f.pos_seek(node.data_offset_);

        // read properties
        visitor.visit_node_prop_begin(f, node);
        for (size_t i = 0; i < node.prop_count_; ++i)
            prop_deserialize(f, visitor.prop_table_ref(i), visitor);
        visitor.visit_node_prop_end(f, node);

        //  children 4-byte aligned
        size_t pos = f.pos_peek();
        f.pos_seek(pos + (4 - (pos % 4)) % 4);

        // read children
        visitor.visit_node_children_begin(f, node);
        for (size_t i = 0; i < node.child_count_; ++i)
            node_deserialize(f, visitor.child_table_ref(i), visitor);
        visitor.visit_node_children_end(f, node);

        visitor.visit_node_end(f, node);
    }

    bool file_deserialize(DecaBufferFile &f, Container &container, Visitor &visitor) {
        container.magic_ = f.read<u32>();

        if (container.magic_ != 'CPTR')
            db_exception(std::stringstream() << "Bad MAGIC " << to_hex(container.magic_, 8));

        container.version_ = f.read<u32>();

        visitor.visit_container_begin(f, container);

        node_deserialize(f, visitor.root_ref(), visitor);

        visitor.visit_container_end(f, container);

        return true;
    }

    struct PropertyStorage : public Property {

    };

    struct NodeStorage : public Node {
        std::vector<Property> prop_table_;
        std::vector<Node> child_table_;
    };

    struct ContainerStorage : public Container {
        Node root_node_;
    };

    class VisitorDataStack
    : public Visitor
    {
    public:
        VisitorDataStack()
        : working_property_()
        , node_stack_()
        {
        }

        Property working_property_;
        std::list<Node> node_stack_;

        void node_push() { this->node_stack_.emplace_back(Node()); }
        void node_pop() { this->node_stack_.pop_back(); }
        Node & node_bot() { return *this->node_stack_.begin(); }
        Node & node_top() { return *this->node_stack_.rbegin(); }

        void visit_prop_data_strz(Property const & prop, c8 const* ptr) override {
//            db_print("visit_prop_data_strz");
            u32_push(prop.name_hash_);
            str_push(ptr);
            dict_field_set();
        }

        void visit_prop_data_u8(Property const & prop, u32 n, u8 const* ptr) override {
//            db_print("visit_prop_data_u8");
            u32_push(prop.name_hash_);
            u8s_push(ptr, n);
            dict_field_set();
        }

        void visit_prop_data_u32(Property const & prop, u32 n, u32 const* ptr) override {
//            db_print("visit_prop_data_u32");
            u32_push(prop.name_hash_);
            u32s_push(ptr, n);
            dict_field_set();
        }

        void visit_prop_data_u64(Property const & prop, u32 n, u64 const* ptr) override {
//            db_print("visit_prop_data_strz");
            u32_push(prop.name_hash_);
            u64s_push(ptr, n);
            dict_field_set();
        }

        void visit_prop_data_f32(Property const & prop, u32 n, f32 const* ptr) override {
//            db_print("visit_prop_data_f32");
            u32_push(prop.name_hash_);
            f32s_push(ptr, n);
            dict_field_set();
        }

        void visit_node_begin(DecaBufferFile & f, Node const & node) override {
//            db_print("visit_node_begin");
            dict_push();
            node_push();
        }
        void visit_node_end(DecaBufferFile & f, Node const & node) override {
//            db_print("visit_node_end");
            list_append();
            node_pop();
        }

        void visit_node_prop_begin(DecaBufferFile & f, Node const & node) override
        {
//            db_print("visit_node_prop_begin");
            str_push("properties");
            dict_push();
        }
        void visit_node_prop_end(DecaBufferFile & f, Node const & node) override
        {
//            db_print("visit_node_prop_end");
            dict_field_set();
        }
        void visit_node_children_begin(DecaBufferFile & f, Node const & node) override
        {
//            db_print("visit_node_children_begin");
            str_push("children");
            list_push();
        }
        void visit_node_children_end(DecaBufferFile & f, Node const & node) override
        {
//            db_print("visit_node_children_end");
            dict_field_set();
        }

        void visit_container_begin(DecaBufferFile & f, Container const & container) override {
//            db_print("visit_container_begin");
            list_push();
            node_push();
        }

        void visit_container_end(DecaBufferFile & f, Container const & container) override {
//            db_print("visit_container_end");
            node_pop();
        }

        Property & prop_table_ref(u32 index) override {
//            db_print("prop_table_ref");
            return this->working_property_;
        }

        Node & child_table_ref(u32 index) override {
//            db_print("child_table_ref");
            return node_top();
        }

        Node & root_ref() override {
//            db_print("root_ref");
            return node_bot();
        }
    };

    template<typename T_> struct XmlSerializeType { typedef T_ Type; };
    template<> struct XmlSerializeType<u8> { typedef u32 Type; };

    class VisitorXml
    : public Visitor
    {
    public:
        VisitorXml(FileHnd file_hnd)
        : file_hnd_(file_hnd)
        , node_stack_()
        , working_property_()
        {}

        FileHnd file_hnd_;
        Property working_property_;

//        std::vector<std::shared_ptr<Node>> node_stack_;
        std::list<Node> node_stack_;

        void node_push() { this->node_stack_.emplace_back(Node()); }
        void node_pop() { this->node_stack_.pop_back(); }
        Node & node_bot() { return *this->node_stack_.begin(); }
        Node & node_top() { return *this->node_stack_.rbegin(); }

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

        void visit_node_begin(DecaBufferFile & f, Node const & node) override {
            xml_w_element_begin(file_hnd_, "object");
            xml_w_attr_set(file_hnd_, "id", "0x" + to_hex(node.name_hash_, 8));
            node_push();
        }
        void visit_node_end(DecaBufferFile & f, Node const & node) override {
            xml_w_element_end(file_hnd_, "object");
            node_pop();
        }

        void visit_node_prop_begin(DecaBufferFile & f, Node const & node) override {}
        void visit_node_prop_end(DecaBufferFile & f, Node const & node) override {}
        void visit_node_children_begin(DecaBufferFile & f, Node const & node) override {}
        void visit_node_children_end(DecaBufferFile & f, Node const & node) override {}

        void visit_container_begin(DecaBufferFile & f, Container const & container) override {
            xml_w_element_begin(file_hnd_, "container");
            xml_w_attr_set(file_hnd_, "format", "deca.rtpc");
            node_push();
        }

        void visit_container_end(DecaBufferFile & f, Container const & container) override {
            xml_w_element_end(file_hnd_, "container");
            node_pop();
        }

        Property & prop_table_ref(u32 index) override {
            return this->working_property_;
        }

        Node & child_table_ref(u32 index) override {
            return node_top();
        }

        Node & root_ref() override {
            return node_bot();
        }
    };
}

extern "C" {

EMSCRIPTEN_KEEPALIVE
bool process_rtpc(FileHnd file_hnd, c8 const* buffer, u64 buffer_sz)
{
    DecaBufferFile f(buffer, buffer + buffer_sz);
    Rtpc::Container rtpc;
    Rtpc::VisitorXml visitor(file_hnd);
//    Rtpc::VisitorDataStack visitor{};

    if(!Rtpc::file_deserialize(f, rtpc, visitor))
        return false;

    return true;
}

}