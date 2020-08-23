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
    lib.files_map['rtpc_out'] = bytearray()

    t1 = time.time()

    input_wasm = lib.instance().memory.uint8_view(input_wasm_offset)
    input_wasm[:input_sz] = input_buffer[:]

    file_hnd_out = lib.file_open_local('rtpc_out', '')

    t2 = time.time()

    value = lib.instance().exports.process_rtpc_mem(file_hnd_out, input_wasm_offset, input_sz)

    lib.file_close(file_hnd_out)

    t3 = time.time()

    print(f'Mem: {fn}, value = {value}, Time: {t3-t0} = {t1-t0} + {t2-t1} + {t3-t2}\n')

    fnn = fn + '.mem.xml'
    rtpc_str = None
    if lib.adf_stack:
        rtpc = lib.adf_stack.pop()
        if isinstance(rtpc, list) or isinstance(rtpc, dict):
            pass
        else:
            rtpc_str = tostring(rtpc)
    else:
        rtpc_str = lib.files_map['rtpc_out']

    if rtpc_str is not None:
        with open(fnn, 'wb') as f:
            f.write(rtpc_str)

        rtpc_str = minidom.parseString(rtpc_str)
        rtpc_str = rtpc_str.toprettyxml(indent=' ')
        with open(fnn, 'w') as f:
            f.write(rtpc_str)

        test_xml.append(fnn)


def process_rtpc_to_xml_file(fn, input_buffer):
    print(f'process_rtpc_to_xml_file {fn}')
    input_sz = len(input_buffer)

    lib.adf_stack = []

    # mem mapped file
    t0 = time.time()

    lib.files_map['rtpc_in'] = input_buffer
    lib.files_map['rtpc_out'] = bytearray()

    t1 = time.time()

    file_hnd_in = lib.file_open_local('rtpc_in', '')
    file_hnd_out = lib.file_open_local('rtpc_out', '')

    t2 = time.time()

    value = lib.instance().exports.process_rtpc_file(file_hnd_out, file_hnd_in)

    lib.file_close(file_hnd_out)
    lib.file_close(file_hnd_in)

    t3 = time.time()

    print(f'File: {fn}, value = {value}, Time: {t3-t0} = {t1-t0} + {t2-t1} + {t3-t2}\n')

    fnn = fn + '.file.xml'
    rtpc_str = None
    if lib.adf_stack:
        rtpc = lib.adf_stack.pop()
        if isinstance(rtpc, list) or isinstance(rtpc, dict):
            pass
        else:
            rtpc_str = tostring(rtpc)
    else:
        rtpc_str = lib.files_map['rtpc_out']

    if rtpc_str is not None:
        with open(fnn, 'wb') as f:
            f.write(rtpc_str)

        rtpc_str = minidom.parseString(rtpc_str)
        rtpc_str = rtpc_str.toprettyxml(indent=' ')
        with open(fnn, 'w') as f:
            f.write(rtpc_str)

        test_xml.append(fnn)


def main():
    lib.timing_test_run()

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
