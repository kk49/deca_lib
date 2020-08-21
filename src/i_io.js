mergeInto(LibraryManager.library, {
    file_open: function(path_ptr, path_sz, mode_ptr, mode_sz) {
        const path_raw = new Uint8Array(Module.HEAPU8.buffer, path_ptr, path_sz);
        const path = new TextDecoder("utf-8").decode(path_raw)
        const mode_raw = new Uint8Array(Module.HEAPU8.buffer, mode_ptr, mode_sz);
        const mode = new TextDecoder("utf-8").decode(mode_raw)
        console.log(`file_open(${path},${mode})`,);
    },

    file_close: function(file_hnd) {
        console.log(`file_close(${file_hnd})`);
    },

    file_size: function(file_hnd) {
        console.log(`file_size(${file_hnd})`);
        return 0;
    },

    file_write: function(file_hnd, pos, offset, sz) {
        const str_raw = new Uint8Array(Module.HEAPU8.buffer, offset, sz);
        const str = new TextDecoder("utf-8").decode(str_raw)
        console.log(`file_write(${file_hnd},${pos},${str})`);
    },

    file_read: function(file_hnd, pos, offset, sz) {
        const str_raw = new Uint8Array(Module.HEAPU8.buffer, offset, sz);
        const str = new TextDecoder("utf-8").decode(str_raw)
        console.log(`file_write(${file_hnd},${pos},${str})`);
    },

});