/*
    Little Smalltalk memory management
    Written by Tim Budd, budd@cs.orst.edu

    Uses baker two-space garbage collection algorithm

    Relicensed under BSD 3-clause license per permission from Dr. Budd by
    Kyle Hayes.

    See LICENSE file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include "memory.h"
#include "interp.h"
#include "globals.h"
#include "err.h"


int64_t gc_count = 0;
int64_t gc_total_time = 0;
int64_t gc_max_time = 0;
int64_t gc_total_mem_copied = 0;
int64_t gc_mem_max_copied = 0;

/*
    static memory space -- never recovered
*/
static struct object *staticBase, *staticTop, *staticPointer;

/*
    dynamic (managed) memory space
    recovered using garbage collection
*/

int spaceSize;
struct object *spaceOne;
struct object *spaceTwo;
int inSpaceOne;

struct object *memoryBase;
struct object *memoryPointer;
struct object *memoryTop;

static struct object *oldBase, *oldTop;

/*
    roots for memory access
    used as bases for garbage collection algorithm
*/
struct object *rootStack[ROOTSTACKLIMIT];
int rootTop = 0;
#define STATICROOTLIMIT (200)
static struct object **staticRoots[STATICROOTLIMIT];
static int staticRootTop = 0;



/* local routines */
//static int64_t time_usec();
void do_gc();


/*
    test routine to see if a pointer is in dynamic memory
    area or not.

    FIXME - this is not portable.
*/

int isDynamicMemory(struct object *x)
{
    return ((x >= spaceOne) && (x <= (spaceOne + spaceSize))) ||
           ((x >= spaceTwo) && (x <= (spaceTwo + spaceSize)));
}

/*
    gcinit -- initialize the memory management system
*/
void gcinit(int staticsz, int dynamicsz)
{
    /* allocate the memory areas */
    staticBase = (struct object *)calloc((size_t)staticsz, sizeof(struct object));
    spaceOne = (struct object *)calloc((size_t)dynamicsz, sizeof(struct object));
    spaceTwo = (struct object *)calloc((size_t)dynamicsz, sizeof(struct object));

    if ((staticBase == NULL) || (spaceOne == NULL) || (spaceTwo == NULL)) {
        error("gcinit(): not enough memory for object space allocations!");
    }

    staticTop = staticBase + staticsz;
    staticPointer = staticTop;

    spaceSize = dynamicsz;
    memoryBase = spaceOne;
    memoryPointer = memoryBase + spaceSize;
    memoryTop = memoryPointer;

    if (debugging) {
        info("space one 0x%p, top 0x%p,"
               " space two 0x%p , top 0x%p",
               (void *)spaceOne, (void *)(spaceOne + spaceSize),
               (void *)spaceTwo, (void *)(spaceTwo + spaceSize));
    }

    inSpaceOne = 1;
}


/*
    gc_move is the heart of the garbage collection algorithm.
    It takes as argument a pointer to a value in the old space,
    and moves it, and everything it points to, into the new space
    The returned value is the address in the new space.
*/

#define IN_NEWSPACE(obj) (((intptr_t)obj >= (intptr_t)memoryBase) && ((intptr_t)obj <= (intptr_t)memoryTop))
#define IN_OLDSPACE(obj) (((intptr_t)obj >= (intptr_t)oldBase) && ((intptr_t)obj <= (intptr_t)oldTop))

static struct object *gc_move(struct mobject *ptr)
{
    struct mobject *old_address = ptr, *previous_object = 0,*new_address = 0, *replacement  = 0;
    int sz;

