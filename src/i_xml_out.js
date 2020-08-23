mergeInto(LibraryManager.library, {
    xml_w_element_begin: function(file_hnd, offset, sz) {
        const str_raw = new Uint8Array(Module.HEAPU8.buffer, offset, sz);
        const str = new TextDecoder("utf-8").decode(str_raw)
        console.log('xml_w_element_begin:', str);
    },
    xml_w_element_end: function(file_hnd, offset, sz) {
        const str_raw = new Uint8Array(Module.HEAPU8.buffer, offset, sz);
        const str = new TextDecoder("utf-8").decode(str_raw)
        console.log('xml_w_element_end:', str);
    },
    xml_w_element_close: function(file_hnd, offset, sz) {
        const str_raw = new Uint8Array(Module.HEAPU8.buffer, offset, sz);
        const str = new TextDecoder("utf-8").decode(str_raw)
        console.log('xml_w_element_close:', str);
    },

    xml_w_value_set: function(file_hnd, offset, sz) {
        const str_raw = new Uint8Array(Module.HEAPU8.buffer, offset, sz);
        const str = new TextDecoder("utf-8").decode(str_raw)
        console.log('xml_w_value_set:', str);
    },

    xml_w_attr_set: function(file_hnd, id_offset, id_sz, value_offset, value_sz) {
        const id_raw = new Uint8Array(Module.HEAPU8.buffer, id_offset, id_sz);
        const id_str = new TextDecoder("utf-8").decode(id_raw)
        const value_raw = new Uint8Array(Module.HEAPU8.buffer, value_offset, value_sz);
        const value_str = new TextDecoder("utf-8").decode(value_raw)
        console.log('xml_w_attr_set:', id_str, value_str);
    },
});