#include "history.hpp"
#include <iostream>
#include <cstring>
#include <readline/history.h>

namespace History {

void listHistory(int num_entries) {
    auto *hist_state = history_get_history_state();

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

void appendHistoryToFile(int &cmds_since_append, const char *filename) {
    int err = append_history(cmds_since_append, filename);
    if (err != 0) {
        std::cout << "Error: " << strerror(err) << '\n';
    }

    cmds_since_append = 0;
}

void parseHistoryFlag(int &numcmds, const std::vector<std::string> &args) {
    if (args.size() <= 2) {
        return;
    }

    const char *filename = args[2].c_str();

    if (args[1] == "-r") {
        readHistoryFromFile(filename);
    } else if (args[1] == "-w") {
        writeHistoryToFile(filename);
    } else if (args[1] == "-a") {
        appendHistoryToFile(numcmds, filename);
    }
}

} // namespace History
