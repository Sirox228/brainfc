cmake_minimum_required(VERSION 3.20)
project(brainfc LANGUAGES CXX VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
  src/main.cpp
  src/lexer.cpp
  src/codegen.cpp
  src/objcodegen.cpp
)

find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

execute_process(COMMAND llvm-config --libs OUTPUT_VARIABLE llvmlibs)
execute_process(COMMAND llvm-config --system-libs OUTPUT_VARIABLE llvmsyslibs)
execute_process(COMMAND llvm-config --ldflags OUTPUT_VARIABLE llvmldf)

string(STRIP ${llvmlibs} llvmlibs)
string(STRIP ${llvmsyslibs} llvmsyslibs)
string(STRIP ${llvmldf} llvmldf)

execute_process(COMMAND llvm-config --cxxflags OUTPUT_VARIABLE CMAKE_CXX_FLAGS)
string(STRIP ${CMAKE_CXX_FLAGS} CMAKE_CXX_FLAGS)

add_executable(brainfc ${SOURCES})
target_link_libraries(brainfc ${llvmlibs} ${llvmsyslibs} ${llvmldf})