    while (1) {

        /*
         * part 1.  Walking down the tree
         * keep stacking objects to be moved until we find
         * one that we can handle
         */
        for (;;) {
            /*
             * SmallInt's are not proper memory pointers,
             * so catch them first.  Their "object pointer"
             * value can be used as-is in the new space.
             */
            if (IS_SMALLINT(old_address)) {
                replacement = old_address;
                old_address = previous_object;
                break;

                /*
                 * If we find a pointer in the current space
                 * to the new space (other than indirections) then
                 * something is very wrong
                 */
            } else if (IN_NEWSPACE(old_address)) {
                error("gc_move(): GC invariant failure -- old address points to new space %p", old_address);

                /* else see if not  in old space */
            } else if (!IN_OLDSPACE(old_address)) {
                replacement = old_address;
                old_address = previous_object;
                break;

                /* else see if already forwarded */
            } else if (IS_GCDONE(old_address))  {
                if (IS_BINOBJ(old_address)) {
                    /* reuse class element. */
                    replacement = old_address->data[0];
                } else {
                    sz = SIZE(old_address);
                    replacement = old_address->data[sz];
                }
                old_address = previous_object;
                break;

                /* else see if binary object */
            } else if (IS_BINOBJ(old_address)) {
                int isz;

                isz = SIZE(old_address);
                sz = TO_WORDS(isz);
                memoryPointer = WORDSDOWN(memoryPointer,
                                          sz + 2);
                new_address = (struct mobject *)memoryPointer;
                SET_SIZE(new_address, isz);
                SET_BINOBJ(new_address);
                /* FIXME - use memcpy */
                while (sz) {
                    new_address->data[sz] =
                        old_address->data[sz];
                    sz--;
                }
                SET_GCDONE(old_address);
                new_address->data[0] = previous_object;
                previous_object = old_address;
                old_address = old_address->data[0];
                previous_object->data[0] = new_address;
                /* now go chase down class pointer */

                /* must be non-binary object */
            } else  {
                sz = SIZE(old_address);
                memoryPointer = WORDSDOWN(memoryPointer,
                                          sz + 2);
                new_address = (struct mobject *)memoryPointer;
                SET_SIZE(new_address, sz);
                SET_GCDONE(old_address);
                new_address->data[sz] = previous_object;
                previous_object = old_address;
                old_address = old_address->data[sz];
                previous_object->data[sz] = new_address;
            }
        }

        /*
         * part 2.  Fix up pointers,
         * move back up tree as long as possible
         * old_address points to an object in the old space,
         * which in turns points to an object in the new space,
         * which holds a pointer that is now to be replaced.
         * the value in replacement is the new value
         */
        for (;;) {
            /* backed out entirely */
            if (old_address == 0) {
                return (struct object *) replacement;
            }

            /* case 1, binary or last value */
            if (IS_BINOBJ(old_address) ||
                (SIZE(old_address) == 0)) {

                /* fix up class pointer */
                new_address = old_address->data[0];
                previous_object = new_address->data[0];
                new_address->data[0] = replacement;
                old_address->data[0] = new_address;
                replacement = new_address;
                old_address = previous_object;
            } else {
                sz = SIZE(old_address);
                new_address = old_address->data[sz];
                previous_object = new_address->data[sz];
                new_address->data[sz] = replacement;
                sz--;

                /*
                 * quick cheat for recovering zero fields
                 */
                while (sz && (old_address->data[sz] == 0)) {
                    new_address->data[sz--] = 0;
                }

                SET_SIZE(old_address, sz);
                SET_GCDONE(old_address);
                new_address->data[sz] = previous_object;
                previous_object = old_address;
                old_address = old_address->data[sz];
                previous_object->data[sz] = new_address;
                break; /* go track down this value */
            }
        }
    }

    /* make the compiler happy */
    return (struct object *)0;
}



void do_gc()
{
    int i;
    int64_t start = time_usec();
    int64_t end = 0;

    /* first change spaces */
    if (inSpaceOne) {
        memoryBase = spaceTwo;
        inSpaceOne = 0;
        oldBase = spaceOne;
    } else {
        memoryBase = spaceOne;
        inSpaceOne = 1;
        oldBase = spaceTwo;
    }

    memoryPointer = memoryTop = memoryBase + spaceSize;
    oldTop = oldBase + spaceSize;

    /* then do the collection */
    for (i = 0; i < rootTop; i++) {
        rootStack[i] = gc_move((struct mobject *) rootStack[i]);
    }
    for (i = 0; i < staticRootTop; i++) {
        (* staticRoots[i]) =  gc_move((struct mobject *)
                                      *staticRoots[i]);
    }

    flushCache();

    gc_total_mem_copied += ((char *)memoryTop - (char *)memoryPointer);
    if(((char *)memoryTop - (char *)memoryPointer) > gc_mem_max_copied) {
        gc_mem_max_copied = ((char *)memoryTop - (char *)memoryPointer);
    }

    end = time_usec();

    /* calculate stats about the GC runs. */
    gc_count++;
    gc_total_time += (end - start);

    if(gc_max_time < (end - start)) {
        gc_max_time = (end - start);
    }
}


/*
    gcollect -- garbage collection entry point
*/


struct object *gcollect(int sz)
{
    /* force a GC */
    do_gc();

    /* then see if there is room for allocation */
    memoryPointer = WORDSDOWN(memoryPointer, sz + 2);
    if ((intptr_t)memoryPointer < (intptr_t)memoryBase) {
        error("insufficient memory after garbage collection when allocating object of size %d!", sz);
    }
    SET_SIZE(memoryPointer, sz);

    return(memoryPointer);
}

/*
    static allocation -- tries to allocate values in an area
    that will not be subject to garbage collection
*/

//struct object *staticAllocate(int sz)
//{
//    staticPointer = WORDSDOWN(staticPointer, sz + 2);
//    if (staticPointer < staticBase) {
//        sysError("insufficient static memory");
//    }
//    SETSIZE(staticPointer, sz);
//    return(staticPointer);
//}

//struct object *staticIAllocate(int sz)
//{
//    int trueSize;
//    struct object *result;
//
//    trueSize = (sz + BytesPerWord - 1) / BytesPerWord;
//    result = staticAllocate(trueSize);
//    SETSIZE(result, sz);
//    result->size |= FLAG_BIN;
//    return result;
//}

