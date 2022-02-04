set(TARGET Client)

set(CPP main.cpp)

add_executable(${TARGET} ${CPP})

target_link_libraries(${TARGET} PRIVATE CONAN_PKG::boost Common)