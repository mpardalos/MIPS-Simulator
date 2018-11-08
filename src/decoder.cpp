#include <fstream>
#include <vector>
#include <iostream>

#include "mapbox/variant.hpp"

#include "opcodes.hpp"
#include "debug.hpp"
#include "exceptions.hpp"
#include "show.hpp"

using namespace std;

unsigned short int get_opcode(unsigned int word) { return word >> 26; }

R_Instruction decode_R_type(unsigned int word) {
    unsigned short int opcode_bin = get_opcode(word);
    if (opcode_bin != 0) { exit(1); }

    RegisterId src1 = (word & 0x03E00000) >> 21;
    RegisterId src2 = (word & 0x001F0000) >> 16;
    RegisterId dest = (word & 0x0000F800) >> 11;
    unsigned short int shft = (word & 0x000007C0) >>  6;
    unsigned short int func_bin = (word & 0x0000003F);

    OpFunction func;
    
    switch (func_bin) {
        case 0b001001: func = OpFunction::JALR; break;
        case 0b001000: func = OpFunction::JR; break;
        case 0b000000: func = OpFunction::SLL; break;
        case 0b000100: func = OpFunction::SLLV; break;
        case 0b000011: func = OpFunction::SRA; break;
        case 0b000111: func = OpFunction::SRAV; break;
        case 0b000010: func = OpFunction::SRL; break;
        case 0b000110: func = OpFunction::SRLV; break;
        case 0b101010: func = OpFunction::SLT; break;
        case 0b101011: func = OpFunction::SLTU; break;
        case 0b100000: func = OpFunction::ADD; break;
        case 0b100001: func = OpFunction::ADDU; break;
        case 0b100010: func = OpFunction::SUB; break;
        case 0b100011: func = OpFunction::SUBU; break;
        case 0b011010: func = OpFunction::DIV; break;
        case 0b011011: func = OpFunction::DIVU; break;
        case 0b010000: func = OpFunction::MFHI; break;
        case 0b010010: func = OpFunction::MFLO; break;
        case 0b010001: func = OpFunction::MTHI; break;
        case 0b010011: func = OpFunction::MTLO; break;
        case 0b011000: func = OpFunction::MULT; break;
        case 0b011001: func = OpFunction::MULTU; break;
        case 0b100110: func = OpFunction::XOR; break;
        case 0b100101: func = OpFunction::OR; break;
        case 0b100100: func = OpFunction::AND; break;

        default: 
            throw InvalidInstructionError("Could not match function code " + show(as_bin(opcode_bin)));
            break;
    }

    return R_Instruction { func, dest, src1, src2, shft };
}

