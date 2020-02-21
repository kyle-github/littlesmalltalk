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




/* look up a global entry by name */

struct object *lookupGlobal(char *name)
{
    struct object *dict;
    struct object *keys;
    struct object *key;
    struct object *vals;
    int low,high,mid;
    int result;

    dict = globalsObject;
    keys = dict->data[0];
    low = 0;
    high = (int)SIZE(keys);

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
            vals = dict->data[1];
            return vals->data[mid];
        } else {
            if (result < 0) {
                high = mid;
            } else {
                low = mid+1;
            }
        }
    }

    return 0;
}






/*
    method cache for speeding method lookup
*/

method_cache_entry cache[METHOD_CACHE_SIZE];

int64_t cache_hit = 0;
int64_t cache_miss = 0;




/* flush dynamic methods when GC occurs */
void flushCache(void)
{
    int i;

    for (i = 0; i < METHOD_CACHE_SIZE; i++) {
        cache[i].name = 0;  /* force refill */
    }
}
