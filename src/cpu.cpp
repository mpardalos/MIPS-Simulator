#include <algorithm>

#include "cpu.hpp"
#include "exceptions.hpp"
#include "opcodes.hpp"

CPU::CPU(std::unique_ptr<std::vector<Word>> instructions) : 
    memory(std::move(instructions)),
    registers() {}

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

uint8_t CPU::run() { return run(false); }

uint8_t CPU::run(bool trace) {
    while (true) {
        if (PC == 0) break;

        Word inst_bin = memory.get_word(PC);
        if (inst_bin == 0) {
            advance_pc(4);
            continue;
        }

        Instruction inst = decode(inst_bin);
        if (trace) cout << show(inst) << endl;
        execute_instruction(inst);
    }
    return get_register(2) & 0xFF;
}

void CPU::execute_instruction(Instruction instruction) {
    try {
        instruction.match(
            [&] (R_Instruction       inst) {      execute_r_type(inst); },
            [&] (I_Instruction       inst) {      execute_i_type(inst); },
            [&] (J_Instruction       inst) {      execute_j_type(inst); },
            [&] (REGIMM_Instruction  inst) { execute_REGIMM_type(inst); },
            [&] (Special_Instruction inst) { execute_Special_type(inst); }
        );
    } catch (MIPSError &err) {
        cerr << err.error_message << endl;
        std::exit(err.get_error_code());
    };
}

void CPU::execute_Special_type(Special_Instruction inst) {
    switch (inst.opcode) {
        case SpecialOpcode::REGDUMP:
            std::cout << "PC:\t"  << std::to_string(PC)  << std::endl;
            std::cout << "nPC:\t" << std::to_string(nPC) << std::endl;
            for (int i = 0; i <= 31; i++) {
                std::cout << "$" << i << ":\t" << std::to_string(get_register(i)) << std::endl;
            }
            break;
    }
}

