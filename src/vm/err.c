#include <stdio.h>
#include <stdlib.h>
#include "globs.h"
#include "memory.h"
#include "err.h"
#include "interp.h"

void sysError(char * a)
{
    fprintf(stderr,"unrecoverable system error: %s\n", a);
    exit(1);
}


void sysErrorInt(char * a, intptr_t b)
{
    fprintf(stderr,"unrecoverable system error: %s %ld\n", a, b);
    exit(1);
}


void sysErrorStr(char * a, char * b)
{
    fprintf(stderr,"unrecoverable system error: %s %s\n", a, b);
    exit(1);
}


void backTrace(struct object * aContext)
{
    printf("back trace\n");
    while (aContext && (aContext != nilObject)) {
        struct object * arguments;
        int i;
        printf("message %s ",
               bytePtr(aContext->data[methodInContext]
                       ->data[nameInMethod]));
        arguments = aContext->data[argumentsInContext];
        if (arguments && (arguments != nilObject)) {
            printf("(");
            for (i = 0; i < SIZE(arguments); i++)
                printf("%s%s",
                       ((i == 0) ? "" : ", "),
                       bytePtr(arguments->data[i]->class->
                               data[nameInClass]));
            printf(")");
        }
        printf("\n");
        aContext = aContext->data[previousContextInContext];
    }
}
