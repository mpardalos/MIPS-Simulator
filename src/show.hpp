#pragma once

#include <string>

template<typename T> std::string show(const T& val);

struct as_hex { 
    unsigned int val; 
    inline as_hex(unsigned int ival): val(ival) {};
};

// Replicate all to_string functionstemplate<> std::string show( const long& val               ) { return std::to_string(val); };
template<> std::string show( const long long& val          );
template<> std::string show( const unsigned& val           );
template<> std::string show( const unsigned long& val      );
template<> std::string show( const unsigned long long& val );
template<> std::string show( const float& val              );
template<> std::string show( const double& val             );
template<> std::string show( const long double& val        );
template<> std::string show( const unsigned char& val      );
template<> std::string show( const short& val              );
template<> std::string show( const unsigned short& val     );
template<> std::string show( const as_hex& val);

