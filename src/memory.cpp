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
bool is_instruction(Address addr) {
    return addr >= instruction_start && addr < (instruction_start+instruction_size);
}

/**
 * Check if and address is within data memory
 */
bool is_data(Address addr) {
    return addr >= data_start && addr < (data_start+data_size);
}

/**
 * Check if and address is within data memory
 */
bool is_getc(Address addr) {
    return addr >= 0x30000000 && addr < 0x30000004;
}

/**
 * Check if and address is within data memory
 */
bool is_putc(Address addr) {
    return addr >= 0x30000004 && addr < 0x30000008;
}

void Memory::memwrite(Address addr, std::function<Word(Word current)> cb) {
    Address word_address = addr & (~0b11);

    auto current = memread_word(word_address);
    write_word(word_address, cb(current));
}


/**
 * Read the word in which an address is contained
 **/
Word Memory::memread_word(Address addr) const {
    Address word_address = addr & (~0b11);

    if (is_instruction(word_address)) {
        unsigned int inst_index = (word_address - instruction_start) / 4;
        if (inst_index >= instruction_memory->size()) {
            return 0;
        } else {
            return instruction_memory->at(inst_index);
        }
    } else if (is_data(word_address)) {
        unsigned int data_index = (word_address - data_start) / 4;
        if (data_index >= data_memory->size()) {
            return 0;
        } else {
            return data_memory->at(data_index);
        }

    } else if (is_putc(word_address)) {
        return 0;
    } else if (is_getc(word_address)) {
        return getchar();
    } else {
        throw MemoryError("Address " + show(as_hex(word_address)) + " is out of bounds");
    }
    
}

/**
 * Get a word (4 bytes) from memory.
 *
 * Throws invalid_argument if the address is not word-aligned (i.e. it refers to a byte)
 * or if it is out of bounds of the instruction and data memories.
 */
Word Memory::get_word(Address addr) const {
    DEBUG_PRINT("Reading word " << show(as_hex(addr)));
    if (addr % 4 != 0) 
        throw MemoryError("Word access must be word-aligned");

    if (is_putc(addr)) 
        throw MemoryError("Can't read from putc address");
    
    return memread_word(addr);
}

/**
 * Get a halfword from memory.
 *
 * Throws invalid_argument if the address is out of bounds of the instruction and data memories.
 */
Halfword Memory::get_halfword(Address addr) const {
    DEBUG_PRINT("Reading halfword " + show(as_hex(addr)));
    if((addr % 2) != 0) {
        throw MemoryError("Address not naturally-aligned");
    }
    // Gets the word to which the halfword belongs
    // by bitmasking the low 2 bits
    Word word = get_word(addr & (~0b11));

    // Shifts the bits we want to the lowest 16 bits
    // addr % 4 can only be 0 or 2 since we are checking that addr % 2 == 0.
    // If it's zero then we are trying to read the upper halfword and so we need to shift 
    // right by 16.
    // If it's 2 then we are trying to read the lower halfword and so we don't need to shift
    Word shifted = word >> (8 * (2 - (addr % 4)));

    // And shifts them to the lowest 16 bits
    Halfword halfword = shifted & 0xFFFF;

    return halfword;
}

/**
 * Get a byte from memory.
 *
 * Throws invalid_argument if the address is out of bounds of the instruction and data memories.
 */
Byte Memory::get_byte(Address addr) const {
    DEBUG_PRINT("Reading byte " + show(as_hex(addr)));
    int shift_amount = 8 * (3 - (addr % 4));

    // Gets the word to which the byte belongs
    // by masking the low 2 bits
    Word word = get_word(addr & (~0b11)); 

    // Shifts the bits we want to the lowest 8 bits
    Word shifted = word >> shift_amount;
    // And masks everything else
    Byte byte = shifted & 0xFF;

    return byte;
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
        unsigned int data_index = (addr - data_start) / 4;

        if (data_index >= data_memory->size()) {
            data_memory->resize(data_memory->size() + data_index + 100, 0);
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
    memwrite(addr, [&addr, &value] (Word current) {
        Word result = 0;
        switch (addr % 4) {
            case 0: result = (static_cast<Word>(value) << 24) | (current & 0x00FFFFFF); break;
            case 1: result = (static_cast<Word>(value) << 16) | (current & 0xFF00FFFF); break;
            case 2: result = (static_cast<Word>(value) << 8 ) | (current & 0xFFFF00FF); break;
            case 3: result = (static_cast<Word>(value)      ) | (current & 0xFFFFFF00); break;
        }
        return result;
    });
}