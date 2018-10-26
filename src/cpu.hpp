#include <vector>
#include <array>

#include "typedefs.hpp"
#include "decoder.hpp"
#include "memory.hpp"

class CPU {
    private:
        Memory memory;
        // 31 because register 0 is always 0
        std::array<int, 31> registers;

        Address PC = 0x10000000;
        Address nPC = 0x10000004;
        int LO = 0;
        int HI = 0;

        int get_register(RegisterId reg) const;
        void set_register(RegisterId reg, int value);
        void advance_pc(Address offset);

        friend void run_code(std::vector<Instruction>);

    public:
        CPU(std::unique_ptr<std::vector<Word>> instructions); 

        void run();
        void execute_instruction(Instruction instruction);
        void execute_r_type(R_Instruction inst);
        void execute_j_type(J_Instruction);
        void execute_i_type(I_Instruction);
        void execute_REGIMM_type(REGIMM_Instruction);
};

void run_code(std::vector<Instruction>);
