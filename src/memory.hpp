#ifndef RUNTIME_HPP
#define RUNTIME_HPP

#include <iostream>
#include <array>
#include <vector>
#include <exception>
#include <memory>

#include "opcodes.hpp"
#include "typedefs.hpp"

/**
 * Byte-addressable memory for the MIPS CPU. Contains separate instruction and data memory segments
 * Instruction memory is write-once (through the constructor) while data memory is read-write.
 */
class Memory {
    private:
        // Where each memory segment is located and how big it is.
        // Basically exactly as on the spec
        const unsigned int instruction_start = 0x10000000;
        const unsigned int instruction_size = 0xF000000;
        const unsigned int data_start = 0x20000000;
        const unsigned int data_size = 0x4000000;

        // The actual memory storage.
        // The const marker in the beginning only refers to the unique_ptr. I.e. the address of the vector is const.
        // However, for the instruction memory the vector is marked const too. Therefore instruction memory can't be
        // written to.
        const std::unique_ptr<const std::vector<Word>> instruction_memory;
        const std::unique_ptr<std::vector<Word>> data_memory;

        bool is_instruction(Address addr) const;
        bool is_data(Address addr) const;
        bool is_putc(Address addr) const;
        bool is_getc(Address addr) const;

    public:
        Memory(std::unique_ptr<std::vector<Word>> i_instruction_memory);
        Word get_word(Address) const;
        void write_word(Address, Word);

        Byte get_byte(Address addr) const;
        void write_byte(Address, Byte);

        Halfword get_halfword(Address) const;
        void write_halfword(Address, Halfword);

};

#endif
