#pragma once

#include <vector>
#include <string>
#include <memory>

std::unique_ptr<std::vector<uint32_t>> read_file(std::string filename);