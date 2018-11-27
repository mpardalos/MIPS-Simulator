#include <algorithm>

#include "cpu.hpp"
#include "exceptions.hpp"
#include "opcodes.hpp"
#include "memory.hpp"

CPU::CPU(std::unique_ptr<std::vector<Word>> instructions) :
    memory(std::move(instructions)),
    registers() {}

int CPU::get_register(RegisterId regId) const {
    uint8_t reg = regId.value;
    if (reg == 0) { return 0; }
    else          { return registers[reg-1]; };
};

void CPU::set_register(RegisterId regId, int value) {
    uint8_t reg = regId.value;
    if (reg == 0) { return; }
    else          { registers[reg-1] = value; }
}

void CPU::advance_pc(Address offset) {
    PC = nPC;
    nPC += offset;
}

uint8_t CPU::run() { return run(false); }

uint8_t CPU::run(bool trace) {
    try {
        while (true) {
            // Jump to 0x0 means terminate
            if (PC == 0) break;
            // Executing outside of instruction memory is a a memory error
            if (!is_instruction(PC)) throw MemoryError("Tried to execute address " + show(as_hex(PC)));

            Word inst_bin = memory.get_word(PC);

            // Skip no-ops
            if (inst_bin == 0) {
                advance_pc(4);
                continue;
            }

            Instruction inst = decode(inst_bin);
            if (trace) cout << show(as_hex(PC)) << ": " << show(inst) << endl;
            execute_instruction(inst);
        }
        return get_register(RegisterId{2}) & 0xFF;
    } catch (MIPSError &err) {
        cerr << err.error_message << endl;
        std::exit(err.get_error_code());
    };
}

void CPU::execute_instruction(Instruction instruction) {
    instruction.match(
        [&] (R_Instruction       inst) {      execute_r_type(inst); },
        [&] (I_Instruction       inst) {      execute_i_type(inst); },
        [&] (J_Instruction       inst) {      execute_j_type(inst); },
        [&] (REGIMM_Instruction  inst) { execute_REGIMM_type(inst); },
        [&] (Special_Instruction inst) { execute_Special_type(inst); }
    );
}

void CPU::execute_Special_type(Special_Instruction inst) {
    switch (inst.opcode) {
        case SpecialOpcode::REGDUMP:
            std::cout << "PC:\t"  << show(as_hex(PC))  << std::endl;
            std::cout << "nPC:\t" << show(as_hex(nPC)) << std::endl;
            for (uint8_t i = 0; i <= 31; i++) {
                RegisterId ri = RegisterId{i};
                std::cout << show(ri) << ":\t" << show(as_hex(get_register(ri))) << std::endl;
            }
            advance_pc(4);
            break;
    }
}

