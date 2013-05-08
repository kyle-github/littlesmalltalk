/*
	memory management for the Little Smalltalk system
	Uses a variation on the Baker two-space algorithm

	Written by Tim Budd, Oregon State University, 1994
	Comments to budd@cs.orst.edu
	All rights reserved, no guarantees given whatsoever.
	May be freely redistributed if not for profit.
*/

/*
	The fundamental data type is the object.
	The first field in an object is a size, the low order two
		bits being used to maintain:
			* binary flag, used if data is binary
			* indirection flag, used if object has been relocated
	The next data field is the class
	The following fields are either objects, or character values

	The initial image is loaded into static memory space --
	space which is never garbage collected
	This improves speed, as these items are not moved during GC
*/
#include <sys/types.h>

struct object {
	uint size;
	struct object *class;
	struct object *data[0];
};

/*
	byte objects are used to represent strings and symbols
		bytes per word must be correct
*/

struct byteObject {
	uint size;
	struct object *class;
	unsigned char bytes[0];
};

# define BytesPerWord 4
# define bytePtr(x) (((struct byteObject *) x)->bytes)
#define WORDSUP(ptr, amt) ((struct object *)(((char *)(ptr)) + \
	((amt) * BytesPerWord)))
#define WORDSDOWN(ptr, amt) WORDSUP(ptr, 0 - (amt))

/*
 * SmallInt objects are used to represent short integers.  They are
 * encoded as 31 bits, signed, with the low bit set to 1.  This
 * distinguishes them from all other objects, which are longword
 * aligned and are proper C memory pointers.
 */
#include <limits.h>

#define IS_SMALLINT(x) ((((int)(x)) & 0x01) != 0)
#define FITS_SMALLINT(x) ((((int)(x)) >= INT_MIN/2) && \
	(((int)(x)) <= INT_MAX/2))
#define CLASS(x) (IS_SMALLINT(x) ? SmallIntClass : ((x)->class))
#define integerValue(x) (((int)(x)) >> 1)
#define newInteger(x) ((struct object *)((((int)(x)) << 1) | 0x01))

/*
 * The "size" field is the top 30 bits; the bottom two are flags
 */
#define SIZE(op) ((op)->size >> 2)
#define SETSIZE(op, val) ((op)->size = ((val) << 2))
#define FLAG_GCDONE (0x01)
#define FLAG_BIN (0x02)

/*
	memoryBase holds the pointer to the current space,
	memoryPointer is the pointer into this space.
	To allocate, decrement memoryPointer by the correct amount.
	If the result is less than memoryBase, then garbage collection
	must take place

*/

extern struct object *memoryPointer, *memoryBase;

/*
	roots for the memory space
	these are traced down during memory management
	rootStack is the dynamic stack
	staticRoots are values in static memory that point to
	dynamic values
*/
# define ROOTSTACKLIMIT 50
extern struct object *rootStack[];
extern int rootTop;
extern void addStaticRoot(struct object **);

/*
	The following are roots for the file out 
*/

extern struct object *nilObject, *trueObject,
	*falseObject, *SmallIntClass, *ArrayClass, *BlockClass,
	*ContextClass, *globalsObject, *initialMethod,
	*binaryMessages[3], *IntegerClass, *badMethodSym;

/*
	entry points
*/

extern void gcinit(int, int);
extern struct object *gcollect(int), *staticAllocate(int),
	*staticIAllocate(int), *gcialloc(int);
extern void exchangeObjects(struct object *, struct object *, uint);
extern int symstrcomp(struct object * left, const char *right);
extern int strsymcomp(const char *left, struct object *right);

extern int isDynamicMemory(struct object *);

#define gcalloc(sz) (((memoryPointer = WORDSDOWN(memoryPointer, (sz) + 2)) < \
	memoryBase) ? gcollect(sz) : \
	(SETSIZE(memoryPointer, (sz)), memoryPointer))

#ifndef gcalloc
extern struct object *gcalloc(int);
#endif


/* these define the flags used for writing and reading images.
The bytes per word or size is usually stored in the lower bits */
# define LST_ERROR_TYPE		(0)

/* normal objects */
# define LST_OBJ_TYPE		(1<<5)

/* positive small integers */
# define LST_PINT_TYPE		(2<<5)

/* negative small integers */
# define LST_NINT_TYPE		(3<<5)

/* byte arrays */
# define LST_BARRAY_TYPE	(4<<5)

/* previously dumped object */
# define LST_POBJ_TYPE		(5<<5)

/* nil object */
# define LST_NIL_TYPE		(6<<5)

# define LST_SMALL_TAG_LIMIT	0x0F
# define LST_LARGE_TAG_FLAG	0x10
# define LST_TAG_SIZE_MASK	0x1F
# define LST_TAG_VALUE_MASK     LST_SMALL_TAG_LIMIT
# define LST_TAG_TYPE_MASK	0xE0
