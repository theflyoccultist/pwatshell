#pragma once

#include "pipeline.hpp"
#include <string>
#include <vector>

namespace Parser {

std::string parseUsrInput();
PipelinePlan parse(const std::vector<std::string> &tokens);

} // namespace Parser
