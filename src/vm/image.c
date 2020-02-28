/*
 * Functions for image handling.
 */

#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "err.h"
#include "globals.h"
#include "image.h"
#include "memory.h"

/*
 * An image header is 8 bytes.
 *
 * 4 bytes for the "magic" part, "lst!".
 * 4 bytes for an unsigned version number in little-endian order.
 */
#define HEADER_SIZE (8)



static int fileIn_version_0(FILE *img);
static int fileIn_version_1(FILE *img);
static int fileIn_version_5(FILE *img);

/* helpers for version 0 and 1 images. */
static int get_byte(FILE *fp);
static void readTag(FILE *fp, int *type, int *val);
static struct object *read_object_0(FILE *fp);


//static int dump_image(void);
//static struct object *dump_object(int obj_num, struct object *obj);
//static struct object *fixup_class(int obj_num, struct object *obj);

void write_object_5(FILE *img, int obj_num, struct object *obj);
static int obj_to_index(struct object *obj);
#define TO_INDEX(o) ((uint32_t)(uintptr_t)((IS_SMALLINT(o) ? (smallint_t)(intptr_t)(o): (smallint_t)(obj_to_index(o) << 1))))
#define FROM_INDEX(i) ((IS_SMALLINT(i) ? (struct object *)(intptr_t)(i) : obj_table[((uint32_t)(intptr_t)(i)) >> 1]))


static int write_uint32(FILE *img, uint32_t val);
int write_uint8(FILE *img, uint8_t val);
int read_uint32(FILE *img, uint32_t *val);
int read_uint8(FILE *img, uint8_t *val);

static uint32_t get_image_header(FILE *img);
static void put_image_header(FILE *img, uint32_t version);






static int obj_table_size = 0;
static struct object **obj_table;

/*
 * Image handling entry points.  These will dispatch on the actual
 * version of the image (at least fileIn()).
 */

/*

New image format process:

write image:
  scan objects low addr to high.
    for each object, add entry in unused space
       struct object *obj_table[];
    put object address into obj_table[]
  write total number of objects out.
  scan objects high addr to low again
    write object size out as uint32_t.
    get class pointer
    do binary search in obj_table to find index of class object.data
    write out object table index << 1 as class object index.
    if bin obj
      write out binary data in single bytes.
    if normal obj
      for each instance data object
        find the object index and write it out
  find object index of globals and write it out
  find object index of other special objects and write them out.

read image

  read in # of objects.
  set up obj_table in unused space
  read in special object indexes and set them aside.
  read in object data
    store current object pointer in obj_table[];
    read in object size
    if object is bin object
      read in single bytes.
      calculate total number of cells.
    if object is normal object
      read in object indexes into data[] fields
        take care to handle object pointers vs. SmallInts
      calculate total number of cells.
    bump allocation pointer to next object
    increment object index
  once all objects are read in
  scan through object memory low to high.
    lookup all struct object * data (class and data[]) in the obj_table and replace the index with the pointer
      take care with SmallInt vs. object index (SmallInt has low bit set)
  read in special objects (as indexes)
  update the special object indexes to pointers

  load the special classes from the globals object.
*/




int fileIn(FILE *fp)
{
    uint32_t version = get_image_header(fp);

    switch(version) {
    case IMAGE_VERSION_0:
        info("Reading in version 0 image.");
        return fileIn_version_0(fp);
        break;

    case IMAGE_VERSION_1:
        info("Reading in version 1 image.");
        return fileIn_version_1(fp);
        break;

    case IMAGE_VERSION_5:
        info("Reading in version 5 image.");
        return fileIn_version_5(fp);
        break;

    default:
        error("Unsupported image file version: %u", version);
        break;
    }

    return 0;
}




