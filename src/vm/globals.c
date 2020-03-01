/*
 * globals.c
 *	Global defs for VM modules
 */

#include <stddef.h>
#include <stdint.h>
#include <sys/time.h>
#include "memory.h"
#include "globals.h"

/* global debugging flag */
unsigned int debugging = 0;




/* commonly used objects */
struct object *badMethodSym = NULL;
struct object *binaryMessages[3] = {0,};
struct object *falseObject = NULL;
struct object *globalsObject = NULL;
struct object *initialMethod = NULL;
struct object *nilObject = NULL;
struct object *trueObject = NULL;

/* commonly used classes */
struct object *ArrayClass = NULL;
struct object *BlockClass = NULL;
struct object *ByteArrayClass = NULL;
struct object *ContextClass = NULL;
struct object *DictionaryClass = NULL;
struct object *IntegerClass = NULL;
struct object *SmallIntClass = NULL;
struct object *StringClass = NULL;
struct object *SymbolClass = NULL;



/* Misc helpers */

int64_t time_usec()
{
    struct timeval tv;

    gettimeofday(&tv, NULL);

    return ((int64_t)1000000 * tv.tv_sec) + tv.tv_usec;
}


/* lookup dictionary entry by C string. */
struct object *dictLookup(struct object *dict, char *name)
{
    struct object *keys;
    struct object *key;
    struct object *vals;
    int low,high,mid;
    int result;

    keys = dict->data[keysInDictionary];
    low = 0;
    high = SIZE(keys);

    /*
    * Do a binary search through its keys, which are Symbols.
    */
    while (low < high) {
        mid = (low + high) / 2;
        key = keys->data[mid];

        /*
        * If we find the key, return the
        * object.
        */

        result = strsymcomp(name,key);

        if (result == 0) {
            vals = dict->data[valuesInDictionary];
            return vals->data[mid];
        } else {
            if (result < 0) {
                high = mid;
            } else {
                low = mid+1;
            }
        }
    }

    return NULL;
}

/* look up a global entry by name */

struct object *lookupGlobal(char *name)
{
    return dictLookup(globalsObject, name);
//    struct object *dict;
//    struct object *keys;
//    struct object *key;
//    struct object *vals;
//    int low,high,mid;
//    int result;
//
//    dict = globalsObject;
//    keys = dict->data[0];
//    low = 0;
//    high = SIZE(keys);
//
//    /*
//    * Do a binary search through its keys, which are Symbols.
//    */
//    while (low < high) {
//        mid = (low + high) / 2;
//        key = keys->data[mid];
//
//        /*
//        * If we find the key, return the
//        * object.
//        */
//
//        result = strsymcomp(name,key);
//
//        if (result == 0) {
//            vals = dict->data[1];
//            return vals->data[mid];
//        } else {
//            if (result < 0) {
//                high = mid;
//            } else {
//                low = mid+1;
//            }
//        }
//    }
//
//    return 0;
}




