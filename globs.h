/*
 * globs.h
 *	Global defs for VM modules
 */
#ifndef GLOBS_H
#define GLOBS_H
#include <sys/types.h>
#include <stdio.h>

extern int fileIn(FILE * fp), fileOut(FILE * fp);
extern void sysError(char *);
extern void sysErrorInt(char *, int);
extern void sysErrorStr(char *, char *);
extern void flushCache(void);
extern struct object *primitive(int, struct object *, int *);

#endif /* GLOBS_H */
