#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "memory.h"
#include "globals.h"
#include "err.h"


//#define OK (1)


typedef struct {
    FILE *src_file;
    char *class_name;
    char *parent_class_name;
    char *instance_var_str;
    char *class_var_str;
} class_source;


/* classes used through the bootstrapping process. */
static struct object *ObjectClass = NULL;
static struct object *MetaObjectClass = NULL;
static struct object *ClassClass = NULL;
static struct object *NilClass = NULL;
static struct object *TrueClass = NULL;
static struct object *FalseClass = NULL;
//static struct object *StringClass = NULL;
//static struct object *SymbolClass = NULL;
static struct object *TreeClass = NULL;
//static struct object *DictionaryClass = NULL;
static struct object *OrderedArrayClass = NULL;
static struct object *MetaClassClass = NULL;
//static struct object *ByteArrayClass = NULL;

/* globals */
static struct object *globalValues = NULL;


/* temporary storage for globals during bootstrap. */
#define MAX_GLOBALS (100)
static int globalTop = 0;
static char *globalNames[MAX_GLOBALS];
static struct object *globals[MAX_GLOBALS];

/* temporary storage for symbols during bootstrap. */
#define MAX_SYMBOLS (10000)
static int symbolTop = 0;
static struct object *oldSymbols[MAX_SYMBOLS];



static class_source **get_sources(char **source_files, int num_sources);
static void open_source_file(class_source *source, const char *class_file_name);
static void sort_source_files(class_source **st_sources, int num_sources);
static void bigBang(void);

/* helper routines */
static char *get_delimited_token(char **str, char start_char, char end_char);
static void addGlobalName(char *name, struct object *value);
static struct object *lookupGlobalName(char *name, int ok_missing);
static struct object *newSymbol(char *text);
static struct object *newClass(char *name);
static struct object *newDictionary(void);
static struct object *newArray(int size);
static struct object *newOrderedArray(int size);
static int symbolCmp(struct object *left, struct object *right);
static int symbolBareCmp(const uint8_t *left, int leftsize, const uint8_t *right, int rightsize);






int main(int argc, char **argv)
{
    class_source **st_sources = NULL;

    info("Starting up....");

    st_sources = get_sources(++argv, argc - 1);

    /* set up the initial objects required to bootstrap. */
    bigBang();

    return 0;
}


class_source **get_sources(char **source_files, int num_sources)
{
    class_source **st_sources = NULL;

    info("\tFinding and opening class source files.");

    if(num_sources < 1) {
        error("You must pass the Smalltalk files to put into the image on the command line!");
    }

    /* allocate memory for the source structs. */
    st_sources = calloc((size_t)num_sources, sizeof(class_source*));
    if(!st_sources) {
        error("Unable to allocate class source struct array!");
    }

    for(int i=0; i < num_sources; i++) {
        st_sources[i] = calloc(1, sizeof(class_source));
        if(!st_sources[i]) {
            error("Unable to allocate class source struct!");
        }
    }

    /* find the object creation header in each one. */
    for(int i=0; i < num_sources; i++) {
        info("\t\tOpening source file %s.", source_files[i]);
        open_source_file(st_sources[i], source_files[i]);
    }

    sort_source_files(st_sources, num_sources);

    /* create all the classes. */
    for(int i=0; i < num_sources; i++) {
        printf("Creating class %s subclass of %s, with instance vars #( %s ) and class vars #( %s ).\n",
            st_sources[i]->class_name,
            st_sources[i]->parent_class_name,
            st_sources[i]->instance_var_str,
            st_sources[i]->class_var_str);
    }

    return st_sources;
}

void open_source_file(class_source *source, const char *class_file_name)
{
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    ssize_t line_size;

    /* open the file or die */
    source->src_file = fopen(class_file_name, "r");
    if(!source->src_file) {
        error("Unable to open source file %s!", class_file_name);
    }

    /*
     * find the lines starting with "+".  This line
     * is the header that describes the class to create (and its meta class)
     */

    /* get the first line of the file. */
    line_size = getline(&line_buf, &line_buf_size, source->src_file);

    /* loop until we find the header line. */
    while(line_size >= 0) {
        /* check for the marker character. */
        if(line_buf[0] == '+') {
            char *p = line_buf;

            source->parent_class_name = get_delimited_token(&p, '+', ' ');
            source->class_name = get_delimited_token(&p, '#', ' ');
            source->instance_var_str = get_delimited_token(&p, '(', ')');
            source->class_var_str = get_delimited_token(&p, '(', ')');

            if(line_buf) free(line_buf);
            return;
        }
        /* Get the next line */
        line_size = getline(&line_buf, &line_buf_size, source->src_file);
    }

    /* Free the allocated line buffer */
    free(line_buf);

    error("Unable to find the class creation line in the source file %s!", class_file_name);
}


