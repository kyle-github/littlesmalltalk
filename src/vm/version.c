#include <stdint.h>
#include "version.h"

/*
 * The library version in various ways.
 *
 * The defines are for building in specific versions and then
 * checking them against a dynamically linked library.
 */

const char *VERSION=LIB_VER_STRING;
const uint64_t version_major = LIB_VER_MAJOR;
const uint64_t version_minor = LIB_VER_MINOR;
const uint64_t version_patch = LIB_VER_PATCH;
