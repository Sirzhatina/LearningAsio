#pragma once

#include <cstdint>
#include <string>

typedef std::uint16_t Header;

constexpr std::size_t headerSize = sizeof(Header);

struct Message
{
    Header      size = 0;
    std::string buffer;
};