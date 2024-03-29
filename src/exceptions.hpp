#pragma once

#include <stdexcept>
#include "debug.hpp"

class MIPSError : public std::exception { 
    public:
        std::string error_message;
        MIPSError(std::string msg) : error_message(msg) {};

        const char* what() const noexcept { return error_message.c_str(); }
        virtual inline int get_error_code() { return -20; };
};

class ArithmeticError : public MIPSError {
    public:
        using MIPSError::MIPSError;
        inline int get_error_code() override { return -10; };
};

class MemoryError : public MIPSError {
    public:
        using MIPSError::MIPSError;
        inline int get_error_code() override { return -11; };
};

class InvalidInstructionError : public MIPSError {
    public:
        using MIPSError::MIPSError;
        inline int get_error_code() override { return -12; };
};