void CPU::execute_r_type(R_Instruction inst) {
    switch (inst.function) {
        case OpFunction::JALR:
            if(get_register(inst.src2) == 0) {
                set_register(31, PC);
                nPC = get_register(inst.src1);
            } else {
                set_register(get_register(inst.src2), PC);
                nPC = get_register(inst.src1);
            }
            break;
        case OpFunction::JR:
            PC = nPC;
            nPC = get_register(inst.src1);
            break;
        case OpFunction::SLL:
            set_register(inst.dest, get_register(inst.src2) << inst.shift);
            advance_pc(4);
            break;
        case OpFunction::SLLV:
            set_register(inst.dest, (unsigned int) get_register(inst.src2) << (unsigned int) get_register(inst.src1));
            advance_pc(4);
            break;
        case OpFunction::SRA:
            set_register(inst.dest, (int) get_register(inst.src2) >> (int) inst.shift);
            advance_pc(4);
            break;
        case OpFunction::SRAV:
            set_register(inst.dest, (int) get_register(inst.src2) >> (int) get_register(inst.src1));
            advance_pc(4);
            break;
        case OpFunction::SRL:
            set_register(inst.dest, (unsigned int) get_register(inst.src2) >> (unsigned int) inst.shift);
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
            if((get_register(inst.src1) + get_register(inst.src2) >= 0) && (get_register(inst.src1) < 0 && get_register(inst.src2) < 0)) {
                throw ArithmeticError("Overflow");
            }
            if((get_register(inst.src1) > 0 && get_register(inst.src2) > 0) && ((get_register(inst.src1) + get_register(inst.src2) <= 0))) {
                throw ArithmeticError("Overflow");
            }
            set_register(inst.dest, get_register(inst.src1) + get_register(inst.src2));
            advance_pc(4);
            break;
        case OpFunction::ADDU:
            set_register(inst.dest, get_register(inst.src2) + get_register(inst.src1));
            advance_pc(4);
            break;
        case OpFunction::SUB:
            if((get_register(inst.src1) - get_register(inst.src2) >= 0) && (get_register(inst.src1) < 0 && get_register(inst.src2) >= 0)) {
                throw ArithmeticError("Overflow");
            }
            if((get_register(inst.src1) >= 0 && get_register(inst.src2) < 0) && ((get_register(inst.src1) - get_register(inst.src2) < 0))) {
                throw ArithmeticError("Overflow");
            }
            set_register(inst.dest, get_register(inst.src1) - get_register(inst.src2));
            advance_pc(4);
            break;
        case OpFunction::SUBU:
            set_register(inst.dest, get_register(inst.src1) - get_register(inst.src2));
            advance_pc(4);
            break;
        case OpFunction::DIV:
            if(get_register(inst.src2) == 0) {
                throw ArithmeticError("Division by zero");
            }
            LO = get_register(inst.src1) / get_register(inst.src2);
            HI = get_register(inst.src1) % get_register(inst.src2);
            advance_pc(4);
            break;
        case OpFunction::DIVU:
            if(get_register(inst.src2) == 0) {
                throw ArithmeticError("Division by zero");
            }
            LO = get_register(inst.src1) / get_register(inst.src2);
            HI = get_register(inst.src1) % get_register(inst.src2);
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
        case OpFunction::MULT: {
            Product product = get_register(inst.src1) * get_register(inst.src2);
            LO = product & 0xFFFFFFFF;
            HI = (product >> 32) & 0xFFFFFFFF;
            advance_pc(4);
            break;
        }
        case OpFunction::MULTU: {
            U_Product product = static_cast<U_Product>(get_register(inst.src1)) * static_cast<U_Product>(get_register(inst.src2));
            LO = product & 0xFFFFFFFF;
            HI = (product >> 32) & 0xFFFFFFFF;
            advance_pc(4);
            break;
        }
        case OpFunction::XOR:
            set_register(inst.dest, get_register(inst.src1) ^ get_register(inst.src2));
            advance_pc(4);
            break;
        case OpFunction::OR:
            set_register(inst.dest, get_register(inst.src1) | get_register(inst.src2));
            advance_pc(4);
            break;
        case OpFunction::AND: {
            set_register(inst.dest, get_register(inst.src1) & get_register(inst.src2));
            advance_pc(4);
            break;
        }
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
            set_register(inst.dest, (get_register(inst.dest) | (inst.immediate << 16)));
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
            set_register(inst.dest, (memory.get_byte(inst.immediate + get_register(inst.src)) & 0x00FF) | (get_register(inst.dest) & 0xFF00));
            advance_pc(4);
            break;
        case IOpCode::SB:
            memory.write_byte(get_register(inst.src)+inst.immediate, get_register(inst.src) & 0xFF);
            advance_pc(4);
            break;
        case IOpCode::SH:
            memory.write_halfword(get_register(inst.src)+inst.immediate, get_register(inst.src) & 0xFFFF);
            advance_pc(4);
            break;
        case IOpCode::SW:
            memory.write_word(get_register(inst.src)+inst.immediate, get_register(inst.dest));
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
            if((unsigned int) get_register(inst.src) < (unsigned int) inst.immediate) {
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
            if((get_register(inst.src) + get_register(inst.immediate) >= 0) && (get_register(inst.src) < 0 && get_register(inst.immediate < 0))) {
                throw ArithmeticError("Overflow");
            }
            if((get_register(inst.src) > 0 && get_register(inst.immediate) > 0) && ((get_register(inst.src) + get_register(inst.immediate) <= 0))) {
                throw ArithmeticError("Overflow");
            }
            set_register(inst.dest, get_register(inst.src) + inst.immediate);
            advance_pc(4);
            break;
        case IOpCode::ADDIU:
            set_register(inst.dest, (unsigned int) get_register(inst.src) + inst.immediate);
            advance_pc(4);
            break;
    }
}

void run_code(std::vector<Instruction> instructions) {
    auto inst_mem = std::unique_ptr<std::vector<Word>>(new std::vector<Word> {});
    CPU cpu(std::move(inst_mem));

    for (auto inst = instructions.begin(); inst < instructions.end(); inst++) {
        if (cpu.PC == 0) break;
        cpu.execute_instruction(*inst);
    }
}