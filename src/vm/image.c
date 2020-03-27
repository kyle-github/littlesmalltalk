

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "err.h"
#include "globals.h"
#include "image.h"
#include "memory.h"



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

# define LST_SMALL_TAG_LIMIT    0x0F
# define LST_LARGE_TAG_FLAG 0x10
# define LST_TAG_SIZE_MASK  0x1F
# define LST_TAG_VALUE_MASK     LST_SMALL_TAG_LIMIT
# define LST_TAG_TYPE_MASK  0xE0




#define PTR_BETWEEN(p, low, high) (((intptr_t)p >= (intptr_t)low) && ((intptr_t)p < (intptr_t)high))


//static uint8_t get_image_version(FILE *fp);
//static void put_image_version(FILE *fp, uint8_t version);

static int fileIn_version_0(FILE *fp);
static int fileIn_version_1(FILE *fp);
//static int fileOut_version_1(FILE *fp);
static int getIntSize(int val);
static void objectWrite(FILE * fp, struct object *obj);
static void writeTag(FILE * fp, int type, int val);

static struct object *objectRead(FILE *fp);
static void readTag(FILE *fp, int *type, int *val);
static int get_byte(FILE *fp);

//static int fileOut_version_2(FILE *fp);
static int fileIn_version_2(FILE *fp);
static struct object *fix_offset(struct object *old, int64_t offset);
static struct object *object_fix_up(struct object *obj, int64_t offset);

static int fileIn_version_3(FILE *fp);
static int fileOut_object_version_3(FILE *img, struct object *globs);



/* used for image pointer remapping */
static int indirtop = 0;
static struct object **indirArray;


/* used for bootstrap version 1 image */
#define imageMaxNumberOfObjects 10000
//static struct object **writtenObjects = NULL;
//static int imageTop = 0;




/* used for version 2 images */
//struct object *imageBase;
//struct object *imagePointer;
//struct object *imageTop;



int fileIn(FILE *fp)
{
    uint8_t version = get_image_version(fp);

    switch(version) {
    case IMAGE_VERSION_0:
        info("Reading in version 0 image.");
        return fileIn_version_0(fp);
        break;

    case IMAGE_VERSION_1:
        info("Reading in version 1 image.");
        return fileIn_version_1(fp);
        break;

    case IMAGE_VERSION_2:
        info("Reading in version 2 image.");
        return fileIn_version_2(fp);
        break;

    case IMAGE_VERSION_3:
        info("Reading in version 3 image.");
        return fileIn_version_3(fp);
        break;

    default:
        error("Unsupported image file version: %u.", version);
        break;
    }

    return 0;
}




int fileOut(FILE *fp)
{
    return fileOut_object(fp, globalsObject);
}


