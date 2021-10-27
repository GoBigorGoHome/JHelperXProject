cmake_minimum_required(VERSION 3.13)
project(jhelper-example-project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(testrunner
        testrunner/main.cpp
        testrunner/test_runner.cpp
        testrunner/compare_floats.cpp
        testrunner/string_utils.cpp
        testrunner/checker.cpp
        testrunner/print_test.cpp
        testrunner/color.cpp)
target_link_libraries(testrunner PRIVATE pthread)
target_include_directories(testrunner PRIVATE library)
# use libstdc++ debug mode
target_compile_definitions(testrunner PRIVATE _GLIBCXX_DEBUG LOCAL)
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # specify standards conformance mode
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive- /Zc:preprocessor /Zc:__cplusplus")
else ()
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=vla")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftemplate-backtrace-limit=0")
    # use lld to speed up linking
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld")
    # Set stack size to 256 MiB
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,268435456")
    target_compile_options(testrunner PRIVATE -Werror)
    target_compile_options(testrunner PRIVATE -Werror=conversion)
    target_compile_options(testrunner PRIVATE -Wno-sign-conversion)
    target_compile_options(testrunner PRIVATE -Wno-trigraphs)
    target_compile_options(testrunner PRIVATE -Wno-sign-compare)
    target_compile_options(testrunner PRIVATE -Wno-unused)
    target_compile_options(testrunner PRIVATE -Wno-logical-op-parentheses)
    target_compile_options(testrunner PRIVATE -Wno-parentheses)
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(testrunner PRIVATE -Wno-shift-op-parentheses)
    target_compile_options(testrunner PRIVATE -Wno-missing-braces)
    if (NOT CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
        # fix linking error when using std::future with Clang
        # see https://stackoverflow.com/a/31639828/6793559 for detail
        target_compile_options(testrunner PRIVATE -femulated-tls)
    endif ()
endif ()

if (MSVC)
    #    target_compile_options(testrunner PRIVATE -fsanitize=address /MT)
    #    target_link_options(testrunner PRIVATE /libpath:D:\\LLVM\\lib\\clang\\10.0.0\\lib\\windows)
    #    target_link_libraries(testrunner PRIVATE clang_rt.asan-x86_64.lib)
endif ()

include_directories(library)

add_executable(exp exp.cpp)

add_executable(my checker/my.cpp)
target_compile_definitions(my PRIVATE LOCAL)
add_executable(ac checker/ac.cpp)
include_directories(ac-library)
