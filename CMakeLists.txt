cmake_minimum_required(VERSION 3.15)
project(pypag LANGUAGES CXX)

if (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
    set(PLATFORM linux)
endif ()

if (CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" OR $CMAKE_SYSTEM_PROCESSOR STREQUAL "amd64")
    set(ARCH x64)
endif ()


set(CMAKE_CXX_STANDARD 11 CACHE STRING "C++ version selection") 
add_compile_definitions(_GLIBCXX_USE_CXX11_ABI=0)

find_package(Python COMPONENTS Interpreter Development REQUIRED)
add_subdirectory(pybind11)

set(PAG_PATH libpag)
include_directories(${PAG_PATH}/include)
include_directories(${PAG_PATH}/vendor/ffavc/include)

file(GLOB PYPAG_STATIC_LIBS libs/${PLATFORM}/${ARCH}/*.a)
file(GLOB PYPAG_SHARED_LIBS libs/${PLATFORM}/${ARCH}/*.so)

message(STATUS "PYPAG_SHARED_LIBS=${PYPAG_SHARED_LIBS}")
message(STATUS "PYPAG_STATIC_LIBS=${PYPAG_STATIC_LIBS}")

add_library(pypag MODULE src/cpp/pypag.cpp)

target_link_libraries(pypag PRIVATE ${PYPAG_STATIC_LIBS} ${PYPAG_SHARED_LIBS} pybind11::module pybind11::lto)

pybind11_extension(pypag)
pybind11_strip(pypag)