int fileOut_object(FILE *fp, struct object *obj)
{
    return fileOut_object_version_3(fp, obj);
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
    addStaticRoot(&nilObject);

    fprintf(stderr, "reading true object.\n");
    trueObject = objectRead(fp);
    addStaticRoot(&trueObject);

    fprintf(stderr, "reading false object.\n");
    falseObject = objectRead(fp);
    addStaticRoot(&falseObject);

    fprintf(stderr, "reading globals object.\n");
    globalsObject = objectRead(fp);
    addStaticRoot(&globalsObject);

    fprintf(stderr, "reading SmallInt class.\n");
    SmallIntClass = objectRead(fp);
    addStaticRoot(&SmallIntClass);

    fprintf(stderr, "reading Integer class.\n");
    IntegerClass = objectRead(fp);
    addStaticRoot(&IntegerClass);

    fprintf(stderr, "reading Array class.\n");
    ArrayClass = objectRead(fp);
    addStaticRoot(&ArrayClass);

    fprintf(stderr, "reading Block class.\n");
    BlockClass = objectRead(fp);
    addStaticRoot(&BlockClass);

    fprintf(stderr, "reading Context class.\n");
    ContextClass = objectRead(fp);
    addStaticRoot(&ContextClass);

    fprintf(stderr, "reading initial method.\n");
    initialMethod = objectRead(fp);
    addStaticRoot(&initialMethod);

    fprintf(stderr, "reading binary message objects.\n");
    for (i = 0; i < 3; i++) {
        binaryMessages[i] = objectRead(fp);
        addStaticRoot(&binaryMessages[i]);
    }

    fprintf(stderr, "reading bad method symbol.\n");
    badMethodSym = objectRead(fp);
    addStaticRoot(&badMethodSym);

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
    addStaticRoot(&globalsObject);

    fprintf(stderr, "reading initial method.\n");
    initialMethod = objectRead(fp);
    addStaticRoot(&initialMethod);

    fprintf(stderr, "reading binary message objects.\n");
    for (i = 0; i < 3; i++) {
        binaryMessages[i] = objectRead(fp);
        addStaticRoot(&binaryMessages[i]);
    }

    fprintf(stderr, "reading bad method symbol.\n");
    badMethodSym = objectRead(fp);
    addStaticRoot(&badMethodSym);

    /* fix up everything from globals. */
    nilObject = lookupGlobal("nil");
    addStaticRoot(&nilObject);

    trueObject = lookupGlobal("true");
    addStaticRoot(&trueObject);

    falseObject = lookupGlobal("false");
    addStaticRoot(&falseObject);

    ArrayClass = lookupGlobal("Array");
    addStaticRoot(&ArrayClass);

    BlockClass = lookupGlobal("Block");
    addStaticRoot(&BlockClass);

    ByteArrayClass = lookupGlobal("ByteArray");
    addStaticRoot(&ByteArrayClass);

    ContextClass = lookupGlobal("Context");
    addStaticRoot(&ContextClass);

    DictionaryClass = lookupGlobal("Dictionary");
    addStaticRoot(&DictionaryClass);

    IntegerClass = lookupGlobal("Integer");
    addStaticRoot(&IntegerClass);

    SmallIntClass = lookupGlobal("SmallInt");
    addStaticRoot(&SmallIntClass);

    StringClass = lookupGlobal("String");
    addStaticRoot(&StringClass);

    SymbolClass = lookupGlobal("Symbol");
    addStaticRoot(&SymbolClass);

    UndefinedClass = lookupGlobal("Undefined");
    addStaticRoot(&UndefinedClass);

    /* clean up after ourselves. */
    memset((void *) indirArray,(int)0,(size_t)((size_t)spaceSize * sizeof(struct object)));

    fprintf(stderr, "Read in %d objects.\n", indirtop);

    return indirtop;
}




//int fileOut_version_1(FILE *img)
//{
//    /* use the currently unused space for the indir pointers */
//    if (inSpaceOne) {
//        indirArray = (struct object * *) spaceTwo;
//    } else {
//        indirArray = (struct object * *) spaceOne;
//    }
//    indirtop = 0;
//
//    info("Writing out image version 1.");
//
//    /* write the header. */
//    put_image_version(img, IMAGE_VERSION_1);
//
//    /* write the main objects. */
//    objectWrite(img, globalsObject);
//    objectWrite(img, initialMethod);
//    objectWrite(img, binaryMessages[0]);
//    objectWrite(img, binaryMessages[1]);
//    objectWrite(img, binaryMessages[2]);
//    objectWrite(img, badMethodSym);
//
//    return indirtop;
//}





