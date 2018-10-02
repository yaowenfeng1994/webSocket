//
// Created by 姚文锋 on 2018/10/2.
//

#ifndef _BASE64_H
#define _BASE64_H

#include <string>

using namespace std;

string base64_encode(unsigned char const*, unsigned int len);
string base64_decode(string const& s);

#endif //_BASE64_H
