#pragma once

#include <vector>
#include "opcodes.hpp"

std::vector<Instruction> add_and_print = {
    // Load memory start onto $10
    // memory start: 100000000000000000000000000
    I_Instruction {
        IOpCode::ORI, 10, 10, 1
    },
    R_Instruction {
        OpFunction::SLL, 10, 0, 10, 26 
    },
    // Load putc onto $11
    // getc: 110000000000000000000000000000
    I_Instruction {
        IOpCode::ORI, 11, 11, 0b11
    },
    R_Instruction {
        OpFunction::SLL, 11, 0, 11, 28
    },

    // $1 = 10
    I_Instruction {
        IOpCode::ORI, 1, 1, 10
    },
    // $2 = 20
    I_Instruction {
        IOpCode::ORI, 2, 2, 20
    },
    // $1 = $1 + $2
    R_Instruction {
        OpFunction::ADD, 1, 1, 2, 0 
    }, 
    // print $1
    I_Instruction {
        IOpCode::SW, 1, 11, 4
    }
};

std::vector<Instruction> divide_by_zero = {
    R_Instruction {
        OpFunction::DIV, 1, 1, 0, 0
    }
};