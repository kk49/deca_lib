import wasmer
import numpy as np
import sys
import os


class DecaAdfEnumValue:
    def __init__(self, value, value_str):
        self.value = value
        self.value_str = value_str


class DecaLibWasm:
    def __init__(self, *args, **kwargs):
        lib_env = {
            'db_print': self.db_print,
            'db_warn': self.db_warn,
            'db_error': self.db_error,

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

    def instance(self):
        return self._instance

    def db_print(self, offset, sz):
        pass

    def db_warn(self, offset, sz):
        pass

    def db_error(self, offset, sz):
        pass

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


class DecaLibWasmStack(DecaLibWasm):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.adf_stack = []

    def instance(self):
        return self._instance

    def db_print(self, offset, sz):
        v = memoryview(self._instance.memory.buffer)[offset:offset + sz]
        v = bytes(v)
        print(f'db_print({offset}, {sz}) == {v}')

    def db_warn(self, offset, sz):
        v = memoryview(self._instance.memory.buffer)[offset:offset + sz]
        v = bytes(v)
        print(f'db_warn({offset}, {sz}) == {v}', file=sys.stderr)

    def db_error(self, offset, sz):
        print(self.adf_stack)
        v = memoryview(self._instance.memory.buffer)[offset:offset + sz]
        v = bytes(v)
        print(f'db_error({offset}, {sz}) == {v}', file=sys.stderr)

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