#define WRITE_OOP(o) \
    info("object at " #o "=%p (%u)", o, TO_INDEX(o)); \
    write_uint32(img, TO_INDEX(o));


/* version 5 */
int fileOut(FILE *img)
{
    struct object *current_obj = NULL;
    int object_count = 0;

    info("starting to file out image version %d.", IMAGE_VERSION_5);

    /* force a GC to clean up the image */
    do_gc();

    /* point the object table base to the unused memory space. */
    if(inSpaceOne) {
        obj_table = (struct object **)spaceTwo;
    } else {
        obj_table = (struct object **)spaceOne;
    }

    /*
     * build the object table from low to high addresses.
     * It is easier to do this because we know that the first object
     * starts at memoryPointer since allocation grows downward.  If we wanted
     * to start at the top we would have to scan downward to find what looked
     * like an object header.   Not reliable.
     */
    object_count = 0;
    current_obj = memoryPointer;
    while((intptr_t)current_obj < (intptr_t)memoryTop) {
        smallint_t size = SIZE(current_obj);

        /* save this object's address. */
        obj_table[object_count] = current_obj;

        /* adjust the pointer to the next object. */
        current_obj = WORDSUP(current_obj, 2 + (IS_BINOBJ(current_obj) ? TO_BPW(size) : size));

        object_count++;
    }

    /* save the top of the object table. */
    obj_table_size = object_count;

    /* start writing out the image. */

    /* FIXME - catch return value! */
    put_image_header(img, IMAGE_VERSION_5);

    /* how much to write?   FIXME - check for overflow! */
    info("image contains %d objects.", object_count);

    /* save the amount out to the image. */
    write_uint32(img, (uint32_t)object_count);

    /*
     * write out objects from the top down.
     *
     * Yes, we scan up to build the table and scan down to
     * to save out the image.   We want to read in the image
     * in top to bottom order so that we can bump the allocation
     * pointer in order to make room for the new object.  This
     * allows us to fill in the object table as we read in the
     * image.
     */
    for(--object_count; object_count >= 0; object_count--) {
        current_obj = obj_table[object_count];
        write_object_5(img, object_count, current_obj);
    }

    /* write out core objects. Do this after we dumped the rest of the image. */

    WRITE_OOP(globalsObject);
    WRITE_OOP(initialMethod);

    for(int i=0; i < 3; i++) {
        WRITE_OOP(binaryMessages[i]);
    }

    //dump_image();

    return (int)obj_table_size;
}






/*
 * Below are the actual implementations of fileIn()
 */




int fileIn_version_0(FILE *fp)
{
    int i;

    /* use the currently unused space for the indir pointers */
    if (inSpaceOne) {
        obj_table = (struct object * *) spaceTwo;
    } else {
        obj_table = (struct object * *) spaceOne;
    }
    obj_table_size = 0;

    /* read in the image file */
    fprintf(stderr, "reading nil object.\n");
    nilObject = read_object_0(fp);
    staticRoots[staticRootTop++] = &nilObject;

    fprintf(stderr, "reading true object.\n");
    trueObject = read_object_0(fp);
    staticRoots[staticRootTop++] = &trueObject;

    fprintf(stderr, "reading false object.\n");
    falseObject = read_object_0(fp);
    staticRoots[staticRootTop++] = &falseObject;

    fprintf(stderr, "reading globals object.\n");
    globalsObject = read_object_0(fp);
    staticRoots[staticRootTop++] = &globalsObject;

    fprintf(stderr, "reading SmallInt class.\n");
    SmallIntClass = read_object_0(fp);
    staticRoots[staticRootTop++] = &SmallIntClass;

    fprintf(stderr, "reading Integer class.\n");
    IntegerClass = read_object_0(fp);
    staticRoots[staticRootTop++] = &IntegerClass;

    fprintf(stderr, "reading Array class.\n");
    ArrayClass = read_object_0(fp);
    staticRoots[staticRootTop++] = &ArrayClass;

    fprintf(stderr, "reading Block class.\n");
    BlockClass = read_object_0(fp);
    staticRoots[staticRootTop++] = &BlockClass;

    fprintf(stderr, "reading Context class.\n");
    ContextClass = read_object_0(fp);
    staticRoots[staticRootTop++] = &ContextClass;

    fprintf(stderr, "reading initial method.\n");
    initialMethod = read_object_0(fp);
    staticRoots[staticRootTop++] = &initialMethod;

    fprintf(stderr, "reading binary message objects.\n");
    for (i = 0; i < 3; i++) {
        binaryMessages[i] = read_object_0(fp);
        staticRoots[staticRootTop++] = &binaryMessages[i];
    }

    fprintf(stderr, "reading bad method symbol.\n");
    badMethodSym = read_object_0(fp);
    staticRoots[staticRootTop++] = &badMethodSym;

    /* clean up after ourselves. */
    memset((void *) obj_table,(int)0,(size_t)((size_t)spaceSize * sizeof(struct object)));

    fprintf(stderr, "Read in %d objects.\n", obj_table_size);

    return obj_table_size;
}



int fileIn_version_1(FILE *fp)
{
    int i;

    /* use the currently unused space for the indir pointers */
    if (inSpaceOne) {
        obj_table = (struct object * *) spaceTwo;
    } else {
        obj_table = (struct object * *) spaceOne;
    }
    obj_table_size = 0;

    /* read the base objects from the image file. */

    fprintf(stderr, "reading globals object.\n");
    globalsObject = read_object_0(fp);
    staticRoots[staticRootTop++] = &globalsObject;

    fprintf(stderr, "reading initial method.\n");
    initialMethod = read_object_0(fp);
    staticRoots[staticRootTop++] = &initialMethod;

    fprintf(stderr, "reading binary message objects.\n");
    for (i = 0; i < 3; i++) {
        binaryMessages[i] = read_object_0(fp);
        staticRoots[staticRootTop++] = &binaryMessages[i];
    }

    fprintf(stderr, "reading bad method symbol.\n");
    badMethodSym = read_object_0(fp);
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
    memset((void *) obj_table,(int)0,(size_t)((size_t)spaceSize * sizeof(struct object)));

    fprintf(stderr, "Read in %d objects.\n", obj_table_size);

    return obj_table_size;
}






#define READ_OOP_5(o) \
    read_uint32(img, (uint32_t*)&index); \
    info("object at " #o "=%p (%d)", FROM_INDEX(index), index); \
    o = FROM_INDEX(index); \
    staticRoots[staticRootTop++] = &(o);


int fileIn_version_5(FILE *img)
{
    struct object *obj = NULL;
    int index = 0;
    smallint_t size = 0;
    smallint_t class_index = 0;

    info("Starting to file in image version %d.", IMAGE_VERSION_5);

    /* determine where the obj_table lives */
    if(inSpaceOne) {
        obj_table = (struct object **)spaceTwo;
    } else {
        obj_table = (struct object **)spaceOne;
    }

    /* How many objects to read? */
    read_uint32(img, (uint32_t*)&obj_table_size);

    info("image contains %d objects.", obj_table_size);

    /* The objects are in top to bottom order.  Scan in the objects with indexes then fix them up. */
    obj = memoryTop;
    for(index = obj_table_size - 1; index >= 0; index--) {
        /* get the size word. */
        read_uint32(img, (uint32_t *)&size);

        /* get the class index. */
        read_uint32(img, (uint32_t *)&class_index);

        /* calculate the position in memory and save it in the table. */
        obj_table[index] = obj = WORDSDOWN(obj, 2 + ((size & (smallint_t)FLAG_BIN) ? TO_BPW(size/4) : (size/4)));

        /* cast so that we do not sign extend. */
        obj->size = (intptr_t)(uint32_t)size;
        obj->class = (struct object *)(intptr_t)(uint32_t)class_index;

        /* get direct size. */
        size = SIZE(obj);

        /* is it a binary object? */
        if(IS_BINOBJ(obj)) {
            /* size is in bytes. */

            /* alias for convenience. */
            struct byteObject *bobj = (struct byteObject *)obj;

            info("Object %d(%d) is a binary object of %d bytes.", index, (int)((intptr_t*)memoryTop - (intptr_t*)bobj), size);

            /* read in the data for the binary object. */
            for(int byte_index = 0; byte_index < size; byte_index++) {
                read_uint8(img, &(bobj->bytes[byte_index]));
            }
        } else {
            /* regular object. */

            info("Object %d(%d) is an object of %d fields.", index, (int)((intptr_t*)memoryTop - (intptr_t*)obj), size);

            /* read in the data for the object. */
            for(int data_index = 0; data_index < size; data_index++) {
                uint32_t tmp_obj_index = 0;

                read_uint32(img, &tmp_obj_index);
                obj->data[data_index] = (struct object *)(uintptr_t)tmp_obj_index;
            }
        }
    }

    /* set the bump pointer */
    memoryPointer = obj;

    info("memoryTop = %p", memoryTop);
    info("memoryPointer = %p", memoryPointer);

    /* now fix up the pointers. */
    for(index = 0; index < obj_table_size; index++) {
        obj = obj_table[index];
        size = SIZE(obj);

        if(IS_BINOBJ(obj)) {
            /* only need to fix up the class. */
            obj->class = FROM_INDEX(obj->class);
        } else {
            struct mobject *mobj = (struct mobject *)obj;

            for(int data_index = 0; data_index <= size; data_index++) {
                mobj->data[data_index] = (struct mobject *)FROM_INDEX(mobj->data[data_index]);
            }
        }
    }

    /* read in core objects. */
    READ_OOP_5(globalsObject);
    READ_OOP_5(initialMethod);

    for(int i=0; i < 3; i++) {
        READ_OOP_5(binaryMessages[i]);
    }

    READ_OOP_5(badMethodSym);

    /* lookup up everything from globals. */
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

    /* fix up any dangling or old references to classes. */
//    info("Fix up dangling classes.");
//    obj = memoryPointer;
//    index=0;
//    while((intptr_t)obj < (intptr_t)memoryTop) {
//        obj = fixup_class(index, obj);
//        index++;
//    }

    //dump_image();

    return (int)obj_table_size;
}




//
//struct object *fixup_class(int obj_num, struct object *obj)
//{
//    struct object *old_class;
//    struct object *new_class;
//    struct object *class_name_symbol;
//    smallint_t size;
//
//    /* check for illegal object */
//    if (obj == NULL) {
//        error("Cannot read to a null pointer!");
//    }
//
//    /* get the class and the class name. */
//    old_class = CLASS(obj);
//    class_name_symbol = old_class->data[nameInClass];
//
//    new_class = lookupGlobal((char *)bytePtr(class_name_symbol));
//
//    if(new_class && new_class != nilObject) {
//        if(new_class != old_class) {
//            info("object %d points to class %s that is not the class in globals!", obj_num, bytePtr(class_name_symbol));
//            //obj->class = new_class;
//        } else {
//            //info("object %d points to correct class %s.", obj_num, bytePtr(class_name_symbol));
//        }
//    }
//
//    /* skip to the next object. */
//    size = SIZE(obj);
//
//    /* byte objects are sized differently */
//    if (IS_BINOBJ(obj)) {
//        /* convert size into BytesPerWord units. */
//        size = TO_BPW(size);
//    }
//
//    /* size is number of fields plus header plus class */
//    return WORDSUP(obj, size + 2);
//}
//




/**
* read_object_0
*
* Read in an object from the input image file.  Several kinds of object are
* handled as special cases.  The routine readTag above does most of the work
* of figuring out what type of object it is and how big it is.
*/

struct object *read_object_0(FILE *fp)
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
        error("Read in a null object %p", newObj);

        break;

    case LST_OBJ_TYPE:  /* ordinary object */
        size = val;
        newObj = gcalloc(size);
        obj_table[obj_table_size++] = newObj;
        newObj->class = read_object_0(fp);

        /* get object field values. */
        for (i = 0; i < size; i++) {
            newObj->data[i] = read_object_0(fp);
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
        newObj = gcialloc(size);
        obj_table[obj_table_size++] = newObj;
        bnewObj = (struct byteObject *) newObj;
        for (i = 0; i < size; i++) {
            /* FIXME check for EOF! */
            bnewObj->bytes[i] = (uint8_t)get_byte(fp);
        }

        bnewObj->class = read_object_0(fp);
        break;

    case LST_POBJ_TYPE: /* previous object */
        if(val>obj_table_size) {
            error("Out of bounds previous object index %d",val);
        }

        newObj = obj_table[val];

        break;

    case LST_NIL_TYPE:  /* object 0 (nil object) */
        newObj = obj_table[0];
        break;

    default:
        error("Illegal tag type: %d",type);
        break;
    }

    return newObj;
}





//int dump_image(void)
//{
//    uint32_t totalCells = 0;
//    struct object *current_obj = NULL;
//    int object_count = 0;
//
//    info("starting to dump image");
//
//    /* how much to write?   FIXME - check for overflow! */
//    totalCells = (uint32_t)(((intptr_t)memoryTop - (intptr_t)memoryPointer)/(intptr_t)BytesPerWord);
//
//    info("image contains %u total cells.", totalCells);
//
//    /* dump out object image. */
//    current_obj = memoryPointer;
//    while((intptr_t)current_obj < (intptr_t)memoryTop) {
//        //info("Writing out object %p", current_obj);
//        current_obj = dump_object(object_count, current_obj);
//        object_count++;
//    }
//
//    info("image contains %d objects.", object_count);
//
//    return (int)totalCells;
//}
//
//
//
//struct object *dump_object(int obj_num, struct object *obj)
//{
//    int size;
//    ptrdiff_t offset = addr_to_offset(obj);
//
//    /* check for illegal object */
//    if (obj == NULL) {
//        error("Dumping a null object!");
//    }
//
//    /*
//     * all objects have the same header:
//     *    size + flags (32-bits)
//     *    class OOP
//     */
//
//    /* get the size, we'll use it regardless of the object type. */
//    size = SIZE(obj);
//    /* byte objects, write out the data. */
//    if (IS_BINOBJ(obj)) {
//        struct byteObject *bobj = (struct byteObject *) obj;
//        info("Object %d (offset 0x%x) is a binary object of %d bytes:", obj_num, (uint32_t)offset, size);
//        printClass(obj);
//
//        for(int i=0; i < size; i++) {
//            info("\tbytes[%d]=0x%x (%c)", i, bobj->bytes[i], (isalnum((char)bobj->bytes[i]) ? (char)bobj->bytes[i] : '?'));
//        }
//
//        /* convert size into BytesPerWord units. */
//        size = TO_BPW(size);
//    } else {
//        /* ordinary objects */
//        info("Object %d (offset 0x%x) is an object of %d instance vars:", obj_num, (uint32_t)offset, size);
//        printClass(obj);
//
//        /* write the instance variables of the object */
//        for (int i = 0; i < size; i++) {
//            /* we only need to stitch this up if it is not a SmallInt. */
//            if(obj->data[i] != NULL) {
//                if(IS_SMALLINT(obj->data[i])) {
//                    info("\tdata[%d]=SMALLINT(%d)", i, integerValue(obj->data[i]));
//                } else {
//                    info("\tdata[%d]=0x%x", i, (uint32_t)addr_to_offset(obj->data[i]));
//                }
//            } else {
//                /* fix up the value to the nil object. */
//                info("\tdata[%d]= nil", i);
//            }
//        }
//    }
//
//    /* size is number of fields plus header plus class */
//    return WORDSUP(obj, size + 2);
//}
//




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
            error("Error reading image file: tag value field exceeds machine word size.  Image created on another machine? Value=%d", tempSize);
        }

        for(i=0; i<tempSize; i++) {
            inByte = get_byte(fp);

            if(inByte == EOF) {
                error("Unexpected EOF reading image file: reading extended value and expecting byte count=%d", tempSize);
            }

            tmp = tmp  | (((uint)inByte & (uint)0xFF) << ((uint)8*(uint)i));
        }

        *val = (int)tmp;
    } else {
        *val = tempSize;
    }
}




