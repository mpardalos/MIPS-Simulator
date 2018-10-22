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
            set_register(31, PC);
            nPC = get_register(inst.src1);
            break;
        case OpFunction::JR:
            PC = nPC;
            nPC = get_register(inst.src1);
            break;
        case OpFunction::SLL:
            set_register(inst.dest, (unsigned int) get_register(inst.src2) << (unsigned int) get_register(inst.shift));
            advance_pc(4);
            break;
        case OpFunction::SLLV:
            set_register(inst.dest, (unsigned int) get_register(inst.src2) << (unsigned int) get_register(inst.src1));
            advance_pc(4);
            break;
        case OpFunction::SRA:
            set_register(inst.dest, (int) get_register(inst.src2) >> (int) get_register(inst.shift));
            advance_pc(4);
            break; 
        case OpFunction::SRAV:
            set_register(inst.dest, (int) get_register(inst.src2) >> (int) get_register(inst.src1));
            advance_pc(4);
            break;
        case OpFunction::SRL:
            set_register(inst.dest, (unsigned int) get_register(inst.src2) >> (unsigned int) get_register(inst.shift));
            advance_pc(4);
            break;
        case OpFunction::SRLV:
            set_register(inst.dest, (unsigned int) get_register(inst.src2) >> (unsigned int) get_register(inst.src1));
            advance_pc(4);
            break;
        case OpFunction::SLT:
            if (get_register(inst.src2) < get_register(inst.src1)) {
                set_register(inst.dest, 1);
            } else {
                set_register(inst.dest, 0);
            }
            advance_pc(4);
            break;
        case OpFunction::SLTU:
            if ((unsigned int) get_register(inst.src2) < (unsigned int) get_register(inst.shift)) {
                set_register(inst.dest, 1);
            } else {
                set_register(inst.dest, 0);
            }
            advance_pc(4);
            break;
        case OpFunction::ADD:
            set_register(inst.dest, get_register(inst.src1) + get_register(inst.src2));
            advance_pc(4);
            break;
        case OpFunction::ADDU:
            set_register(inst.dest, (unsigned int) get_register(inst.src2) + (unsigned int) get_register(inst.src1));
            advance_pc(4);
            break;
        case OpFunction::SUB:
            set_register(inst.dest, get_register(inst.src1) - get_register(inst.src2));
            advance_pc(4);
            break;
        case OpFunction::SUBU:
            set_register(inst.dest, (unsigned int) get_register(inst.src1) - (unsigned int) get_register(inst.src2));
            advance_pc(4);
            break;
        case OpFunction::DIV:
            LO = get_register(inst.src1) / get_register(inst.src2);
            HI = get_register(inst.src1) % get_register(inst.src2);
            advance_pc(4);
            break;
        case OpFunction::DIVU:
            LO = (unsigned int) get_register(inst.src1) / (unsigned int) get_register(inst.src2);
            HI = (unsigned int) get_register(inst.src1) % (unsigned int) get_register(inst.src2);
            advance_pc(4);
            break;
        case OpFunction::MFHI:
            set_register(inst.dest, HI);
            advance_pc(4);
            break;
        case OpFunction::MFLO:
            set_register(inst.dest, LO);
            advance_pc(4);
            break;
        case OpFunction::MTHI:
            HI = get_register(inst.src1);
            advance_pc(4);
            break;
        case OpFunction::MTLO:
            LO = get_register(inst.src1);
            advance_pc(4);
            break;
        case OpFunction::MULT:
            LO = get_register(inst.src1) * get_register(inst.src2);
            advance_pc(4);
            break;
        case OpFunction::MULTU:
            LO = (unsigned int) get_register(inst.src1) * (unsigned int) get_register(inst.src2);
            advance_pc(4);
            break;
        case OpFunction::XOR:
            set_register(inst.dest, get_register(inst.src1) ^ get_register(inst.src2));
            advance_pc(4);
            break;
        case OpFunction::OR:
            set_register(inst.dest, get_register(inst.src1) | get_register(inst.src2));
            advance_pc(4);
            break;
        case OpFunction::AND:
            set_register(inst.dest, get_register(inst.src1) & get_register(inst.src2));
            advance_pc(4);
            break;
        default: break;
    }
}

void CPU::execute_j_type(J_Instruction) { 
}

void CPU::execute_i_type(I_Instruction inst) {
    switch (inst.opcode) {
        case IOpCode::LB:
            set_register(inst.src2, memory.get_byte(inst.src1 + inst.immediate));
            advance_pc(4);
            break;
        case IOpCode::LBU:
            break;
        case IOpCode::LH:
            break;
        case IOpCode::LHU:
            break;
        case IOpCode::LUI:
            break;
        case IOpCode::LW:
            break;
        case IOpCode::LWL:
            break;
        case IOpCode::LWR:
            break;
        case IOpCode::SB:
            break;
        case IOpCode::SH:
            break;
        case IOpCode::SW:
            break;
        case IOpCode::BEQ:
            break;
        case IOpCode::BGTZ:
            break;
        case IOpCode::BLEZ:
            break;
        case IOpCode::BLTZ:
            break;
        case IOpCode::BLTZAL:
            break;
        case IOpCode::BNE:
            break;
        case IOpCode::ORI:
            break;
        case IOpCode::ANDI:
            break;
        case IOpCode::SLTI:
            break;
        case IOpCode::SLTIU:
            break;
        case IOpCode::XORI:
            break;
        case IOpCode::ADDI:
            break;
        case IOpCode::ADDIU:
            break;
    }
}