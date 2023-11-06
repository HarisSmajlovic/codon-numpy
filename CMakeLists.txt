cmake_minimum_required(VERSION 3.14)
project(Codon-NumPy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wno-return-type-c-linkage")
set(CMAKE_CXX_FLAGS_DEBUG "-g -fno-limit-debug-info")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
include_directories(.)

set(CPM_DOWNLOAD_VERSION 0.32.3)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake...")
    file(DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif()
include(${CPM_DOWNLOAD_LOCATION})

enable_language(Fortran)
CPMAddPackage(
    NAME openblas
    GITHUB_REPOSITORY "xianyi/OpenBLAS"
    GIT_TAG 039e27545f93fd54f9a04113f6883ee47a690004
    EXCLUDE_FROM_ALL YES
    OPTIONS "BUILD_TESTING OFF")

set(NUMPY_FILES
    ir/numpy.cpp
    ir/numpy.h)
add_library(numpy SHARED ${NUMPY_FILES})
add_dependencies(numpy openblas)

if(APPLE)
  target_link_libraries(numpy PRIVATE /opt/homebrew/Cellar/gcc/12.2.0/lib/gcc/12/libgfortran.dylib)
  target_link_libraries(numpy PRIVATE -Wl,-force_load,$<TARGET_FILE:openblas>)
else()
  target_link_libraries(numpy PRIVATE /usr/lib/gcc/x86_64-redhat-linux/8/libgfortran.so)
  target_link_libraries(numpy PRIVATE -Wl,--whole-archive $<TARGET_FILE:openblas> -Wl,--no-whole-archive)
endif()

find_package(LLVM REQUIRED CONFIG)
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

if(NOT CODON_PATH)
    set(CODON_PATH "$ENV{HOME}/.codon")
endif()

target_include_directories(numpy PRIVATE "${CODON_PATH}/include" ${LLVM_INCLUDE_DIRS})
target_link_directories(numpy PRIVATE "${CODON_PATH}/lib/codon")
target_link_libraries(numpy PRIVATE codonrt codonc)

# # Download and unpack googletest at configure time
# include(FetchContent)
# FetchContent_Declare(googletest
#   URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip)
# # For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)
# enable_testing()
# add_executable(numpytest test/main.cpp)
# target_include_directories(numpytest PRIVATE "${CODON_PATH}/include" ${LLVM_INCLUDE_DIRS})
# target_link_directories(numpytest PRIVATE "${CODON_PATH}/lib/codon")
# target_link_libraries(numpytest codonrt codonc gtest_main)
# target_compile_definitions(numpytest PRIVATE TEST_DIR="${CMAKE_CURRENT_SOURCE_DIR}/test")
