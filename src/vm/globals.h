/*
 * globs.h
 *	Global defs for VM modules
 */

#pragma once

#include <sys/types.h>
#include <stdio.h>
#include <stdint.h>

/* get the current time in microseconds. Used mostly for debugging. */
extern int64_t time_usec();

/* used all over for looking up classes and other globals */
extern struct object *lookupGlobal(char *name);

/* method cache */

typedef struct {
    struct object *name;
    struct object *class;
    struct object *method;
} method_cache_entry;

#define METHOD_CACHE_MASK (0x3FF)

extern method_cache_entry cache[];

extern int64_t cache_hit;
extern int64_t cache_miss;

extern void flushCache(void);


/* global debugging flag */
extern unsigned int debugging;


/* commonly used objects */
extern struct object *badMethodSym;
extern struct object *binaryMessages[3];
extern struct object *falseObject;
extern struct object *globalsObject;
extern struct object *initialMethod;
extern struct object *nilObject;
extern struct object *trueObject;

/* commonly used classes */
extern struct object *ArrayClass;
extern struct object *BlockClass;
extern struct object *ByteArrayClass;
extern struct object *ContextClass;
extern struct object *DictionaryClass;
extern struct object *IntegerClass;
extern struct object *SmallIntClass;
extern struct object *StringClass;
extern struct object *SymbolClass;

/*
    The following are the objects with ``hard-wired''
    field offsets
*/
/*
    A Process has two fields
        * a current context
        * status of process (running, waiting, etc)
*/
# define processSize 3
# define contextInProcess 0
# define statusInProcess 1
# define resultInProcess 2

/*
    A Context has:
        * method (which has bytecode pointer)
        * bytecode offset (an integer)
        * arguments
        * temporaries and stack
        * stack pointer
*/

# define contextSize 7
# define methodInContext 0
# define argumentsInContext 1
# define temporariesInContext 2
# define stackInContext 3
# define bytePointerInContext 4
# define stackTopInContext 5
# define previousContextInContext 6

/*
    A Block is subclassed from Context
    shares fields with creator, plus a couple new ones
*/

# define blockSize 10
# define methodInBlock methodInContext
# define argumentsInBlock argumentsInContext
# define temporariesInBlock temporariesInContext
# define stackInBlock stackInContext
# define stackTopInBlock stackTopInContext
# define previousContextInBlock previousContextInContext
# define argumentLocationInBlock 7
# define creatingContextInBlock 8
/* the following are saved in different place so they don't get clobbered*/
# define bytePointerInBlock 9

/*
    A Method has:
        * name (a Symbol)
        * bytecodes
        * literals
        * stack size
        * temp size
        * class that owns the method
        * method source code
*/

# define methodSize 7
# define nameInMethod 0
# define byteCodesInMethod 1
# define literalsInMethod 2
# define stackSizeInMethod 3
# define temporarySizeInMethod 4
# define classInMethod 5
# define textInMethod 6

/*
    A Class has:
        * pointer to parent class
        * pointer to tree of methods
*/

# define ClassSize 5
# define nameInClass 0
# define parentClassInClass 1
# define methodsInClass 2
# define instanceSizeInClass 3
# define variablesInClass 4

/*
    A node in a tree has
        * value field
        * left subtree
        * right subtree
*/

# define valueInNode 0
# define leftInNode 1
# define rightInNode 2

/*
    misc defines
*/

# define rootInTree 0
# define receiverInArguments 0
# define symbolsInSymbol 5
