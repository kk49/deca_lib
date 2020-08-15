mergeInto(LibraryManager.library, {
    db_print: function(offset, sz) {
        const str_raw = new Uint8Array(Module.HEAPU8.buffer, offset, sz);
        const str = new TextDecoder("utf-8").decode(str_raw)
        console.log('db_print:', str);
    },

    db_warn: function(offset, sz) {
        const str_raw = new Uint8Array(Module.HEAPU8.buffer, offset, sz);
        const str = new TextDecoder("utf-8").decode(str_raw)
        console.warn('db_warn:', str);
    },

    db_error: function(offset, sz) {
        const str_raw = new Uint8Array(Module.HEAPU8.buffer, offset, sz);
        const str = new TextDecoder("utf-8").decode(str_raw)
        console.error('db_error:', str);
    },
});