#pragma once

#include "autocompletemanager.hpp"
#include "pipeline.hpp"
#include <string>
#include <vector>

class Parser {
  public:
    std::string parseUsrInput();
    PipelinePlan parse(const std::vector<std::string> &tokens);

  private:
    static constexpr size_t MAX_LINE_LEN = 4096;
    static constexpr char BACKSPACE = 127;

    AutoCompleteManager autocompletemanager;
};
