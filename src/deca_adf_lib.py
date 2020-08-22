import wasmer
import numpy as np
import sys
import io
import os
from collections import defaultdict
from xml.etree.ElementTree import Element, SubElement, Comment, tostring
import xml.etree.ElementTree as ET


class DecaAdfEnumValue:
    def __init__(self, value, value_str):
        self.value = value
        self.value_str = value_str


class DecaLibWasm:
    def __init__(self, *args, **kwargs):
        lib_env = {
            'file_open': self.file_open,
            'file_close': self.file_close,
            'file_size': self.file_size,
            'file_read': self.file_read,
            'file_write': self.file_write,

            # 'db_print': self.db_print,
            # 'db_warn': self.db_warn,
            # 'db_error': self.db_error,

            'dict_push': self.dict_push,
            'dict_field_set': self.dict_field_set,
            'list_push': self.list_push,
            'list_append': self.list_append,
            'hash_register': self.hash_register,

            'hash32_push': self.hash32_push,
            'hash48_push': self.hash48_push,
            'hash64_push': self.hash64_push,

            'bool_push': self.bool_push,
            's8_push': self.s8_push,
            'u8_push': self.u8_push,
            's16_push': self.s16_push,
            'u16_push': self.u16_push,
            's32_push': self.s32_push,
            'u32_push': self.u32_push,
            's64_push': self.s64_push,
            'u64_push': self.u64_push,
            'f32_push': self.f32_push,
            'f64_push': self.f64_push,

            'str_push': self.str_push,
            'enum_push': self.enum_push,

            's8s_push': self.s8s_push,
            'u8s_push': self.u8s_push,
            's16s_push': self.s16s_push,
            'u16s_push': self.u16s_push,
            's32s_push': self.s32s_push,
            'u32s_push': self.u32s_push,
            's64s_push': self.s64s_push,
            'u64s_push': self.u64s_push,
            'f32s_push': self.f32s_push,
            'f64s_push': self.f64s_push,

            'xml_w_element_begin': self.xml_w_element_begin,
            'xml_w_element_end': self.xml_w_element_end,
            'xml_w_attr_set': self.xml_w_attr_set,
            'xml_w_value_set': self.xml_w_value_set,

        }

        with open('deca_lib.wasm', 'rb') as f:
            wasm_bytes = f.read()

        self._module = wasmer.Module(wasm_bytes)
        if self._module.is_wasi_module:
            self._wasi = wasmer.Wasi('deca_lib_wasm')
            import_object = self._wasi.generate_import_object_for_module(self._module)
        else:
            self._wasi = None
            import_object = self._module.generate_import_object()

        env = {}
        for impt in self._module.imports:
            nm = impt['name']
            if nm in lib_env:
                env[nm] = lib_env[nm]

        import_object.extend({"env": env})

        self._instance = wasmer.Instance(wasm_bytes, import_object)

        self._instance.exports._initialize()

    def instance(self):
        return self._instance

    def file_open(self, path_ptr, path_sz, mode_ptr, mode_sz):
        pass

    def file_close(self, hnd):
        pass

    def file_size(self, hnd):
        pass

    def file_read(self, hnd, pos, ptr, sz):
        pass

    def file_write(self, hnd, pos, ptr, sz):
        pass

    # def db_print(self, offset, sz):
    #     pass
    #
    # def db_warn(self, offset, sz):
    #     pass
    #
    # def db_error(self, offset, sz):
    #     pass

    def dict_push(self):
        pass

    def dict_field_set(self):
        pass

    def list_push(self):
        pass

    def list_append(self):
        pass

    def hash_register(self, hash, offset, sz):
        pass

    def hash32_push(self, value):
        pass

    def hash48_push(self, value):
        pass

    def hash64_push(self, value):
        pass

    def bool_push(self, value):
        pass

    def s8_push(self, value):
        pass

    def u8_push(self, value):
        pass

    def s16_push(self, value):
        pass

    def u16_push(self, value):
        pass

    def s32_push(self, value):
        pass

    def u32_push(self, value):
        pass

    def s64_push(self, value):
        pass

    def u64_push(self, value):
        pass

    def f32_push(self, value):
        pass

    def f64_push(self, value):
        pass

    def str_push(self, offset, sz):
        pass

    def enum_push(self, value, offset, sz):
        pass

    def s8s_push(self, offset, cnt):
        pass

    def u8s_push(self, offset, cnt):
        pass

    def s16s_push(self, offset, cnt):
        pass

    def u16s_push(self, offset, cnt):
        pass

    def s32s_push(self, offset, cnt):
        pass

    def u32s_push(self, offset, cnt):
        pass

    def s64s_push(self, offset, cnt):
        pass

    def u64s_push(self, offset, cnt):
        pass

    def f32s_push(self, offset, cnt):
        pass

    def f64s_push(self, offset, cnt):
        pass

    def xml_w_element_begin(self, file_hnd, offset, sz):
        pass

    def xml_w_element_end(self, file_hnd, offset, sz):
        pass

    def xml_w_attr_set(self, file_hnd, id_offset, id_sz, value_offset, value_sz):
        pass

    def xml_w_value_set(self, file_hnd, offset, sz):
        pass


