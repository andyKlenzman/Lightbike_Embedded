# -----------------------------------------------------------------------------
# Project Configuration
# -----------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.11)
project(Lightbike)

SET(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
SET(HAVE_FLAG_SEARCH_PATHS_FIRST FALSE)
SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_C_LINK_FLAGS "")
SET(CMAKE_CXX_LINK_FLAGS "")

# APP_VERSION is a hexadecimal byte
SET(APP_VERSION 1)
SET(APP_VERSION_STR "1.0.0")

# -----------------------------------------------------------------------------
# Python Interpreter Selection
# -----------------------------------------------------------------------------
set(PYTHON_EXECUTABLE "/Users/andyklenzman/CLionProjects/coldwave-evk/venv/bin/python3.9")
find_package (Python COMPONENTS Interpreter Development)
message(STATUS "Found Python Interpreter: ${Python_EXECUTABLE}")

# -----------------------------------------------------------------------------
# Add Definitions
# -----------------------------------------------------------------------------
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)
    list(APPEND C_COMMON_FLAGS -falign-functions=4 -Og -g3)
endif ()

if (CMAKE_BUILD_TYPE MATCHES Release)
    remove_definitions(-DDEBUG)
    list(APPEND C_COMMON_FLAGS -falign-functions=4 -O3 )
endif ()

add_definitions(-D__COLDWAVEOS__)
add_definitions(-DAPP_VERSION_STR="${APP_VERSION_STR}")
add_definitions(-DAPP_VERSION=${APP_VERSION})
add_definitions(-DARM_MATH_CM33)
add_definitions(-D__FPU_PRESENT)

# -----------------------------------------------------------------------------
# TCPIP
# -----------------------------------------------------------------------------
if (CONFIG_NO_TCPIP)
    list (APPEND CW_COMMON_LINKER_FLAGS
            -Wl,--wrap=closesocket
            -Wl,--wrap=lwip_read
            -Wl,--wrap=lwip_write
            -Wl,--wrap=lwip_ioctl
            -Wl,--wrap=lwip_fcntl
            -Wl,--wrap=tcpip_init
    )
endif()

