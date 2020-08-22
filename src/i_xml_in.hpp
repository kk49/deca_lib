#pragma once
#include "basedef.hpp"
#include "i_base.hpp"
#include "i_io.hpp"
#include <map>
#include <list>
#include <memory>
#include <cassert>


class XmlFormat;
class XmlFile;
class XmlProcessor;

class XmlFormatContext {
public:
    XmlFormatContext()
    : element_depth_(0)
    , child_()
    {
    }

    virtual ~XmlFormatContext() = default;

    void element_begin(XmlProcessor & processor, XmlFile & file, StringRef const& name) {
        if(child_ == nullptr) {
            element_begin_work(processor, file, name);
        } else {
            ++element_depth_;
            child_->element_begin(processor, file, name);
        }
    }

    void element_end(XmlProcessor & processor, XmlFile & file, StringRef const& name) {
        if(child_ == nullptr) {
            element_end_work(processor, file, name);
        } else {
            child_->element_end(processor, file, name);
        }
    }

    void element_close(XmlProcessor & processor, XmlFile & file, StringRef const& name) {
        if(element_depth_ == 0 && child_ != nullptr)
            child_.reset();

        if(child_ == nullptr) {
            element_close_work(processor, file, name);
        } else {
            child_->element_close(processor, file, name);
            --element_depth_;  //we are guaranteed that element_depth_ != 0
        }
    }

    void attr(XmlProcessor & processor, XmlFile & file, StringRef const& id, StringRef const& value)  {
        if(child_ == nullptr) {
            attr_work(processor, file, id, value);
        } else {
            child_->attr(processor, file, id, value);
        }
    }

    void value(XmlProcessor & processor, XmlFile & file, StringRef const& value) {
        if(child_ == nullptr) {
            value_work(processor, file, value);
        } else {
            child_->value(processor, file, value);
        }
    }

    void comment(XmlProcessor & processor, XmlFile & file, StringRef const& value) {
        if(child_ == nullptr) {
            comment_work(processor, file, value);
        } else {
            child_->comment(processor, file, value);
        }
    }

protected:
    virtual void element_begin_work(XmlProcessor & processor, XmlFile & file, StringRef const& name) = 0;
    virtual void element_end_work(XmlProcessor & processor, XmlFile & file, StringRef const& name) = 0;
    virtual void element_close_work(XmlProcessor & processor, XmlFile & file, StringRef const& name) = 0;
    virtual void attr_work(XmlProcessor & processor, XmlFile & file, StringRef const& id, StringRef const& value) = 0;
    virtual void value_work(XmlProcessor & processor, XmlFile & file, StringRef const& value) = 0;
    virtual void comment_work(XmlProcessor & processor, XmlFile & file, StringRef const& value) = 0;

protected:
    s32 element_depth_;
    std::shared_ptr<XmlFormatContext> child_;
};

typedef XmlFormatContext*(*XmlFormatContextFactory)();

class XmlFormatContainer
: public XmlFormatContext {
public:

    XmlFormatContainer()
    : is_container_(false)
    , format_()
    {}

    std::map<std::string, XmlFormatContextFactory> format_map_;

    bool is_container_;
    StringRef format_;

    void element_begin_work(XmlProcessor & processor, XmlFile & file, StringRef const& name) override
    {
        assert(!is_container_);
        assert(name == "container");

        is_container_ = true;
    }

    void element_end_work(XmlProcessor & processor, XmlFile & file, StringRef const& name) override
    {
        assert(is_container_);
        assert(name == "container");

        auto it = format_map_.find(to_string(format_));
        if(format_map_.end() == it)
            db_exception(std::stringstream() << "Unknown container format = " << to_string(format_));
        else
            child_.reset(it->second());
    }

    void element_close_work(XmlProcessor & processor, XmlFile & file, StringRef const& name) override
    {
        assert(is_container_);
        assert(name == "container");
        is_container_ = false;
    }

    void attr_work(XmlProcessor & processor, XmlFile & file, StringRef const& id, StringRef const& value) override
    {
        assert(is_container_);
        assert(id == "format");
        format_ = value;
    }

    void value_work(XmlProcessor & processor, XmlFile & file, StringRef const& value) override
    {
        assert(false); // should not get here
    }

    void comment_work(XmlProcessor & processor, XmlFile & file, StringRef const& value) override
    {

    }

};

