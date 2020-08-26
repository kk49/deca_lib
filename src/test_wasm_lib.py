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
    [0, '../test/rtpc/abandoned_foa_facility_acoustics.blo'],
    [0, '../test/rtpc/global_gui.blo'],
    [0, '../test/rtpc/global_aesir.blo'],
    [0, '../test/rtpc/island01_n035.blo'],
    [0, '../test/rtpc/northcoast_town_hagaboda_west.blo'],
]

test_xml = []


def process_rtpc_to_xml(fn, input_buffer, out_call, in_mem):
    input_sz = len(input_buffer)

    lib.adf_stack = []

    # mem mapped file
    t0 = time.time()

    input_wasm_offset = lib.instance().exports.alloc_bin(input_sz)
    lib.files_map['rtpc_out'] = bytearray()

    lib.files_map['rtpc_in'] = input_buffer
    lib.files_map['rtpc_out'] = bytearray()

    t1 = time.time()

    input_wasm = lib.instance().memory.uint8_view(input_wasm_offset)
    input_wasm[:input_sz] = input_buffer[:]

    file_hnd_in = lib.file_open_local('rtpc_in', '')
    file_hnd_out = lib.file_open_local('rtpc_out', '')

    t2 = time.time()

    if out_call:
        out_str = 'call'
        if in_mem:
            in_str = 'mem'
            value = lib.instance().exports.process_rtpc_out_call_in_mem(file_hnd_out, input_wasm_offset, input_sz)
        else:
            in_str = 'file'
            value = lib.instance().exports.process_rtpc_out_call_in_file(file_hnd_out, file_hnd_in)
    else:
        out_str = 'file'
        if in_mem:
            in_str = 'mem'
            value = lib.instance().exports.process_rtpc_out_file_in_mem(file_hnd_out, input_wasm_offset, input_sz)
        else:
            in_str = 'file'
            value = lib.instance().exports.process_rtpc_out_file_in_file(file_hnd_out, file_hnd_in)

    lib.file_close(file_hnd_out)
    lib.file_close(file_hnd_in)

    t3 = time.time()

    print(f'{in_str} => {out_str}: {fn}, value = {value}, Time: {t3-t0} = {t1-t0} + {t2-t1} + {t3-t2}')

    fnn = fn + f'.{out_str}.{in_str}.xml'
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
        print(f'process_rtpc_to_xml_mem {fn}')
        for i in range(4):
            with open(fn, 'rb') as f:
                input_buffer = f.read()
            input_buffer = input_buffer[skip_bytes:]
            process_rtpc_to_xml(fn, input_buffer, i % 2 == 0, i < 2)
        print()

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
