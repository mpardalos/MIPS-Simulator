#include <vector>
#include <array>
#include <stdexcept>
#include <algorithm>
#include <sstream>

#include "opcodes.hpp"
#include "typedefs.hpp"
#include "memory.hpp"

using namespace std;

// We pass in the vector by unique_ptr so that the memory object "owns" the vector.
// That means that there can't be any other pointers to this vector and thus no one else
// can modify it.
Memory::Memory(
        unique_ptr<vector<Word>> i_instruction_memory) :
    instruction_memory(std::move(i_instruction_memory)),
    data_memory(new vector<Word>(data_size, 0)) {
        assert (instruction_start+(instruction_memory->size()*4) <= data_start);
    }

/**
 * Check if and address is within instruction memory
 */
bool Memory::is_instruction(Address addr) const {
    return addr > instruction_start && addr < (instruction_start+instruction_size);
}

/**
 * Check if and address is within data memory
 */
bool Memory::is_data(Address addr) const {
    return addr > instruction_start && addr < (instruction_start+instruction_size);
}

/**
 * Get a word (4 bytes) from memory.
 *
 * Throws std::invalid_argument if the address is not word-aligned (i.e. it refers to a byte)
 * or if it is out of bounds of the instruction and data memories.
 */
Word Memory::get_word(Address addr) const {
    if (addr % 4 != 0) throw invalid_argument("Word access must be word-aligned");

    if (is_instruction(addr)) {
        int inst_index = (addr - instruction_start) / 4;
        if (inst_index >= instruction_memory->size()) {
            return 0;
        } else {
            return instruction_memory->at(inst_index);
        }
    } else if (is_data(addr)) {
        int data_index = (addr - data_start) / 4;
        return data_memory->at(data_index);
    } else {
        std::stringstream ss;
        ss << "Address " << addr << " is out of bounds";
        throw invalid_argument(ss.str());
    }
}

/**
 * Get a halfword from memory.
 *
 * Throws std::invalid_argument if the address is out of bounds of the instruction and data memories.
 */
Halfword Memory::get_halfword(Address addr) const {
    // Gets the word to which the byte belongs
    Word word = get_word(addr / 2);
    // Selects the 16 bits we are interested in
    Word bitmasked = word & (0xFFFF << addr % 2);
    // And shifts them to the lowest 16 bits
    Halfword = bitmasked >> (addr % 2);

    return Halfword;
}

/**
 * Get a byte from memory.
 *
 * Throws std::invalid_argument if the address is out of bounds of the instruction and data memories.
 */
Byte Memory::get_byte(Address addr) const {
    // Gets the word to which the byte belongs
    Word word = get_word(addr / 4);
    // Selects the 8 bits we are interested in
    Word bitmasked = word & (0xFF << addr % 4);
    // And shifts them to the lowest 8 bits
    Byte byte = bitmasked >> (addr % 4);

    return byte;
}

/**
 * Write a word to memory
 *
 * Throws std::invalid_argument if the address is out of bounds of the instruction and data memories.
 */
void Memory::write_word(Address addr, Word value) {
    if (addr % 4 != 0) throw invalid_argument("Word access must be word-aligned");

    if (is_instruction(addr)) {
        throw invalid_argument("Instruction memory is read-only");
    } else if (is_data(addr)) {
        int data_index = (addr - data_start) / 4;
        data_memory->at(data_index) = value;
    } else {
        std::stringstream ss;
        ss << "Address " << addr << " is out of bounds";
        throw invalid_argument(ss.str());
    }
}
