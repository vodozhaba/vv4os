/******************************************************************************
 * File:       /vv4os/stdlib/string.c                                         *
 * Author:     vodozhaba                                                      *
 * Created on: Apr 22, 2017                                                   *
 * Purpose:    Provides str* functions.                                       *
 ******************************************************************************/

#include "string.h"
#include <stddef.h>

int strcmp(const char* str1, const char* str2) {
	size_t i;
	for(i = 0; str1[i] != 0 && str2[i] != 0 && str1[i] == str2[i]; i++);
	if(str1[i] < str2[i])
		return -1;
	else if(str1[i] == str2[i])
		return 0;
	else
		return 1;
}

size_t strlen(const char* str) {
	size_t i;
	for(i = 0; str[i] != 0; i++);
	return i;
}
