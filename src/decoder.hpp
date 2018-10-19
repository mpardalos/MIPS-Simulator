#ifndef DECODE_HPP
#define DECODE_HPP

#include <vector>
#include "opcodes.hpp"

void decode(unsigned int word);
std::vector<Instruction> load_file(std::string path);

#endif
