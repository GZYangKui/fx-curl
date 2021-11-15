cmake_minimum_required(VERSION 3.20)
project(fxcurl C)

set(CMAKE_C_STANDARD 99)

#添加编译参数
set(CMAKE_C_FLAGS  "-rdynamic")
set(CMAKE_CXX_FLAGS  "-rdynamic")


find_package(PkgConfig REQUIRED)


pkg_search_module(GTK3 REQUIRED gtk+-3.0)

aux_source_directory(. SOURCE_DIR)
aux_source_directory(./headers HEADER_DIR)

MESSAGE(${GTK3_LIBRARIES})

add_executable(c_api ${SOURCE_DIR} ${HEADER_DIR})
include_directories(${GTK3_INCLUDE_DIRS})
target_link_libraries(c_api ${GTK3_LIBRARIES})
