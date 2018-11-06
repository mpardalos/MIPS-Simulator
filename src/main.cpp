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

/**
 *  Decode filename and print all the decoded instructions to stdout
 */
void decode_and_dump(string filename) {
    auto instructions_bin = read_file(filename);

    try {
        for (auto inst_bin = instructions_bin->begin(); inst_bin < instructions_bin->end(); inst_bin++) {
            cout << show(decode(*inst_bin)) << endl;
        }
    } catch (MIPSError &err) {
        cerr << err.what() << endl;
        exit(err.get_error_code());
    }
}

int main(int argc, char** argv) {
    if (argc >= 2 && string(argv[1]) == string("memtest")) {
        memtest();
    } else if (argc >= 3 && string(argv[1]) == string("decode")) {
        decode_and_dump(argv[2]);
    } else if (argc >= 2) {
        bool trace = (argc >= 3 && argv[1] == string("trace"));
        CPU cpu(read_file(argv[argc-1]));
        cpu.run(trace);
    } else {
        cerr << "Not enough arguments" << endl;
        exit(1);
    }

    return 0;
}
