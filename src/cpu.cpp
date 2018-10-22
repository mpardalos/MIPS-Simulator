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
            if (get_register(inst.src1) < get_register(inst.src2)) {
                set_register(inst.dest, 1);
                advance_pc(4);
            } else {
                set_register(inst.dest, 0);
                advance_pc(4);
            }
            advance_pc(4);
            break;
        case OpFunction::SLTU:
            if ((unsigned int) get_register(inst.src1) < (unsigned int) get_register(inst.src2)) {
                set_register(inst.dest, 1);
                advance_pc(4);
            } else {
                set_register(inst.dest, 0);
                advance_pc(4);
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
            set_register( (unsigned int) inst.src2, memory.get_byte( (unsigned int) inst.src1 + inst.immediate));
            advance_pc(4);
            break;
        case IOpCode::LH:
            set_register(inst.src2, memory.get_halfword(inst.src1 + inst.immediate));
            advance_pc(4);
            break;
        case IOpCode::LHU:
            set_register( (unsigned int) inst.src2, memory.get_halfword( (unsigned int) inst.src1 + inst.immediate));
            advance_pc(4);
            break;
        case IOpCode::LUI:
            set_register(inst.src2, (inst.immediate << 16));
            advance_pc(4);
            break;
        case IOpCode::LW:
            set_register(inst.src2, memory.get_word(get_register(inst.src1) + inst.immediate));
            advance_pc(4);
            break;
        case IOpCode::LWL: //Double Check This
            //Gets the first 8 bits of the word and replaces it the first 8 bits in the dest reg
            set_register(inst.src2, (memory.get_byte(inst.immediate + inst.src1) & 0xFF00) + (get_register(inst.src2) & 0x00FF));
            advance_pc(4);
            break;
        case IOpCode::LWR: //Double Check this
            set_register(inst.src2, (memory.get_byte(inst.immediate + inst.src1) & 0x00FF) + (get_register(inst.src2) & 0xFF00));
            advance_pc(4);
            break;
        case IOpCode::SB: //Confused as how to store ??

            advance_pc(4);
            break;
        case IOpCode::SH:

            advance_pc(4);
            break;
        case IOpCode::SW:

            advance_pc(4);
            break;
        case IOpCode::BEQ:
            if(get_register(inst.src1) == get_register(inst.src2)) {
                advance_pc(inst.immediate << 2);
            } else {
                advance_pc(4);
            }
            break;
        case IOpCode::BQEZ:
            if(get_register(inst.src1) >= 0) {
                advance_pc(inst.immediate << 2);
            } else {
                advance_pc(4);
            }
            break;
        case IOpCode::BGTZ:
            if(get_register(inst.src1) > 0) {
                advance_pc(inst.immediate << 2);
            } else {
                advance_pc(4);
            }
            break;
        case IOpCode::BGEZAL:
            if(get_register(inst.src1) >= 0) {
                set_register(31) = PC + 8;
                advance_pc(inst.immediate << 2);
            } else {
                advance_pc(4);
            }
            break;
        case IOpCode::BLEZ:
            if(get_register(inst.src1) <= 0) {
                advance_pc(inst.immediate << 2);
            } else {
                advance_pc(4);
            }
            break;
        case IOpCode::BLTZ:
            if(get_register(inst.src1) < 0) {
                advance_pc(inst.immediate << 2);
            } else {
                advance_pc(4);
            }
            break;
        case IOpCode::BLTZAL:
            if(get_register(inst.src1) < 0) {
                set_register(31) = PC + 8;
                advance_pc(inst.immediate << 2);
            } else {
                advance_pc(4);
            }
            break;
        case IOpCode::BNE:
            if(get_register(inst.src1) != get_register(inst.src2)) {
                advance_pc(inst.immediate << 2);
            } else {
                advance_pc(4);
            }
            break;
        case IOpCode::ORI: //Do I need to call immediate or can I leave it as inst.immediate?
            set_register(inst.src2, get_register(inst.src1) | inst.immediate);
            advance_pc(4);
            break;
        case IOpCode::ANDI:
            set_register(inst.src2, get_register(inst.src1) & inst.immediate);
            advance_pc(4);
            break;
        case IOpCode::SLTI:
            if(get_register(inst.src1) < inst.immediate) {
                set_register(inst.src2, 1);
                advance_pc(4);
            } else {
                set_register(inst.src2, 0);
                advance_pc(4);
            }
            break;
        case IOpCode::SLTIU:
            if(get_register(inst.src1) < (unsigned int) inst.immediate) {
                set_register(inst.src2, 1);
                advance_pc(4);
            } else {
                set_register(inst.src2, 0);
                advance_pc(4);
            }
            break;
        case IOpCode::XORI:
            set_register(inst.src2, get_register(inst.src1) ^ inst.immediate);
            advance_pc(4);
            break;
        case IOpCode::ADDI:
            set_register(inst.src2, get_register(inst.src1) + inst.immediate);
            advance_pc(4);
            break;
        case IOpCode::ADDIU:
            set_register(inst.src2, (unsigned int) get_register(inst.src1) + inst.immediate);
            advance_pc(4);
            break;
    }
}
