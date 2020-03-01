
#pragma once

#include <stdio.h>
#include <stdint.h>

extern int fileOut(FILE *fp);
extern int fileIn(FILE *fp);

/* used for bootstrap */
extern void objectWrite(FILE * fp, struct object *obj);

/* image information */
struct image_header {
    char magic[4];
    uint32_t version;
} __attribute__((packed));


#define IMAGE_VERSION_0 (0)
#define IMAGE_VERSION_1 (1)
#define IMAGE_VERSION_2 (2)