# -----------------------------------------------------------------------------
#
# Generate the build from the commit count
# commit_count will contain the count of commits in the Git Repo
#
# -----------------------------------------------------------------------------
execute_process(
        COMMAND git rev-list HEAD
        COMMAND wc -l
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        OUTPUT_VARIABLE _commit_count
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
string(STRIP ${_commit_count} CW_OS_VERSION_BUILD)

set(CW_COMMON_LINKER_FLAGS
        -mfloat-abi=hard
        -mfpu=fpv5-sp-d16
        -mcpu=cortex-m33
        -Wl,--gc-sections
        -Wl,--entry=_os_init
        -mlong-calls
        -mthumb
        -Wl,--print-memory-usage
        -specs=nano.specs
        -Wl,-wrap=malloc
        -Wl,-wrap=free
        -Wl,-wrap=calloc
        -Wl,-wrap=realloc
        -Wl,-wrap=_malloc_r
        -Wl,-wrap=_free_r
        -Wl,-wrap=_calloc_r
        -Wl,-wrap=_realloc_r
        -Wl,-wrap=__aeabi_unwind_cpp_pr0
        -Wl,-wrap=__aeabi_unwind_cpp_pr1
        -Wl,-wrap=__aeabi_unwind_cpp_pr2
        -Wl,--no-warn-rwx-segment
        -T${CMAKE_CURRENT_LIST_DIR}/coldwaveos/linker.ld
        -Wl,-whole-archive ${CMAKE_CURRENT_LIST_DIR}/coldwaveos/lib/libcoldwaveos-efr32mg24b310f1536im48-2.0.4+1584.a -Wl,-no-whole-archive
        -Wl,-whole-archive ${CMAKE_CURRENT_LIST_DIR}/coldwaveos/lib/libarm_ARMv8MMLldfsp_math.a -Wl,-no-whole-archive
        ${CMAKE_CURRENT_LIST_DIR}/coldwaveos/lib/libbluetooth.a
        ${CMAKE_CURRENT_LIST_DIR}/coldwaveos/lib/librail_efr32xg24_gcc_release.a


)

# -----------------------------------------------------------------------------
# Executables Configuration
# -----------------------------------------------------------------------------
add_executable(Lightbike
        src/main.cpp
        src/sysconfig.c

        src/utils/utils.cpp
        src/utils/combine_bytes.c
        src/utils/map_value.c
        src/utils/gamma8_table.c

        src/drivers/ws2812b.c
        src/drivers/icm20649.cpp

        src/led_filters/LEDFilter.cpp
        src/led_filters/LEDFilter_Basic.cpp
        src/led_filters/LEDFilter_Smooth.cpp
        src/led_filters/LEDFilter_Fade.cpp
        src/defines.h
)


# Bootloader
add_executable(${CMAKE_PROJECT_NAME}-boot ${CMAKE_CURRENT_LIST_DIR}/coldwaveos/keys.c)
target_compile_options(${CMAKE_PROJECT_NAME}-boot
        PUBLIC
        -DBOOTLOADER
        -D__CXOS__
        -falign-functions=4 -Os)

# Bootloader - linker options and link libraries
target_link_libraries(${CMAKE_PROJECT_NAME}-boot  PUBLIC
        -mfloat-abi=hard
        -mfpu=fpv5-sp-d16
        -mcpu=cortex-m33
        -Wl,--gc-sections
        -mlong-calls
        -mthumb
        -Wl,--print-memory-usage
        -specs=nano.specs
        -Wl,--print-memory-usage
        -Wl,-whole-archive ${CMAKE_CURRENT_LIST_DIR}/coldwaveos/lib/libcoldwaveos-efr32mg24b310f1536im48-bl-2.0.4+1584.a -Wl,-no-whole-archive
        -T${CMAKE_CURRENT_LIST_DIR}/coldwaveos/boot.ld
)

# -----------------------------------------------------------------------------
# ????
# -----------------------------------------------------------------------------
set(COMPILER_COMMON_FLAGS
        -mthumb
        -mfloat-abi=hard
        -mfpu=fpv5-sp-d16
        -mcpu=cortex-m33
)

include_directories(
        ${CMAKE_CURRENT_LIST_DIR}/coldwaveos/include
        ${CMAKE_CURRENT_LIST_DIR}/coldwaveos/include/platform/
        ${CMAKE_CURRENT_LIST_DIR}/coldwaveos/include/crypto/
)

string(REPLACE ";" " " STRING_CW_COMMON_LINKER_FLAGS "${CW_COMMON_LINKER_FLAGS}")
target_link_libraries(${CMAKE_PROJECT_NAME}
        ${CMAKE_EXE_LINKER_FLAGS}
        ${CW_COMMON_LINKER_FLAGS})

string(REPLACE ";" " " STRING_COMPILER_COMMON_FLAGS "${COMPILER_COMMON_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${STRING_COMPILER_COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${STRING_COMPILER_COMMON_FLAGS}")





if(NOT APP_VERSION_STR)
    message( FATAL_ERROR "Please define APP_VERSION_STR in your CMakeLists.txt")
endif()

# -----------------------------------------------------------------------------
# DIST folder generation
# -----------------------------------------------------------------------------
file (MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/dist)


add_custom_command(OUTPUT ${CMAKE_PROJECT_NAME}.hex
        COMMAND ${CMAKE_OBJCOPY} ARGS -I binary --change-addresses 0x8000 -O ihex ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.signed.bin ${PROJECT_SOURCE_DIR}/dist/${CMAKE_PROJECT_NAME}-${APP_VERSION_STR}+${CW_OS_VERSION_BUILD}.hex
        DEPENDS ${CMAKE_PROJECT_NAME}-signed.bin
        COMMENT "building signed distribution hex image")

add_custom_command(OUTPUT ${CMAKE_PROJECT_NAME}-bl.hex
        COMMAND ${CMAKE_OBJCOPY} ARGS -O ihex ${CMAKE_PROJECT_NAME}-boot ${PROJECT_SOURCE_DIR}/dist/${CMAKE_PROJECT_NAME}-${APP_VERSION_STR}+${CW_OS_VERSION_BUILD}-boot.hex
        COMMENT "building distribution bootloader hex image")

add_custom_command(OUTPUT  ${CMAKE_PROJECT_NAME}.dsym
        COMMAND ${CMAKE_OBJCOPY} ARGS --only-keep-debug "${CMAKE_PROJECT_NAME}" "${PROJECT_SOURCE_DIR}/dist/${CMAKE_PROJECT_NAME}-${APP_VERSION_STR}+${CW_OS_VERSION_BUILD}.dsym"
        COMMENT "building debug info")

file(GLOB ZIP_SOURCES "${PROJECT_SOURCE_DIR}/src/*")

add_custom_command(OUTPUT ${CMAKE_PROJECT_NAME}.zip
        COMMAND  ${CMAKE_COMMAND} -E tar "cfv" "${PROJECT_SOURCE_DIR}/dist/${CMAKE_PROJECT_NAME}-${APP_VERSION_STR}+${CW_OS_VERSION_BUILD}.src.zip" --format=zip -- ${ZIP_SOURCES}
        COMMENT "zip'ing sources")

add_custom_command(OUTPUT ${CMAKE_PROJECT_NAME}-signed.bin
        COMMAND ${PYTHON_EXECUTABLE} ARGS ${CMAKE_CURRENT_LIST_DIR}/coldwaveos/imgtool.py sign --version ${APP_VERSION_STR}+${CW_OS_VERSION_BUILD} --boot-record AppFW --header-size 0x200 -S 0xf7000 --security-counter auto --align 4 -k ${PROJECT_SOURCE_DIR}/keys.pem
        ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin
        ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.signed.bin
        DEPENDS ${CMAKE_PROJECT_NAME}.bin
        COMMENT "copying signed binary to dist folder, which means CWIK-Signed bin. It runs the python script and directory to the key. Makes sure .bin file is built before running this.")

add_custom_command(OUTPUT ${CMAKE_PROJECT_NAME}-signed-dist.bin
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.signed.bin
        ${PROJECT_SOURCE_DIR}/dist/${CMAKE_PROJECT_NAME}-${APP_VERSION_STR}+${CW_OS_VERSION_BUILD}.signed.bin
        DEPENDS ${CMAKE_PROJECT_NAME}-signed.bin
        COMMENT "signing image")


add_custom_command(OUTPUT ${CMAKE_PROJECT_NAME}.bin
        COMMAND ${CMAKE_OBJCOPY} ARGS -v -O binary ${CMAKE_PROJECT_NAME} ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin
        COMMENT "building distribution binary image"
        DEPENDS ${CMAKE_PROJECT_NAME})

message("CMAKE SYNTAX LESSON: ALL means it is built by default, DEPENDS means it depends on the following things to be built first")
add_custom_target(${CMAKE_PROJECT_NAME}-dist ALL DEPENDS
        ${CMAKE_PROJECT_NAME}.hex
        ${CMAKE_PROJECT_NAME}-signed-dist.bin
        ${CMAKE_PROJECT_NAME}.zip
        ${CMAKE_PROJECT_NAME}.dsym
        ${CMAKE_PROJECT_NAME}-bl.hex
        ${CMAKE_PROJECT_NAME}-boot)


# -----------------------------------------------------------------------------
# Auto signing
#
#
#  We support automatic signing and upload.
#  To use it, add "-DSIGN_AND_UPLOAD=TRUE -DPATH_TO_JLINK_COMMANDER_EXE=/path/to/JLinkExe" to your cmake commandline
# -----------------------------------------------------------------------------

if (SIGN_AND_UPLOAD AND NOT PATH_TO_JLINK_COMMANDER_EXE)
    message(SEND_ERROR "To use automated sign and upload, please set the Variable PATH_TO_JLINK_COMMANDER_EXE or disable SIGN_AND_UPLOAD")
else()
    message("Auto sign and upload marked as TRUE. CMAKE is uploading a binary file to Jsegger, so it can be flashed onto our processor")
    file (WRITE ${CMAKE_CURRENT_LIST_DIR}/coldwaveos/upload.jLink "device EFR32MG24BXXXF1536
        si 1
        speed auto
        r
        h
        loadfile ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.signed.bin 0x8008000
        q")

    message("")
    add_custom_command(OUTPUT upload-signed-bin
            COMMAND ${PATH_TO_JLINK_COMMANDER_EXE} ARGS -commanderscript ${CMAKE_CURRENT_LIST_DIR}/coldwaveos/upload.jLink
            COMMENT "uploading signed image, which comes from a custom command in CMAKE file and depends on the signed bin file existing."
            DEPENDS ${CMAKE_PROJECT_NAME}-signed.bin)

    add_custom_target(${CMAKE_PROJECT_NAME}-signed ALL DEPENDS
            upload-signed-bin
    )
endif()

target_include_directories(Lightbike PRIVATE
        ${PROJECT_SOURCE_DIR}/src/
        )

target_compile_options(Lightbike PRIVATE
        -pipe
        -fno-strict-aliasing
        -Wall
        -Werror-implicit-function-declaration
        -Wpointer-arith
        -ffunction-sections
        -fdata-sections
        -Wchar-subscripts
        -Wcomment
        -Wmain
        -Wparentheses
        -Wsequence-point
        -Wreturn-type
        -Wswitch
        -Wtrigraphs
        -Wuninitialized
        -Wunknown-pragmas
        -Wwrite-strings
        -Wsign-compare
        -Wmissing-format-attribute
        -Wno-deprecated-declarations
        -Wunreachable-code
        --param max-inline-insns-single=500
        -mlong-calls
        -Wstack-usage=1024
        -fno-common
        -falign-functions=4
        )