#pragma once

#include "autocomplete.hpp"
#include <string>

enum class AutoCompleteState : uint8_t { IDLE, LCP_COMPLETED, LIST_SHOWN, CYCLING_NESTED };

struct AutoCompleteContext {
    AutoCompleteState state = AutoCompleteState::IDLE;
    std::string lastInput = "";
};

class AutoCompleteManager {
  public:
    void completion(std::string &usrInput);

  private:
    AutoComplete autocomplete;
    AutoCompleteContext context;

    void execLCP(const std::vector<FileInfo> &matchingExecutables, std::string &usrInput);
    void execMultipleMatches(const std::vector<FileInfo> &matchingExecutables,
                             const std::string &usrInput);

    void fileLCP(const std::vector<FileInfo> &matchingFiles, std::string &usrInput,
                 const std::string &fileName);
    void fileMultipleMatches(const std::vector<FileInfo> &matchingFiles,
                             const std::string &usrInput);
    void nestedFileCompletion(const std::vector<FileInfo> &matchingFiles, std::string &usrInput);
};
