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
        [&] (J_Instruction inst) {execute_j_type(inst);},
        [&] (REGIMM_Instruction inst) {execute_REGIMM_type(inst);}
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

void CPU::execute_j_type(J_Instruction inst) {
    switch (inst.opcode) {
        case JOpCode::J:
            PC = nPC;
            nPC = (PC & 0xF0000000) | (inst.address << 2);
            break;
        case JOpCode::JAL:
            set_register(31, PC + 8);
            PC = nPC;
            nPC = (PC & 0xF0000000) | (inst.address << 2);
            break;
    }
}

void CPU::execute_REGIMM_type(REGIMM_Instruction inst) {
    switch (inst.code) {
        case REGIMMCode::BGEZ:
            if(get_register(inst.src) >= 0) {
                advance_pc(inst.offset << 2);
            } else {
                advance_pc(4);
            }
            break;
        case REGIMMCode::BGEZAL:
            if(get_register(inst.src) >= 0) {
                set_register(31, PC + 8);
                advance_pc(inst.offset << 2);
            } else {
                advance_pc(4);
            }
            break;
        case REGIMMCode::BLTZ:
            if(get_register(inst.src) < 0) {
                advance_pc(inst.offset << 2);
            } else {
                advance_pc(4);
            }
            break;
        case REGIMMCode::BLTZAL:
            if(get_register(inst.src) < 0) {
                set_register(31, PC + 8);
                advance_pc(inst.offset << 2);
            } else {
                advance_pc(4);
            }
            break;
    }
}

void CPU::execute_i_type(I_Instruction inst) {
    switch (inst.opcode) {
        case IOpCode::LB:
            set_register(inst.dest, memory.get_byte(inst.src + inst.immediate));
            advance_pc(4);
            break;
        case IOpCode::LBU:
            set_register( (unsigned int) inst.dest, memory.get_byte( (unsigned int) inst.src + inst.immediate));
            advance_pc(4);
            break;
        case IOpCode::LH:
            set_register(inst.dest, memory.get_halfword(inst.src + inst.immediate));
            advance_pc(4);
            break;
        case IOpCode::LHU:
            set_register( (unsigned int) inst.dest, memory.get_halfword( (unsigned int) inst.src + inst.immediate));
            advance_pc(4);
            break;
        case IOpCode::LUI:
            set_register(inst.dest, (inst.immediate << 16));
            advance_pc(4);
            break;
        case IOpCode::LW:
            set_register(inst.dest, memory.get_word(get_register(inst.src) + inst.immediate));
            advance_pc(4);
            break;
        case IOpCode::LWL:
            //Gets the first 8 bits of the word and replaces it the first 8 bits in the dest reg
            set_register(inst.dest, (memory.get_byte(inst.immediate + inst.src) & 0xFF00) | (get_register(inst.dest) & 0x00FF));
            advance_pc(4);
            break;
        case IOpCode::LWR: 
            set_register(inst.dest, (memory.get_byte(inst.immediate + inst.src) & 0x00FF) | (get_register(inst.dest) & 0xFF00));
            advance_pc(4);
            break;
        case IOpCode::SB:
            memory.write_byte(inst.dest+inst.immediate, get_register(inst.src) & 0xFF);
            advance_pc(4);
            break;
        case IOpCode::SH:
            memory.write_halfword(inst.dest+inst.immediate, get_register(inst.src) & 0xFFFF);
            advance_pc(4);
            break;
        case IOpCode::SW:
            memory.write_word(inst.dest+inst.immediate, get_register(inst.src));
            advance_pc(4);
            break;
        case IOpCode::BEQ:
            if(get_register(inst.src) == get_register(inst.dest)) {
                advance_pc(inst.immediate << 2);
            } else {
                advance_pc(4);
            }
            break;
        case IOpCode::BGTZ:
            if(get_register(inst.src) > 0) {
                advance_pc(inst.immediate << 2);
            } else {
                advance_pc(4);
            }
            break;
        case IOpCode::BLEZ:
            if(get_register(inst.src) <= 0) {
                advance_pc(inst.immediate << 2);
            } else {
                advance_pc(4);
            }
            break;
        case IOpCode::BNE:
            if(get_register(inst.src) != get_register(inst.dest)) {
                advance_pc(inst.immediate << 2);
            } else {
                advance_pc(4);
            }
            break;
        case IOpCode::ORI:
            set_register(inst.dest, get_register(inst.src) | inst.immediate);
            advance_pc(4);
            break;
        case IOpCode::ANDI:
            set_register(inst.dest, get_register(inst.src) & inst.immediate);
            advance_pc(4);
            break;
        case IOpCode::SLTI:
            if(get_register(inst.src) < inst.immediate) {
                set_register(inst.dest, 1);
                advance_pc(4);
            } else {
                set_register(inst.dest, 0);
                advance_pc(4);
            }
            break;
        case IOpCode::SLTIU:
            if(get_register(inst.src) < (unsigned int) inst.immediate) {
                set_register(inst.dest, 1);
                advance_pc(4);
            } else {
                set_register(inst.dest, 0);
                advance_pc(4);
            }
            break;
        case IOpCode::XORI:
            set_register(inst.dest, get_register(inst.src) ^ inst.immediate);
            advance_pc(4);
            break;
        case IOpCode::ADDI:
            set_register(inst.dest, get_register(inst.src) + inst.immediate);
            advance_pc(4);
            break;
        case IOpCode::ADDIU:
            set_register(inst.dest, (unsigned int) get_register(inst.src) + inst.immediate);
            advance_pc(4);
            break;
    }
}