void sort_source_files(class_source **st_sources, int num_sources)
{
    int parent_idx, child_idx, possible_child_idx;

    /* first, find Object and move it to the front. */
    for(parent_idx = 1; parent_idx < num_sources; parent_idx++) {
        if(strcmp("Object", st_sources[parent_idx]->class_name) == 0) {
            /* found Object.  Swap it into the front. */
            class_source *tmp = st_sources[0];
            st_sources[0] = st_sources[parent_idx];
            st_sources[parent_idx] = tmp;
            break;
        }
    }

    /* loop over all the possible classes for parents. */
    for(parent_idx = 0, child_idx = 1; parent_idx < num_sources; parent_idx++) {
        //printf("Finding subclasses of class %s (index %d).\n", st_sources[parent_idx]->class_name, parent_idx);

        for(possible_child_idx = child_idx; possible_child_idx < num_sources; possible_child_idx++) {

            //printf("  Checking class %s (index %d).\n", st_sources[possible_child_idx]->class_name, possible_child_idx);

            /* does the parent match? */
            if(strcmp(st_sources[possible_child_idx]->parent_class_name, st_sources[parent_idx]->class_name) == 0) {
                //printf("  %s is a subclass of %s.\n", st_sources[possible_child_idx]->class_name, st_sources[parent_idx]->class_name);

                /* yes? then swap */
                class_source *tmp = st_sources[child_idx];
                st_sources[child_idx] = st_sources[possible_child_idx];
                st_sources[possible_child_idx] = tmp;
                child_idx++;
            }
        }
    }
}



/* ------------------------------------------------------------- */
/*	                        big bang                             */
/* ------------------------------------------------------------- */

void bigBang(void)
{

    /*
     * First, make the nil (undefined) object;
     * notice its class is wrong
     */
    nilObject = gcalloc(0);

    /*
     * Second, make class for Symbol;
     * this will allow newClass to work correctly
     */
    SymbolClass = gcalloc(ClassSize + 1);
    addGlobalName("Symbol", SymbolClass);
    SymbolClass->data[nameInClass] = newSymbol("Symbol");

    /* now we can fix up nil's class */
    NilClass = newClass("Undefined");
    addGlobalName("Undefined", NilClass);
    nilObject->class = NilClass;
    addGlobalName("nil", nilObject);

    /* make up the object / metaobject mess */
    ObjectClass = newClass("Object");
    addGlobalName("Object", ObjectClass);
    MetaObjectClass = newClass("MetaObject");
    addGlobalName("MetaObject", MetaObjectClass);
    ObjectClass->class = MetaObjectClass;
    ObjectClass->data[parentClassInClass] = nilObject;

    /* And the Class/MetaClass mess */
    ClassClass = newClass("Class");
    addGlobalName("Class", ClassClass);
    MetaClassClass = newClass("MetaClass");
    addGlobalName("MetaClass", MetaClassClass);
    ClassClass->class = MetaClassClass;

    /* now make up a bunch of other classes */
    BlockClass = newClass("Block");
    addGlobalName("Block", BlockClass);

    /* SmallInt has an extra class variable, just like Symbol */
    SmallIntClass = gcalloc(ClassSize + 1);
    addGlobalName("SmallInt", SmallIntClass);
    SmallIntClass->data[nameInClass] = newSymbol("SmallInt");

    IntegerClass = newClass("Integer");
    addGlobalName("Integer", IntegerClass);

    TrueClass = newClass("True");
    addGlobalName("True", TrueClass);
    trueObject = gcalloc(0);
    trueObject->class = TrueClass;
    addGlobalName("true", trueObject);

    FalseClass = newClass("False");
    addGlobalName("False", FalseClass);
    falseObject = gcalloc(0);
    falseObject->class = FalseClass;
    addGlobalName("false", falseObject);

    ArrayClass = newClass("Array");
    addGlobalName("Array", ArrayClass);
    ByteArrayClass = newClass("ByteArray");
    addGlobalName("ByteArray", ByteArrayClass);

    OrderedArrayClass = newClass("OrderedArray");
    addGlobalName("OrderedArray", OrderedArrayClass);

    StringClass = newClass("String");
    addGlobalName("String", StringClass);

    TreeClass = newClass("Tree");
    addGlobalName("Tree", TreeClass);

    DictionaryClass = newClass("Dictionary");
    addGlobalName("Dictionary", DictionaryClass);

    /* finally, we can fill in the fields in class Object */
    ObjectClass->data[methodsInClass] = newDictionary();
    ObjectClass->data[instanceSizeInClass] = newInteger(0);
    ClassClass->data[instanceSizeInClass] = newInteger(0);

    /* can make global name, but can't fill it in */
    globalValues = gcalloc(2);
    addGlobalName("globals", globalValues);
}




