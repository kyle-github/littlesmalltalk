
#pragma once

#include <stdint.h>
#include "memory.h"

//extern void sysError(char * a);
//extern void sysErrorInt(char * a, intptr_t b);
//extern void sysErrorStr(char * a, char * b);

extern void info_impl(const char *func, int line_num, const char *templ, ...);
extern void error_impl(const char *func, int line_num, const char *templ, ...);

#define info(...) info_impl(__func__, __LINE__, __VA_ARGS__)
#define error(...) error_impl(__func__, __LINE__, __VA_ARGS__)

//extern void error(const char *templ, ...);
//extern void info(const char *templ, ...);
extern void backTrace(struct object * aContext);
