/*
 * Functions for image handling.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "err.h"
#include "globals.h"
#include "image.h"
#include "memory.h"



static int fileIn_version_2(FILE *img);
static int fileIn_version_1(FILE *img);
static int fileIn_version_0(FILE *img);
static void readTag(FILE *fp, int *type, int *val);
static struct object *objectRead(FILE *fp);

static struct object *FIX_OFFSET(struct object *old, int64_t offset);


static uint32_t le2host32(uint32_t le_int);
static uint32_t host2le32(uint32_t host_int);
static struct image_header get_image_header(FILE *img);
static int put_image_header(FILE *img, uint32_t version);




static int indirtop = 0;
static struct object **indirArray;

/*
 * Image handling entry points.  These will dispatch on the actual
 * version of the image (at least fileIn()).
 */


int fileIn(FILE *fp)
{
    struct image_header header = get_image_header(fp);

    switch(header.version) {
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
        sysErrorInt("Unsupported image file version: ", header.version);
        break;
    }

    return 0;
}


int write_uint32(FILE *img, uint32_t val)
{
    uint8_t buf[4];

    /* everything is little endian */
    buf[0] = val & 0xFF;
    buf[1] = (val >> 8) & 0xFF;
    buf[2] = (val >> 16) & 0xFF;
    buf[3] = (val >> 24) & 0xFF;

    return (int)fwrite(buf, sizeof buf, 1, img);
}



int write_uint8(FILE *img, uint8_t val)
{
    return (int)fwrite(&val, 1, 1, img);
}



int read_uint32(FILE *img, uint32_t *val)
{
    uint8_t buf[4];

    /* everything is little endian */

    if(fread(buf, sizeof buf, 1, img) == 1) {
        *val = 0;
        *val = (uint32_t)(buf[0])
             + (uint32_t)(buf[1] << 8)
             + (uint32_t)(buf[2] << 16)
             + (uint32_t)(buf[3] << 24);
    } else {
        return 0;
    }

    return 1;
}


int read_uint8(FILE *img, uint8_t *val)
{
    return (int)fread(val, 1, 1, img);
}


/* offset in BytesPerWord units! */
//#define WORDSUP(ptr, amt) ((struct object *)(((char *)(ptr)) + ((amt) * BytesPerWord)))
//#define WORDSDOWN(ptr, amt) WORDSUP(ptr, 0 - (amt))

uint32_t addr_to_offset(struct object *oop)
{
    intptr_t oop_int = (intptr_t)oop;
    intptr_t top_int = (intptr_t)memoryTop;
    ptrdiff_t offset = (top_int - oop_int)/BytesPerWord;

    info("oop=%p, top=%p", oop, memoryTop);

    if(offset > UINT32_MAX) {
        error("Object pointer offset is greater than UINT32_MAX!");
    }

    if(offset < 0) {
        error("Object pointer offset is less than zero!");
    }

    return (uint32_t)offset;
}


struct object *offset_to_addr(uint32_t raw_offset)
{
    ptrdiff_t offset = (ptrdiff_t)raw_offset;
    intptr_t top_int = (intptr_t)memoryTop;
    intptr_t oop_int = top_int - (offset * BytesPerWord);

    return (struct object *)oop_int;
}



struct object *write_object(FILE *img, struct object *obj)
{
    int size;

    /* check for illegal object */
    if (obj == NULL) {
        error("Fixing up a null object!");
    }

    /*
     * all objects have the same header:
     *    size + flags (32-bits)
     *    class OOP
     */

    /* get the size, we'll use it regardless of the object type. */
    size = SIZE(obj);
    write_uint32(img, (uint32_t)obj->size); /* we write the _unconverted size with the flags! */

    /* now we write out the class. */
    write_uint32(img, addr_to_offset(obj->class));

    /* byte objects, write out the data. */
    if (IS_BINOBJ(obj)) {
        struct byteObject *bobj = (struct byteObject *) obj;
        info("Object %p is a binary object of %d bytes.", (void *)obj, size);

        for(int i=0; i < size; i++) {
            write_uint8(img, bobj->bytes[i]);
        }

        /* convert size into BytesPerWord units. */
        size = (size + (BytesPerWord -1))/BytesPerWord;
    } else {
        /* ordinary objects */
        info("Object %p is an ordinary object with %d fields.", (void *)obj, size);

        /* write the instance variables of the object */
        for (int i = 0; i < size; i++) {
            /* we only need to stitch this up if it is not a SmallInt. */
            if(obj->data[i] != NULL) {
                if(IS_SMALLINT(obj->data[i])) {
                    write_uint32(img, (uint32_t)(intptr_t)obj->data[i]);
                } else {
                    write_uint32(img, addr_to_offset(obj->data[i]));
                }
            } else {
                /* fix up the value to the nil object. */
                write_uint32(img, addr_to_offset(nilObject));
            }
        }
    }

