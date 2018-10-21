#include <iostream>
#include "memory.hpp"

using namespace std;

int main() {
    cout << "This will test memory:\n";
    cout << "----------------------\n\n";

    vector<Word> init_mem = {};
    Memory memory((unique_ptr<vector<Word>>(&init_mem)));
    
    Address address;
    Word data;
    string option;
    while (true) {
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
        }

    };
}
