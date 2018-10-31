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
        OpFunction::SLL, 11, 0, 11, 30
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

std::vector<Instruction> overflow = {
    I_Instruction {
        IOpCode::ORI, 1, 1, 0xFFFF,
    },
    R_Instruction {
        OpFunction::SLL, 1, 0, 1, 16
    },
    I_Instruction {
        IOpCode::ORI, 1, 1, 0xFFFF,
    },
    I_Instruction {
        IOpCode::ADDI, 1, 1, 1
    }
};

std::vector<Instruction> just_addi = {
    I_Instruction {
        IOpCode::ADDI, 1, 1, 500
    }
};

std::vector<Instruction> print_A = {
    R_Instruction {
        OpFunction::JR, 0, 0, 0, 0
    },
    // Load getc onto $11
    // getc: 110000000000000000000000000000
    I_Instruction {
        IOpCode::ORI, 11, 11, 0b11
    },
    Special_Instruction {
        SpecialOpcode::REGDUMP
    },
    R_Instruction {
        OpFunction::SLL, 11, 0, 11, 28
    },
    // Store the char 'A' in $1
    I_Instruction {
        IOpCode::ORI, 1, 1, 65
    },
    // print $1
    I_Instruction {
        IOpCode::SW, 11, 1, 4
    },
};