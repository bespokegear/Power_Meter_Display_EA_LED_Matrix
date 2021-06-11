#include "Util.h"

#include <stdint.h>

bool isInteger(const String s)
{
    for(uint16_t i=0; i<s.length(); i++) {
        if (s[i] < '0' || s[i] > '9') {
            return false;
        }
    }
    return true;
}

bool isInteger(const char* s)
{
    for(uint16_t i=0; s[i]!=0; i++) {
        if (s[i] < '0' || s[i] > '9') {
            return false;
        }
    }
    return true;
}
