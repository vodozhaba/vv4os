/******************************************************************************
 * File:       /vv4os/stdlib/stdlib.c                                         *
 * Author:     velikiyv4                                                      *
 * Created on: Aug 21, 2016                                                   *
 * Purpose:    Provides standard functions.                                   *
 ******************************************************************************/

#include <stddef.h>

static unsigned int digits(unsigned int x, int base) {
    int pow = base, ret;
    for(ret = 1; x >= (unsigned int) pow; ++ret, pow *= base);
    return ret;
}

char* utoa(unsigned int value, char* str, int base) {
    if(base < 2 || base > 36)
        return NULL;
    static const char* alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int len = digits(value, base);
    for(int i = len - 1; i >= 0; --i) {
        str[i] = alphabet[value % base];
        value /= base;
    }
    str[len] = 0;
    return str;
}

char* itoa(int value, char* str, int base) {
    if(value < 0) {
        str[0] = '-';
        return utoa(-value, str + 1, base);
    }
    return utoa(value, str, base);
}
