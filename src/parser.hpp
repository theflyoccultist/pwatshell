#pragma once

#include "pipeline.hpp"
#include <string>
#include <vector>

namespace Parser {

PipelinePlan parse(const std::vector<std::string> &tokens);

}
