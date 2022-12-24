/*
    Little Smalltalk
    Written by Tim Budd, budd@cs.orst.edu

    Relicensed under BSD 3-clause license per permission from Dr. Budd by
    Kyle Hayes.

    See LICENSE file.
*/

/*
    The fundamental data type is the object.
    The first field in an object is a size, the low order two
    bits are used to maintain:
            * binary flag, used if data is binary
            * indirection flag, used if object has been relocated
            
    The next data field is the class.

    The following fields are either objects, or byte/character values


    Now we use a simple two-space algorithm without a static space.
    This is not as efficient for GC as the minimal generational
    implementation used before, but is a lot nicer for image writing.
*/

#pragma once

#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

/* Basic object memory structure */

struct object {
    uintptr_t header;
    struct object *class;
    struct object *data[];
};

/*
    byte objects are used to represent strings and symbols
        bytes per word must be correct
*/

struct byteObject {
    uintptr_t header;
    struct object *class;
    uint8_t bytes[];
};

/*
 * Used for GC.  Exposed here to keep all the
 * object definitions in one place.
 */

struct mobject {
    uintptr_t header;
    struct mobject *data[];
};


#define BytesPerWord ((int)(sizeof (intptr_t)))
#define bytePtr(x) (((struct byteObject *) x)->bytes)
#define WORDSUP(ptr, amt) ((struct object *)(((char *)(ptr)) + ((amt) * BytesPerWord)))
#define WORDSDOWN(ptr, amt) WORDSUP(ptr, 0 - (amt))

#define TO_WORDS(isz) (((isz) + BytesPerWord - 1)/BytesPerWord)

/*
 * SmallInt objects are used to represent short integers.  They are
 * encoded as 31 bits, signed, with the low bit set to 1.  This
 * distinguishes them from all other objects, which are longword
 * aligned and are proper C memory pointers.
 */

#define IS_SMALLINT(x) ((((intptr_t)(x)) & 0x01) != 0)
#define FITS_SMALLINT(x) ((((intptr_t)(x)) >= INT_MIN/2) && \
                          (((intptr_t)(x)) <= INT_MAX/2))
#define CLASS(x) (IS_SMALLINT(x) ? SmallIntClass : ((x)->class))
#define integerValue(x) ((int)(((intptr_t)(x)) >> 1))
#define newInteger(x) ((struct object *)((((intptr_t)(x)) * 2) | 0x01))

/*
 * The "size" field is the top 30 bits; the bottom two are flags
 */
#define SIZE(op) ((uint32_t)(((struct object *)(op))->header) >> 2)
#define SET_SIZE(op, val) (((struct object *)(op))->header = (uintptr_t)((uint32_t)(val) << 2))

/* handle the other flags in the header. */
#define FLAG_GCDONE (0x01)
#define IS_GCDONE(o) (((struct object *)(o))->header & (uintptr_t)FLAG_GCDONE)
#define SET_GCDONE(o) (((struct object *)(o))->header |= (uintptr_t)FLAG_GCDONE)

#define FLAG_BIN (0x02)
#define IS_BINOBJ(o) (((struct object *)(o))->header & (uintptr_t)FLAG_BIN)
#define SET_BINOBJ(o) (((struct object *)(o))->header |= (uintptr_t)FLAG_BIN)

#define NOT_NIL(o) ((o) && ((o) != nilObject))

/*
    memoryBase holds the pointer to the current space,
    memoryPointer is the pointer into this space.
    To allocate, decrement memoryPointer by the correct amount.
    If the result is less than memoryBase, then garbage collection
    must take place
*/

extern int spaceSize;
extern struct object *spaceOne;
extern struct object *spaceTwo;
extern int inSpaceOne;

extern struct object *memoryBase;
extern struct object *memoryPointer;
extern struct object *memoryTop;


/*
    roots for the memory space
    these are traced down during memory management
    rootStack is the dynamic stack
    staticRoots are values in static memory that point to
    dynamic values
*/
# define ROOTSTACKLIMIT 2000
extern struct object *rootStack[];
extern int rootTop;


#define PUSH_ROOT(o) (rootStack[rootTop++] = (o))
#define PEEK_ROOT()  (rootStack[rootTop - 1])
#define POP_ROOT()   (rootStack[--rootTop])

extern void addStaticRoot(struct object **);

/* image reading/writing */
extern int fileIn(FILE *fp);
extern int fileOut(FILE *fp);

/*
    entry points
*/

extern void gcinit(int, int);
extern struct object *gcollect(int);
extern struct object *staticAllocate(int);
extern struct object *staticIAllocate(int);
extern struct object *gcialloc(int);
extern void do_gc();
extern void exchangeObjects(struct object *, struct object *, int size);
extern int symstrcomp(struct object *left, const char *right);
extern int strsymcomp(const char *left, struct object *right);
extern int isDynamicMemory(struct object *);

#define STATICROOTLIMIT (200)
extern struct object **staticRoots[STATICROOTLIMIT];
extern int staticRootTop;


/* count args */
#define COUNT_NARG(...)                                                \
         COUNT_NARG_(__VA_ARGS__,COUNT_RSEQ_N())

#define COUNT_NARG_(...)                                               \
         COUNT_ARG_N(__VA_ARGS__)

#define COUNT_ARG_N(                                                   \
          _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
         _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
         _61,_62,_63,N,...) N

#define COUNT_RSEQ_N()                                                 \
         63,62,61,60,                   \
         59,58,57,56,55,54,53,52,51,50, \
         49,48,47,46,45,44,43,42,41,40, \
         39,38,37,36,35,34,33,32,31,30, \
         29,28,27,26,25,24,23,22,21,20, \
         19,18,17,16,15,14,13,12,11,10, \
         9,8,7,6,5,4,3,2,1,0


#ifndef BOOTSTRAP

    // #define gcalloc(sz) (((intptr_t)(memoryPointer = WORDSDOWN(memoryPointer, (sz) + 2)) < \
    //                       (intptr_t)memoryBase) ? gcollect(sz) : \
    //                      (SET_SIZE(memoryPointer, (sz)), memoryPointer))

    inline static struct object *gcalloc(int size) 
    {
        return (((intptr_t)(memoryPointer = WORDSDOWN(memoryPointer, (size) + 2)) < (intptr_t)memoryBase) ? 
                                gcollect(size) : 
                                (SET_SIZE(memoryPointer, (size)), memoryPointer));
    }

    inline static struct object *gcalloc_protect_impl(int size, int num_protect_objs, ...) 
    {
        struct object *result = NULL;
        int oldStaticRootTop = staticRootTop;
        va_list va;

        fprintf(stderr, "size=%d, number of objects to protect %d.\n", size, num_protect_objs);

        /* push all the objects to be protected on the static root stack */
        va_start(va, num_protect_objs);
        for(int i=0; i < num_protect_objs; i++) {
            struct object **protected_obj = va_arg(va, struct object **);
            staticRoots[staticRootTop++] = protected_obj;
        }
        va_end(va);

        result = gcalloc(size);

        /* restore the old stack top */
        staticRootTop = oldStaticRootTop;

        return result;
    }

    #define gcalloc_protect(size, ...) gcalloc_protect_impl(size, COUNT_NARG(__VA_ARGS__), __VA_ARGS__)
#else
    extern struct object *gcalloc(int);
#endif


extern int64_t gc_count;
extern int64_t gc_total_time;
extern int64_t gc_max_time;
extern int64_t gc_total_mem_copied;
extern int64_t gc_mem_max_copied;
