
#pragma once

#include <stdint.h>
#include "memory.h"

extern void sysError(char * a);
extern void sysErrorInt(char * a, intptr_t b);
extern void sysErrorStr(char * a, char * b);
extern void backTrace(struct object * aContext);
