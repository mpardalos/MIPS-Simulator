#include <fstream>
#include <vector>
#include <iostream>

#include "mapbox/variant.hpp"

#include "opcodes.hpp"

unsigned short int get_opcode(unsigned int word) { return word >> 26; }

R_Instruction decode_R_type(unsigned int word) {
    unsigned short int opcode_bin = get_opcode(word);
    if (opcode_bin != 0) { exit(1); }
    RegisterId src1 = (word & 0x03E00000) >> 21;
    RegisterId src2 = (word & 0x001F0000) >> 16;
    RegisterId dest = (word & 0x0000F800) >> 11;
    unsigned short int shft = (word & 0x000007C0) >>  6;
    unsigned short int func_bin = (word & 0x0000003F);

    ROpCode opcode = ROpCode::SPECIAL;
    OpFunction func;
    
    switch (func_bin) {
        default: exit(1); break;
    }

    return { opcode, dest, src1, src2, shft, func };
}

I_Instruction decode_I_type(unsigned int word) {
    unsigned short int opcode_bin = get_opcode(word);
    RegisterId src                = (word & 0x03E00000) >> 21;
    RegisterId dest               = (word & 0x001F0000) >> 16;
    int immediate                 = (word & 0x0000FFFF);

    IOpCode opcode;

    switch (opcode_bin) {
        default: exit(1); break;
    }
    
    return I_Instruction { opcode, src, dest, immediate };
}

J_Instruction decode_J_type(unsigned int word) {
    JOpCode opcode;
    Address address = word & 0x3FFFFFF;

    switch (get_opcode(word)) {
        case 2: opcode = JOpCode::J; break;
        case 3: opcode = JOpCode::JAL; break;   
        default: exit(1); break;
    }
    
    return J_Instruction { opcode, address };
}

/**
 * This should return Instruction
 */
Instruction decode(unsigned int word) {
    unsigned short int opcode = get_opcode(word); 

    switch (opcode) {
        case 0: 
            return decode_R_type(word);
        case 2: case 3: 
            return decode_J_type(word);
        default: 
            return decode_I_type(word);
    }
}

std::vector<Instruction> load_file(std::string path) {
    std::ifstream inFile;
    std::vector<unsigned int> instructions;

    inFile.open(path.c_str());
    if (!inFile.is_open()) {
        std::cout << "File does not exiiiist\n";
        exit(1);
    }

    auto current_instruction = instructions.begin();
    while (inFile >> *current_instruction) {
        current_instruction++;
    }

    std::vector<Instruction> result;
    for (auto instruction=instructions.begin(); instruction < instructions.end(); instruction++) {
        /* result.push_back(decode(instruction)); */
    }

    return result;
}


