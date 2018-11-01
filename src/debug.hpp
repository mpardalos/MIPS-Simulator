#pragma once

#include <iostream>

// #define DEBUG

#ifdef DEBUG
#   define DEBUG_PRINT(str) std::cerr << "DEBUG: " << str << std::endl;
#else
#   define DEBUG_PRINT(str) do {} while (0);
#endif

