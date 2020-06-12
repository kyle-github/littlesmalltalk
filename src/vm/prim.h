#pragma once

#include <stdint.h>
#include "memory.h"

extern char *tmpdir;

extern struct object *primitive(int primitiveNumber, struct object *args, int *failed);
extern struct object *newLInteger(int64_t val);
extern struct object *do_Integer(int op, struct object *low, struct object *high);

