set(TARGET Common)

set(HPP Common.hpp)

add_library(${TARGET} INTERFACE ${HPP})
target_include_directories(${TARGET} INTERFACE ${CMAKE_SOURCE_DIR})
