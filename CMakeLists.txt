cmake_minimum_required(VERSION 3.20)
project(fxcurl C)

set(CMAKE_C_STANDARD 99)

#添加编译参数
set(CMAKE_C_FLAGS "-rdynamic")
set(CMAKE_CXX_FLAGS "-rdynamic")

configure_file(fx_config.bin fx_config.h)


find_package(PkgConfig REQUIRED)

pkg_search_module(GTK3 REQUIRED gtk+-3.0)
pkg_search_module(LIBSOUP REQUIRED libsoup-2.4)



aux_source_directory(. SOURCE_DIR)
aux_source_directory(include SOURCE_DIR)
aux_source_directory(event SOURCE_DIR)
aux_source_directory(widget SOURCE_DIR)
link_directories(lib/linux)


add_executable(fxcurl ${SOURCE_DIR})
include_directories("${PROJECT_BINARY_DIR}")
include_directories(${GTK3_INCLUDE_DIRS})
include_directories(${LIBSOUP_INCLUDE_DIRS})

target_link_libraries(fxcurl ${GTK3_LIBRARIES})
target_link_libraries(fxcurl ${LIBSOUP_LIBRARIES})
target_link_libraries(fxcurl sqlite3)
