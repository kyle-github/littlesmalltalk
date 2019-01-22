/*
 * globs.h
 *	Global defs for VM modules
 */
#ifndef GLOBS_H
#define GLOBS_H
#include <sys/types.h>
#include <stdio.h>
#include <stdint.h>

extern int fileIn(FILE * fp), fileOut(FILE * fp);
extern void sysError(char *);
extern void sysErrorInt(char *, intptr_t);
extern void sysErrorStr(char *, char *);
extern void flushCache(void);
extern struct object *primitive(int, struct object *, int *);

extern int64_t time_usec();


#endif /* GLOBS_H */
