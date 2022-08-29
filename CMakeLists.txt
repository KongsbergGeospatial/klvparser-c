cmake_minimum_required(VERSION 3.10)

project(KlvParser VERSION 1.0)

set(CMAKE_C_FLAGS "-D__STDC_CONSTANT_MACROS")

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

set(LIB_KLV_SRC src/libklv/libklv.c src/libklv/libklv.h src/libklv/list.h)

add_executable(${PROJECT_NAME} src/other_klvparser.c ${LIB_KLV_SRC})

if (MSVC)
    set(CMAKE_C_FLAGS_RELEASE "/O2 /MD")
    set(CMAKE_C_FLAGS_DEBUG "/MDd")
    target_compile_options(${PROJECT_NAME} PRIVATE /EHsc /W3)
else ()
    set(CMAKE_C_FLAGS_DEBUG "-g")
    set(CMAKE_C_FLAGS_RELEASE "-O3")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif ()


configure_file(Config.h.in include/Config.h)

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")