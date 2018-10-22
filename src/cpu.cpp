#include "cpu.hpp"

CPU::CPU(std::unique_ptr<std::vector<Word>> instructions) : memory(std::move(instructions)) {}

int CPU::get_register(RegisterId reg) const {
    if (reg == 0) { return 0; }
    else          { return registers[reg-1]; };
};

void CPU::set_register(RegisterId reg, int value) {
    if (reg == 0) { return; }
    else          { registers[reg-1] = value; }
}

void CPU::advance_pc(Address offset) {
    PC = nPC;
    nPC += offset;
}

void CPU::run() {
    while (true) {
        Instruction inst = decode(memory.get_word(PC));
        execute_instruction(inst);
    }
}

void CPU::execute_instruction(Instruction instruction) {
    instruction.match(
        [&] (R_Instruction inst) {execute_r_type(inst);},
        [&] (I_Instruction inst) {execute_i_type(inst);},
        [&] (J_Instruction inst) {execute_j_type(inst);}
    );
}

void CPU::execute_r_type(R_Instruction inst) { 
    switch (inst.function) {
        case OpFunction::JALR:
            break;
        case OpFunction::JR: 
        case OpFunction::SLL: 
        case OpFunction::SLLV: 
        case OpFunction::SRA: 
        case OpFunction::SRAV: 
        case OpFunction::SRL: 
        case OpFunction::SRLV: 
        case OpFunction::SLT: 
        case OpFunction::SLTU: 
        case OpFunction::ADD:
            set_register(inst.dest, get_register(inst.src1) + get_register(inst.src2));
            advance_pc(4);
            break;
        case OpFunction::ADDU: 
        case OpFunction::SUB: 
        case OpFunction::SUBU: 
        case OpFunction::DIV: 
        case OpFunction::DIVU: 
        case OpFunction::MFHI: 
        case OpFunction::MFLO: 
        case OpFunction::MTHI: 
        case OpFunction::MTLO: 
        case OpFunction::MULT: 
        case OpFunction::MULTU: 
        case OpFunction::XOR: 
        case OpFunction::OR: 
        case OpFunction::AND: break;
        default: break;
    }
}

void CPU::execute_j_type(J_Instruction) { 
}

void CPU::execute_i_type(I_Instruction inst) {
    switch (inst.opcode) {
        case IOpCode::LW:
            set_register(inst.dest, memory.get_word(inst.immediate));
            break;
    }
}