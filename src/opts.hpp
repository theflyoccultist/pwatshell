#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

enum class Options : uint8_t {
    Echo,
    Type,
    Pwd,
    History,
    Jobs,
    Cd,
    Exit,
    Executable,
};

namespace opts {

static const std::unordered_map<std::string, Options> optionsMap = {
    {"echo", Options::Echo},       {"type", Options::Type}, {"pwd", Options::Pwd},
    {"history", Options::History}, {"jobs", Options::Jobs}, {"cd", Options::Cd},
    {"exit", Options::Exit},
};

static Options resolveOption(const std::string &input) {
    auto it = optionsMap.find(input);
    if (it != optionsMap.end()) {
        return it->second;
    }

    return Options::Executable;
}

} // namespace opts