void write_object_5(FILE *img, int obj_num, struct object *obj)
{
    smallint_t size;

    /* check for illegal object */
    lst_assert(obj != NULL, "Object %d is a null pointer!", obj_num);

    /* The size is the object header and is small enough to write directly. */
    write_uint32(img, (uint32_t)(uintptr_t)(obj->size)); /* we write the unconverted size with the flags! */

    /* now we write out the class. */
    write_uint32(img, TO_INDEX(obj->class));

    /* we need the number of elements in the object, but this is type-specific. */
    size = SIZE(obj);

    /* byte objects, write out the data. */
    if (IS_BINOBJ(obj)) {
        struct byteObject *bobj = (struct byteObject *) obj;
//        info("Object %d is a binary object of %d bytes.", obj_num, size);

        for(int i=0; i < size; i++) {
            write_uint8(img, bobj->bytes[i]);
        }
    } else {
        /* ordinary objects */
//        info("Object %d is an ordinary object with %d fields.", obj_num, size);

        /* write the instance variables of the object */
        for (int i = 0; i < size; i++) {
            /* fix up NULL to nilObject. */
            if(obj->data[i] == NULL) {
                obj->data[i] = nilObject;
            }

            write_uint32(img, TO_INDEX(obj->data[i]));
        }
    }
}






