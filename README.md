# About
This will contain the "library" parts of the [DECA](http://github/kk49/deca) app,
so that it can be used by multiple projects.

# Goals
* WebAssembly modules (for http://www.mathartbang.com, and DECA proper)
* Python Support
* Javascript Support
* Other languages via WebAssembly? C#?

# Requires
* CMake 3.16+
* emscripten
* Python 3.7+ (for WASM interface generation, and test code)

# Build instructions
Activate emscripten environment, something like...

```
source ~/prj/emsdk/emsdk_env.sh
```

Then build deca_lib

```
cd deca_lib
mkdir -p build/emscripten
cd build/emscripten
cmake -DCMAKE_TOOLCHAIN_FILE=${EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake ../../
make
```