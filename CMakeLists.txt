CMAKE_MINIMUM_REQUIRED(VERSION 3.17)
PROJECT(deca_lib)

SET(CMAKE_CXX_STANDARD 14)

set(
        SOURCE_FILES
        src/basedef.cpp
        src/i_base.cpp
        src/i_io.cpp
        src/i_data_stack.cpp
        src/ff/adf.cpp
        src/ff/rtpc.cpp
)

ADD_EXECUTABLE(deca_lib ${SOURCE_FILES})

set(CMAKE_EXECUTABLE_SUFFIX ".wasm")

SET(
        EMCC_ARGS_LIST
        -v
        -O3
        --no-entry
        --js-library ${PROJECT_SOURCE_DIR}/src/i_base.js
        --js-library ${PROJECT_SOURCE_DIR}/src/i_data_stack.js
        --pre-js ${PROJECT_SOURCE_DIR}/src/i_data_stack_pre.js
        -s INITIAL_MEMORY=256MB
        -s MAXIMUM_MEMORY=1GB
)
#-s EXPORT_ALL=0
#-s EXPORTED_FUNCTIONS="[]"

function(JOIN VALUES GLUE OUTPUT)
    string (REGEX REPLACE "([^\\]|^);" "\\1${GLUE}" _TMP_STR "${VALUES}")
    string (REGEX REPLACE "[\\](.)" "\\1" _TMP_STR "${_TMP_STR}") #fixes escaping
    set (${OUTPUT} "${_TMP_STR}" PARENT_SCOPE)
endfunction()

JOIN("${EMCC_ARGS_LIST}" " " EMCC_ARGS)

SET_TARGET_PROPERTIES(deca_lib PROPERTIES LINK_FLAGS ${EMCC_ARGS})

# emcc adf.cpp -o deca_adf.js -O3 -v --js-library i_base.js --js-library i_data_stack.js  -s INITIAL_MEMORY=256MB -s MAXIMUM_MEMORY=1GB -s EXPORT_ALL=0 -s EXPORTED_FUNCTIONS="[]"
# emcc adf.cpp -o deca_lib.wasm -O3 -v --js-library i_base.js --js-library i_data_stack.js -s INITIAL_MEMORY=256MB -s MAXIMUM_MEMORY=1GB -s EXPORT_ALL=0 -s EXPORTED_FUNCTIONS="[]"