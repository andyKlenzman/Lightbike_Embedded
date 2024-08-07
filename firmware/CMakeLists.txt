cmake_minimum_required(VERSION 3.11)

SET(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
SET(HAVE_FLAG_SEARCH_PATHS_FIRST FALSE)
SET(CMAKE_SYSTEM_NAME Generic)

project(Lightbike)

# APP_VERSION is a hexadecimal byte
SET(APP_VERSION 1)
SET(APP_VERSION_STR "1.0.0")

add_definitions(-DDEBUG)
list(APPEND C_COMMON_FLAGS -falign-functions=4 -Og -g3)

#[[I have not yet created a release build]]
if (CMAKE_BUILD_TYPE MATCHES Release)
    remove_definitions(-DDEBUG)
    list(APPEND C_COMMON_FLAGS -falign-functions=4 -O3 )
endif ()


    SET(LIGHTBIKE_FILES
            src/main.cpp
            src/sysconfig.c

            src/utils/utils.cpp
            src/utils/combine_bytes.h
            src/utils/map_value.h
            src/utils/gamma8_table.c

            src/ws2812b/ws2812b.c
            src/icm20649/icm20649.cpp

            src/led_filters/LEDFilter_Basic.cpp
            src/led_filters/LEDFilter_Smooth.cpp
            src/globals.h
            src/buttons/buttons.cpp
            src/filter_handler/filter_handler.cpp
            src/buttons/buttons.h
            src/utils/power_toggle.c
            src/led_filters/LEDFilter_Wave.cpp
            src/led_filters/LEDFilter_BikeWheel.cpp
    )

add_executable(Lightbike ${LIGHTBIKE_FILES})

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