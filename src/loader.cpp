#include <vector>
#include <string>
#include <fstream>
#include <memory>

#include "debug.hpp"
#include "typedefs.hpp"
#include "show.hpp"

using namespace std;

unique_ptr<vector<Word>> read_file(string filename) {
    ifstream istream(filename, ios::binary);

    if (!istream.is_open()) {
        std::exit(-21);
    }

    unique_ptr<vector<Word>> result(new vector<Word>());

    char buf[4];
    while (!istream.eof()) {
        istream.read(buf, 4);
        istream.peek(); // Check for eof on the next byte

        uint32_t word = (static_cast<unsigned char>(buf[0]) << 24) 
            | (static_cast<unsigned char>(buf[1]) << 16) 
            | (static_cast<unsigned char>(buf[2]) << 8) 
            | static_cast<unsigned char>(buf[3]);
        result->push_back(word);
    }

    return result;
}