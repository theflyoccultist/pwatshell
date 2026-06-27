#pragma once

#include "pipeline.hpp"
#include <string>
#include <vector>

class Parser {
  public:
    [[nodiscard]] PipelinePlan parse(const std::vector<std::string> &tokens) const;

  private:
    static constexpr size_t MAX_LINE_LEN = 4096;
    static constexpr char BACKSPACE = 127;
};