int fileIn_version_3(FILE *fp)
{
    struct object *specialSymbols = NULL;

    /* use the currently unused space for the indir pointers */
    if (inSpaceOne) {
        indirArray = (struct object * *) spaceTwo;
    } else {
        indirArray = (struct object * *) spaceOne;
    }
    indirtop = 0;

    /* read the base objects from the image file. */

    info("reading globals object.");
    globalsObject = objectRead(fp);
    addStaticRoot(&globalsObject);

    /* fix up everything from globals. */
    info("Finding special symbols.");
    specialSymbols = lookupGlobal("specialSymbols");

    binaryMessages[0] = specialSymbols->data[0];
    addStaticRoot(&binaryMessages[0]);

    binaryMessages[1] = specialSymbols->data[1];
    addStaticRoot(&binaryMessages[1]);

    binaryMessages[2] = specialSymbols->data[2];
    addStaticRoot(&binaryMessages[2]);

    badMethodSym = specialSymbols->data[3];
    addStaticRoot(&badMethodSym);

    /* look up the rest in the globals object. */
    nilObject = lookupGlobal("nil");
    addStaticRoot(&nilObject);

    trueObject = lookupGlobal("true");
    addStaticRoot(&trueObject);

    falseObject = lookupGlobal("false");
    addStaticRoot(&falseObject);

    ArrayClass = lookupGlobal("Array");
    addStaticRoot(&ArrayClass);

    BlockClass = lookupGlobal("Block");
    addStaticRoot(&BlockClass);

    ByteArrayClass = lookupGlobal("ByteArray");
    addStaticRoot(&ByteArrayClass);

    ContextClass = lookupGlobal("Context");
    addStaticRoot(&ContextClass);

    DictionaryClass = lookupGlobal("Dictionary");
    addStaticRoot(&DictionaryClass);

    IntegerClass = lookupGlobal("Integer");
    addStaticRoot(&IntegerClass);

    SmallIntClass = lookupGlobal("SmallInt");
    addStaticRoot(&SmallIntClass);

    StringClass = lookupGlobal("String");
    addStaticRoot(&StringClass);

    SymbolClass = lookupGlobal("Symbol");
    addStaticRoot(&SymbolClass);

    UndefinedClass = lookupGlobal("Undefined");
    addStaticRoot(&UndefinedClass);

    info("Finding initial method.");
    if(!(initialMethod = lookupGlobal("start"))) {
        info("Could not find #start method in globals, looking for #main in Undefined.");
        initialMethod = dictLookup(UndefinedClass->data[methodsInClass], "main");
    }
    addStaticRoot(&initialMethod);

    info("Memory top %p", memoryTop);
    info("Memory pointer %p", memoryPointer);

    info("Read in %d objects.", indirtop);

    return indirtop;
}




int fileOut_object_version_3(FILE *img, struct object *globs)
{
    /* use the currently unused space for the indir pointers */
    if (inSpaceOne) {
        indirArray = (struct object * *) spaceTwo;
    } else {
        indirArray = (struct object * *) spaceOne;
    }
    indirtop = 0;

    info("Writing out image version 3.");

    /* write the header. */
    put_image_version(img, IMAGE_VERSION_3);

    /* write the main objects. */
    objectWrite(img, globs);

    return indirtop;
}


/* return the size in bytes necessary to accurately handle the integer
value passed.  Note that negatives will always get BytesPerWord size.
This will return zero if the passed value is less than LST_SMALL_TAG_LIMIT.
In this case, the value can be packed into the tag it self when read or
written. */

int getIntSize(int val)
{
    int i;

    /* negatives need sign extension.  this is a to do. */
    if (val < 0) {
        info("Writing negative value to image without using negative int tag type!");
        return BytesPerWord;
    }

    if (val < LST_SMALL_TAG_LIMIT) {
        return 0;
    }

    /* how many bytes? */

    for (i = 1; i < BytesPerWord; i++) {
        if (val < (1 << (8 * i))) {
            return i;
        }
    }

    return BytesPerWord;
}




/**
* writeTag
*
* This write a special tag to the output file.  This tag has three bits
* for a type field and five bits for either a value or a size.
*/

void writeTag(FILE * fp, int type, int val)
{
    int tempSize;
    int i;

    /* get the number of bytes required to store the value */
    tempSize = getIntSize(val);

    if (tempSize) {
        /*write the tag byte */
        fputc((type | tempSize | LST_LARGE_TAG_FLAG), fp);

        for (i = 0; i < tempSize; i++)
            fputc((val >> (8 * i)), fp);
    } else {
        fputc((type | val), fp);
    }
}



/**
* objectWrite
*
* This routine writes an object to the output image file.  This is for a
* version 1 image file.
*/

