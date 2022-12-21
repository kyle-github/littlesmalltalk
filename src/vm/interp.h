/*
    Little Smalltalk, version 4
    Written by Tim Budd, Oregon State University, budd@cs.orst.edu
    All rights reserved, no guarantees given whatsoever.
    May be freely redistributed if not for profit.

    Changes Copyright(c) Kyle Hayes 2022

    Provided under the BSD license.   See LICENSE.
*/

#pragma once

#include <stdint.h>
#include <memory.h>







/*
 * symbolic definitions for the bytecodes
 */

# define Extended 0
# define PushInstance 1
# define PushArgument 2
# define PushTemporary 3
# define PushLiteral 4
# define PushConstant 5
# define AssignInstance 6
# define AssignTemporary 7
# define MarkArguments 8
# define SendMessage 9
# define SendUnary 10
# define SendBinary 11
# define PushBlock 12
# define DoPrimitive 13
# define DoSpecial 15

/* types of special instructions (opcode 15) */

# define SelfReturn 1
# define StackReturn 2
# define BlockReturn 3
# define Duplicate 4
# define PopTop 5
# define Branch 6
# define BranchIfTrue 7
# define BranchIfFalse 8
# define SendToSuper 11
# define Breakpoint 12

/* special constants */

/* constants 0 to 9 are the integers 0 to 9 */
# define nilConst 10
# define trueConst 11
# define falseConst 12

/* Return values from doExecute: primitive */
#define ReturnError 2       /* error: called */
#define ReturnBadMethod 3   /* Unknown method for object */
#define ReturnReturned 4    /* Top level method returned */
#define ReturnTimeExpired 5 /* Time quantum exhausted */
#define ReturnBreak 6       /* Breakpoint instruction */



/* 
 * handy struct definitions to access common objects
 * that the interpreter knows about. 
 */


#define SPECIAL_OBJ_SIZE(obj_class) ((sizeof(obj_class) - sizeof(struct object))/sizeof(struct object *))


/*
    A Class has:
        * pointer to parent class
        * pointer to tree of methods
*/

struct classObject {
    uintptr_t header;
    struct object *class; /* MetaClass */

    struct object *name;
    struct object *parentClass;
    struct object *methods;
    struct object *instanceSize;
    struct object *variables;
};

#define classSize SPECIAL_OBJ_SIZE(struct classObject)



/* Dictionary */
struct dictionaryObject {
    uintptr_t header;
    struct object *class;

    struct object *keys;
    struct object *values;
};

#define dictionarySize SPECIAL_OBJ_SIZE(struct dictionaryObject)


/*
    A Process has three fields
        * a current context
        * status of process (running, waiting, etc)
        * a result of the process execution
*/
struct processObject {
    uintptr_t header;
    struct object *class;

    struct object *context;
    struct object *status;
    struct object *result;
};

#define processSize SPECIAL_OBJ_SIZE(struct processObject)




/*
    A Context has:
        * method (which has bytecode pointer)
        * arguments - an array
        * temporaries - an array
        * stack - an array
        * bytecode offset (an integer)
        * stack pointer
        * a reference to the previous context in the chain.
*/

struct contextObject {
    uintptr_t header;
    struct object *class;

    struct object *method;
    struct object *arguments;
    struct object *temporaries;
    struct object *stack;
    struct object *bytePointer;
    struct object *stackTop;
    struct contextObject *previousContext;
};

#define contextSize SPECIAL_OBJ_SIZE(struct contextObject)



/*
    A Block is subclassed from Context
    shares fields with creator, plus a couple new ones
*/

struct blockContextObject {
    uintptr_t header;
    struct object *class;

    /* same as context */
    struct object *method;
    struct object *arguments;
    struct object *temporaries;
    struct object *stack;
    struct object *bytePointer;
    struct object *stackTop;
    struct contextObject *previousContext;

    /* specific to block */
    struct object *argumentLocation;
    struct contextObject *creatingContext;
    struct object *blockBytePointer;
};

#define blockSize SPECIAL_OBJ_SIZE(struct blockContextObject)



/*
    A Method has:
        * name (a Symbol)
        * bytecodes (a ByteArray)
        * literals (an Array)
        * stack size
        * temp size
        * class that owns the method
        * method source code
*/

struct methodObject {
    uintptr_t header;
    struct object *class;

    struct object *name;
    struct object *byteCodes;
    struct object *literals;
    struct object *stackSize;
    struct object *temporarySize;
    struct object *owningClass;
    struct object *text;
};

#define methodSize SPECIAL_OBJ_SIZE(struct methodObject)


extern int execute(struct processObject *aProcess, int ticks);
extern void flushCache(void);

extern int64_t cache_hit;
extern int64_t cache_miss;

