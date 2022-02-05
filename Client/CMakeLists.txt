set(TARGET Client)

set(HPP          Client.hpp)
set(CPP main.cpp Client.cpp)

add_executable(${TARGET} ${HPP} ${CPP})

target_link_libraries(${TARGET} PRIVATE CONAN_PKG::boost Common)