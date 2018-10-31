#include <string>
#include "show.hpp"
#include "opcodes.hpp"

using namespace std;

template<>
string show(const JOpCode& code) {
    switch (code) {
        case JOpCode::J:   return "J";
        case JOpCode::JAL: return "JAL";
    }
}
 
template<>
string show(const IOpCode& code) {
    switch (code) {
        case IOpCode::LB:    return "LB";
        case IOpCode::LBU:   return "LBU";
        case IOpCode::LH:    return "LH";
        case IOpCode::LHU:   return "LHU";
        case IOpCode::LUI:   return "LUI";
        case IOpCode::LW:    return "LW";
        case IOpCode::LWL:   return "LWL";
        case IOpCode::LWR:   return "LWR";
        case IOpCode::SB:    return "SB";
        case IOpCode::SH:    return "SH";
        case IOpCode::SW:    return "SW";
        case IOpCode::BEQ:   return "BEQ";
        case IOpCode::BGTZ:  return "BGTZ";
        case IOpCode::BLEZ:  return "BLEZ";
        case IOpCode::BNE:   return "BNE";
        case IOpCode::ORI:   return "ORI";
        case IOpCode::ANDI:  return "ANDI";
        case IOpCode::SLTI:  return "SLTI";
        case IOpCode::SLTIU: return "SLTIU";
        case IOpCode::XORI:  return "XORI";
        case IOpCode::ADDI:  return "ADDI";
        case IOpCode::ADDIU: return "ADDIU";
    }
}

template<>
string show(const OpFunction& code) {
    switch (code) {
        case OpFunction::JALR: return "JALR";
        case OpFunction::JR: return "JR";
        case OpFunction::SLL: return "SLL";
        case OpFunction::SLLV: return "SLLV";
        case OpFunction::SRA: return "SRA";
        case OpFunction::SRAV: return "SRAV";
        case OpFunction::SRL: return "SRL";
        case OpFunction::SRLV: return "SRLV";
        case OpFunction::SLT: return "SLT";
        case OpFunction::SLTU: return "SLTU";
        case OpFunction::ADD: return "ADD";
        case OpFunction::ADDU: return "ADDU";
        case OpFunction::SUB: return "SUB";
        case OpFunction::SUBU: return "SUBU";
        case OpFunction::DIV: return "DIV";
        case OpFunction::DIVU: return "DIVU";
        case OpFunction::MFHI: return "MFHI";
        case OpFunction::MFLO: return "MFLO";
        case OpFunction::MTHI: return "MTHI";
        case OpFunction::MTLO: return "MTLO";
        case OpFunction::MULT: return "MULT";
        case OpFunction::MULTU: return "MULTU";
        case OpFunction::XOR: return "XOR";
        case OpFunction::OR: return "OR";
        case OpFunction::AND: return "AND";
    }
}

template<> 
string show(const REGIMMCode& code) {
    switch (code) {
        case REGIMMCode::BGEZ:   return "BGEZ";
        case REGIMMCode::BGEZAL: return "BGEZAL";
        case REGIMMCode::BLTZ:   return "BLTZ";
        case REGIMMCode::BLTZAL: return "BLTZAL";
    }
}

template<>
string show(const SpecialOpcode& code) {
    switch (code) {
        case SpecialOpcode::REGDUMP: return "REGDUMP";
    }
}

template<> 
string show(const R_Instruction& inst) {
    return show(inst.function) 
        + "(src1: " + show(inst.src1) + ", "
        +  "src2: " + show(inst.src1) + ", "
        +  "dest: " + show(inst.src1) + ")";
}

template<> std::string show(const I_Instruction& inst) {
    return show(inst.opcode) 
        + "(src: " + show(inst.src) + ", "
        + "dest: " + show(inst.dest) + ", "
        + "immediate: " + show(inst.immediate) + ")";
}

template<> std::string show(const J_Instruction& inst) {
    return show(inst.opcode) 
        + "(address: " + show(inst.address) + ")";
}

template<> std::string show(const REGIMM_Instruction& inst) {
    return show(inst.code) 
        + "(src: " + show(inst.src) + ", "
        + "offset: " + show(inst.offset) + ")";
}

template<> std::string show(const Special_Instruction& inst) {
    return show(inst.opcode) + "()";
}

template<> std::string show(const Instruction& inst) {
    return inst.match(
        [&] (const R_Instruction&       inst) { return show(inst); },
        [&] (const I_Instruction&       inst) { return show(inst); },
        [&] (const J_Instruction&       inst) { return show(inst); },
        [&] (const REGIMM_Instruction&  inst) { return show(inst); },
        [&] (const Special_Instruction& inst) { return show(inst); }
    );
}