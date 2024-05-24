cmake_minimum_required(VERSION 3.11)

SET(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
SET(HAVE_FLAG_SEARCH_PATHS_FIRST FALSE)
SET(CMAKE_SYSTEM_NAME Generic)

project(Lightbike)

# APP_VERSION is a hexadecimal byte
SET(APP_VERSION 1)
SET(APP_VERSION_STR "1.0.0")

message("Build type: " ${CMAKE_BUILD_TYPE})
if (CMAKE_BUILD_TYPE MATCHES Debug)

    add_definitions(-DDEBUG)
    list(APPEND C_COMMON_FLAGS -falign-functions=4 -Og -g3)
endif ()

if (CMAKE_BUILD_TYPE MATCHES Release)
    remove_definitions(-DDEBUG)
    list(APPEND C_COMMON_FLAGS -falign-functions=4 -O3 )
endif ()


SET(LIGHTBIKE_FILES
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
        src/defines.h
    )

add_executable(Lightbike ${LIGHTBIKE_FILES})


SET(LIGHTBIKE_EMULATOR_FILES
        src/main.cpp
        src/sysconfig.c

        src/utils/utils.cpp
        src/utils/combine_bytes.c
        src/utils/map_value.c
        src/utils/gamma8_table.c

        src/drivers/ws2812b.c
        src/drivers/icm20649_emulator.cpp

        src/led_filters/LEDFilter.cpp
        src/led_filters/LEDFilter.h
        src/led_filters/LEDFilter_Basic.cpp
        src/led_filters/LEDFilter_Smooth.cpp
        src/defines.h
)

add_executable(Lightbike-emulator ${LIGHTBIKE_EMULATOR_FILES})



include(coldwaveos/CMakeLists.cmake)

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