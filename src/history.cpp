#include "history.hpp"
#include "paths.hpp"
#include "shellstate.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
#include <readline/history.h>
#include <string>

History::History() {
    std::ifstream in(Paths::getPathHist());
    if (in.is_open()) {
        std::string line;
        while (std::getline(in, line)) {
            add_history(line.c_str());
        }
    }
}

History::~History() {
    std::string histFile = Paths::getPathHist();
    if (!histFile.empty()) {
        this->writeHistoryToFile(histFile.c_str());
    }
}

void History::listHistory(int num_entries) {
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

void History::parseHistoryFlag(const std::vector<std::string> &args) {
    if (args.size() <= 2) {
        return;
    }

    const char *filename = args[2].c_str();

    if (args[1] == "-r") {
        this->readHistoryFromFile(filename);
    } else if (args[1] == "-w") {
        this->writeHistoryToFile(filename);
    } else if (args[1] == "-a") {
        this->appendHistoryToFile(filename);
    }
}

void History::readHistoryFromFile(const char *filename) {
    int err = read_history(filename);
    if (err != 0) {
        std::cout << "Error: " << strerror(err) << '\n';
    }
}

void History::writeHistoryToFile(const char *filename) {
    int err = write_history(filename);
    if (err != 0) {
        std::cout << "Error: " << strerror(err) << '\n';
    }
}

void History::appendHistoryToFile(const char *filename) {
    ShellState state;
    int err = append_history(static_cast<int>(state.cmds_since_append), filename);
    if (err != 0) {
        std::cout << "Error: " << strerror(err) << '\n';
    }

    state.cmds_since_append = 0;
}
