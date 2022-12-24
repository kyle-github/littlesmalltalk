/*
    Little Smalltalk
    Written by Tim Budd, budd@cs.orst.edu

    Relicensed under BSD 3-clause license per permission from Dr. Budd by
    Kyle Hayes.

    See LICENSE file.
*/

#pragma once

#include <stdint.h>

//extern int64_t cache_hit;
//extern int64_t cache_miss;

extern int execute(struct object *aProcess, int ticks);
extern void flushCache(void);

extern int64_t cache_hit;
extern int64_t cache_miss;


/*
    symbolic definitions for the bytecodes
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

