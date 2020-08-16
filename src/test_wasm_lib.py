import time
import pprint
import json
from deca_adf_lib import DecaLibWasm, DecaLibWasmStack, tostring

lib = DecaLibWasmStack()

print(f'wasm memory size = {len(memoryview(lib.instance().memory.buffer))}')

test_adf = [
    [5, '../test/adf/animal_population_0'],
    [0, '../test/adf/pistol_sa_22_01_a.modelc'],
    [0, '../test/adf/savegame'],
]

test_rtpc = [
    [0, '../test/rtpc/global_aesir.blo'],
    [0, '../test/rtpc/global_gui.blo'],
    [0, '../test/rtpc/abandoned_foa_facility_acoustics.blo'],
]

for skip_bytes, fn in test_rtpc:
    with open(fn, 'rb') as f:
        input_buffer = f.read()
    input_sz = len(input_buffer)

    lib.adf_stack = []

    t0 = time.time()

    input_wasm_offset = lib.instance().exports.alloc_bin(input_sz)

    t1 = time.time()

    input_wasm = lib.instance().memory.uint8_view(input_wasm_offset)
    input_wasm[:input_sz] = input_buffer[:]

    t2 = time.time()

    value = lib.instance().exports.process_rtpc(666, input_wasm_offset + skip_bytes, input_sz - skip_bytes)

    t3 = time.time()

    print(f'{fn}, value = {value}, Time: {t3-t0} = {t1-t0} + {t2-t1} + {t3-t2}')

    if lib.adf_stack:
        adf = lib.adf_stack.pop()
        if isinstance(adf, list) or isinstance(adf, dict):
            pass
        else:
            with open(fn + '.xml', 'wb') as f:
                f.write(tostring(adf))
    else:
        adf = None


for skip_bytes, fn in test_adf:
    with open(fn, 'rb') as f:
        input_buffer = f.read()
    input_sz = len(input_buffer)

    lib.adf_stack = []

    t0 = time.time()

    input_wasm_offset = lib.instance().exports.alloc_bin(input_sz)

    t1 = time.time()

    input_wasm = lib.instance().memory.uint8_view(input_wasm_offset)
    input_wasm[:input_sz] = input_buffer[:]

    t2 = time.time()

    value = lib.instance().exports.process_adf(input_wasm_offset + skip_bytes, input_sz - skip_bytes)

    t3 = time.time()

    # pprint.pprint(lib.adf_stack)

    if lib.adf_stack:
        adf = lib.adf_stack.pop()
    else:
        adf = None

    print(f'{fn}, value = {value}, Time: {t3-t0} = {t1-t0} + {t2-t1} + {t3-t2}')

