#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "globals.h"
#include "memory.h"
#include "err.h"
#include "interp.h"

//void sysError(char * a)
//{
//    fprintf(stderr,"unrecoverable system error: %s\n", a);
//    exit(1);
//}
//
//
//void sysErrorInt(char * a, intptr_t b)
//{
//    fprintf(stderr,"unrecoverable system error: %s %ld\n", a, b);
//    exit(1);
//}
//
//
//void sysErrorStr(char * a, char * b)
//{
//    fprintf(stderr,"unrecoverable system error: %s %s\n", a, b);
//    exit(1);
//}




void error(const char *templ, ...)
{
    va_list va;

    /* print it out. */
    va_start(va,templ);
    vfprintf(stderr,templ,va);
    va_end(va);
    fprintf(stderr,"\n");

    exit(1);
}



void info(const char *templ, ...)
{
    va_list va;

    /* print it out. */
    va_start(va,templ);
    vfprintf(stderr,templ,va);
    va_end(va);
    fprintf(stderr,"\n");
}



void backTrace(struct object * aContext)
{
    printf("back trace\n");

    /* check if we actually got a Context. */
    if(NOT_NIL(aContext)) {
        if(! isDynamicMemory(aContext)) {
            info("  Cannot dump context, pointer is not to a live object!");
            return;

            if(aContext->class != ContextClass) {
                info("  Cannot dump context, pointer is not to a Context object!");
                return;
            }
        }
    } else {
        info("   Cannot dump context, point is NULL or points to the nil object!");
        return;
    }

    while (NOT_NIL(aContext)) {
        struct object *method = aContext->data[methodInContext];
        struct object *class;
        struct object *symbol;
        struct object *arguments;
        int i;

        if(NOT_NIL(method)) {
            class = method->data[classInMethod];

            if(NOT_NIL(class)) {
                symbol = class->data[nameInClass];

                if(NOT_NIL(symbol)) {
                    printf("class %.*s", SIZE(symbol), bytePtr(symbol));
                } else {
                    printf("nil class name");
                }
            } else {
                printf("nil class");
            }

            symbol = method->data[nameInMethod];
            if(NOT_NIL(symbol)) {
                printf(" #%.*s ", SIZE(symbol), bytePtr(symbol));
            } else {
                printf(" nil symbol ");
            }
        } else {
            printf(" nil method ");
        }

        arguments = aContext->data[argumentsInContext];
        if (NOT_NIL(arguments)) {
            printf("(");
            for (i = 0; i < (int)SIZE(arguments); i++) {
                if(i > 0) { printf(", "); }
                if(arguments->data[i]) {
                    struct object *arg = arguments->data[i];

                    if(IS_SMALLINT(arg)) {
                        printf("%d", integerValue(arg));
                    } else {
                        class = CLASS(arg);

                        if(NOT_NIL(class)) {
                            symbol = class->data[nameInClass];

                            if(NOT_NIL(symbol)) {
                                printf("%.*s", SIZE(symbol), bytePtr(symbol));
                            } else {
                                printf("nil class name");
                            }
                        } else {
                            printf("nil class");
                        }
                    }
                } else {
                    printf("nil arg");
                }
            }

            printf(")");
        }
        printf("\n");

        aContext = aContext->data[previousContextInContext];
    }
}

