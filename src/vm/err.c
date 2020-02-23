#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "globals.h"
#include "memory.h"
#include "err.h"
#include "interp.h"



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
        int i;

        printf("message %s ", bytePtr(aContext->data[methodInContext]->data[nameInMethod]));

        arguments = aContext->data[argumentsInContext];
        if (arguments && (arguments != nilObject)) {
            printf("(");
            for (i = 0; i < SIZE(arguments); i++) {
                printf("%s%s",
                       ((i == 0) ? "" : ", "),
                       bytePtr(arguments->data[i]->class->
                               data[nameInClass]));
            }

            printf(")");
        }
        printf("\n");
        aContext = aContext->data[previousContextInContext];
    }
}


#define BUF_SIZE (256)

void printClass(struct object *obj)
{
    if(IS_SMALLINT(obj)) {
        info("object class SmallInt");
    } else {
        char buf[BUF_SIZE];
        struct byteObject *class_name = (struct byteObject *)obj->class->data[nameInClass];

        memset(buf, 0, sizeof(buf));

        for(int i=0; i < (int)sizeof(buf) && i < (int)SIZE(class_name); i++) {
            buf[i] = (char)bytePtr(class_name)[i];
        }

        buf[BUF_SIZE -1] = (char)0;

        info("object class %s", buf);
    }
}

