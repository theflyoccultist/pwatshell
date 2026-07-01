#pragma once

#include <cstddef>

struct ShellState {
    static inline std::size_t cmds_since_append = 0;
};