void objectWrite(FILE * fp, struct object *obj)
{
    int i;
    int size;
    int intVal;

    if(!indirArray) {
        indirArray = calloc(sizeof(struct object *), imageMaxNumberOfObjects);

        if(!indirArray) {
            error("Unable to allocate object mapping array!");
        }
    }

    if (indirtop > imageMaxNumberOfObjects) {
        error("objectWrite(): too many indirect objects.");
    }

    /* check for illegal object */
    if (obj == NULL) {
        error("objectWrite(): writing out a NULL object!");
    }

    /* small integer?, if so, treat this specially as this is not a pointer */

    if (IS_SMALLINT(obj)) {	/* SmallInt */
        intVal = integerValue(obj);

        /* if it is negative, we use the positive value and use a special tag. */
        if (intVal < 0)
            writeTag(fp, LST_NINT_TYPE, -intVal);
        else
            writeTag(fp, LST_PINT_TYPE, intVal);
        return;
    }

    /* see if already written */
    for (i = 0; i < indirtop; i++)
        if (obj == indirArray[i]) {
            if (i == 0)
                writeTag(fp, LST_NIL_TYPE, 0);
            else {
                writeTag(fp, LST_POBJ_TYPE, i);
            }
            return;
        }

    /* not written, do it now */
    indirArray[indirtop++] = obj;

    /* byte objects */
    if (IS_BINOBJ(obj)) {
        struct byteObject *bobj = (struct byteObject *) obj;

        size = (int)SIZE(obj);

        /* write the header tag */
        writeTag(fp, LST_BARRAY_TYPE, size);

        /*write out bytes */
        for (i = 0; i < size; i++)
            fputc(bobj->bytes[i], fp);

        objectWrite(fp, obj->class);

        return;
    }

    /* ordinary objects */
    size = SIZE(obj);

    writeTag(fp, LST_OBJ_TYPE, size);

    /* write the class first */
    objectWrite(fp, obj->class);

    /* write the instance variables of the object */
    for (i = 0; i < size; i++)
        objectWrite(fp, obj->data[i]);
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
        error("objectRead(): Read in a NULL object!");

        break;

    case LST_OBJ_TYPE:  /* ordinary object */
        size = val;
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
            error("Illegal previous object index %d (max %d)!",val, indirtop);
        }

        newObj = indirArray[val];

        break;

    case LST_NIL_TYPE:  /* object 0 (nil object) */
        newObj = indirArray[0];
        break;

    default:
        error("Illegal tag type %d!",type);
        break;
    }

    return newObj;
}


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
        error("Unexpected EOF reading image file: reading tag byte.");
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
            error("readTag(): Error reading image file: tag value field exceeds machine word size.  Image created on another machine? Field size=%d", tempSize);
        }

        for(i=0; i<tempSize; i++) {
            inByte = get_byte(fp);

            if(inByte == EOF) {
                error("readTag(): Unexpected EOF reading image file!");
            }

            tmp = tmp  | (((uint)inByte & (uint)0xFF) << ((uint)8*(uint)i));
        }

        *val = (int)tmp;
    } else {
        *val = tempSize;
    }
}





/*
 * Version 2 Image
 */



