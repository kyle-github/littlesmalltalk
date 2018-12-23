#pragma once

#include "memory.h"

extern char *tmpdir;

struct object *primitive(int primitiveNumber, struct object *args, int *failed);
