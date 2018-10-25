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
    if (addr % 4 != 0) std::exit(-11);
        //throw invalid_argument("Word access must be word-aligned");

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
        std::exit(-11); 
        // std::stringstream ss;
        // ss << "Address " << addr << " is out of bounds";
        // throw invalid_argument(ss.str());
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
    Halfword halfword = bitmasked >> (addr % 2);

    return halfword;
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
    if (addr % 4 != 0) std::exit(-11);
        //throw invalid_argument("Word access must be word-aligned");

    if (is_instruction(addr)) {
        std::exit(-11);
        // throw invalid_argument("Instruction memory is read-only");
    } else if (is_data(addr)) {
        int data_index = (addr - data_start) / 4;
        data_memory->at(data_index) = value;
    } else {
        std::exit(-11);
    //     std::stringstream ss;
    //     ss << "Address " << addr << " is out of bounds";
    //     throw invalid_argument(ss.str());
    }
}

/**
 * Write a word to memory
 *
 * Throws std::invalid_argument if the address is out of bounds of the instruction and data memories.
 */
void Memory::write_halfword(Address addr, Halfword value) {
    if (addr % 2 != 0) std::exit(-11); 
    //throw invalid_argument("Halfword access must be halfword-aligned");

    if (is_instruction(addr)) {
        std::exit(-11);
        //throw invalid_argument("Instruction memory is read-only");
    } else if (is_data(addr)) {
        Address data_index = (addr - data_start) / 4;
        Word current = get_word(data_index);

         data_memory->at(data_index) = (addr % 4 == 0)
            ? ((Word) value << 16) | (current & 0x0000FFFF)
            : ((Word) value)       | (current & 0xFFFF0000);
    } else {
        std::exit(-11);
        // std::stringstream ss;
        // ss << "Address " << addr << " is out of bounds";
        // throw invalid_argument(ss.str());
    }
}

/**
 * Write a word to memory
 *
 * Throws std::invalid_argument if the address is out of bounds of the instruction and data memories.
 */
void Memory::write_byte(Address addr, Byte value) {
    if (addr % 2 != 0) std::exit(-11);
    // throw invalid_argument("Halfword access must be halfword-aligned");

    if (is_instruction(addr)) {
        std::exit(-11);
        //throw invalid_argument("Instruction memory is read-only");
    } else if (is_data(addr)) {
        Address data_index = (addr - data_start) / 4;
        Word current = get_word(data_index);

        switch (addr % 4) {
            case 0: data_memory->at(data_index) = ((Word) value << 24) | (current & 0x00FFFFFF); break;
            case 1: data_memory->at(data_index) = ((Word) value << 16) | (current & 0xFF00FFFF); break;
            case 2: data_memory->at(data_index) = ((Word) value << 8 ) | (current & 0xFFFF00FF); break;
            case 3: data_memory->at(data_index) = ((Word) value      ) | (current & 0xFFFFFF00); break;

        }
    } else {
        std::exit(-11);
        // std::stringstream ss;
        // ss << "Address " << addr << " is out of bounds";
        // throw invalid_argument(ss.str());
    }
}