/* binary search for the object in the table. */
int obj_to_index(struct object *obj)
{
    int low,high,mid;

    low = 0;
    high = obj_table_size;

    lst_assert(obj != NULL, "obj_to_index() passed %p object pointer!", obj);

    while(low < high) {
        mid = (low + high) / 2;

        if((intptr_t)obj == (intptr_t)(obj_table[mid])) {
            return mid;
        } else {
            if((intptr_t)obj < (intptr_t)(obj_table[mid])) {
                high = mid;
            } else {
                low = mid + 1;
            }
        }
    }

    /* fatal if we did not find the object. */
    error("object %p not found in object table!", obj);

    /* never actually get here because error() does not return. */
    return -1;
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



//
//uint32_t le2host32(uint32_t le_int)
//{
//    /* use type punning. */
//    union {
//        uint32_t u_int;
//        uint8_t u_bytes[4];
//    } punner;
//    uint32_t result = 0;
//
//    punner.u_int = le_int;
//
//    result =  (uint32_t)(punner.u_bytes[0])
//           + ((uint32_t)(punner.u_bytes[1]) << 8)
//           + ((uint32_t)(punner.u_bytes[2]) << 16)
//           + ((uint32_t)(punner.u_bytes[3]) << 24);
//
//    return result;
//}
//
//
//uint32_t host2le32(uint32_t host_int)
//{
//    /* use type punning. */
//    union {
//        uint32_t u_int;
//        uint8_t u_bytes[4];
//    } punner;
//
//    punner.u_bytes[0] = (uint8_t)(host_int & 0xFF);
//    punner.u_bytes[1] = (uint8_t)((host_int >> 8) & 0xFF);
//    punner.u_bytes[2] = (uint8_t)((host_int >> 16) & 0xFF);
//    punner.u_bytes[3] = (uint8_t)((host_int >> 24) & 0xFF);
//
//    return punner.u_int;
//}
//
//


uint32_t get_image_header(FILE *img)
{
    uint8_t buf[HEADER_SIZE];
    uint32_t result = IMAGE_VERSION_ERROR;

    memset(buf, 0, sizeof(buf));

    for(int i=0; i < (int)sizeof(buf); i++) {
        if(!read_uint8(img,&buf[i])) {
            error("Error reading image header!");
        }
    }

    if(buf[0] == 'l' && buf[1] == 's' && buf[2] == 't' && buf[3] == '!') {
        result =  (uint32_t)(buf[4])
               + ((uint32_t)(buf[5]) << 8)
               + ((uint32_t)(buf[6]) << 16)
               + ((uint32_t)(buf[7]) << 24);
    } else {
        /* not a newer version, seek back to the start. */
        fseek(img, 0, SEEK_SET);
        result = IMAGE_VERSION_0;
    }

    return result;
}



void put_image_header(FILE *img, uint32_t version)
{
    if(!write_uint8(img, (uint8_t)'l')) { error("Error writing image header!"); }
    if(!write_uint8(img, (uint8_t)'s')) { error("Error writing image header!"); }
    if(!write_uint8(img, (uint8_t)'t')) { error("Error writing image header!"); }
    if(!write_uint8(img, (uint8_t)'!')) { error("Error writing image header!"); }
    if(!write_uint32(img, version))     { error("Error writing image header!"); }
}