I_Instruction decode_I_type(unsigned int word) {
    unsigned short int opcode_bin = get_opcode(word);
    RegisterId src                = (word & 0x03E00000) >> 21;
    RegisterId dest               = (word & 0x001F0000) >> 16;
    Offset immediate              = (word & 0x0000FFFF);

    IOpCode opcode;

    switch (opcode_bin) {
        case 0b100000: opcode = IOpCode::LB; break;      //   Load byte [..] 0b100000 or 32
        case 0b100100: opcode = IOpCode::LBU; break;     //   Load byte unsigned [..] 0b100100 or 36
        case 0b100001: opcode = IOpCode::LH; break;      //   Load half-word [..] 0b100001 or 33
        case 0b100101: opcode = IOpCode::LHU; break;     //   Load half-word unsigned [..] 0b100101 or 37
        case 0b001111: opcode = IOpCode::LUI; break;     //   Load upper immediate [..] 0b001111 or 15 [..] Src = 0
        case 0b100011: opcode = IOpCode::LW; break;      //   Load word [..] 0b100011 or 35
        case 0b100010: opcode = IOpCode::LWL; break;     //   Load word left [..] 0b100010 or 34
        case 0b100110: opcode = IOpCode::LWR; break;     //   Load word right [..] 0b100110 or 38
        case 0b101000: opcode = IOpCode::SB; break;      //   Store byte [..] 0b101000 or 40
        case 0b101001: opcode = IOpCode::SH; break;      //   Store half-word [..] 0b101001 or 41
        case 0b101011: opcode = IOpCode::SW; break;      //   Store word [..] 0b101011 or 43
        case 0b000100: opcode = IOpCode::BEQ; break;     //   Branch on equal [..] 0b000100 or 4
        case 0b000111: opcode = IOpCode::BGTZ; break;    //   Branch on greater than zero [..] 0b000111 or 7 [..] Dest = 0b00000
        case 0b000110: opcode = IOpCode::BLEZ; break;    //   Branch on less than or equal to zero [..] 0b000110 or 6 [..] Dest = 0b00000
        case 0b000101: opcode = IOpCode::BNE; break;     //   Branch on not equal [..] 0b000101 or 5
        case 0b001101: opcode = IOpCode::ORI; break;     //   Bitwise or immediate [..] 0b001101 or 13
        case 0b001100: opcode = IOpCode::ANDI; break;    //   Bitwise and immediate [..] 0b001100 or 12
        case 0b001010: opcode = IOpCode::SLTI; break;    //   Set on less than immediate (signed) [..] 0b001010 or 10
        case 0b001011: opcode = IOpCode::SLTIU; break;   //   Set on less than immediate unsigned [..] 0b001011 or 11
        case 0b001110: opcode = IOpCode::XORI; break;    //   Bitwise exclusive or immediate [..] 0b001110 or 14
        case 0b001000: opcode = IOpCode::ADDI; break;    //   Add immediate (with overflow) [..] 0b001000 or 8
        case 0b001001: opcode = IOpCode::ADDIU; break;   //   Add immediate unsigned (no overflow) [..] 0b001001 or 9

        default: 
            throw InvalidInstructionError("Could not match i type opcode " + show(as_bin(opcode_bin)));
            break;
    }
    
    return I_Instruction { opcode, dest, src, immediate };
}

J_Instruction decode_J_type(unsigned int word) {
    JOpCode opcode;
    Address address = word & 0x3FFFFFF;
    uint8_t opcode_bin = get_opcode(word);

    switch (opcode_bin) {
        case 2: opcode = JOpCode::J; break;
        case 3: opcode = JOpCode::JAL; break;   
        default: 
            throw InvalidInstructionError("Could not match j type opcode " + show(as_bin(opcode_bin)));
            break;
    }
    
    return J_Instruction { opcode, address };
}

REGIMM_Instruction decode_REGIMM(Word word) {
    uint8_t regimm_code_bin = (word & 0xF8000  ) >> 15;
    RegisterId src          = (word & 0x1F00000) >> 20;
    Offset offset           =  word & 0xFFFF;

    REGIMMCode code;
    switch (regimm_code_bin) {
        case 0b00001: code = REGIMMCode::BGEZ; break;
        case 0b10001: code = REGIMMCode::BGEZAL; break;
        case 0b00000: code = REGIMMCode::BLTZ; break;
        case 0b10000: code = REGIMMCode::BLTZAL; break;
        default: 
            throw InvalidInstructionError("Could not match REGIMM code " + show(as_bin(regimm_code_bin)));
            break;
    }
    return REGIMM_Instruction { code, src, offset };
}

/**
 * Decode an instruction.
 * 
 * Special case: decodes BREAK to a REGDUMP if compiled with BREAK_IS_REGDUMP
 */
Instruction decode(unsigned int word) {
    unsigned short int opcode = get_opcode(word); 

    #ifdef BREAK_IS_REGDUMP
    if (opcode == 0 && (word & 0x3F) == 13) {
        return Special_Instruction{ SpecialOpcode::REGDUMP };
    }
    #endif

    switch (opcode) {
        case 0:         return decode_R_type(word);
        case 1:         return decode_REGIMM(word);
        case 2: case 3: return decode_J_type(word);
        default:        return decode_I_type(word);
    }
}