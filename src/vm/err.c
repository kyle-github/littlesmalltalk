#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "globals.h"
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
    while (aContext && (aContext != nilObject)) {
        struct object * arguments;
        uint32_t i;
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
