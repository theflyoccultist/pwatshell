#pragma once

#include <iostream>
#include <readline/history.h>

namespace History {

void listHistory(int num_entries = 0) {
    HISTORY_STATE *hist_state = history_get_history_state();

    if (num_entries == 0) {
        for (int i = 0; i < hist_state->length; i++) {
            std::cout << '\t' << i + 1 << "  " << hist_state->entries[i]->line << '\n';
        }
    } else {
        for (int i = (hist_state->length - num_entries); i < hist_state->length; i++) {
            std::cout << '\t' << i + 1 << "  " << hist_state->entries[i]->line << '\n';
        }
    }
}

} // namespace History