class DecaLibWasmStack(DecaLibWasm):
    def __init__(self, *args, **kwargs):
        self.adf_stack = []
        self.files_map = {}
        self.files_open = {}
        self.files_hnd = 0

        self.files_map['stdout'] = sys.stdout
        self.files_map['stderr'] = sys.stderr
        self.files_map['stdin'] = sys.stdin

        super().__init__(*args, **kwargs)

    def xml_parse(self, fn, fn_out):

        with open(fn, 'rb') as f:
            buf_in = f.read()

        self.files_map['xml_in'] = buf_in

        hnd_in = self.file_open_local('xml_in', '')
        hnd_out = self.file_open_local('xml_out', '')

        self._instance.exports.xml_process(hnd_in, hnd_out)

        with open(fn_out, 'wb') as f:
            f.write(self.files_open[hnd_out])

        self.file_close(hnd_out)
        self.file_close(hnd_in)

    def file_open_local(self, path, mode):
        # print(f'file_open({path},{mode})')

        if path in self.files_map:
            file = self.files_map[path]
        else:
            file = bytearray()
            self.files_map[path] = file

        hnd = self.files_hnd
        self.files_hnd += 1
        self.files_open[hnd] = file
        return hnd

    def file_open(self, path_ptr, path_sz, mode_ptr, mode_sz):
        path = memoryview(self._instance.memory.buffer)[path_ptr:path_ptr + path_sz]
        path = bytes(path).decode('utf-8')
        mode = memoryview(self._instance.memory.buffer)[mode_ptr:mode_ptr + mode_sz]
        mode = bytes(mode).decode('utf-8')
        return self.file_open_local(path, mode)

    def file_close(self, hnd):
        # print(f'file_close({hnd})')
        if hnd in self.files_open:
            self.files_open.pop(hnd, None)

    def file_size(self, hnd):
        # print(f'file_size({hnd})')
        if hnd in self.files_open:
            file = self.files_open.get(hnd)
            if file is bytearray:
                return len(file)
            elif isinstance(file, io.TextIOBase):
                return 0

        return -1

    def file_read(self, hnd, pos, ptr, sz):
        # print(f'file_read({hnd}, {pos}, {ptr}, {sz})')
        if hnd in self.files_open:
            file = self.files_open[hnd]
            sz_final = min(sz, len(file)-pos)
            if sz_final > 0:
                input_wasm = self._instance.memory.uint8_view(ptr)
                input_wasm[:sz_final] = file[pos:pos+sz_final]
            return sz_final
        else:
            return -1

    def file_write(self, hnd, pos, ptr, sz):
        buffer = memoryview(self._instance.memory.buffer)[ptr:ptr + sz]
        buffer = bytes(buffer)

        if hnd in self.files_open:
            file = self.files_open[hnd]
            if file is bytearray or file is bytes:
                if pos > len(file) or pos < 0:
                    return -1

                dsz = (pos+sz) - len(file)
                if dsz > 0:
                    file[pos:pos+(sz-dsz)] = buffer[0:(sz-dsz)]
                    file.extend(buffer[(sz-dsz):])
                else:
                    file[pos:pos+sz] = buffer[:]

                return sz
            elif isinstance(file, io.TextIOBase):
                file.write(buffer.decode('utf-8'))
                return sz
            else:
                return -1
        else:
            return -1

    # def db_print(self, offset, sz):
    #     v = memoryview(self._instance.memory.buffer)[offset:offset + sz]
    #     v = bytes(v)
    #     print(f'db_print({offset}, {sz}) == {v}')
    #
    # def db_warn(self, offset, sz):
    #     v = memoryview(self._instance.memory.buffer)[offset:offset + sz]
    #     v = bytes(v)
    #     print(f'db_warn({offset}, {sz}) == {v}', file=sys.stderr)
    #
    # def db_error(self, offset, sz):
    #     print(self.adf_stack)
    #     v = memoryview(self._instance.memory.buffer)[offset:offset + sz]
    #     v = bytes(v)
    #     print(f'db_error({offset}, {sz}) == {v}', file=sys.stderr)

    def dict_push(self):
        self.adf_stack.append({})

    def dict_field_set(self):
        self.adf_stack[-3][self.adf_stack[-2]] = self.adf_stack[-1]
        self.adf_stack.pop()
        self.adf_stack.pop()

    def list_push(self):
        self.adf_stack.append([])

    def list_append(self):
        self.adf_stack[-2].append(self.adf_stack[-1])
        self.adf_stack.pop()

    def hash_register(self, hash, offset, sz):
        v = memoryview(self._instance.memory.buffer)[offset:offset + sz]
        v = bytes(v)
        print(f'hash_register({hash:016x}, {offset}, {sz}) == {v}')

    def hash32_push(self, value):
        self.adf_stack.append(value)

    def hash48_push(self, value):
        self.adf_stack.append(value)

    def hash64_push(self, value):
        self.adf_stack.append(value)

    def bool_push(self, value):
        self.adf_stack.append(value)

    def s8_push(self, value):
        self.adf_stack.append(value)

    def u8_push(self, value):
        self.adf_stack.append(value)

    def s16_push(self, value):
        self.adf_stack.append(value)

    def u16_push(self, value):
        self.adf_stack.append(value)

    def s32_push(self, value):
        self.adf_stack.append(value)

    def u32_push(self, value):
        self.adf_stack.append(value)

    def s64_push(self, value):
        self.adf_stack.append(value)

    def u64_push(self, value):
        self.adf_stack.append(value)

    def f32_push(self, value):
        self.adf_stack.append(value)

    def f64_push(self, value):
        self.adf_stack.append(value)

    def str_push(self, offset, sz):
        value = memoryview(self._instance.memory.buffer)[offset:offset + sz]
        value = bytes(value).decode('utf-8')
        self.adf_stack.append(value)

    def enum_push(self, value, offset, sz):
        value_str = memoryview(self._instance.memory.buffer)[offset:offset + sz]
        value_str = bytes(value_str).decode('utf-8')
        self.adf_stack.append(DecaAdfEnumValue(value, value_str))

    def s8s_push(self, offset, cnt):
        value = memoryview(self._instance.memory.buffer)[offset:offset + cnt * 1]
        value = np.frombuffer(value, np.int8)
        self.adf_stack.append(value)

    def u8s_push(self, offset, cnt):
        value = memoryview(self._instance.memory.buffer)[offset:offset + cnt * 1]
        value = np.frombuffer(value, np.uint8)
        self.adf_stack.append(value)

    def s16s_push(self, offset, cnt):
        value = memoryview(self._instance.memory.buffer)[offset:offset + cnt * 2]
        value = np.frombuffer(value, np.int16)
        self.adf_stack.append(value)

    def u16s_push(self, offset, cnt):
        value = memoryview(self._instance.memory.buffer)[offset:offset + cnt * 2]
        value = np.frombuffer(value, np.uint16)
        self.adf_stack.append(value)

    def s32s_push(self, offset, cnt):
        value = memoryview(self._instance.memory.buffer)[offset:offset + cnt * 4]
        value = np.frombuffer(value, np.int32)
        self.adf_stack.append(value)

    def u32s_push(self, offset, cnt):
        value = memoryview(self._instance.memory.buffer)[offset:offset + cnt * 4]
        value = np.frombuffer(value, np.uint32)
        self.adf_stack.append(value)

    def s64s_push(self, offset, cnt):
        value = memoryview(self._instance.memory.buffer)[offset:offset + cnt * 8]
        value = np.frombuffer(value, np.int64)
        self.adf_stack.append(value)

    def u64s_push(self, offset, cnt):
        value = memoryview(self._instance.memory.buffer)[offset:offset + cnt * 8]
        value = np.frombuffer(value, np.uint64)
        self.adf_stack.append(value)

    def f32s_push(self, offset, cnt):
        value = memoryview(self._instance.memory.buffer)[offset:offset + cnt * 4]
        value = np.frombuffer(value, np.float32)
        self.adf_stack.append(value)

    def f64s_push(self, offset, cnt):
        value = memoryview(self._instance.memory.buffer)[offset:offset + cnt * 8]
        value = np.frombuffer(value, np.float64)
        self.adf_stack.append(value)

    def xml_w_element_begin(self, file_hnd, offset, sz):
        value = memoryview(self._instance.memory.buffer)[offset:offset + sz]
        value = bytes(value).decode('utf-8')
        if self.adf_stack:
            ele = SubElement(self.adf_stack[-1], value)
        else:
            ele = Element(value)
        self.adf_stack.append(ele)

    def xml_w_element_end(self, file_hnd, offset, sz):
        value = memoryview(self._instance.memory.buffer)[offset:offset + sz]
        value = bytes(value).decode('utf-8')
        # print('xml_w_element_end', file_hnd, value)
        if len(self.adf_stack) > 1:
            self.adf_stack.pop()

    def xml_w_attr_set(self, file_hnd, id_offset, id_sz, value_offset, value_sz):
        id_raw = memoryview(self._instance.memory.buffer)[id_offset:id_offset + id_sz]
        id_str = bytes(id_raw).decode('utf-8')
        value = memoryview(self._instance.memory.buffer)[value_offset:value_offset + value_sz]
        value = bytes(value).decode('utf-8')
        if self.adf_stack:
            self.adf_stack[-1].set(id_str, value)

    def xml_w_value_set(self, file_hnd, offset, sz):
        value = memoryview(self._instance.memory.buffer)[offset:offset + sz]
        value = bytes(value).decode('utf-8')
        if self.adf_stack:
            self.adf_stack[-1].text = value
