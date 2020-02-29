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

static struct object *spaceOne, *spaceTwo;
static int spaceSize;

struct object *memoryBase, *memoryPointer, *memoryTop;

static int inSpaceOne;
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
static void do_gc();


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
        sysError("not enough memory for space allocations\n");
    }

    staticTop = staticBase + staticsz;
    staticPointer = staticTop;

    spaceSize = dynamicsz;
    memoryBase = spaceOne;
    memoryPointer = memoryBase + spaceSize;
    memoryTop = memoryPointer;

    if (debugging) {
        printf("space one 0x%lx, top 0x%lx,"
               " space two 0x%lx , top 0x%lx\n",
               (intptr_t)spaceOne, (intptr_t)(spaceOne + spaceSize),
               (intptr_t)spaceTwo, (intptr_t)(spaceTwo + spaceSize));
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
                sysErrorInt("GC invariant failure -- address in new space",
                            (intptr_t)old_address);

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



static void do_gc()
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
        sysErrorInt("insufficient memory after garbage collection", sz);
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
    File in and file out of Smalltalk images
*/

static int indirtop = 0;
static struct object **indirArray;




/* return the size in bytes necessary to accurately handle the integer
value passed.  Note that negatives will always get BytesPerWord size.
This will return zero if the passed value is less than LST_SMALL_TAG_LIMIT.
In this case, the value can be packed into the tag it self when read or
written. */

//static int getIntSize(int val)
//{
//    int i;
//    /* negatives need sign extension.  this is a to do. */
//    if(val<0) {
//        return BytesPerWord;
//    }
//
//    if(val<LST_SMALL_TAG_LIMIT) {
//        return 0;
//    }
//
//    /* how many bytes? */
//
//    for(i=1; i<BytesPerWord; i++)
//        if(val < (1<<(8*i))) {
//            return i;
//        }
//
//    return BytesPerWord;
//}





static int get_byte(FILE *fp)
{
    static int offset = 0;
    int inByte;

    inByte = fgetc(fp);
    offset++;

    //fprintf(stderr, "%06d: %u (%x)\n", offset, inByte, inByte);

    return inByte;
}


/* image file reading routines */


static void readTag(FILE *fp, int *type, int *val)
{
    int i;
    int tempSize;
    int inByte;

    inByte = get_byte(fp);
    if (inByte == EOF) {
        sysError("Unexpected EOF reading image file: reading tag byte.");
    }

    tempSize = (int)(inByte & LST_TAG_SIZE_MASK);
    *type = (int)(inByte & LST_TAG_TYPE_MASK);

    if(tempSize & LST_LARGE_TAG_FLAG) {
        uint tmp = 0;

        /* large size, actual value is in succeeding
        bytes (tempSize bytes). The value is not sign
        extended. */
        *val = 0;

        /* get the number of bytes in the size field */
        tempSize = tempSize & LST_SMALL_TAG_LIMIT;

        if(tempSize>BytesPerWord) {
            sysErrorInt("Error reading image file: tag value field exceeds machine word size.  Image created on another machine? Value=", tempSize);
        }

        for(i=0; i<tempSize; i++) {
            inByte = get_byte(fp);

            if(inByte == EOF) {
                sysErrorInt("Unexpected EOF reading image file: reading extended value and expecting byte count=", tempSize);
            }

            tmp = tmp  | (((uint)inByte & (uint)0xFF) << ((uint)8*(uint)i));
        }

        *val = (int)tmp;
    } else {
        *val = tempSize;
    }
}


/**
* objectRead
*
* Read in an object from the input image file.  Several kinds of object are
* handled as special cases.  The routine readTag above does most of the work
* of figuring out what type of object it is and how big it is.
*/

struct object *objectRead(FILE *fp)
{
    int type;
    int size;
    int val;
    int i;
    struct object *newObj=(struct object *)0;
    struct byteObject *bnewObj;

    /* get the tag header for the object, this has a type and value */
    readTag(fp,&type,&val);

    switch(type) {
    case LST_ERROR_TYPE:    /* nil obj */
        sysErrorInt("Read in a null object", (intptr_t)newObj);

        break;

    case LST_OBJ_TYPE:  /* ordinary object */
        size = val;
//        newObj = staticAllocate(size);
        newObj = gcalloc(size);
        indirArray[indirtop++] = newObj;
        newObj->class = objectRead(fp);

        /* get object field values. */
        for (i = 0; i < size; i++) {
            newObj->data[i] = objectRead(fp);
        }

        break;

    case LST_PINT_TYPE: /* positive integer */
        newObj = newInteger(val);
        break;

    case LST_NINT_TYPE: /* negative integer */
        newObj = newInteger(-val);
        break;

    case LST_BARRAY_TYPE:   /* byte arrays */
        size = val;
//        newObj = staticIAllocate(size);
        newObj = gcialloc(size);
        indirArray[indirtop++] = newObj;
        bnewObj = (struct byteObject *) newObj;
        for (i = 0; i < size; i++) {
            /* FIXME check for EOF! */
            bnewObj->bytes[i] = (uint8_t)get_byte(fp);
        }

        bnewObj->class = objectRead(fp);
        break;

    case LST_POBJ_TYPE: /* previous object */
        if(val>indirtop) {
            sysErrorInt("Illegal previous object index",val);
        }

        newObj = indirArray[val];

        break;

    case LST_NIL_TYPE:  /* object 0 (nil object) */
        newObj = indirArray[0];
        break;

    default:
        sysErrorInt("Illegal tag type: ",type);
        break;
    }

    return newObj;
}



static uint32_t get_image_version(FILE *fp)
{
    struct image_header header;
    int rc;

    rc = (int)fread(&header, sizeof(header), 1, fp);

    if(rc != 1) {
        sysError("Unable to read version header data!");
    }

    if(header.magic[0] == 'l' && header.magic[1] == 's' && header.magic[2] == 't' && header.magic[3] == '!') {
        return header.version;
    } else {
        /* not a newer version, seek back to the start. */
        fseek(fp, 0, SEEK_SET);
        return 0;
    }
}




int fileIn_version_0(FILE *fp)
{
    int i;

    /* use the currently unused space for the indir pointers */
    if (inSpaceOne) {
        indirArray = (struct object * *) spaceTwo;
    } else {
        indirArray = (struct object * *) spaceOne;
    }
    indirtop = 0;

    /* read in the image file */
    fprintf(stderr, "reading nil object.\n");
    nilObject = objectRead(fp);
    staticRoots[staticRootTop++] = &nilObject;

    fprintf(stderr, "reading true object.\n");
    trueObject = objectRead(fp);
    staticRoots[staticRootTop++] = &trueObject;

    fprintf(stderr, "reading false object.\n");
    falseObject = objectRead(fp);
    staticRoots[staticRootTop++] = &falseObject;

    fprintf(stderr, "reading globals object.\n");
    globalsObject = objectRead(fp);
    staticRoots[staticRootTop++] = &globalsObject;

    fprintf(stderr, "reading SmallInt class.\n");
    SmallIntClass = objectRead(fp);
    staticRoots[staticRootTop++] = &SmallIntClass;

    fprintf(stderr, "reading Integer class.\n");
    IntegerClass = objectRead(fp);
    staticRoots[staticRootTop++] = &IntegerClass;

    fprintf(stderr, "reading Array class.\n");
    ArrayClass = objectRead(fp);
    staticRoots[staticRootTop++] = &ArrayClass;

    fprintf(stderr, "reading Block class.\n");
    BlockClass = objectRead(fp);
    staticRoots[staticRootTop++] = &BlockClass;

    fprintf(stderr, "reading Context class.\n");
    ContextClass = objectRead(fp);
    staticRoots[staticRootTop++] = &ContextClass;

    fprintf(stderr, "reading initial method.\n");
    initialMethod = objectRead(fp);
    staticRoots[staticRootTop++] = &initialMethod;

    fprintf(stderr, "reading binary message objects.\n");
    for (i = 0; i < 3; i++) {
        binaryMessages[i] = objectRead(fp);
        staticRoots[staticRootTop++] = &binaryMessages[i];
    }

    fprintf(stderr, "reading bad method symbol.\n");
    badMethodSym = objectRead(fp);
    staticRoots[staticRootTop++] = &badMethodSym;

    /* clean up after ourselves. */
    memset((void *) indirArray,(int)0,(size_t)((size_t)spaceSize * sizeof(struct object)));

    fprintf(stderr, "Read in %d objects.\n", indirtop);

    return indirtop;
}




int fileIn_version_1(FILE *fp)
{
    int i;

    /* use the currently unused space for the indir pointers */
    if (inSpaceOne) {
        indirArray = (struct object * *) spaceTwo;
    } else {
        indirArray = (struct object * *) spaceOne;
    }
    indirtop = 0;

    /* read the base objects from the image file. */

    fprintf(stderr, "reading globals object.\n");
    globalsObject = objectRead(fp);
    staticRoots[staticRootTop++] = &globalsObject;

    fprintf(stderr, "reading initial method.\n");
    initialMethod = objectRead(fp);
    staticRoots[staticRootTop++] = &initialMethod;

    fprintf(stderr, "reading binary message objects.\n");
    for (i = 0; i < 3; i++) {
        binaryMessages[i] = objectRead(fp);
        staticRoots[staticRootTop++] = &binaryMessages[i];
    }

    fprintf(stderr, "reading bad method symbol.\n");
    badMethodSym = objectRead(fp);
    staticRoots[staticRootTop++] = &badMethodSym;

    /* fix up everything from globals. */
    nilObject = lookupGlobal("nil");
    staticRoots[staticRootTop++] = &nilObject;

    trueObject = lookupGlobal("true");
    staticRoots[staticRootTop++] = &trueObject;

    falseObject = lookupGlobal("false");
    staticRoots[staticRootTop++] = &falseObject;

    SmallIntClass = lookupGlobal("SmallInt");
    staticRoots[staticRootTop++] = &SmallIntClass;

    IntegerClass = lookupGlobal("Integer");
    staticRoots[staticRootTop++] = &IntegerClass;

    ArrayClass = lookupGlobal("Array");
    staticRoots[staticRootTop++] = &ArrayClass;

    BlockClass = lookupGlobal("Block");
    staticRoots[staticRootTop++] = &BlockClass;

    ContextClass = lookupGlobal("Context");
    staticRoots[staticRootTop++] = &ContextClass;

    /* clean up after ourselves. */
    memset((void *) indirArray,(int)0,(size_t)((size_t)spaceSize * sizeof(struct object)));

    fprintf(stderr, "Read in %d objects.\n", indirtop);

    return indirtop;
}


/* FIXME global to fake the define below */

struct object *imageBase;
struct object *imagePointer;
struct object *imageTop;

//#define FIX_OFFSET(ptr, amt) ((struct object *)(((char *)(ptr)) + ((amt) * BytesPerWord)))

#define PTR_BETWEEN(p, low, high) (((intptr_t)p >= (intptr_t)low) && ((intptr_t)p < (intptr_t)high))

static struct object *FIX_OFFSET(struct object *old, int64_t offset)
{
    struct object *tmp;

    /* sanity checking, is the old pointer in the old image range? */
    if(!PTR_BETWEEN(old, imagePointer, imageTop)) {
        fprintf(stderr, "!!! old pointer=%p, low bound=%p, high bound=%p\n", (void *)old, (void *)imagePointer, (void *)imageTop);
        sysErrorInt("pointer from image is not within image address range! oop=", (intptr_t)old);
    }

    tmp = (struct object *)((intptr_t)old + (offset * (intptr_t)BytesPerWord));

    /* sanity checking, is the new pointer in the new memory range? */
    if(!PTR_BETWEEN(tmp, memoryPointer, memoryTop)) {
        fprintf(stderr, "!!! old pointer=%p, offset=%ld, low bound=%p, high bound=%p, new pointer=%p\n", (void *)old, offset, (void *)memoryPointer, (void *)memoryTop, (void *)tmp);
        sysErrorInt("!!! swizzled pointer from image does not point into new address range! oop=", (intptr_t)tmp);
    }

    return tmp;
}



static struct object *object_fix_up(struct object *obj, int64_t offset)
{
    int i;
    int size;

    /* check for illegal object */
    if (obj == NULL) {
        sysErrorInt("Fixing up a null object! obj=", (intptr_t)obj);
    }

    /* get the size, we'll use it regardless of the object type. */
    size = SIZE(obj);

    /* byte objects, just fix up the class. */
    if (IS_BINOBJ(obj)) {
//        fprintf(stderr, "Object %p is a binary object of %d bytes.\n", (void *)obj, size);
        struct byteObject *bobj = (struct byteObject *) obj;

        /* fix up class offset */
        bobj->class = FIX_OFFSET(bobj->class, offset);
//        fprintf(stderr, "  class is object %p.\n", (void *)(bobj->class));

        /* fix up size, size of binary objects is in bytes! */
        size = (int)((size + BytesPerWord - 1)/BytesPerWord);
    } else {
        /* ordinary objects */
//        fprintf(stderr, "Object %p is an ordinary object with %d fields.\n", (void *)obj, size);

        /* fix the class first */
//        fprintf(stderr, "  class is object %p.\n", (void *)(obj->class));

        obj->class = FIX_OFFSET(obj->class, offset);

        /* write the instance variables of the object */
        for (i = 0; i < size; i++) {
            /* we only need to stitch this up if it is not a SmallInt. */
            if(obj->data[i] != NULL) {
                if(!IS_SMALLINT(obj->data[i])) {
                    obj->data[i] = FIX_OFFSET(obj->data[i], offset);
//                    fprintf(stderr, "  field %d is object %p.\n", i, (void *)(obj->data[i]));
                } else {
//                    fprintf(stderr, "  field %d is SmallInt %d.\n", i, integerValue(obj->data[i]));
                }
            } else {
//                fprintf(stderr, "  field %d is NULL, fixing up to nil object.\n", i);
                obj->data[i] = nilObject;
            }
        }
    }

    /* size is number of fields plus header plus class */
    return WORDSUP(obj, size + 2);
}









int fileIn_version_2(FILE *fp)
{
    int i;
    struct object *fixer;
    int64_t newOffset;
    int64_t totalCells = 0;

    /* read in the bottom, pointer and top of the image */
    fread(&imageBase, sizeof imageBase, 1, fp);
    fprintf(stderr, "Read in image imageBase=%p\n", (void *)imageBase);

    fread(&imagePointer, sizeof imagePointer, 1, fp);
    fprintf(stderr, "Read in image imagePointer=%p\n", (void *)imagePointer);

    fread(&imageTop, sizeof imageTop, 1, fp);
    fprintf(stderr, "Read in image imageTop=%p\n", (void *)imageTop);

    /* how many cells? */
    totalCells = ((int)(((intptr_t)imageTop - (intptr_t)imagePointer)))/(int)BytesPerWord;
    fprintf(stderr, "Image has %ld cells.\n", totalCells);

    fprintf(stderr, "memoryBase is %p\n", (void *)memoryBase);
    fprintf(stderr, "memoryPointer is %p\n", (void *)memoryPointer);
    fprintf(stderr, "memoryTop is %p\n", (void *)memoryTop);

    /* what is the offset between the saved pointer and our current pointer? In cells! */
    newOffset = (int64_t)((intptr_t)memoryBase - (intptr_t)imageBase);
    fprintf(stderr, "Address offset between image and current memory pointer is %ld bytes.\n", newOffset);

    newOffset = (int64_t)newOffset/(int64_t)BytesPerWord;
    fprintf(stderr, "Address offset between image and current memory pointer is %ld cells.\n", newOffset);

    /* set up lower pointer */
    memoryPointer = WORDSDOWN(memoryTop, totalCells);


    /* read in core object pointers. */

    /* everything starts with globals */
    fread(&globalsObject, sizeof globalsObject, 1, fp);
    globalsObject = FIX_OFFSET(globalsObject, newOffset);
    staticRoots[staticRootTop++] = &globalsObject;
    fprintf(stderr, "Read in globals object=%p\n", (void *)globalsObject);

    fread(&initialMethod, sizeof initialMethod, 1, fp);
    initialMethod = FIX_OFFSET(initialMethod, newOffset);
    staticRoots[staticRootTop++] = &initialMethod;
    fprintf(stderr, "Read in initial method=%p\n", (void *)initialMethod);

    fprintf(stderr, "Reading binary message objects.\n");
    for (i = 0; i < 3; i++) {
        fread(&(binaryMessages[i]), sizeof binaryMessages[i], 1, fp);
        binaryMessages[i] = FIX_OFFSET(binaryMessages[i], newOffset);
        staticRoots[staticRootTop++] = &(binaryMessages[i]);
        fprintf(stderr, "  Read in binary message %d=%p\n", i, (void *)binaryMessages[i]);
    }

    fread(&badMethodSym, sizeof badMethodSym, 1, fp);
    badMethodSym = FIX_OFFSET(badMethodSym, newOffset);
    staticRoots[staticRootTop++] = &badMethodSym;
    fprintf(stderr, "Read in doesNotUnderstand: symbol=%p\n", (void *)badMethodSym);

    /* read in the raw image data. */
    fread(memoryPointer, BytesPerWord, (size_t)totalCells, fp);

    /* fix up the rest of the objects. */

    int64_t start = time_usec();
    fixer = memoryPointer;
    while(fixer < memoryTop) {
        //fprintf(stderr, "Fixing up object %d: ", obj_count++);
        fixer = object_fix_up(fixer, newOffset);
    }
    int64_t end = time_usec();

    /* fix up everything from globals. */
    nilObject = lookupGlobal("nil");
    staticRoots[staticRootTop++] = &nilObject;

    trueObject = lookupGlobal("true");
    staticRoots[staticRootTop++] = &trueObject;

    falseObject = lookupGlobal("false");
    staticRoots[staticRootTop++] = &falseObject;

    SmallIntClass = lookupGlobal("SmallInt");
    staticRoots[staticRootTop++] = &SmallIntClass;

    IntegerClass = lookupGlobal("Integer");
    staticRoots[staticRootTop++] = &IntegerClass;

    ArrayClass = lookupGlobal("Array");
    staticRoots[staticRootTop++] = &ArrayClass;

    BlockClass = lookupGlobal("Block");
    staticRoots[staticRootTop++] = &BlockClass;

    ContextClass = lookupGlobal("Context");
    staticRoots[staticRootTop++] = &ContextClass;


    /* useful classes */
    StringClass = lookupGlobal("String");
    staticRoots[staticRootTop++] = &StringClass;

    ByteArrayClass = lookupGlobal("ByteArray");
    staticRoots[staticRootTop++] = &ByteArrayClass;

    fprintf(stderr, "Object fix up took %d usec.\n", (int)(end - start));

    fprintf(stderr, "Read in %ld cells.\n", totalCells);

    return (int)totalCells;
}



int fileIn(FILE *fp)
{
    uint32_t version = get_image_version(fp);

    switch(version) {
    case IMAGE_VERSION_0:
        fprintf(stderr, "Reading in version 0 image.\n");
        return fileIn_version_0(fp);
        break;

    case IMAGE_VERSION_1:
        fprintf(stderr, "Reading in version 1 image.\n");
        return fileIn_version_1(fp);
        break;

    case IMAGE_VERSION_2:
        fprintf(stderr, "Reading in version 2 image.\n");
        return fileIn_version_2(fp);
        break;

    default:
        sysErrorInt("Unsupported image file version: ", version);
        break;
    }

    return 0;
}





int fileOut(FILE *fp)
{
    int i;
    struct image_header header;
    int64_t totalCells = 0;

    printf("starting to file out\n");

    memset(&header, 0, sizeof(header));

    /* force a GC to clean up the image */
    do_gc();

    /* output header. */
    header.magic[0] = 'l';
    header.magic[1] = 's';
    header.magic[2] = 't';
    header.magic[3] = '!';
    header.version = IMAGE_VERSION_2;

    fwrite(&header, sizeof header, 1, fp);

    /* how much to write? */
    totalCells = ((intptr_t)memoryTop - (intptr_t)memoryPointer)/(int)BytesPerWord;

    /* write out image bounds */
    fprintf(stderr, "writing out memoryBase=%p\n", (void *)memoryBase);
    fwrite(&memoryBase, sizeof memoryBase, 1, fp);

    fprintf(stderr, "writing out memoryPointer=%p\n", (void *)memoryPointer);
    fwrite(&memoryPointer, sizeof memoryPointer, 1, fp);

    fprintf(stderr, "writing out memoryTop=%p\n", (void *)memoryTop);
    fwrite(&memoryTop, sizeof memoryTop, 1, fp);


    /* write out core objects. */
    fprintf(stderr, "writing out globals object=%p\n", (void *)globalsObject);
    fwrite(&globalsObject, sizeof globalsObject, 1, fp);

    fprintf(stderr, "writing out initial method=%p\n", (void *)initialMethod);
    fwrite(&initialMethod, sizeof initialMethod, 1, fp);

    fprintf(stderr, "writing binary message objects.\n");
    for (i = 0; i < 3; i++) {
        fprintf(stderr, "    writing out binary object[%d]=%p\n", i, (void *)binaryMessages[i]);
        fwrite(&(binaryMessages[i]), sizeof (binaryMessages[i]), 1, fp);
    }

    fprintf(stderr, "writing out doesNotUnderstand: symbol=%p\n", (void *)badMethodSym);
    fwrite(&badMethodSym, sizeof badMethodSym, 1, fp);


    /* write out raw image data. */
    fprintf(stderr, "writing out %ld cells of image data.\n", totalCells);
    fwrite(memoryPointer, BytesPerWord, (size_t)totalCells, fp);

    return (int)totalCells;
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
        sysErrorInt("addStaticRoot: too many static references",
                    (intptr_t)objp);
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



