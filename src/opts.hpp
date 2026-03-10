#pragma once

#include <cstdint>
#include <string>

enum class Options : uint8_t {
  Echo,
  Type,
  Exit,
  Invalid,
};

namespace opts {
Options resolveOption(std::string input);
} // namespace opts
