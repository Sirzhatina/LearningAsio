set(TARGET Server)


set(HPP          Server.hpp)
set(CPP main.cpp Server.cpp)

add_executable(${TARGET} ${CPP} ${HPP})

target_link_libraries(${TARGET} PRIVATE CONAN_PKG::boost Common)