//int fileOut_version_2(FILE *fp)
//{
//    int i;
//    struct image_header header;
//    int64_t totalCells = 0;
//
//    printf("starting to file out\n");
//
//    memset(&header, 0, sizeof(header));
//
//    /* force a GC to clean up the image */
//    do_gc();
//
//    /* output header. */
//    header.magic[0] = 'l';
//    header.magic[1] = 's';
//    header.magic[2] = 't';
//    header.magic[3] = '!';
//    header.version = IMAGE_VERSION_2;
//
//    fwrite(&header, sizeof header, 1, fp);
//
//    /* how much to write? */
//    totalCells = ((intptr_t)memoryTop - (intptr_t)memoryPointer)/(int)BytesPerWord;
//
//    /* write out image bounds */
//    fprintf(stderr, "writing out memoryBase=%p\n", (void *)memoryBase);
//    fwrite(&memoryBase, sizeof memoryBase, 1, fp);
//
//    fprintf(stderr, "writing out memoryPointer=%p\n", (void *)memoryPointer);
//    fwrite(&memoryPointer, sizeof memoryPointer, 1, fp);
//
//    fprintf(stderr, "writing out memoryTop=%p\n", (void *)memoryTop);
//    fwrite(&memoryTop, sizeof memoryTop, 1, fp);
//
//
//    /* write out core objects. */
//    fprintf(stderr, "writing out globals object=%p\n", (void *)globalsObject);
//    fwrite(&globalsObject, sizeof globalsObject, 1, fp);
//
//    fprintf(stderr, "writing out initial method=%p\n", (void *)initialMethod);
//    fwrite(&initialMethod, sizeof initialMethod, 1, fp);
//
//    fprintf(stderr, "writing binary message objects.\n");
//    for (i = 0; i < 3; i++) {
//        fprintf(stderr, "    writing out binary object[%d]=%p\n", i, (void *)binaryMessages[i]);
//        fwrite(&(binaryMessages[i]), sizeof (binaryMessages[i]), 1, fp);
//    }
//
//    fprintf(stderr, "writing out doesNotUnderstand: symbol=%p\n", (void *)badMethodSym);
//    fwrite(&badMethodSym, sizeof badMethodSym, 1, fp);
//
//
//    /* write out raw image data. */
//    fprintf(stderr, "writing out %ld cells of image data.\n", totalCells);
//    fwrite(memoryPointer, BytesPerWord, (size_t)totalCells, fp);
//
//    return (int)totalCells;
//}
//







int fileIn_version_2(FILE *fp)
{
    int i;
    struct object *fixer;
    int64_t newOffset;
    int64_t totalCells = 0;
    struct object *imageBase;
    struct object *imagePointer;
    struct object *imageTop;

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
    globalsObject = fix_offset(globalsObject, newOffset);
    addStaticRoot(&globalsObject);
    fprintf(stderr, "Read in globals object=%p\n", (void *)globalsObject);

    fread(&initialMethod, sizeof initialMethod, 1, fp);
    initialMethod = fix_offset(initialMethod, newOffset);
    addStaticRoot(&initialMethod);
    fprintf(stderr, "Read in initial method=%p\n", (void *)initialMethod);

    fprintf(stderr, "Reading binary message objects.\n");
    for (i = 0; i < 3; i++) {
        fread(&(binaryMessages[i]), sizeof binaryMessages[i], 1, fp);
        binaryMessages[i] = fix_offset(binaryMessages[i], newOffset);
        addStaticRoot(&binaryMessages[i]);
        fprintf(stderr, "  Read in binary message %d=%p\n", i, (void *)binaryMessages[i]);
    }

    fread(&badMethodSym, sizeof badMethodSym, 1, fp);
    badMethodSym = fix_offset(badMethodSym, newOffset);
    addStaticRoot(&badMethodSym);
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
    addStaticRoot(&nilObject);

    trueObject = lookupGlobal("true");
    addStaticRoot(&trueObject);

    falseObject = lookupGlobal("false");
    addStaticRoot(&falseObject);

    ArrayClass = lookupGlobal("Array");
    addStaticRoot(&ArrayClass);

    BlockClass = lookupGlobal("Block");
    addStaticRoot(&BlockClass);

    ByteArrayClass = lookupGlobal("ByteArray");
    addStaticRoot(&ByteArrayClass);

    ContextClass = lookupGlobal("Context");
    addStaticRoot(&ContextClass);

    DictionaryClass = lookupGlobal("Dictionary");
    addStaticRoot(&DictionaryClass);

    IntegerClass = lookupGlobal("Integer");
    addStaticRoot(&IntegerClass);

    SmallIntClass = lookupGlobal("SmallInt");
    addStaticRoot(&SmallIntClass);

    StringClass = lookupGlobal("String");
    addStaticRoot(&StringClass);

    SymbolClass = lookupGlobal("Symbol");
    addStaticRoot(&SymbolClass);

    UndefinedClass = lookupGlobal("Undefined");
    addStaticRoot(&UndefinedClass);

    fprintf(stderr, "Object fix up took %d usec.\n", (int)(end - start));

    fprintf(stderr, "Read in %ld cells.\n", totalCells);

    return (int)totalCells;
}





struct object *fix_offset(struct object *old, int64_t offset)
{
    struct object *tmp;

