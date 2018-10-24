#include <vector>
#include <string>
#include <fstream>

#include "debug.hpp"

using namespace std;

vector<uint32_t> read_file(string filename) {
    ifstream istream(filename, ios::binary);

    if (!istream.is_open()) {
        throw invalid_argument(string("Could not open") + filename);
    }

    vector<uint32_t> result;

    char buf[4];
    while (!istream.eof()) {
        istream.read(buf, 4);
        istream.peek(); // Check for eof on the next byte
        uint32_t word = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
        result.push_back(word);
    }

    return result;
}