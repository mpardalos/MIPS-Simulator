#include <vector>
#include <array>
#include <stdexcept>
#include <algorithm>
#include <sstream>

#include "opcodes.hpp"
#include "typedefs.hpp"
#include "memory.hpp"
#include "exceptions.hpp"
#include "show.hpp"

using namespace std;

// We pass in the vector by unique_ptr so that the memory object "owns" the vector.
// That means that there can't be any other pointers to this vector and thus no one else
// can modify it.
Memory::Memory(
        unique_ptr<vector<Word>> i_instruction_memory) :
    instruction_memory(move(i_instruction_memory)),
    data_memory(new vector<Word>()) {
        assert (instruction_start+(instruction_memory->size()*4) <= data_start);
    }

/**
 * Check if and address is within instruction memory
 */
bool Memory::is_instruction(Address addr) const {
    return addr >= instruction_start && addr < (instruction_start+instruction_size);
}

/**
 * Check if and address is within data memory
 */
bool Memory::is_data(Address addr) const {
    return addr > instruction_start && addr < (instruction_start+instruction_size);
}

/**
 * Check if and address is within data memory
 */
bool Memory::is_getc(Address addr) const {
    return addr >= 0x30000000 && addr < 0x30000004;
}

/**
 * Check if and address is within data memory
 */
bool Memory::is_putc(Address addr) const {
    return addr >= 0x30000004 && addr < 0x30000008;
}

/**
 * Get a word (4 bytes) from memory.
 *
 * Throws invalid_argument if the address is not word-aligned (i.e. it refers to a byte)
 * or if it is out of bounds of the instruction and data memories.
 */
Word Memory::get_word(Address addr) const {
    if (addr % 4 != 0) throw MemoryError("Word access must be word-aligned");

    if (is_instruction(addr)) {
        unsigned int inst_index = (addr - instruction_start) / 4;
        if (inst_index >= instruction_memory->size()) {
            return 0;
        } else {
            return instruction_memory->at(inst_index);
        }
    } else if (is_data(addr)) {
        int data_index = (addr - data_start) / 4;
        return data_memory->at(data_index);
    } else if (is_putc(addr)) {
        throw MemoryError("Can't read from putc address");
    } else if (is_getc(addr)) {
        char input;
        cin >> input;
        return static_cast<Word>(input);
    } else {
        throw MemoryError("Address " + show(as_hex(addr)) + " is out of bounds");
    }
}

/**
 * Get a halfword from memory.
 *
 * Throws invalid_argument if the address is out of bounds of the instruction and data memories.
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
 * Throws invalid_argument if the address is out of bounds of the instruction and data memories.
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

void Memory::memwrite(Address addr, std::function<Word(Word current)> cb) {
    auto current = get_word(addr);
    write_word(addr, cb(current));
}

/**
 * Write a word to memory
 *
 * Throws invalid_argument if the address is out of bounds of the instruction and data memories.
 */
void Memory::write_word(Address addr, Word value) {
    DEBUG_PRINT("mem(" << show(as_hex(addr)) << ") = " << show(value))
    if (addr % 4 != 0) throw MemoryError("Word access must be word-aligned");

    if (is_instruction(addr)) {
        throw MemoryError("Instruction memory is read-only");
    } else if (is_data(addr)) {

        int data_index = (addr - data_start) / 4;

        if (data_index > data_memory->capacity()) {
            data_memory->reserve(data_index+100);
        }
        data_memory->at(data_index) = value;

    } else if (is_putc(addr)) {
        cout << static_cast<char>(value & 0xFF);
    } else if (is_getc(addr)) {
        throw MemoryError("Can't write to getc address");
    } else {
        throw MemoryError("Address " + show(as_hex(addr)) + " is out of bounds");
    }
}

/**
 * Write a word to memory
 *
 * Throws invalid_argument if the address is out of bounds of the instruction and data memories.
 */
void Memory::write_halfword(Address addr, Halfword value) {
    if (addr % 2 != 0) throw MemoryError("Halfword access must be halfword-aligned"); 

    memwrite(addr, [&addr, &value] (Word current) {
         return (addr % 4 == 0)
            ? ((Word) value << 16) | (current & 0x0000FFFF)
            : ((Word) value)       | (current & 0xFFFF0000);

    });
}

/**
 * Write a word to memory
 *
 * Throws invalid_argument if the address is out of bounds of the instruction and data memories.
 */
void Memory::write_byte(Address addr, Byte value) {
    if (addr % 2 != 0) throw MemoryError("Halfword access must be halfword-aligned");

    memwrite(addr, [&addr, &value] (Word current) {
        switch (addr % 4) {
            case 0: return ((Word) value << 24) | (current & 0x00FFFFFF); break;
            case 1: return ((Word) value << 16) | (current & 0xFF00FFFF); break;
            case 2: return ((Word) value << 8 ) | (current & 0xFFFF00FF); break;
            case 3: return ((Word) value      ) | (current & 0xFFFFFF00); break;
        }
    });
}