    /* size is number of fields plus header plus class */
    return WORDSUP(obj, size + 2);
}




#define WRITE_OOP(o) \
    info("object at " #o "=%p (%u)", o, addr_to_offset(o)); \
    write_uint32(img, addr_to_offset(o));



int fileOut(FILE *img)
{
    struct image_header header;
    uint32_t totalCells = 0;
    struct object *current_obj = NULL;
    int object_count = 0;

    info("starting to file out image version %d.", IMAGE_VERSION_3);

    memset(&header, 0, sizeof(header));

    /* force a GC to clean up the image */
    do_gc();

    /* FIXME - catch return value! */
    put_image_header(img, IMAGE_VERSION_3);

    /* how much to write?   FIXME - check for overflow! */
    totalCells = (uint32_t)(((intptr_t)memoryTop - (intptr_t)memoryPointer)/(intptr_t)BytesPerWord);

    info("image contains %u total cells.", totalCells);

    /* save the amount out to the image. */
    write_uint32(img, totalCells);

    /* write out core objects. */

    WRITE_OOP(globalsObject);
    WRITE_OOP(initialMethod);

    for(int i=0; i < 3; i++) {
        WRITE_OOP(binaryMessages[i]);
    }

    WRITE_OOP(badMethodSym);

    /* write out object image. */
    current_obj = memoryPointer;
    while((intptr_t)current_obj < (intptr_t)memoryTop) {
        //info("Writing out object %p", current_obj);
        current_obj = write_object(img, current_obj);
        object_count++;
    }

    info("image contains %d objects.", object_count);

    return (int)totalCells;
}






struct object *read_object(FILE *img, struct object *obj)
{
    int size;
    uint32_t offset;

    /* check for illegal object */
    if (obj == NULL) {
        error("Cannot read to a null pointer!");
    }

    /*
     * all objects have the same header:
     *    size + flags (32-bits)
     *    class OOP offset.
     */

    /* get the size. Note that we need to get the raw value then the fixed up value. */
    read_uint32(img, (uint32_t*)&size);
    obj->size = (int)size;
    size = SIZE(obj);

    /* get the class. */
    read_uint32(img, &offset);
    obj->class = offset_to_addr(offset);

    /* byte objects are read in differently */
    if (IS_BINOBJ(obj)) {
        struct byteObject *bobj = (struct byteObject *) obj;

        info("Object %p is a binary object of %d bytes.", (void *)obj, size);

        for(int i=0; i < size; i++) {
            read_uint8(img, &(bobj->bytes[i]));
        }
    } else {
        /* ordinary objects */
        info("Object %p is an ordinary object with %d fields.", (void *)obj, size);

        /* read the instance variables of the object */
        for (int i = 0; i < size; i++) {
            read_uint32(img, &offset);

            /* handle SmallInt */
            if(offset & 0x01) {
                obj->data[i] = newInteger((int32_t)(offset >> 1));
            } else {
                /* handle normal object. */
                obj->data[i] = offset_to_addr(offset);
            }
        }
    }

    /* size is number of fields plus header plus class */
    return WORDSUP(obj, size + 2);
}




#define READ_OOP(o) \
    read_uint32(img, &obj_offset); \
    info("object at " #o "=%p (%u)", offset_to_addr(obj_offset), obj_offset); \
    o = offset_to_addr(obj_offset); \
    staticRoots[staticRootTop++] = &(o);



int fileIn_version_3(FILE *img)
{
    uint32_t totalCells = 0;
    uint32_t obj_offset = 0;
    struct object *current_obj = NULL;
    int object_count = 0;

    info("Starting to file in image version %d.", IMAGE_VERSION_3);

    /* How many cells to read? */
    read_uint32(img, &totalCells);

    info("image contains %u total cells.", totalCells);

    /* calculate the pointers. */
    memoryPointer = WORDSDOWN(memoryTop, totalCells);

    /* read in core objects. */
    READ_OOP(globalsObject);
    READ_OOP(initialMethod);

    for(int i=0; i < 3; i++) {
        READ_OOP(binaryMessages[i]);
    }

    READ_OOP(badMethodSym);

    /* read in object image. */
    int64_t start = time_usec();
    current_obj = memoryPointer;
    while((intptr_t)current_obj < (intptr_t)memoryTop) {
        //info("Reading in object at %p", current_obj);
        current_obj = read_object(img, current_obj);
        object_count++;
    }
    int64_t end = time_usec();

    info("image contains %d objects.", object_count);

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

    printf("Image read and fixup took %d usec.\n", (int)(end - start));

    printf("Read in %d cells.\n", (int)totalCells);

    return (int)totalCells;
}







/*
 * Below are the actual implementations of fileIn()
 */


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





struct object *imageBase;
struct object *imagePointer;
struct object *imageTop;

static struct object *object_fix_up(struct object *obj, int64_t offset);


int fileIn_version_2(FILE *fp)
{
    int i;
    struct object *fixer;
    int64_t newOffset;
    int64_t totalCells = 0;
    int obj_count = 0;

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





static int get_byte(FILE *fp);
static void readTag(FILE *fp, int *type, int *val);
static struct object *objectRead(FILE *fp);


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


int get_byte(FILE *fp)
{
    static int offset = 0;
    int inByte;

    inByte = fgetc(fp);
    offset++;

    //fprintf(stderr, "%06d: %u (%x)\n", offset, inByte, inByte);

    return inByte;
}



void readTag(FILE *fp, int *type, int *val)
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



/* FIXME global to fake the define below */


//#define FIX_OFFSET(ptr, amt) ((struct object *)(((char *)(ptr)) + ((amt) * BytesPerWord)))

#define PTR_BETWEEN(p, low, high) (((intptr_t)p >= (intptr_t)low) && ((intptr_t)p < (intptr_t)high))

struct object *FIX_OFFSET(struct object *old, int64_t offset)
{
    struct object *tmp;

    /* sanity checking, is the old pointer in the old image range? */
    if(!PTR_BETWEEN(old, imagePointer, imageTop)) {
        info("!!! old pointer=%p, low bound=%p, high bound=%p\n", (void *)old, (void *)imagePointer, (void *)imageTop);
        error("pointer from image is not within image address range! oop=%p", (intptr_t)old);
    }

    tmp = (struct object *)((intptr_t)old + (offset * (intptr_t)BytesPerWord));

    /* sanity checking, is the new pointer in the new memory range? */
    if(!PTR_BETWEEN(tmp, memoryPointer, memoryTop)) {
        info("!!! old pointer=%p, offset=%ld, low bound=%p, high bound=%p, new pointer=%p\n", (void *)old, offset, (void *)memoryPointer, (void *)memoryTop, (void *)tmp);
        error("swizzled pointer from image does not point into new address range! oop=%p", (intptr_t)tmp);
    }

    return tmp;
}



struct object *object_fix_up(struct object *obj, int64_t offset)
{
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
        for (int i = 0; i < size; i++) {
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





uint32_t le2host32(uint32_t le_int)
{
    /* use type punning. */
    union {
        uint32_t u_int;
        uint8_t u_bytes[4];
    } punner;
    uint32_t result = 0;

    punner.u_int = le_int;

    result =  (uint32_t)(punner.u_bytes[0])
           + ((uint32_t)(punner.u_bytes[1]) << 8)
           + ((uint32_t)(punner.u_bytes[2]) << 16)
           + ((uint32_t)(punner.u_bytes[3]) << 24);

    return result;
}


uint32_t host2le32(uint32_t host_int)
{
    /* use type punning. */
    union {
        uint32_t u_int;
        uint8_t u_bytes[4];
    } punner;

    punner.u_bytes[0] = (uint8_t)(host_int & 0xFF);
    punner.u_bytes[1] = (uint8_t)((host_int >> 8) & 0xFF);
    punner.u_bytes[2] = (uint8_t)((host_int >> 16) & 0xFF);
    punner.u_bytes[3] = (uint8_t)((host_int >> 24) & 0xFF);

    return punner.u_int;
}




struct image_header get_image_header(FILE *img)
{
    struct image_header header;
    int rc;

    memset(&header, 0, sizeof(header));

    rc = (int)fread(&header, sizeof(header), 1, img);
    if(rc == 1) {
        if(header.magic[0] == 'l' && header.magic[1] == 's' && header.magic[2] == 't' && header.magic[3] == '!') {
            header.version = le2host32(header.version);
        } else {
            /* not a newer version, seek back to the start. */
            fseek(img, 0, SEEK_SET);
            header.version = 0;
        }

    } else {
        /* error reading the file! This is fatal. */
        error("Error reading the image file!");
    }

    return header;
}



int put_image_header(FILE *img, uint32_t version)
{
    struct image_header header;

    memset(&header, 0, sizeof(header));

    /* output header. */
    header.magic[0] = 'l';
    header.magic[1] = 's';
    header.magic[2] = 't';
    header.magic[3] = '!';
    header.version = host2le32(version);

    return (int)fwrite(&header, sizeof header, 1, img);
}

