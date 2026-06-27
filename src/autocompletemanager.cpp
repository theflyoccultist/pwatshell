#include <cctype>
#include <cstddef>
#include <cstring>
#include <readline/readline.h>
#include <string>
#include "autocompletemanager.hpp"
#include "autocomplete.hpp"

namespace AutoCompleteManager {

const AutoComplete autocomplete;

char *command_generator(const char *text, int state) {
    static std::vector<std::string> matches;
    static size_t match_index = 0;

    if (state == 0) {
        matches.clear();
        match_index = 0;
        matches = autocomplete.execMatch(std::string(text));
    }

    if (match_index < matches.size()) {
        return strdup(matches[match_index++].c_str());
    }

    return nullptr;
}

char **shellCompletion(const char *usrInput, int start, int end) {
    char **matches = nullptr;

    if (start == 0) {
        return rl_completion_matches(usrInput, command_generator);
    }

    return nullptr;
}

} // namespace AutoCompleteManager
