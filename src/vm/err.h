
#pragma once

#include <stdint.h>
#include "memory.h"
#include "interp.h"


extern void info_impl(const char *func, int line_num, const char *templ, ...);
extern void error_impl(const char *func, int line_num, const char *templ, ...);

#define info(...) info_impl(__func__, __LINE__, __VA_ARGS__)
#define error(...) error_impl(__func__, __LINE__, __VA_ARGS__)

extern void backTrace(struct contextObject * aContext);
extern void dumpObject(struct object *obj, int indent);
extern void dumpMethod(struct object *aMethod, int indent);
