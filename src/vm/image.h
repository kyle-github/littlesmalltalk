
#pragma once

#include <stdio.h>
#include <stdint.h>
#include "memory.h"

extern uint8_t get_image_version(FILE *fp);
extern void put_image_version(FILE *fp, uint8_t version);

extern int fileOut(FILE *fp);
extern int fileIn(FILE *fp);

/* used for bootstrap */
extern void objectWrite(FILE * fp, struct object *obj);

/* image information */
struct image_header {
    char magic[4];
    uint32_t version;
} __attribute__((packed));


/*
 * Image header is 5 bytes:
 *    lst!<version>
 *
 * Note that versions up through version 2 use a 4 byte value
 * for the version, but all others use a single byte.
 */

#define IMAGE_HEADER_SIZE (5)
#define IMAGE_HEADER_SIZE_OLD (8)

#define IMAGE_VERSION_0 (0)
#define IMAGE_VERSION_1 (1)
#define IMAGE_VERSION_2 (2)

