/*
 * Declarations for image-related data and functions.
 */

#pragma once

#include <stdint.h>
#include <stdio.h>

struct image_header {
    char magic[4];
    uint32_t version;
} __attribute__((packed));

#define IMAGE_VERSION_ERROR (0xFFFFFFFF)
#define IMAGE_VERSION_0 (0)
#define IMAGE_VERSION_1 (1)
#define IMAGE_VERSION_2 (2)
#define IMAGE_VERSION_3 (3)
#define IMAGE_VERSION_4 (4)
#define IMAGE_VERSION_5 (5)


/* image reading/writing */
extern int fileIn(FILE *fp);
extern int fileOut(FILE *fp);


/* these define the flags used for writing and reading images.
The bytes per word or size is usually stored in the lower bits */
# define LST_ERROR_TYPE     (0)

/* normal objects */
# define LST_OBJ_TYPE       (1<<5)

/* positive small integers */
# define LST_PINT_TYPE      (2<<5)

/* negative small integers */
# define LST_NINT_TYPE      (3<<5)

/* byte arrays */
# define LST_BARRAY_TYPE    (4<<5)

/* previously dumped object */
# define LST_POBJ_TYPE      (5<<5)

/* nil object */
# define LST_NIL_TYPE       (6<<5)

# define LST_SMALL_TAG_LIMIT    (0x0F)
# define LST_LARGE_TAG_FLAG     (0x10)
# define LST_TAG_SIZE_MASK      (0x1F)
# define LST_TAG_VALUE_MASK     LST_SMALL_TAG_LIMIT
# define LST_TAG_TYPE_MASK      (0xE0)

