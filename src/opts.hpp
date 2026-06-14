#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

enum class Options : uint8_t {
    Echo,
    Type,
    Pwd,
    Cd,
    Cat,
    Exit,
    Executable,
};

namespace opts {
static Options resolveOption(const std::string &input) {
    static const std::unordered_map<std::string, Options> optionsMap = {
        {"echo", Options::Echo}, {"type", Options::Type}, {"pwd", Options::Pwd},
        {"cat", Options::Cat},   {"cd", Options::Cd},     {"exit", Options::Exit},
    };

    auto it = optionsMap.find(input);
    if (it != optionsMap.end()) {
        return it->second;
    }

    return Options::Executable;
}
} // namespace opts