    /* sanity checking, is the old pointer in the old image range? */
//    if(!PTR_BETWEEN(old, imagePointer, imageTop)) {
//        fprintf(stderr, "!!! old pointer=%p, low bound=%p, high bound=%p\n", (void *)old, (void *)imagePointer, (void *)imageTop);
//        sysErrorInt("pointer from image is not within image address range! oop=", (intptr_t)old);
//    }

    tmp = (struct object *)((intptr_t)old + (offset * (intptr_t)BytesPerWord));

    /* sanity checking, is the new pointer in the new memory range? */
    if(!PTR_BETWEEN(tmp, memoryPointer, memoryTop)) {
        info("!!! old pointer=%p, offset=%ld, low bound=%p, high bound=%p, new pointer=%p", (void *)old, offset, (void *)memoryPointer, (void *)memoryTop, (void *)tmp);
        error("fix_offset(): swizzled pointer from image does not point into new address range! oop=%p", (intptr_t)tmp);
    }

    return tmp;
}



struct object *object_fix_up(struct object *obj, int64_t offset)
{
    int i;
    int size;

    /* check for illegal object */
    if (obj == NULL) {
        error("Fixing up a null object!");
    }

    /* get the size, we'll use it regardless of the object type. */
    size = SIZE(obj);

    /* byte objects, just fix up the class. */
    if (IS_BINOBJ(obj)) {
//        fprintf(stderr, "Object %p is a binary object of %d bytes.\n", (void *)obj, size);
        struct byteObject *bobj = (struct byteObject *) obj;

        /* fix up class offset */
        bobj->class = fix_offset(bobj->class, offset);
//        fprintf(stderr, "  class is object %p.\n", (void *)(bobj->class));

        /* fix up size, size of binary objects is in bytes! */
        size = (int)((size + BytesPerWord - 1)/BytesPerWord);
    } else {
        /* ordinary objects */
//        fprintf(stderr, "Object %p is an ordinary object with %d fields.\n", (void *)obj, size);

        /* fix the class first */
//        fprintf(stderr, "  class is object %p.\n", (void *)(obj->class));

        obj->class = fix_offset(obj->class, offset);

        /* write the instance variables of the object */
        for (i = 0; i < size; i++) {
            /* we only need to stitch this up if it is not a SmallInt. */
            if(obj->data[i] != NULL) {
                if(!IS_SMALLINT(obj->data[i])) {
                    obj->data[i] = fix_offset(obj->data[i], offset);
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




uint8_t get_image_version(FILE *fp)
{
    uint8_t header[IMAGE_HEADER_SIZE];
    int rc;

    rc = (int)fread(&header, sizeof(header), 1, fp);

    if(rc != 1) {
        error("get_image_version(): Unable to read version header data!");
    }

    if(header[0] == 'l' && header[1] == 's' && header[2] == 't' && header[3] == '!') {
        uint8_t ver = header[4];

        if(ver <= IMAGE_VERSION_2) {
            /* old version, seek past the extra 3 bytes. */
            fseek(fp, IMAGE_HEADER_SIZE_OLD, SEEK_SET);
        }

        return ver;
    } else {
        /* not a version with a header, seek back to the start. */
        fseek(fp, 0, SEEK_SET);
        return 0;
    }
}


void put_image_version(FILE *fp, uint8_t version)
{
    uint8_t header[IMAGE_HEADER_SIZE_OLD];
    size_t header_size = 0;
    int rc = 0;

    header[0] = (uint8_t)'l';
    header[1] = (uint8_t)'s';
    header[2] = (uint8_t)'t';
    header[3] = (uint8_t)'!';
    header[4] = version;

    if(version <= IMAGE_VERSION_2) {
        /* add the padding. */
        header[5] = (uint8_t)0;
        header[6] = (uint8_t)0;
        header[7] = (uint8_t)0;

        header_size = IMAGE_HEADER_SIZE_OLD;
    } else {
        header_size = IMAGE_HEADER_SIZE;
    }

    rc = (int)fwrite(&header, header_size, 1, fp);

    if(rc != 1) {
        error("put_image_version(): Unable to write version header data!");
    }
}


