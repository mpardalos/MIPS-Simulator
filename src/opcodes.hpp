#ifndef OPCODES_HPP
#define OPCODES_HPP

#include "mapbox/variant.hpp"
#include "typedefs.hpp"

enum class JOpCode {
    J,       //   Jump
    JAL,     //   Jump and link
};

enum class IOpCode {
    // Loads
    LB,      //   Load byte [..] 0b100000 or 32
    LBU,     //   Load byte unsigned [..] 0b100100 or 36
    LH,      //   Load half-word [..] 0b100001 or 33
    LHU,     //   Load half-word unsigned [..] 0b100101 or 37
    LUI,     //   Load upper immediate [..] 0b001111 or 15 [..] Src = 0
    LW,      //   Load word [..] 0b100011 or 35
    LWL,     //   Load word left [..] 0b100010 or 34
    LWR,     //   Load word right [..] 0b100110 or 38

    // Stores
    SB,      //   Store byte [..] 0b101000 or 40
    SH,      //   Store half-word [..] 0b101001 or 41
    SW,      //   Store word [..] 0b101011 or 43

    // Branching
    BEQ,     //   Branch on equal [..] 0b000100 or 4
    BGTZ,    //   Branch on greater than zero [..] 0b000111 or 7 [..] Dest = 0b00000
    BLEZ,    //   Branch on less than or equal to zero [..] 0b000110 or 6 [..] Dest = 0b00000
    BLTZ,    //   Branch on less than zero [..] 0b000001 or 1 [..] Dest = 0b00000
    BLTZAL,  //   Branch on less than zero and link [..] 0b000001 or 1 [..] Dest = 0b10000
    BNE,     //   Branch on not equal [..] 0b000101 or 5

    // Logical
    ORI,     //   Bitwise or immediate [..] 0b001101 or 13
    ANDI,    //   Bitwise and immediate [..] 0b001100 or 12
    SLTI,    //   Set on less than immediate (signed) [..] 0b001010 or 10
    SLTIU,   //   Set on less than immediate unsigned [..] 0b001011 or 11
    XORI,    //   Bitwise exclusive or immediate [..] 0b001110 or 14

    // Arithmetic
    ADDI,    //   Add immediate (with overflow) [..] 0b001000 or 8
    ADDIU,   //   Add immediate unsigned (no overflow) [..] 0b001001 or 9

};

enum class REGIMMCode {
    BGEZ, // Branch on Greater Than or Equal to Zero (0b00001)
    BGEZAL, // Branch on Greater Than or Equal to Zero and Link (0b10001)
    BLTZ, // Branch on less than zero (0b00000)
    BLTZAL, // Branch on less than zero and link (0b10000) 
};

enum class OpFunction {
    // Jumps
    JALR,    //   Jump and link register
    JR,      //   Jump register

    // Shifts [..] All Opcode = 0
    SLL,     //   Shift left logical [..] Func = 0 [..] Src1 = 0
    SLLV,    //   Shift left logical variable [..] Shift = 0 [..] Func = 0b000100 or 4
    SRA,     //   Shift right arithmetic [..] Src1 = 0 [..] Func = 0b000011 or 3
    SRAV,    //   Shift right arithmetic [..] Shift = 0 [..] Func = 0b000111 or 7
    SRL,     //   Shift right logical [..] Src1 = 0 [..] Func = 0b000010 or 2
    SRLV,    //   Shift right logical variable [..] Shift = 0 [..] Func = 0b000110 or 5

    // Sets [..] Opcode = 0 [..] Shift = 0
    SLT,     //   Set on less than (signed) [..] Func = 0b101010 or 42
    SLTU,    //   Set on less than unsigned [..] Func = 0b101011 or 43

    //Arithmetic [..] Opcode = 0
    ADD,     //   Add (with overflow) [..] 0b100000 or 32
    ADDU,    //   Add unsigned (no overflow) [..] Shift = 0 [..] Func 0b100001 or 33
    SUB,     //   Subtract [..] Shift = 0 [..] Func 0b100010 or 34
    SUBU,    //   Subtract unsigned [..] Shift = 0 [..] Func 0b100011 or 35
    DIV,     //   Divide [..] Dest = 0 [..] Shift = 0 [..] Func 0b011010 or 26
    DIVU,    //   Divide unsigned [..] Dest = 0 [..] Shift = 0 [..] Func = 0b011011 or 27
    MFHI,    //   Move from HI [..] Src 1 & 2 = 0 [..] Shift = 0 [..] Func 0b010000 or 16
    MFLO,    //   Move from LO [..] Src 1 & 2 = 0 [..] Shift = 0 [..] Func 0b010010 or 18
    MTHI,    //   Move to HI [..] Src 2 & Dest & Shift = 0 [..] Func 0b010001 or 17
    MTLO,    //   Move to LO [..] Src 2 & Dest & Shift = 0 [..] Func 0b010011 or 19
    MULT,    //   Multiply [..] Dest & Shift = 0 [..] Func = 0b011000 or 24
    MULTU,   //   Multiply unsigned [..] Dest & Shift = 0 [..] Func 0b011001 or 25

    // Logical [..] Opcode $ Shift = 0
    XOR,     //   Bitwise exclusive or [..] Func 0b100110 or 38
    OR,      //   Bitwise or [..] Func 0b100101 or 37
    AND,     //   Bitwise and [..] Func 0b100100 or 36
};

enum class SpecialOpCode {
    INTERNAL,//   Not associated with a specific instruction
    FUNCTION,//   Testing the ability to support functions
    STACK    //   Testing for functions using the stack

};

struct R_Instruction {
    RegisterId dest;
    RegisterId src1; 
    RegisterId src2; 
    unsigned short int shift;
    OpFunction function;
};

struct I_Instruction {
    IOpCode opcode;

    RegisterId src;
    RegisterId dest;

    Offset immediate;
};

struct REGIMM_Instruction {
    RegisterId src;
    REGIMMCode code;
    Offset offset;
};

struct J_Instruction {
    JOpCode opcode;
    Address address;
};

typedef mapbox::util::variant<R_Instruction, I_Instruction, J_Instruction> Instruction;

#endif