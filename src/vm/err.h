
#pragma once

#include <stdint.h>
#include "memory.h"

extern void sysError(char * a);
extern void sysErrorInt(char * a, intptr_t b);
extern void sysErrorStr(char * a, char * b);
extern void error(const char *templ, ...);
extern void info(const char *templ, ...);
extern void backTrace(struct object * aContext);
extern void printClass(struct object *obj);
