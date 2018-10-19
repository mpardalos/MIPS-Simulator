#include "include/mapbox/variant.hpp"

typedef unsigned short Register;
typedef int Address;


enum class ROpCode { 
    SPECIAL // This opcode is always 0
 };

enum class JOpCode {
    J,       //   Jump
    JAL,     //   Jump and link
};

enum class IOpCode {
    // Loads
    LB,      //   Load byte
    LBU,     //   Load byte unsigned
    LH,      //   Load half-word
    LHU,     //   Load half-word unsigned
    LUI,     //   Load upper immediate
    LW,      //   Load word
    LWL,     //   Load word left
    LWR,     //   Load word right

    // Stores
    SB,      //   Store byte
    SH,      //   Store half-word
    SW,      //   Store word

    // Branching
    BEQ,     //   Branch on equal
    BGEZ,    //   Branch on greater than or equal to zero
    BGEZAL,  //   Branch on non-negative (>=0) and link
    BGTZ,    //   Branch on greater than zero
    BLEZ,    //   Branch on less than or equal to zero
    BLTZ,    //   Branch on less than zero
    BLTZAL,  //   Branch on less than zero and link
    BNE,     //   Branch on not equal

    // Logical
    ORI,     //   Bitwise or immediate
    ANDI,    //   Bitwise and immediate
    SLTI,    //   Set on less than immediate (signed)
    SLTIU,   //   Set on less than immediate unsigned
    XORI,    //   Bitwise exclusive or immediate

    // Arithmetic
    ADDI,    //   Add immediate (with overflow)
    ADDIU,   //   Add immediate unsigned (no overflow)

};

enum class OpFunction {
    // Jumps
    JALR,    //   Jump and link register
    JR,      //   Jump register

    // Shifts
    SLL,     //   Shift left logical
    SLLV,    //   Shift left logical variable
    SRA,     //   Shift right arithmetic
    SRAV,    //   Shift right arithmetic
    SRL,     //   Shift right logical
    SRLV,    //   Shift right logical variable

    // Sets
    SLT,     //   Set on less than (signed)
    SLTU,    //   Set on less than unsigned

    //Arithmetic
    ADD,     //   Add (with overflow)
    ADDU,    //   Add unsigned (no overflow)
    SUB,     //   Subtract
    SUBU,    //   Subtract unsigned
    DIV,     //   Divide
    DIVU,    //   Divide unsigned
    MFHI,    //   Move from HI
    MFLO,    //   Move from LO
    MTHI,    //   Move to HI
    MTLO,    //   Move to LO
    MULT,    //   Multiply
    MULTU,   //   Multiply unsigned

    // Logical
    XOR,     //   Bitwise exclusive or
    OR,      //   Bitwise or
    AND,     //   Bitwise and
};

enum class SpecialOpCode {
    INTERNAL,//   Not associated with a specific instruction
    FUNCTION,//   Testing the ability to support functions
    STACK    //   Testing for functions using the stack

};

struct R_Instruction {
    ROpCode opcode;
    Register dest;
    Register src1; 
    Register src2; 
    unsigned short int shift;
    OpFunction function;
};

struct I_Instruction {
    IOpCode opcode;

    Register src;
    Register dest;

    int immediate;
};

struct J_Instruction {
    JOpCode opcode;
    Address address;
};

typedef mapbox::util::variant<R_Instruction, I_Instruction, J_Instruction> Instruction;