//! handle the processing of an xml file, opened with xml_r_open, ...
class XmlFile {
public:
    XmlFile() = delete;

    explicit XmlFile(XmlFormatContext * context)
    : context_(context)
    {
    }

    std::shared_ptr<XmlFormatContext> context_;

};

//! singleton that will handle different format types
class XmlProcessor {
public:
    XmlProcessor()
    : formats_()
    , files_()
    , file_hnd_tracker_(0)
    {

    }

    std::map< std::string, std::shared_ptr<XmlFormat> > formats_;
    std::map< FileHnd, std::shared_ptr<XmlFile> > files_;
    FileHnd file_hnd_tracker_;

    FileHnd file_open()
    {
        while(files_.find(file_hnd_tracker_) != files_.end()) ++file_hnd_tracker_;

        FileHnd const file_hnd = file_hnd_tracker_;
        files_[file_hnd] = std::make_shared<XmlFile>(new XmlFormatContainer());

        ++file_hnd_tracker_;

        return file_hnd;
    }

    void file_close(FileHnd hnd)
    {
        auto it = files_.find(hnd);
        if(files_.end() != it)
        {
            files_.erase(it);
        }
    }

    void element_begin(FileHnd const hnd, StringRef const& name)
    {
        auto it = files_.find(hnd);
        if(files_.end() != it) {
            XmlFile & file = *(it->second);

            if(file.context_ != nullptr)
                file.context_->element_begin(*this, file, name);
            else
                db_exception(std::stringstream() << "Unhandled element_begin: " << to_string(name));
        }
    }

    void element_end(FileHnd const hnd, StringRef const& name)
    {
        auto it = files_.find(hnd);
        if(files_.end() != it) {
            XmlFile & file = *(it->second);

            if(file.context_ != nullptr)
                file.context_->element_end(*this, file, name);
            else
                db_exception(std::stringstream() << "Unhandled element_end: " << to_string(name));
        }
    }

    void element_close(FileHnd const hnd, StringRef const& name)
    {
        auto it = files_.find(hnd);
        if(files_.end() != it) {
            XmlFile & file = *(it->second);

            if(file.context_ != nullptr)
                file.context_->element_end(*this, file, name);
            else
                db_exception(std::stringstream() << "Unhandled element_close: " << to_string(name));
        }
    }

    void attr(FileHnd const hnd, StringRef const& id, StringRef const& value)
    {
        auto it = files_.find(hnd);
        if(files_.end() != it) {
            XmlFile & file = *(it->second);

            if(file.context_ != nullptr)
                file.context_->attr(*this, file, id, value);
            else
                db_exception(std::stringstream() << "Unhandled attr: " << to_string(id) << ", " << to_string(value));
        }
    }

    void value(FileHnd const hnd, StringRef const& value)
    {
        auto it = files_.find(hnd);
        if(files_.end() != it) {
            XmlFile & file = *(it->second);

            if(file.context_ != nullptr)
                file.context_->value(*this, file, value);
            else
                db_exception(std::stringstream() << "Unhandled value: " << to_string(value));
        }
    }

    void comment(FileHnd const hnd, StringRef const& value)
    {
        auto it = files_.find(hnd);
        if(files_.end() != it) {
            XmlFile & file = *(it->second);

            if(file.context_ != nullptr)
                file.context_->comment(*this, file, value);
            else
                db_exception(std::stringstream() << "Unhandled comment: " << to_string(value));
        }
    }


    // simple singleton
    static XmlProcessor instance_static;
    static XmlProcessor & instance() {
        return instance_static;
    }
};




// expected call order
// xml_r_open E* xml_r_close
// E := xml_r_element_begin xml_r_attr* E* xml_r_value? xml_r_element_close
// E := xml_r_comment
extern "C" {
    void xml_process(FileHnd hnd_input, FileHnd hnd_output);

}


