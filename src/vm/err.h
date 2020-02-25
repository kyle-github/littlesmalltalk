
#pragma once

#include <stdint.h>
#include "memory.h"

extern void error(const char *templ, ...);
extern void info(const char *templ, ...);
#define lst_assert(cond, msg, ...) do { if(!(cond)) { error("%s:%d assert " #cond " failed: " #msg, __func__, __LINE__, __VA_ARGS__); } } while(0)

extern void backTrace(struct object * aContext);
extern void printClass(struct object *obj);
