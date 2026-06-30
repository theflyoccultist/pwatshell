#pragma once

#include <iostream>
#include <readline/history.h>

namespace History {

void listHistory() {
    HISTORY_STATE *hist_state = history_get_history_state();

    for (int i = 0; i < hist_state->length; ++i) {
        std::cout << '\t' << i + 1 << "  " << hist_state->entries[i]->line << '\n';
    }
}

} // namespace History