void CPU::execute_r_type(R_Instruction inst) {
    switch (inst.function) {
        case OpFunction::JALR:
            set_register(inst.dest, PC + 8);
            PC = nPC;
            nPC = get_register(inst.src1);
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
            break;
        case OpFunction::SLTU:
            if ((unsigned int) get_register(inst.src1) < (unsigned int) get_register(inst.src2)) {
                set_register(inst.dest, 1);
                advance_pc(4);
            } else {
                set_register(inst.dest, 0);
                advance_pc(4);
            }
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
            if(get_register(inst.src2) != 0) {
                LO = get_register(inst.src1) / get_register(inst.src2);
                HI = get_register(inst.src1) % get_register(inst.src2);
            }
            advance_pc(4);
            break;
        case OpFunction::DIVU:
            if(get_register(inst.src2) != 0) {
                LO = static_cast<uint32_t>(get_register(inst.src1)) / static_cast<uint32_t> (get_register(inst.src2));
                HI = static_cast<uint32_t>(get_register(inst.src1)) % static_cast<uint32_t> (get_register(inst.src2));
            }
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
            Product product = static_cast<Product>(get_register(inst.src1)) * static_cast<Product>(get_register(inst.src2));
            LO = product & 0xFFFFFFFF;
            HI = (product >> 32) & 0xFFFFFFFF;
            advance_pc(4);
            break;
        }
        case OpFunction::MULTU: {
            U_Product product = static_cast<U_Product>(static_cast<uint32_t>(get_register(inst.src1))) * static_cast<U_Product>(static_cast<uint32_t>(get_register(inst.src2)));
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
            set_register(rRA, PC + 8);
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
            set_register(rRA, PC + 8);
            if(get_register(inst.src) >= 0) {
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
            set_register(rRA, PC + 8);
            if(get_register(inst.src) < 0) {
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
            set_register(inst.dest, static_cast<int32_t>(static_cast<int8_t>(memory.get_byte(get_register(inst.src) + inst.immediate))));
            advance_pc(4);
            break;
        case IOpCode::LBU:
            set_register(inst.dest, (unsigned int) memory.get_byte(get_register(inst.src) + inst.immediate));
            advance_pc(4);
            break;
        case IOpCode::LH:
            set_register(inst.dest, static_cast<int32_t>(static_cast<int16_t>(memory.get_halfword(get_register(inst.src) + inst.immediate))));
            advance_pc(4);
            break;
        case IOpCode::LHU:
            set_register(inst.dest, (unsigned int) memory.get_halfword(get_register(inst.src) + inst.immediate));
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
            if(((inst.immediate + get_register(inst.src)) % 4) == 0) {
                set_register(inst.dest, memory.get_word(get_register(inst.src) + inst.immediate));
            }
            else if(((inst.immediate + get_register(inst.src)) % 4) == 1) {
                set_register(inst.dest, (memory.get_word(get_register(inst.src) + inst.immediate - 1) << 8) | (get_register(inst.dest) & 0x000000FF));
            }
            else if(((inst.immediate + get_register(inst.src)) % 4) == 2) {
                set_register(inst.dest, ((memory.get_word(get_register(inst.src) + inst.immediate - 2) << 16) | (get_register(inst.dest) & 0x0000FFFF)));
            }
            else if(((inst.immediate + get_register(inst.src)) % 4) == 3) {
                set_register(inst.dest, ((memory.get_word(inst.immediate + get_register(inst.src) - 3) << 24) | (get_register(inst.dest) & 0x00FFFFFF)));
            }
            advance_pc(4);
            break;
        case IOpCode::LWR:
            if(((inst.immediate + get_register(inst.src)) % 4) == 0) {
                set_register(inst.dest,
                    (static_cast<uint32_t>(memory.get_word(get_register(inst.src) + inst.immediate)) >> 24) | (get_register(inst.dest) & 0xFFFFFF00));
            }
            else if(((inst.immediate + get_register(inst.src)) % 4) == 1) {
                set_register(inst.dest,
                    (static_cast<uint32_t>(memory.get_word(get_register(inst.src) + inst.immediate - 1)) >> 16) | (get_register(inst.dest) & 0xFFFF0000));
            }
            else if(((inst.immediate + get_register(inst.src)) % 4) == 2) {
                set_register(inst.dest,
                    (static_cast<uint32_t>(memory.get_word(get_register(inst.src) + inst.immediate - 2)) >> 8) | (get_register(inst.dest) & 0xFF000000));
            }
            else if(((inst.immediate + get_register(inst.src)) % 4) == 3) {
                set_register(inst.dest,
                    memory.get_word(inst.immediate + get_register(inst.src) - 3));
            }
            advance_pc(4);
            break;
        case IOpCode::SB:
            memory.write_byte(get_register(inst.src)+inst.immediate, get_register(inst.dest) & 0xFF);
            advance_pc(4);
            break;
        case IOpCode::SH:
            memory.write_halfword(get_register(inst.src)+inst.immediate, get_register(inst.dest) & 0xFFFF);
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
            set_register(inst.dest, get_register(inst.src) | static_cast<uint16_t>(inst.immediate));
            advance_pc(4);
            break;
        case IOpCode::ANDI:
            set_register(inst.dest, get_register(inst.src) & static_cast<uint16_t>(inst.immediate));
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
            if((unsigned int) get_register(inst.src) <  static_cast<uint32_t>(static_cast<int32_t>(inst.immediate))) {
                set_register(inst.dest, 1);
                advance_pc(4);
            } else {
                set_register(inst.dest, 0);
                advance_pc(4);
            }
            break;
        case IOpCode::XORI:
            set_register(inst.dest, get_register(inst.src) ^ static_cast<uint16_t>(inst.immediate));
            advance_pc(4);
            break;
        case IOpCode::ADDI:
            if(((get_register(inst.src) + inst.immediate) >= 0) && ((get_register(inst.src) < 0) && (inst.immediate < 0))) {
                throw ArithmeticError("Overflow");
            }
            if(((get_register(inst.src) > 0) && (inst.immediate > 0)) && (((get_register(inst.src) + inst.immediate) <= 0))) {
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
