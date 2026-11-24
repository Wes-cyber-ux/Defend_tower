cmake_minimum_required(VERSION 3.16)
project(DefensaDelReino CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE FUENTES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_executable(DefensaDelReino ${FUENTES})

if(MSVC)
    target_compile_options(DefensaDelReino PRIVATE /W4 /utf-8)
else()
    target_compile_options(DefensaDelReino PRIVATE -Wall -Wextra -Wpedantic)
endif()

# El historial (historial.txt) se guarda en la carpeta de trabajo del ejecutable.