/******* helper functions *******/

char *get_delimited_token(char **str, char start_char, char end_char)
{
    char *p = *str;
    char *q = NULL;

    while(*p != start_char) {
        //printf("char '%c' is not the start char.\n", *p);
        p++;
    }
    //printf("char '%c' is the start char.\n", *p);

    q = ++p;

    //printf("q set to the start of the token, '%c'.\n", *q);

    while(*p != end_char) {
        //printf("char '%c' is not the end char.\n", *p);
        p++;
    }
    //printf("char '%c' is the end char.\n", *p);

    *p = 0;

    *str = p++;

    //printf("returning \"%s\".\n",q);

    return strdup(q);
}


void addGlobalName(char *name, struct object *value)
{
    char *newName;

    newName = strdup(name);
    if (!newName) {
        sysErrorStr("out of memory", "newname in add global");
    }
    globalNames[globalTop] = newName;
    globals[globalTop] = value;
    globalTop++;
}

struct object *lookupGlobalName(char *name, int ok_missing)
{
    int i;

    for (i = 0; i < globalTop; i++) {
        if (strcmp(name, globalNames[i]) == 0) {
            return globals[i];
        }
    }
    /* not found, return 0 */
    if (!ok_missing) {
        sysErrorStr("Missing global", name);
    }
    return 0;
}

struct object *newSymbol(char *text)
{
    int i;
    struct byteObject *result;

    /* first see if it is already a symbol */
    for (i = 0; i < symbolTop; i++) {
        if (symbolBareCmp((uint8_t *)text, (int)strlen(text), bytePtr(oldSymbols[i]), SIZE(oldSymbols[i])) == 0) {
            return oldSymbols[i];
        }
    }

    /* not there, make a new one */
    result = (struct byteObject *)gcialloc((int)strlen(text));
    for (i = 0; i < (int)strlen(text); i++) {
        result->bytes[i] = (uint8_t)text[i];
    }
    result->class = lookupGlobalName("Symbol", 0);
    oldSymbols[symbolTop++] = (struct object *) result;
    return (struct object *) result;
}

struct object *newClass(char *name)
{
    struct object *newC;

    newC = gcalloc(ClassSize);
    newC->data[nameInClass] = newSymbol(name);
    return newC;
}


struct object *newDictionary(void)
{
    struct object *result;

    result = gcalloc(2);
    result->class = lookupGlobalName("Dictionary", 0);
    result->data[0] = newOrderedArray(0);
    result->data[1] = newArray(0);
    return result;
}


struct object *newArray(int size)
{
    struct object *result;
    int i;

    result = gcalloc(size);
    result->class = lookupGlobalName("Array", 0);
    for (i = 0; i < size; ++i) {
        result->data[i] = nilObject;
    }
    return (result);
}

struct object *newOrderedArray(int size)
{
    struct object *result;
    int i;

    result = gcalloc(size);
    result->class = lookupGlobalName("OrderedArray", 0);
    for (i = 0; i < size; ++i) {
        result->data[i] = nilObject;
    }
    return (result);
}

int symbolCmp(struct object *left, struct object *right)
{
    return symbolBareCmp(bytePtr(left), SIZE(left), bytePtr(right), SIZE(right));
}


int symbolBareCmp(const uint8_t *left, int leftsize, const uint8_t *right, int rightsize)
{
    int32_t minsize = leftsize;
    int32_t i;

    if (rightsize < minsize)
        minsize = rightsize;
    for (i = 0; i < minsize; i++) {
        if (left[i] != right[i]) {
            if (left[i] < right[i]) {
                return -1;
            } else {
                return 1;
            }
        }
    }
    return (int)leftsize - (int)rightsize;
}

