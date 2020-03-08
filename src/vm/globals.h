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
extern struct object *dictLookup(struct object *dict, char *name);
extern struct object *lookupGlobal(char *name);


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
extern struct object *UndefinedClass;


/* values for the current program. */
extern int prog_argc;
extern const char **prog_argv;

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
        * arguments - an array
        * temporaries - an array
        * stack - an array
        * bytecode offset (an integer)
        * stack pointer
        * a reference to the previous context in the chain.
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


/* Dictionary */
#define keysInDictionary    (0)
#define valuesInDictionary  (1)

