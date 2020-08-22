import time
import pprint
import json
from deca_adf_lib import DecaLibWasm, DecaLibWasmStack, tostring
from xml.dom import minidom

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

test_xml = []


def process_rtpc_to_xml_mem(fn, input_buffer):
    print(f'process_rtpc_to_xml_mem {fn}')
    input_sz = len(input_buffer)

    lib.adf_stack = []

    # mem mapped file
    t0 = time.time()

    input_wasm_offset = lib.instance().exports.alloc_bin(input_sz)

    t1 = time.time()

    input_wasm = lib.instance().memory.uint8_view(input_wasm_offset)
    input_wasm[:input_sz] = input_buffer[:]

    t2 = time.time()

    value = lib.instance().exports.process_rtpc_mem(666, input_wasm_offset, input_sz)

    t3 = time.time()

    print(f'Mem: {fn}, value = {value}, Time: {t3-t0} = {t1-t0} + {t2-t1} + {t3-t2}\n')

    if lib.adf_stack:
        adf = lib.adf_stack.pop()
        if isinstance(adf, list) or isinstance(adf, dict):
            pass
        else:
            fnn = fn + '.mem.xml'
            adf_str = tostring(adf)
            with open(fnn, 'wb') as f:
                f.write(adf_str)

            adf_str = minidom.parseString(adf_str)
            adf_str = adf_str.toprettyxml(indent=' ')
            with open(fnn, 'w') as f:
                f.write(adf_str)
            test_xml.append(fnn)
    else:
        adf = None


def process_rtpc_to_xml_file(fn, input_buffer):
    print(f'process_rtpc_to_xml_file {fn}')
    input_sz = len(input_buffer)

    lib.adf_stack = []

    # mem mapped file
    t0 = time.time()

    lib.files_map['rtpc_in'] = input_buffer

    t1 = time.time()

    file_hnd_in = lib.file_open_local('rtpc_in', '')

    t2 = time.time()

    value = lib.instance().exports.process_rtpc_file(666, file_hnd_in)

    lib.file_close(file_hnd_in)

    t3 = time.time()

    print(f'File: {fn}, value = {value}, Time: {t3-t0} = {t1-t0} + {t2-t1} + {t3-t2}\n')

    if lib.adf_stack:
        adf = lib.adf_stack.pop()
        if isinstance(adf, list) or isinstance(adf, dict):
            pass
        else:
            fnn = fn + '.file.xml'
            adf_str = tostring(adf)
            with open(fnn, 'wb') as f:
                f.write(adf_str)

            adf_str = minidom.parseString(adf_str)
            adf_str = adf_str.toprettyxml(indent=' ')
            with open(fnn, 'w') as f:
                f.write(adf_str)

            test_xml.append(fnn)
    else:
        adf = None


def main():
    for skip_bytes, fn in test_rtpc:
        with open(fn, 'rb') as f:
            input_buffer = f.read()
        input_buffer = input_buffer[skip_bytes:]
        process_rtpc_to_xml_mem(fn, input_buffer)

    for skip_bytes, fn in test_rtpc:
        with open(fn, 'rb') as f:
            input_buffer = f.read()
        input_buffer = input_buffer[skip_bytes:]
        process_rtpc_to_xml_file(fn, input_buffer)

    for fn in test_xml:
        lib.xml_parse(fn, fn + '.rtpc')

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

        print(f'{fn}, value = {value}, Time: {t3-t0} = {t1-t0} + {t2-t1} + {t3-t2}\n')


if __name__ == "__main__":
    main()
