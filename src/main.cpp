#include <iostream>
#include "decoder.hpp"

int main() {
    std::cout << "Testing: \n\n";
    unsigned int i = 0b00001010000000000000000000000000;
    decode(i);
}