/*
    if definition is not in-lined, here  is what it should be
*/
#ifndef gcalloc
struct object *gcalloc(int sz)
{
    struct object *result;

    memoryPointer = WORDSDOWN(memoryPointer, sz + 2);
    if (memoryPointer < memoryBase) {
        return gcollect(sz);
    }
    SET_SIZE(memoryPointer, sz);
    return(memoryPointer);
}
# endif

struct object *gcialloc(int sz)
{
    int trueSize;
    struct object *result;

    trueSize = TO_WORDS(sz);
    result = gcalloc(trueSize);
    SET_SIZE(result, sz);
    SET_BINOBJ(result);
    return result;
}



/*
 * addStaticRoot()
 *  Add another object root off a static object
 *
 * Static objects, in general, do not get garbage collected.  When
 * a static object is discovered adding a reference to a non-static
 * object, we link on the reference to our staticRoot table so we can
 * give it proper treatment during garbage collection.
 */
void addStaticRoot(struct object **objp)
{
    int i;

    for (i = 0; i < staticRootTop; ++i) {
        if (objp == staticRoots[i]) {
            return;
        }
    }
    if (staticRootTop >= STATICROOTLIMIT) {
        error("addStaticRoot(): too many static references (max %d) to store object %p!", STATICROOTLIMIT, objp);
    }
    staticRoots[staticRootTop++] = objp;
}

/*
 * map()
 *  Fix an OOP if needed, based on values to be exchanged
 */
static void map(struct object **oop, struct object *a1, struct object *a2, int size)
{
    int x;
    struct object *oo = *oop;

    for (x = 0; x < size; ++x) {
        if (a1->data[x] == oo) {
            *oop = a2->data[x];
            return;
        }
        if (a2->data[x] == oo) {
            *oop = a1->data[x];
            return;
        }
    }
}

/*
 * walk()
 *  Traverse an object space
 */
static void walk(struct object *base, struct object *top,
                 struct object *array1, struct object *array2, int size)
{
    struct object *op, *opnext;
    int x, sz;

    for (op = base; op < top; op = opnext) {
        /*
         * Re-map the class pointer, in case that's the
         * object which has been remapped.
         */
        map(&op->class, array1, array2, size);

        /*
         * Skip our argument arrays, since otherwise things
         * get rather circular.
         */
        sz = SIZE(op);
        if ((op == array1) || (op == array2)) {
            opnext = WORDSUP(op, sz + 2);
            continue;
        }

        /*
         * Don't have to worry about instance variables
         * if it's a binary format.
         */
        if (IS_BINOBJ(op)) {
            int trueSize;

            /*
             * Skip size/class, and enough words to
             * contain the binary bytes.
             */
            trueSize = TO_WORDS(sz);
            opnext = WORDSUP(op, trueSize + 2);
            continue;
        }

        /*
         * For each instance variable slot, fix up the pointer
         * if needed.
         */
        for (x = 0; x < sz; ++x) {
            map(&op->data[x], array1, array2, size);
        }

        /*
         * Walk past this object
         */
        opnext = WORDSUP(op, sz + 2);
    }
}

/*
 * exchangeObjects()
 *  Bulk exchange of object identities
 *
 * For each index to array1/array2, all references in current object
 * memory are modified so that references to the object in array1[]
 * become references to the corresponding object in array2[].  References
 * to the object in array2[] similarly become references to the
 * object in array1[].
 */
void exchangeObjects(struct object *array1, struct object *array2, int size)
{
    int x;

    /*
     * Convert our memory spaces
     */
    walk(memoryPointer, memoryTop, array1, array2, size);
    walk(staticPointer, staticTop, array1, array2, size);

    /*
     * Fix up the root pointers, too
     */
    for (x = 0; x < rootTop; x++) {
        map(&rootStack[x], array1, array2, size);
    }
    for (x = 0; x < staticRootTop; x++) {
        map(staticRoots[x], array1, array2, size);
    }
}





/* symbol table handling routines.  These use internal definitions that
should only be visible here. */

int symstrcomp(struct object *left, const char *right)
{
    int leftsize = SIZE(left);
    int rightsize = (int)strlen(right);
    int minsize = leftsize < rightsize ? leftsize : rightsize;
    register int i;

    if (rightsize < minsize) {
        minsize = rightsize;
    }
    for (i = 0; i < minsize; i++) {
        if ((bytePtr(left)[i]) != (unsigned char)(right[i])) {
            return bytePtr(left)[i]-(unsigned char)(right[i]);
        }
    }
    return leftsize - rightsize;
}


int strsymcomp(const char *left, struct object *right)
{
    /* switch the sign of the result since the comparison
    is the other way */
    return -1 * symstrcomp(right,left);
}



