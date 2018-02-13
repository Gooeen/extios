cmake_minimum_required(VERSION 2.8)

project(extios)

set(sources extios/extcodecvt.cpp extios/extiostream.cpp)
set(headers extios/extcodecvt.h extios/extiostream.h extios/iobuf.hpp)
set(LIBRARY_OUTPUT_PATH libs)

add_compile_options(-std=c++17 -Wall -Wextra)
add_library(extios SHARED ${sources})

install(TARGETS extios LIBRARY DESTINATION lib)
install(FILES ${headers} DESTINATION include/extios)
