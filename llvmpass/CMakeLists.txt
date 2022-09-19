cmake_minimum_required(VERSION 3.4.3)
#===============================================================================
# 1. LOAD LLVM CONFIGURATION
#===============================================================================
# Set this to a valid LLVM installation dir
# set(LT_LLVM_INSTALL_DIR "" CACHE PATH "/usr/lib/llvm-6.0/lib/cmake/llvm")

# Add the location of LLVMConfig.cmake to CMake search paths (so that
# find_package can locate it)
# list(APPEND CMAKE_PREFIX_PATH "${LT_LLVM_INSTALL_DIR}")
set(LLVM_DIR "/opt/toolchain-clang/host/usr/share/llvm/cmake/")
find_package(LLVM REQUIRED CONFIG)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

add_library(LLVMLoopIdAssigner MODULE LoopIdAssigner.cpp)

# Add sub directories
add_subdirectory(dynamic_rt)