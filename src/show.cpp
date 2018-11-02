#include <string>
#include <bitset>
#include <sstream>

#include "show.hpp"

using namespace std;

template<> string show( const long long& val          ) { return to_string(val); };
template<> string show( const unsigned& val           ) { return to_string(val); };
template<> string show( const unsigned long& val      ) { return to_string(val); };
template<> string show( const unsigned long long& val ) { return to_string(val); };
template<> string show( const float& val              ) { return to_string(val); };
template<> string show( const double& val             ) { return to_string(val); };
template<> string show( const long double& val        ) { return to_string(val); };
template<> string show( const unsigned char& val      ) { return to_string(val); };
template<> string show( const short& val              ) { return to_string(val); };
template<> string show( const unsigned short& val     ) { return to_string(val); };
template<> string show( const as_hex& val             ) { 
    ostringstream ss;
    ss << "0x" << hex << val.val;
    return ss.str();
}
template<> string show( const as_bin& val             ) { 
    ostringstream ss;
    ss << "0b" << bitset<32>(val.val);
    return ss.str();
}
