#include <iostream>
#include <string>
#include <algorithm>

#include "memory.hpp"
#include "loader.hpp"
#include "decoder.hpp"
#include "cpu.hpp"
#include "test_programs.hpp"
#include "debug.hpp"
#include "exceptions.hpp"

using namespace std;


void memtest() {
    cout << "This will test memory:\n";
    cout << "----------------------\n\n";

    vector<Word> init_mem = {};
    Memory memory((unique_ptr<vector<Word>>(&init_mem)));
    
    Address address;
    Word data;
    string option;

    bool done = false;
    while (!done) {
        cout << "Do you want to read or write? ";
        cin >> option;

        if (option=="write" || option=="w") {
            cout << "Address> ";
            cin >> address;

            cout << "Data (integer)> ";
            cin >> data;

            memory.write_word(address, data);
        } else if (option=="read" || option=="r") {
            cout << "Address> ";
            cin >> address;

            cout << memory.get_word(address) << "\n";
        } else if (option=="end" || option=="q") {
            done = true;
        }

    }
}

int main(int argc, char** argv) {
    if (argc >= 2 && string(argv[1]) == string("-memtest")) {
        memtest();
    } else if (argc >= 2) {
        auto filename = argv[1];
        auto instructions_bin = read_file(filename);
        CPU cpu(move(instructions_bin));
        cpu.run();
    } else {
        run_code(getcharacter);
    }

    return 0;
}
