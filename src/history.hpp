#pragma once

#include <cstring>
#include <iostream>
#include <readline/history.h>
#include <string>
#include <vector>

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

void readHistoryFromFile(const char *filename) {
    int err = read_history(filename);
    if (err != 0) {
        std::cout << "Error: " << strerror(err) << '\n';
    }
}

void writeHistoryToFile(const char *filename) {
    int err = write_history(filename);
    if (err != 0) {
        std::cout << "Error: " << strerror(err) << '\n';
    }
}

void parseHistoryFlag(const std::vector<std::string> &args) {
    if (args.size() <= 2) {
        return;
    }

    if (args[1] == "-r") {
        readHistoryFromFile(args[2].c_str());
    } else if (args[1] == "-w") {
        writeHistoryToFile(args[2].c_str());
    }
}

} // namespace History
