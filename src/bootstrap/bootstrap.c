/*
    Little Smalltalk
    Written by Tim Budd, budd@cs.orst.edu

    Relicensed under BSD 3-clause license per permission from Dr. Budd by
    Kyle Hayes.

    See LICENSE file.
*/

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

/* must do this before any of the internal includes! */
#define BOOTSTRAP

#include "../vm/err.h"
#include "../vm/globals.h"
#include "../vm/image.h"
#include "../vm/interp.h"
#include "../vm/memory.h"
#include "../vm/version.h"

#ifdef gcalloc
#   undef gcalloc
#endif



static int parseError(char *msg);
//static struct object *gcalloc(int size);
static struct byteObject *binaryAlloc(int size);

static void addGlobalName(char *name, struct object *value);
static struct object *lookupGlobalName(char *name, int ok_missing);

static void inputMethodText();
static void skipSpaces();
static void trimLine();
static int isDigit(char p);
static int isIdentifierChar(char p);
static int isBinary(char p);
static void readBinary();
static int readIdentifier();
static int readInteger();

static int symbolBareCmp(const uint8_t *left, int leftsize,
                         const uint8_t *right, int rightsize);
static int symbolCmp(struct object *left, struct object *right);
static struct object *newString(char *text);
static struct object *newSymbol(char *text);
static struct object *newClass(char *name, int numVars);
static struct object *newNode(struct object *v, struct object *l,
                              struct object *r);
static struct object *newTree(void);
static struct object *newDictionary(void);
static struct object *newArray(int size);
static struct object *newOrderedArray(void);

static void genByte(int v);
static void genVal(int v);
static void genValPos(int pos, int v);
static void genInstruction(int a, int b);
static struct object *buildByteArray();
static int addLiteral(struct object *a);
static struct object *buildLiteralArray(void);
static void addArgument(char *name);
static void addTemporary(char *name);

static int parseInteger(void);
static int parsePrimitive(void);
static int parseString(void);

static int lookupInstance(struct object *class, char *text, int *low);

static int nameTerm(char *name);
static int parseBlock(void);
static int parseSymbol(void);
static int parseChar(void);
static int parseTerm(void);
static int parseUnaryContinuation(void);
static int parseBinaryContinuation(void);
static int optimizeBlock(void);
static int controlFlow(int opt1, char *rest, int opt2);
static int optimizeLoop(int branchInstruction);
static int parseKeywordContinuation(void);
static int doAssignment(char *name);
static int parseExpression(void);
static int parseStatement(void);
static int parseBody(void);
static int parseMethodHeader(struct object *theMethod);
static int parseTemporaries(void);
static int parseMethod(struct object *theMethod);

static struct object *insert(struct object *array, int index,
                             struct object *val);
static void dictionaryInsert(struct object *dict, struct object *index,
                             struct object *value);

static void MethodCommand(void);
static void ClassMethodCommand(void);
static void InstanceMethodCommand(void);
static void MethodCommand(void);
static void ClassCommand(void);

//static int getIntSize(int val);
//static void writeTag(FILE * fp, int type, int val);



//static void objectWrite(FILE * fp, struct object *obj);
static struct object *symbolTreeInsert(struct object *base,
                                       struct object *symNode);
static struct object *fixSymbols(void);
static void fixGlobals(void);
static void checkGlobals(void);

static void bigBang(void);

static void usage(void);






/* ------------------------------------------------------------- */

/*	Globals                                                      */

/* ------------------------------------------------------------- */

static FILE *fin;

#define MAX_BUF (8192)
static char inputBuffer[MAX_BUF];
static int inputLine = 0;
static int inputPosition = 0;
static char *p = NULL;

#define MAX_TOKEN (MAX_BUF)
static char tokenBuffer[MAX_TOKEN];

/* The following are roots for the file out */
struct object *nilObject;
struct object *trueObject;
struct object *falseObject;
struct object *globalValues;
struct object *SmallIntClass;
struct object *ArrayClass;
struct object *BlockClass;
struct object *IntegerClass;
struct object *SymbolClass;

/* used in the Big Bang to set up other objects. */
static struct object *ObjectClass;
static struct object *MetaObjectClass;
static struct object *ClassClass;
static struct object *NilClass;
static struct object *TrueClass;
static struct object *FalseClass;
static struct object *TreeClass;
static struct object *OrderedArrayClass;
static struct object *MetaClassClass;

//static struct object *StringClass;
//static struct object *DictionaryClass;
//static struct object *ByteArrayClass;


static struct object *currentClass;



/* ------------------------------------------------------------- */
/*	main program                                                 */
/* ------------------------------------------------------------- */

int main(int argc, char **argv)
{
    char *image_source = "lst.st";
    char *output_file = "lst.img";
    FILE *fd;
    struct object *specialSymbols = NULL;
    struct object *REPLClass = NULL;
    int num_objs = 0;

    //debugging = 0;

    info("Little Smalltalk bootstrap program starting...");

    for (int i = 1; i < argc; i++) {
        info("\targv[%d]=\"%s\"\n", i, argv[i]);
    }

    /* first parse arguments */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            info("Little Smalltalk, version " VERSION_STRING "\n");
        } else if (strcmp(argv[i], "-g") == 0) {
            debugging = 1;
            info("Turning on debugging.\n");
        } else if (strcmp(argv[i], "-d") == 0) {
            debugging = 1;
            info("Turning on debugging.\n");
        } else if (strcmp(argv[i], "-o") == 0) {
            if(i + 1 >= argc) {
                info("You need to provide an output file if you use the -o option.\n");
                usage();
            }

            output_file = argv[i+1];
            i++;

            info("Output file: \"%s\"\n", output_file);
        } else {
            image_source = argv[i];
            info("Input file: \"%s\"\n", image_source);            
        }
    }

    info("Processing source Little Smalltalk file \"%s\" into output image file \"%s\"\n", image_source, output_file);

    /* big bang -- create the first classes */
    bigBang();

    addArgument("self");

    if ((fin = fopen(image_source, "r")) == NULL) {
        error("Unable to open file \"%s\"!", image_source);
    }

    /* then read the image source file */
    inputLine = 0;
    while (fgets((char *) inputBuffer, sizeof(inputBuffer), fin)) {
        p = inputBuffer;
        skipSpaces();
        inputLine++;
        inputPosition = 0;

        switch(*p) {
            case '+': trimLine(); p++; ClassCommand(); break;
            case '=': trimLine(); p++; ClassMethodCommand(); break;
            case '!': trimLine(); p++; InstanceMethodCommand(); break;
            case 0: /* end of the line */ break;
            case '"': /* nothing to do, get the next line. */ break;
            default:
                error("Character %x ('%c') not a supported action!  Found in line \"%s\" (%d) of input file %s.", *p, *p, inputBuffer, inputLine, image_source);
                break;
        }
    }

    fclose(fin);

    /* set up special symbols */
    info("Setting up special symbols.");
    specialSymbols = newArray(4);
    specialSymbols->data[0] = newSymbol("<");
    specialSymbols->data[1] = newSymbol("<=");
    specialSymbols->data[2] = newSymbol("+");
    specialSymbols->data[3] = newSymbol("doesNotUnderstand:");

    /* add important objects to globals for later lookup. */
    if(!lookupGlobalName("specialSymbols", 1)) {
        addGlobalName("specialSymbols", specialSymbols);
    }

    if(!lookupGlobalName("nil", 1)) {
        addGlobalName("nil", nilObject);
    }

    if(!lookupGlobalName("true", 1)) {
        addGlobalName("true", trueObject);
    }

    if(!lookupGlobalName("false", 1)) {
        addGlobalName("false", falseObject);
    }

    // if((REPLClass = lookupGlobalName("REPL", 1))) {
    //     struct object *startMethod = dictLookup(REPLClass->data[methodsInClass], "start");
    //     addGlobalName("start", startMethod);
    // } else {
    //     error("No REPL class!");
    // }

    info("Fixup symbols.");
    /* then create the tree of symbols */
    SymbolClass->data[symbolsInSymbol] = fixSymbols();

    /* fix up globals. */
    info("Fixup globals.");
    fixGlobals();

    /* see if anything was never defined in the class source */
    info("Check globals.");
    checkGlobals();

    if ((fd = fopen(output_file, "w")) == NULL) {
        error("Unable to open \"%s\" for writing!", output_file);
    }

    num_objs = fileOut_object(fd, globalValues);

    fclose(fd);

    printf("%d objects written to \"%s\".\n", num_objs, output_file);
    printf("Goodbye for now!\n");

    return (0);
}


void usage(void) 
{
    printf(
        "Usage: bootstrap [-v] [-g] [-o outputfile] [inputfile]\n"
        "\t-v\tPrint out the Little Smalltalk version.\n"
        "\t-g\tEnable debugging output.  Warning, this is verbose!\n"
        "\t-o outputfile\tSpecify the output file to use for the final binary image. Default \"lst.img\".\n"
        "\tinputfile\tSpecify the source input file.  Must be valid Little Smalltalk code.  Default \"lst.st\".\n"
        "All arguments are optional.\n"
    );
}

/* ------------------------------------------------------------- */
/*	big bang                                                     */
/* ------------------------------------------------------------- */

void bigBang(void)
{

    /*
     * First, make the nil (undefined) object;
     * notice its class is wrong
     */
    nilObject = gcalloc(0);

    /*
     * Second, make classes for Symbol and Dictionary.
     * this will allow newClass to work correctly
     */

    SymbolClass = gcalloc(ClassSize + 1);
    addGlobalName("Symbol", SymbolClass);
    SymbolClass->data[nameInClass] = newSymbol("Symbol");

    DictionaryClass = gcalloc(ClassSize);
    addGlobalName("Dictionary", DictionaryClass);
    DictionaryClass->data[nameInClass] = newSymbol("Dictionary");

    /* now we can fix up nil's class */
    NilClass = newClass("Undefined", 0);
    addGlobalName("Undefined", NilClass);
    nilObject->class = NilClass;
    addGlobalName("nil", nilObject);

    /* make up the object / metaobject mess */
    ObjectClass = newClass("Object", 0);
    addGlobalName("Object", ObjectClass);
    MetaObjectClass = newClass("MetaObject", 0);
    addGlobalName("MetaObject", MetaObjectClass);
    ObjectClass->class = MetaObjectClass;
    ObjectClass->data[parentClassInClass] = nilObject;

    /* And the Class/MetaClass mess */
    ClassClass = newClass("Class", 0);
    addGlobalName("Class", ClassClass);
    MetaClassClass = newClass("MetaClass", 0);
    addGlobalName("MetaClass", MetaClassClass);
    ClassClass->class = MetaClassClass;

    /* one more tweak. This is needed to stop lookups. */
    MetaObjectClass->data[parentClassInClass] = ClassClass;

    /* now make up a bunch of other classes */
    BlockClass = newClass("Block", 0);
    addGlobalName("Block", BlockClass);

    /* SmallInt has an extra class variable, just like Symbol */
    SmallIntClass = newClass("SmallInt", 1);
    addGlobalName("SmallInt", SmallIntClass);

    IntegerClass = newClass("Integer", 0);
    addGlobalName("Integer", IntegerClass);

    TrueClass = newClass("True", 0);
    addGlobalName("True", TrueClass);
    trueObject = gcalloc(0);
    trueObject->class = TrueClass;
    addGlobalName("true", trueObject);

    FalseClass = newClass("False", 0);
    addGlobalName("False", FalseClass);
    falseObject = gcalloc(0);
    falseObject->class = FalseClass;
    addGlobalName("false", falseObject);

    ArrayClass = newClass("Array", 0);
    addGlobalName("Array", ArrayClass);

    ByteArrayClass = newClass("ByteArray", 0);
    addGlobalName("ByteArray", ByteArrayClass);

    OrderedArrayClass = newClass("OrderedArray", 0);
    addGlobalName("OrderedArray", OrderedArrayClass);

    StringClass = newClass("String", 0);
    addGlobalName("String", StringClass);

    TreeClass = newClass("Tree", 0);
    addGlobalName("Tree", TreeClass);

    /* finally, we can fill in the fields in class Object */
    ObjectClass->data[methodsInClass] = newDictionary();
    ObjectClass->data[instanceSizeInClass] = newInteger(0);
    ClassClass->data[instanceSizeInClass] = newInteger(0);

    /* can make global name, but can't fill it in */
    globalValues = gcalloc(2);
    addGlobalName("globals", globalValues);
}

/* ------------------------------------------------------------- */
/*	Errors                                                       */
/* ------------------------------------------------------------- */
int parseError(char *msg)
{
    info("Parse Error: at line %d, position %d, %s", inputLine, inputPosition, msg);
    info("\"%s\"", inputBuffer);

    /* print a pointer to the location that we found something wrong. */
    for(int i=0; i < (inputPosition-1); i++) {
        fprintf(stderr, "-");
    }

    fprintf(stderr, "^\n");

    fflush(stderr);

    exit(1);
    return 0;
}



/* ------------------------------------------------------------- */
/*	Memory                                                       */
/* ------------------------------------------------------------- */

#ifdef gcalloc
#   undef gcalloc
#endif

struct object *gcalloc(int size)
{
    struct object *result;
    size_t obj_size = sizeof(struct object) + ((sizeof(struct object *) * (size_t)size));

    result = calloc(1, obj_size);
    if (!result) {
        error("gcalloc(): out of memory!");
    }

    SET_SIZE(result, size);

    while (size > 0) {
        result->data[--size] = nilObject;
    }

    return result;
}

struct byteObject *binaryAlloc(int size)
{
    int osize;
    struct byteObject *result;

    osize = TO_WORDS(size);
    result = (struct byteObject *) gcalloc(osize);
    SET_SIZE(result, size);
    SET_BINOBJ(result);
    return result;
}


/* ------------------------------------------------------------- */
/*	Names                                                        */
/* ------------------------------------------------------------- */

#define MAX_GLOBALS (500)
static int globalTop = 0;
static char *globalNames[MAX_GLOBALS];
static struct object *globals[MAX_GLOBALS];

void addGlobalName(char *name, struct object *value)
{
    char *newName;

    /* check for duplicates. */
    for(int i=0; i < globalTop; i++) {
        if(strcmp(globalNames[i], name) == 0) {
            info("Name \"%s\" already exists!", name);
        }
    }

    newName = strdup(name);
    if (!newName) {
        error("addGlobalName(): out of memory!");
    }
    globalNames[globalTop] = newName;
    globals[globalTop] = value;
    globalTop++;

    if(globalTop >= MAX_GLOBALS) {
        error("addGlobalName(): too many globals! globalTop > %d!", MAX_GLOBALS);
    }
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
        error("lookupGlobalName(): Missing global %s!", name);
    }
    return 0;
}

/* ------------------------------------------------------------- */
/*	Lexical Analysis  */
/* ------------------------------------------------------------- */

void inputMethodText()
{
    int c;
    bool foundExclamation = false;

    p = inputBuffer;
    inputPosition = 0;

    while ((ptrdiff_t)(p - inputBuffer) < (ptrdiff_t)(sizeof(inputBuffer))) {
        c = fgetc(fin);

        if(c == EOF) {
            error("Unexpected EOF while reading method text at line %d!", inputLine);
        }

        if(c == '\n') {
            inputPosition = 0;
            inputLine++;
        } else {
            inputPosition++;
        }

        *p++ = (char)c;

        /* only check if this is the first character on a line. */
        if(foundExclamation) {
            if(inputPosition == 0 && c == '\n') {
                *(p-2) = (char)0;
                return;
            } else {
                foundExclamation = false;
            }
        } else if(inputPosition == 1 && c == '!') {
            foundExclamation = true;
        }
    }
}

void skipSpaces()
{
    while ((*p == ' ') || (*p == '\t') || (*p == '\n'))
        p++; inputPosition++;
    if (*p == '\"') {
        p++; inputPosition++;
        while (*p && (*p != '\"'))
            p++; inputPosition++;
        if (*p != '\"')
            parseError("unterminated comment");
        p++; inputPosition++;
        skipSpaces();
    }
}

/* remove whitespace at the end of the line too. */
void trimLine()
{
    int index = (int)strlen(inputBuffer) - 1;

    while(index > 0) {
        if((inputBuffer[index] == ' ') || (inputBuffer[index] == '\t') || (inputBuffer[index] == '\n')) {
            inputBuffer[index--] = 0;
        } else {
            break;
        }
    }
}

int isDigit(char p)
{
    if ((p >= '0') && (p <= '9'))
        return 1;
    return 0;
}

int isIdentifierChar(char p)
{
    if ((p >= 'a') && (p <= 'z'))
        return 1;
    if ((p >= 'A') && (p <= 'Z'))
        return 1;
    return 0;
}

int isBinary(char p)
{
    switch (p) {
    case '+':
    case '*':
    case '-':
    case '/':
    case '<':
    case '=':
    case '>':
    case '@':
    case '~':
    case ',':
        return 1;
    }
    return 0;
}

void readBinary()
{
    tokenBuffer[0] = *p++;
    if (isBinary(*p)) {
        tokenBuffer[1] = *p++;
        tokenBuffer[2] = '\0';
    } else {
        tokenBuffer[1] = '\0';
    }
    skipSpaces();
}

int readIdentifier()
{
    int keyflag;
    char *q = tokenBuffer;

    while ((q < (tokenBuffer + MAX_TOKEN)) && (isIdentifierChar(*p) || isDigit(*p))) {
        *q++ = *p++;
    }

    if(q >= (tokenBuffer + MAX_TOKEN)) {
        error("readIdentifier(): token too long!");
    }

    *q = '\0';
    if (*p == ':') {		/* It's a keyword identifier */
        keyflag = 1;
        *q++ = ':';
        *q = '\0';
        p++;
    } else {
        keyflag = 0;
    }
    skipSpaces();
    return keyflag;
}

int readInteger()
{
    int val, neg = 0;

    if (*p == '-') {
        neg = 1;
        ++p;
    }
    val = *p++ - '0';
    while (isDigit(*p)) {
        val = 10 * val + (*p++ - '0');
    }
    skipSpaces();
    return neg ? -val : val;

}

/* ------------------------------------------------------------- */
/*	new instances of standard things                             */
/* ------------------------------------------------------------- */

static int symbolTop = 0;
static struct object *oldSymbols[5000];

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

int symbolCmp(struct object *left, struct object *right)
{
    return symbolBareCmp(bytePtr(left), SIZE(left), bytePtr(right), SIZE(right));
}

struct object *newString(char *text)
{
    size_t size, i;
    struct byteObject *newObj;

    size = strlen(text);
    newObj = binaryAlloc((int)size);
    for (i = 0; i < size; i++) {
        newObj->bytes[i] = (uint8_t)text[i];
    }
    newObj->class = lookupGlobalName("String", 0);
    return (struct object *) newObj;
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
    result = binaryAlloc((int)strlen(text));
    for (i = 0; i < (int)strlen(text); i++) {
        result->bytes[i] = (uint8_t)text[i];
    }
    result->class = lookupGlobalName("Symbol", 0);
    oldSymbols[symbolTop++] = (struct object *) result;
    return (struct object *) result;
}

struct object *newClass(char *name, int numVars)
{
    struct object *newC;

    newC = gcalloc(ClassSize + numVars);
    newC->data[nameInClass] = newSymbol(name);

    return newC;
}

struct object *newNode(struct object *v, struct object *l, struct object *r)
{
    struct object *result;

    result = gcalloc(3);
    result->class = lookupGlobalName("Node", 0);
    result->data[0] = v;
    result->data[1] = l;
    result->data[2] = r;
    return result;
}

struct object *newTree(void)
{
    struct object *result;

    result = gcalloc(1);
    result->class = lookupGlobalName("Tree", 0);
    return result;
}

struct object *newDictionary(void)
{
    struct object *result;

    result = gcalloc(2);
    result->class = lookupGlobalName("Dictionary", 0);
    result->data[0] = newOrderedArray();
    result->data[1] = newArray(0);
    return result;
}

/*
 * newArray()
 *	Allocate a new array
 *
 * All slots are initialized to nil
 */
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

/*
 * newOrderedArray()
 *	Return a new, empty ordered array
 */
struct object *newOrderedArray(void)
{
    struct object *result;

    result = gcalloc(0);
    result->class = lookupGlobalName("OrderedArray", 0);
    return (result);
}


/* ------------------------------------------------------------- */

/*	Code Generation   */

/* ------------------------------------------------------------- */

#define ByteBufferTop (2048)
static unsigned char byteBuffer[ByteBufferTop];
static int byteTop;

void genByte(int v)
{
    byteBuffer[byteTop++] = (uint8_t)v;
    if (byteTop >= ByteBufferTop) {
        error("too many bytecodes");
    }
}

void genVal(int v)
{
    if ((v < 0) || (v > 0xFFFF)) {
        error("genVal(): illegal value %d!", v);
    }
    genByte(v & 0xFF);
    genByte(v >> 8);
}

void genValPos(int pos, int v)
{
    if ((v < 0) || (v > 0xFFFF)) {
        error("genValPos() illegal value %d!", v);
    }
    byteBuffer[pos] = (uint8_t)(v & 0xFF);
    byteBuffer[pos + 1] = (uint8_t)(v >> 8);
}

void genInstruction(int a, int b)
{
    /*printf("gen instruction %d %d\n", a, b); */
    if (b < 16) {
        genByte(a * 16 + b);
    } else {
        genInstruction(0, a);
        genByte(b);
    }
}

struct object *buildByteArray()
{
    struct byteObject *newObj;
    int i;

    newObj = binaryAlloc((int)byteTop);
    for (i = 0; i < (int)byteTop; i++) {
        newObj->bytes[i] = byteBuffer[i];
    }
    newObj->class = lookupGlobalName("ByteArray", 0);
    return (struct object *) newObj;
}

#define MAX_LITERALS (1000)
static struct object *litBuffer[MAX_LITERALS];
static int litTop = 0;

/* FIXME - remove duplicates. */
int addLiteral(struct object *a)
{
    litBuffer[litTop++] = a;

    if (litTop >= MAX_LITERALS) {
        error("too many literals");
    }

    return (int)(litTop - 1);
}

struct object *buildLiteralArray(void)
{
    int i;
    struct object *result;

    if (litTop == 0)
        return nilObject;
    result = gcalloc((int)litTop);
    result->class = lookupGlobalName("Array", 0);
    for (i = 0; i < (int)litTop; i++)
        result->data[i] = litBuffer[i];
    return result;
}

#define ArgumentBufferTop (64)
static char *argumentNames[ArgumentBufferTop];
static int argumentTop;

void addArgument(char *name)
{
    char *p;

    p = strdup(name);
    if (!p) {
        error("addArgument(): malloc failure");
    }
    argumentNames[argumentTop++] = p;

    if(argumentTop >= ArgumentBufferTop) {
        error("addArgument(): too many arguments!");
    }
}

#define TempBufferTop (500)
static char *tempBuffer[TempBufferTop];
static int tempTop, maxTemp;

void addTemporary(char *name)
{
    char *p;

    p = strdup(name);
    if (!p) {
        error("addTemporary(): malloc failure!");
    }

    tempBuffer[tempTop++] = p;

    if(tempTop >= TempBufferTop) {
        error("addTemporary(): too many temporaries!");
    }

    if (tempTop > maxTemp) {
        maxTemp = tempTop;
    }
}


/* ------------------------------------------------------------- */

/*	Parsing */

/* ------------------------------------------------------------- */

int parseInteger(void)
{
    int i;

    i = readInteger();
    if ((i >= 0) && (i < 10)) {
        genInstruction(PushConstant, i);
    } else {
        genInstruction(PushLiteral, addLiteral(newInteger(i)));
    }
    return 1;
}

int parsePrimitive(void)
{
    int primitiveNumber, argumentCount;

    /* skip over the left bracket */
    p++;
    skipSpaces();

    /* then read the primitive number */
    if (isDigit(*p)) {
        primitiveNumber = readInteger();
    } else {
        return parseError("missing primitive number");
    }

    /* then read the arguments */
    for (argumentCount = 0; *p && (*p != '>'); argumentCount++) {
        if (!parseTerm()) {
            return 0;
        }
    }

    /* make sure we ended correctly */
    if (*p == '>') {
        p++;
        skipSpaces();
    } else {
        return parseError("missing > at end of primitive");
    }

    /* generate instructions */
    genInstruction(DoPrimitive, argumentCount);
    genByte(primitiveNumber);

    /* Success */
    return (1);
}

int parseString(void)
{
    char *q;

    p++;
    for (q = tokenBuffer; q < (tokenBuffer + MAX_TOKEN) && *p && *p != '\'';) {
        *q++ = *p++;
    }

    if(q >= (tokenBuffer + MAX_TOKEN)) {
        error("parseString(): String too long for token buffer!");
    }

    if (*p != '\'')
        return parseError("missing end of string");
    p++;
    skipSpaces();
    *q = '\0';
    genInstruction(PushLiteral, addLiteral(newString(tokenBuffer)));
    return 1;
}

int lookupInstance(struct object *class, char *varName, int *low)
{
    int offset = 0;

    /* 
     * Looks up the instance var name in the current class.  If not found, walk
     * up the class chain.  Once found, determine the offset due to the sizes
     * of any parent classes and return the absolute index of the instance 
     * variable.
     */

    for(struct object *varClass = class; varClass && varClass != nilObject; varClass = varClass->data[parentClassInClass]) {
        struct object *instVars = varClass->data[variablesInClass];
        if(instVars && instVars != nilObject) {
            int numVars = SIZE(instVars);

            for(int varIndex = 0; varIndex < numVars; varIndex++) {
                if(symbolBareCmp((uint8_t*)varName, (int)strlen(varName), bytePtr(instVars->data[varIndex]), SIZE(instVars->data[varIndex])) == 0) {
                    /* found it. Now get the size of the parent class(es) */
                    for(struct object *parentClass = varClass->data[parentClassInClass]; parentClass && parentClass != nilObject; parentClass = parentClass->data[parentClassInClass]) {
                        struct object *parentVars = parentClass->data[variablesInClass];

                        if(parentVars && parentVars != nilObject) {
                            offset += SIZE(parentVars);
                        }
                    }

                    *low = offset + varIndex;

                    info("lookupInstance: found var %s in class %.*s at offset %d.", varName, SIZE(varClass->data[nameInClass]), bytePtr(varClass->data[nameInClass]), *low);

                    return *low;
                }
            }
        } 

    }

    /* not found */

    // printf("lookupInstance: var %s not found in class %.*s.\n", varName, SIZE(class->data[nameInClass]), bytePtr(class->data[nameInClass]));

    return -1;

    // int size, i;
    
    // /* first check superclasses */
    // struct object *parent = class->data[parentClassInClass];
    // if (parent && parent != nilObject) {
    //     size = lookupInstance(parent, varName, low);
    //     if (size >= 0)
    //         return size;
    // } else {			/* no superclass */
    //     *low = 0;
    // }
    
    // /* Check our own list of variables */
    // struct object *var = class->data[variablesInClass];
    // if (var && var != nilObject) {
    //     size = (int)SIZE(var);
    // } else {
    //     size = 0;
    // }
    
    // for (i = 0; i < size; i++) {
    //     if (symbolBareCmp((uint8_t *)varName, (int)strlen(varName), bytePtr(var->data[i]), SIZE(var->data[i])) == 0) {

    //         info("lookupInstance: found var %s in class %.*s at offset %d.", varName, SIZE(class->data[nameInClass]), bytePtr(class->data[nameInClass]), *low);

    //         return (*low);
    //     }
    //     *low += 1;
    // }

    // //printf("lookupInstance: var %s not found in class %.*s.\n", varName, SIZE(class->data[nameInClass]), bytePtr(class->data[nameInClass]));

    // return (-1);
}

static int superMessage = 0;


int nameTerm(char *name)
{
    static char *lowConstants[4] = { "nil", "true", "false", 0 };
    int i;

    /* see if temporary */
    for (i = 0; i < tempTop; i++) {
        if (strcmp(name, tempBuffer[i]) == 0) {
            genInstruction(PushTemporary, i);
            return 1;
        }
    }

    /* see if argument */
    for (i = 0; i < argumentTop; i++) {
        if (strcmp(name, argumentNames[i]) == 0) {
            genInstruction(PushArgument, i);
            return 1;
        }
    }

    /* see if super */
    if (strcmp(name, "super") == 0) {
        genInstruction(PushArgument, 0);
        //info("setting super message\n");
        superMessage = 1;
        return 1;
    }

    /* see if low constant */
    for (i = 0; lowConstants[i]; i++) {
        if (strcmp(lowConstants[i], name) == 0) {
            genInstruction(PushConstant, 10 + i);
            return 1;
        }
    }

    /* see if instance variable */
    if (currentClass) {
        int low;

        info("Looking up possible instance variable \"%s\" in class \"%*s\".", name, SIZE(currentClass->data[nameInClass]), bytePtr(currentClass->data[nameInClass]));

        i = lookupInstance(currentClass, name, &low);
        if (i >= 0) {
            genInstruction(PushInstance, i);
            return 1;
        }

        info("\"%s\" is not an instance variable in this class.");
    } else {
        info("No currentClass while finding \"%s\".", name);
    }

    /* see if global */
    {
        struct object *glob = lookupGlobalName(name, 1);

        if (glob) {
            genInstruction(PushLiteral, addLiteral(glob));
            return 1;
        }
    }

    error("unknown identifier %s at line %d, with buffer %s!", name, inputLine, inputBuffer);

    return 0;
}

static int returnOp;
static char *blockbackup;

int parseBlock(void)
{
    int savedLocation, saveTop, argCount;
    char *savestart;

    savestart = p;
    p++;
    skipSpaces();
    genInstruction(PushBlock, tempTop);
    savedLocation = byteTop;
    genVal(0);

    saveTop = tempTop;
    argCount = 0;
    if (*p == ':') {
        while (1) {
            p++;
            skipSpaces();
            if (!isIdentifierChar(*p))
                return parseError("missing identifier");
            if (readIdentifier())
                return parseError("keyword illegal");
            addTemporary(tokenBuffer);
            argCount++;
            if (*p == '|')
                break;
            if (*p != ':')
                return parseError("missing colon:");
        }
        p++;
        skipSpaces();
    }
    if (*p == ']') {
        genInstruction(PushConstant, nilConst);
    } else {
        int saveReturnOp = returnOp;

        returnOp = BlockReturn;
        while (1) {
            if (!parseStatement()) {
                parseError("Statement syntax inside block");
            }
            if (*p == '.') {
                p++;
                skipSpaces();
            }
            if (*p == ']') {
                break;
            } else {
                genInstruction(DoSpecial, PopTop);
            }
        }
        returnOp = saveReturnOp;
    }
    p++;
    skipSpaces();		/* skip over ] */
    genInstruction(DoSpecial, StackReturn);
    genValPos(savedLocation, byteTop);
    tempTop = saveTop;

    /* set blockbackup to textual start of block */
    blockbackup = savestart;
    return 1;
}

static int parseSymbol(void);
static int parseChar(void);


int parseSymbol(void)
{
    char *q;

    p++;
    for (q = tokenBuffer; isIdentifierChar(*p) || (*p == ':');)
        *q++ = *p++;
    *q = '\0';
    skipSpaces();
    genInstruction(PushLiteral, addLiteral(newSymbol(tokenBuffer)));
    return 1;
}

int parseChar(void)
{
    struct object *newObj;

    p++;
    newObj = gcalloc(1);
    newObj->class = lookupGlobalName("Char", 0);
    newObj->data[0] = newInteger((int) *p);
    genInstruction(PushLiteral, addLiteral(newObj));
    p++;
    skipSpaces();
    return 1;
}

int parseTerm(void)
{
    /* make it so anything other than a block zeros out backup var */
    blockbackup = 0;
    superMessage = 0;

    if (*p == '(') {
        p++;
        skipSpaces();
        if (!parseExpression())
            return 0;
        if (*p != ')')
            return parseError("unbalanced parenthesis");
        p++;
        skipSpaces();
        return 1;
    }
    if (*p == '<')
        return parsePrimitive();
    if (*p == '$')
        return parseChar();
    if (isDigit(*p) || (*p == '-'))
        return parseInteger();
    if (*p == '\'')
        return parseString();
    if (isIdentifierChar(*p)) {
        readIdentifier();
        return nameTerm(tokenBuffer);
    }
    if (*p == '[')
        return parseBlock();
    if (*p == '#')
        return parseSymbol();

    info("Illegal start of expression '%c' (%x) at line %d.", *p, *p, inputLine);
    info("Parsed source:\n\"%s\"", inputBuffer);
    error("Remaining source to parse:\n\"%s\"", p);

    return parseError("illegal start of expression");
}



int parseUnaryContinuation(void)
{
    static char *unaryBuiltIns[] = { "isNil", "notNil", 0 };
    int litNumber, done;
    char *q;

    while (isIdentifierChar(*p)) {
        q = p;
        if (readIdentifier()) {
            p = q;		/* oops, was a keyword */
            break;
        }
        done = 0;
        if (!superMessage) {
            int i;

            for (i = 0; unaryBuiltIns[i]; i++)
                if (strcmp(tokenBuffer, unaryBuiltIns[i]) == 0) {
                    genInstruction(SendUnary, i);
                    done = 1;
                }
        }
        if (!done) {
            genInstruction(MarkArguments, 1);
            litNumber = addLiteral(newSymbol(tokenBuffer));
            /*printf("unary %s\n", tokenBuffer); */
            if (superMessage) {
                genInstruction(DoSpecial, SendToSuper);
                genByte(litNumber);
            } else
                genInstruction(SendMessage, litNumber);
            superMessage = 0;
        }
    }
    return 1;
}

static char *binaryBuiltIns[] = { "<", "<=", "+", 0 };

int parseBinaryContinuation(void)
{
    int messLiteral, i, done;
    char messbuffer[80];

    if (!parseUnaryContinuation())
        return 0;
    while (isBinary(*p)) {
        readBinary();
        /*printf("binary symbol %s\n", tokenBuffer); */
        strcpy(messbuffer, tokenBuffer);
        if (!parseTerm())
            return 0;
        if (!parseUnaryContinuation())
            return 0;

        done = 0;
        if (!superMessage)
            for (i = 0; binaryBuiltIns[i]; i++)
                if (strcmp(messbuffer, binaryBuiltIns[i]) == 0) {
                    genInstruction(SendBinary, i);
                    done = 1;
                }

        if (!done) {
            messLiteral = addLiteral(newSymbol(messbuffer));
            genInstruction(MarkArguments, 2);
            if (superMessage) {
                genInstruction(DoSpecial, SendToSuper);
                genByte(messLiteral);
                superMessage = 0;
            } else
                genInstruction(SendMessage, messLiteral);
        }
    }
    return 1;
}

int optimizeBlock(void)
{
    if (*p != '[') {
        if (!parseTerm())
            return 0;
        parseError("missing block as optimized block argument");
    } else {
        p++;
        skipSpaces();
        if (*p == ']') {
            genInstruction(PushConstant, 0);
            p++;
            skipSpaces();
            return 1;
        }
        while (1) {
            if (!parseStatement())
                return 0;
            if (*p == '.')
                p++, skipSpaces();
            if (*p == ']')
                break;
            genInstruction(DoSpecial, PopTop);
        }
        p++;
        skipSpaces();
        /* just leave last expression on stack */
    }
    return 1;
}

int controlFlow(int opt1, char *rest, int opt2)
{
    int save1, save2;
    char *q;

    genInstruction(DoSpecial, opt1);
    save1 = (int)byteTop;
    genVal(0);
    if (!optimizeBlock()) {
        parseError("syntax error in control flow");
    }
    genInstruction(DoSpecial, Branch);
    save2 = (int)byteTop;
    genVal(0);
    genValPos(save1, (int)byteTop);
    q = p;
    if (isIdentifierChar(*p) && readIdentifier()
        && (strcmp(tokenBuffer, rest) == 0)) {
        if (!optimizeBlock()) {
            parseError("syntax error in control cascade");
        }
    } else {
        p = q;
        genInstruction(PushConstant, opt2);
    }
    genValPos(save2, (int)byteTop);
    return 1;
}

int optimizeLoop(int branchInstruction)
{
    int L1, L2;

    /* back up to start of block and try again */
    p = blockbackup;
    L1 = (int)byteTop;
    optimizeBlock();
    genInstruction(DoSpecial, branchInstruction);
    L2 = (int)byteTop;
    genVal(0);
    if (!(isIdentifierChar(*p) && readIdentifier()))
        return parseError("can't get message again in optimized block");
    /* now read the body */
    optimizeBlock();
    genInstruction(DoSpecial, PopTop);
    genInstruction(DoSpecial, Branch);
    genVal(L1);
    genValPos(L2, (int)byteTop);
    genInstruction(PushConstant, 0);
    return 1;
}


int parseKeywordContinuation(void)
{
    int argCount, i, done, saveSuper;
    char messageBuffer[100];

    saveSuper = superMessage;
    if (!parseBinaryContinuation())
        return 0;
    strcpy(messageBuffer, "");
    argCount = 0;
    if (isIdentifierChar(*p) && readIdentifier()) {
        if (strcmp(tokenBuffer, "ifTrue:") == 0)
            return controlFlow(BranchIfFalse, "ifFalse:", nilConst);
        else if (strcmp(tokenBuffer, "ifFalse:") == 0)
            return controlFlow(BranchIfTrue, "ifTrue:", nilConst);
        else if (strcmp(tokenBuffer, "and:") == 0)
            return controlFlow(BranchIfFalse, "", falseConst);
        else if (strcmp(tokenBuffer, "or:") == 0)
            return controlFlow(BranchIfTrue, "", trueConst);
        else if ((strcmp(tokenBuffer, "whileTrue:") == 0) && blockbackup)
            return optimizeLoop(BranchIfFalse);
        else if ((strcmp(tokenBuffer, "whileFalse:") == 0) && blockbackup)
            return optimizeLoop(BranchIfTrue);
        else
            do {
                strcat(messageBuffer, tokenBuffer);
                argCount++;
                if (!parseTerm())
                    return 0;
                if (!parseBinaryContinuation())
                    return 0;
            } while (isIdentifierChar(*p) && readIdentifier());
    }
    if (argCount > 0) {
        /*printf("keywork message %s\n", messageBuffer); */
        done = 0;
        if (!saveSuper)
            for (i = 0; binaryBuiltIns[i]; i++)
                if (strcmp(messageBuffer, binaryBuiltIns[i]) == 0) {
                    genInstruction(SendBinary, i);
                    done = 1;
                }
        if (!done) {
            genInstruction(MarkArguments, argCount + 1);
            if (saveSuper) {
                genInstruction(DoSpecial, SendToSuper);
                genByte(addLiteral(newSymbol(messageBuffer)));
                superMessage = 0;
            } else
                genInstruction(SendMessage,
                               addLiteral(newSymbol(messageBuffer)));
        }
    }
    return 1;
}

int doAssignment(char *name)
{
    int i;

    for (i = 0; i < tempTop; i++) {
        if (strcmp(name, tempBuffer[i]) == 0) {
            genInstruction(AssignTemporary, i);
            return 1;
        }
    }

    if (currentClass) {
        int low;

        i = lookupInstance(currentClass, name, &low);
        if (i >= 0) {
            genInstruction(AssignInstance, i);
            return 1;
        }
    }

    error("unknown target of assignment");
    
    /* error does not return. */
    return 0;
}

int parseExpression(void)
{
    char nameBuffer[60];

    if (isIdentifierChar(*p)) {
        readIdentifier();
        if ((*p == '<') && (*(p + 1) == '-')) {
            p++;
            p++;
            skipSpaces();
            strcpy(nameBuffer, tokenBuffer);
            if (!parseExpression())
                return 0;
            return doAssignment(nameBuffer);
        }
        if (!nameTerm(tokenBuffer))
            return 0;
    } else {
        if (!parseTerm()) {
            return 0;
        }
    }
    if (!parseKeywordContinuation()) {
        return 0;
    }
    while (*p == ';') {
        p++;
        skipSpaces();
        genInstruction(DoSpecial, Duplicate);
        if (!parseKeywordContinuation())
            return 0;
    }
    return 1;
}

int parseStatement(void)
{
    if (*p == '^') {		/* return statement */
        p++;
        skipSpaces();
        if (!parseExpression())
            return 0;
        genInstruction(DoSpecial, returnOp);
        return 1;
    }
    /* otherwise just an expression */
    if (!parseExpression())
        return 0;
    return 1;
}

int parseBody(void)
{
    returnOp = StackReturn;
    while (*p) {
        if (!parseStatement())
            return 0;
        genInstruction(DoSpecial, PopTop);
        if (*p == '.') {
            p++;
            skipSpaces();
        }
    }
    genInstruction(DoSpecial, SelfReturn);
    return 1;
}

int parseMethodHeader(struct object *theMethod)
{
    char messageBuffer[100], *q;
    int keyflag;

    if (isIdentifierChar(*p)) {
        if (readIdentifier()) {	/* keyword message */
            strcpy(messageBuffer, "");
            keyflag = 1;
            while (keyflag) {
                strcat(messageBuffer, tokenBuffer);
                if (isIdentifierChar(*p) && !readIdentifier())
                    addArgument(tokenBuffer);
                else
                    return parseError("missing argument after keyword");
                q = p;
                if (isIdentifierChar(*p) && readIdentifier())
                    keyflag = 1;
                else {
                    p = q;
                    keyflag = 0;
                }
            }
        } else
            strcpy(messageBuffer, tokenBuffer);	/* unary message */
    } else if (isBinary(*p)) {	/* binary message */
        readBinary();
        strcpy(messageBuffer, tokenBuffer);
        if (!isIdentifierChar(*p))
            return parseError("missing argument");
        readIdentifier();
        addArgument(tokenBuffer);
    } else
        return parseError("ill formed method header");
    theMethod->data[nameInMethod] = newSymbol(messageBuffer);
    info("Method %s", messageBuffer);
    return 1;
}

int parseTemporaries(void)
{
    tempTop = 0;
    maxTemp = 0;
    if (*p != '|')
        return 1;
    p++;
    skipSpaces();
    while (*p != '|') {
        if (!isIdentifierChar(*p))
            return parseError("need identifier");
        if (readIdentifier())
            return parseError("keyword illegal");
        addTemporary(tokenBuffer);
    }
    p++;
    skipSpaces();
    return 1;
}

int parseMethod(struct object *theMethod)
{
    if (!parseMethodHeader(theMethod))
        return 0;
    if (!parseTemporaries())
        return 0;
    if (parseBody()) {
        theMethod->data[literalsInMethod] = buildLiteralArray();
        theMethod->data[byteCodesInMethod] = buildByteArray();
        theMethod->data[stackSizeInMethod] = newInteger(19);
        theMethod->data[temporarySizeInMethod] = newInteger(maxTemp);
        theMethod->data[classInMethod] = currentClass;
        theMethod->data[textInMethod] = newString(inputBuffer);
        return 1;
    }
    return 0;
}


/* ------------------------------------------------------------- */
/*	Input Processing   */
/* ------------------------------------------------------------- */

/*
 * insert()
 *	Insert an element in the array at the given position
 *
 * Creates a new Array-ish object of the same class as "array",
 * and returns it filled in as requested.
 */
struct object *insert(struct object *array, int index, struct object *val)
{
    int i, j;
    struct object *o;

    /*
     * Clone the current object, including class.  Make one
     * extra slot in the Array storage.
     */
    o = gcalloc(SIZE(array) + 1);
    o->class = array->class;

    /*
     * Copy up to the index
     */
    for (i = 0; i < index; ++i) {
        o->data[i] = array->data[i];
    }

    /*
     * Put in the new element at this position
     */
    j = i;
    o->data[i++] = val;

    /*
     * Now copy the rest
     */
    for (; j < (int)SIZE(array); ++j) {
        o->data[i++] = array->data[j];
    }
    return (o);
}

/*
 * dictionaryInsert()
 *	Insert a key/value pair into the Dictionary
 */

void dictionaryInsert(struct object *dict, struct object *index,
                      struct object *value)
{
    struct object *keys = dict->data[keysInDictionary], *vals = dict->data[valuesInDictionary];
    int i, lim, res;

    /*
     * Scan the OrderedArray "keys" to find where we fit in
     */
    for (i = 0, lim = SIZE(keys); i < lim; ++i) {
        res = symbolCmp(index, keys->data[i]);

        /*
         * We should go in before this node
         */
        if (res < 0) {
            dict->data[0] = insert(keys, i, index);
            dict->data[1] = insert(vals, i, value);
            return;
        } else if (res > 0) {
            continue;
        } else {
            error("dictionaryInsert(): duplicate key");
        }
    }

    /*
     * The new element goes at the end
     */
    dict->data[0] = insert(keys, i, index);
    dict->data[1] = insert(vals, i, value);
}


void ClassMethodCommand(void)
{
    /* read class name */
    readIdentifier();

    currentClass = lookupGlobalName(tokenBuffer, 1);
    if (!currentClass) {
        error("ClassMethodCommand(): unknown class in Method %s!", tokenBuffer);
    }

    /* get the class of the class */
    currentClass = currentClass->class;
    if (!currentClass) {
        error("ClassMethodCommand(): unknown superclass in Method %s!", tokenBuffer);
    }

    MethodCommand();
}


void InstanceMethodCommand(void)
{
    /* read class name */
    readIdentifier();

    currentClass = lookupGlobalName(tokenBuffer, 1);
    if (!currentClass) {
        error("InstanceMethodCommand(): unknown class in Method %s!", tokenBuffer);
    }

    MethodCommand();
}



void MethodCommand(void)
{
    struct object *theMethod;

    inputMethodText();

    p = inputBuffer;
    skipSpaces();

    theMethod = gcalloc(methodSize);
    theMethod->class = lookupGlobalName("Method", 0);

    /* fill in method class */
    byteTop = 0;
    litTop = 0;
    argumentTop = 1;

    /*
     * If successful compile, insert into the method dictionary
     */

    info("parsing method for %.*s.", SIZE(currentClass->data[nameInClass]), (char *)bytePtr(currentClass->data[nameInClass]));

    if (parseMethod(theMethod)) {
        dictionaryInsert(currentClass->data[methodsInClass], theMethod->data[nameInMethod], theMethod);
    } else {
        info("parseMethod() failed!");
    }
}


/*
 * ClassCommand()
 *	Build the base and meta classes automatically
 *
 * Doesn't support class variables, but handles most of imageSource
 * cases.
 *
 * Must handle lines like:
 *
 * +nil subclass: #Object variables: #( ) classVariables: #( )
 *
 * +Magnitude subclass: #Association variables: #( key value ) classVariables: #( )
 *
 * +Number subclass: #SmallInt variables: #( ) classVariables: #( seed )
 *
 * A line like below generates the diagram below.
 *
 * SuperClass subclass: #NewClass variables: #( instVars ) classVariables: #( classVars )
 *
 *                       MetaSuperClass
 *                       +-------------+                Object
 *      Class            |             |                   ^
 *        ^              +-------------+                   .
 *        |                     ^    ^      SuperClass     .
 *        |    MetaNewClass     |    |   +-------------+   .
 *        |   +-------------+   |    +---+----class    |   |
 *        +---+-- class     |   |        | parentClass-+---+
 *            |    name     |   |        +-------------+
 *            | parentClass-+---+                    ^
 *            |   methods   |        NewClass        |
 *            |    size     |     +-------------+    |
 *            |  variables  |<----+----class    |    |
 *            +-------------+     |     name    |    |
 *                                | parentClass-+----+
 *                                |   methods   |
 *                                |    size     |
 *                                |  variables  |
 *                                |  classVar1  |
 *                                |  classVar2  |
 *                                |     ...     |
 *                                +-------------+
 *
 *
 */

static int eatSpaces(const char *line, size_t *start)
{
    size_t old_start = *start;

    while(line[*start] == ' ') {
        (*start)++;
    }

    if(old_start != *start) {
        return 1;
    } else {
        return 0;
    }
}


static int matchString(const char *line, size_t *start, const char *match_str)
{
    size_t index = 0;

    while(line[*start+index] == match_str[index]) {
        index++;
    }

    if(index == strlen(match_str)) {
        *start += index;
        return 1;
    } else {
        return 0;
    }
}


static char *getWord(const char *line, size_t *end)
{
    char *result = NULL;
    size_t start = *end;

    /* skip whitespace */
    eatSpaces(line, &start);

    /* skip leading '#' chars. */
    if(line[start] == '#') {
        start++;
    }

    *end = start;

    /* read until we hit space or nul */
    while(line[*end] != ' ' && line[*end] != 0) {
        (*end)++;
    }

    result = calloc(1, (*end - start) + 1);
    if(result) {
        strncpy(result, &line[start], *end - start);
    }

    info("Read word \"%s\".", result);

    return result;


}


int matchVarList(const char *line, size_t *end, char **varList)
{
    size_t start = *end;
    size_t list_start = start;
    size_t list_end = start;

    eatSpaces(line, &start);

    /* match '#' */
    if(line[start] != '#') {
        info("Expected '#' in \"%s\" at position %zu!", line, start);
        return 0;
    }

    start++;

    /* match '(' */
    if(line[start] != '(') {
        info("Expected '(' in \"%s\" at position %zu!", line, start);
        return 0;
    }

    start++;

    /* skip early spaces */
    eatSpaces(line, &start);

    list_start = start;

    /* find the end */
    *end = start;

    while(line[*end] != ')' && line[*end] != 0) {
        (*end)++;
    }

    if(line[*end] == 0) {
        info("Unexpected end of string when expecting closing parenthesis in \"%s\"!", line);
        return 0;
    }

    /* so line[*end] == ')' */
    list_end = (*end) - 1;

    if(list_end <= list_start) {
        list_end = list_start;
    }

    *varList = calloc(1, (list_end - list_start) + 1);
    strncpy(*varList, &line[list_start], (list_end - list_start));

    /* bump end past the ')' character */
    (*end)++;

    return 1;
}


/* 
 * parse lines like:
 * +SuperClass subclass: #NewClass variables: #( instVars ) classVariables: #( classVars )
 */

static int parseClassCommand(const char *line, char **superclassName,  char **instClassName,  char **instVars, char **classVars)
{
    int rc = 0;
    size_t start = 0;
    size_t end = 0;

    if(line[start] != '+') {
        info("Malformed line \"%s\".  Expected first character to be '+'!");
        return rc;
    }

    /* get the superclass name */
    start++; /* skip the + char */
    end = start;

    *superclassName = getWord(line, &end);
    if(! *superclassName) {
        info("Failed to find the superclass name!");
        return 0;
    }

    start = end;

    /* now skip the whitespace */
    eatSpaces(line, &start);

    /* match the string "subclass:" */
    if(!matchString(line, &start, "subclass:")) {
        info("Failed to match \"subclass:\" in class creation string.");
        return 0;
    }

    /* skip any spaces */
    eatSpaces(line, &start);

    end = start;

    /* get the new class name, as a symbol */
    *instClassName = getWord(line, &end);
    if(! *instClassName) {
        info("Failed to find the new class name!");
        return 0;
    }

    start = end;

    /* skip any spaces */
    eatSpaces(line, &start);

    /* match the string "variables:" */
    if(!matchString(line, &start, "variables:")) {
        info("Failed to match \"variables:\" in class creation string.");
        return 0;
    }

    eatSpaces(line, &start);

    /* get the instance var list */
    if(! matchVarList(line, &start, instVars)) {
        info("Unable to find instance var list!");
        return 0;
    }

    /* trim the leading spaces. */
    eatSpaces(line, &start);

    /* match the string "classVariables:" */
    if(!matchString(line, &start, "classVariables:")) {
        info("Failed to match \"classVariables:\" before the class variable list in the class creation string.");
        return 0;
    }

    eatSpaces(line, &start);

    /* get the class var list */
    if(! matchVarList(line, &start, classVars)) {
        info("Unable to find class var list!");
        return 0;
    }

    info("Parsed class creation string successfully: parent class %s, new class %s with instance vars #( %s ) and class vars #( %s ).", *superclassName, *instClassName, *instVars, *classVars);
    return 1;
}



void ClassCommand(void)
{
    char *superclassName = NULL;
    char *instClassName = NULL;
    char *metaclassName = NULL;
    char *instVars = NULL;
    char *classVars = NULL;
    struct object *metaClass, *superClass, *instClass;
    size_t metaclassNameSize;
    int instsize = 0;
    int match_count = 0;


    /* 
     * parse lines like:
     * +SuperClass subclass: #NewClass variables: #( instVars ) classVariables: #( classVars )
     */
    if(!parseClassCommand(inputBuffer, &superclassName, &instClassName, &instVars, &classVars)) {
        parseError("Unable to parse class creation line!");
    }

    info("info: ClassCommand() found superclass=%s, instclass=%s, instvars=%s, classvars=%s", superclassName, instClassName, instVars, classVars);

    /* look up the superclass */
    superClass = lookupGlobalName(superclassName, 1);
    if(!superClass) {
        error("Line %d, unable to find class %s in \"%s\"!", inputLine, superclassName, inputBuffer);
    }

    /* make the metaclass first. */
    metaclassNameSize = strlen("Meta") + strlen(instClassName) + 1; /* one more for the zero terminator */
    metaclassName = malloc(metaclassNameSize);
    snprintf(metaclassName, metaclassNameSize, "Meta%s", instClassName);

    metaClass = lookupGlobalName(metaclassName, 1);
    if(!metaClass) {
        info("Creating metaclass %s.", metaclassName);    
        metaClass = newClass(metaclassName, 0);
        addGlobalName(metaclassName, metaClass);
    } else {
        info("Metaclass %s already exists.", metaclassName);
    }

    /* set up the class tree, weird for metaclasses. */
    metaClass->class = ClassClass;
    if(metaClass != MetaObjectClass) {
        info("setting class %s parent class to %.*s", metaclassName, SIZE(superClass->class->data[nameInClass]), (char *)bytePtr(superClass->class->data[nameInClass]));
        metaClass->data[parentClassInClass] = superClass->class;
    } else {
        info("setting class %s parent class to Class", metaclassName);
        metaClass->data[parentClassInClass] = ClassClass;
    }

    /* get any class vars. */
    litTop = 0;

    if(classVars && strlen(classVars) > 0) {
        /* copy the class vars string into the input buffer. */
        strncpy(inputBuffer, classVars, sizeof(inputBuffer));

        p = inputBuffer;

        /* Now parse the new instance variables */
        while (*p) {
            skipSpaces();
            if (!isIdentifierChar(*p)) {
                info("WARN: looking for variable list at position %d but found character '%c' (%x) instead in line %d, \"%s\".", (int)(p - inputBuffer), *p, *p, inputLine, inputBuffer);
                break;
            }
            readIdentifier();

            info("Found variable %s.", tokenBuffer);
            addLiteral(newSymbol(tokenBuffer));
        }
    }

    /* That's the total of our instance variables */
    instsize = litTop + ClassSize;

    /* fix up as much as possible of the metaclass. */
    metaClass->data[instanceSizeInClass] = newInteger(instsize);
    metaClass->data[variablesInClass] = buildLiteralArray();
    metaClass->data[methodsInClass] = newDictionary();

    /* now make the new class. */
    instClass = lookupGlobalName(instClassName, 1);
    if(!instClass) {
        info("Creating class %s.", instClassName);
        instClass = newClass(instClassName, litTop);
        addGlobalName(instClassName, instClass);
    } else {
        info("Class %s already exists.", instClassName);
    }

    /* get the instance vars */
    litTop = 0;

    if(instVars && strlen(instVars) > 0) {
        strncpy(inputBuffer, instVars, sizeof(inputBuffer));

        p = inputBuffer;

        /* Now parse the new instance variables */
        while (*p) {
            skipSpaces();
            if (!isIdentifierChar(*p)) {
                info("WARN: looking for variable list at position %d but found character '%c' (%x) instead in line %d, \"%s\".", (int)(p - inputBuffer), *p, *p, inputLine, inputBuffer);
                break;
            }
            readIdentifier();

            info("Found variable \"%s\".", tokenBuffer);
            addLiteral(newSymbol(tokenBuffer));
        }
    }

    instsize = litTop;

    /* class Object has a superclass of nil */
    if(superClass && superClass != nilObject) {
        instsize += integerValue(superClass->data[instanceSizeInClass]);
    }

    /* fix up as much as possible of the class. */
    instClass->data[parentClassInClass] = superClass;
    instClass->data[instanceSizeInClass] = newInteger(instsize);
    instClass->data[variablesInClass] = buildLiteralArray();
    instClass->data[methodsInClass] = newDictionary();
    instClass->class = metaClass;

    if(superclassName) free(superclassName);
    if(instClassName) free(instClassName);
    if(metaclassName) free(metaclassName);
    if(instVars) free(instVars);
    if(classVars) free(classVars);
}


/* ------------------------------------------------------------- */
/*	fix up symbol tables                                         */
/* ------------------------------------------------------------- */

struct object *symbolTreeInsert(struct object *base, struct object *symNode)
{
    if (base == nilObject)
        return symNode;
    if (symbolCmp(symNode->data[valueInNode], base->data[valueInNode]) < 0)
        base->data[leftInNode] =
            symbolTreeInsert(base->data[leftInNode], symNode);
    else
        base->data[rightInNode] =
            symbolTreeInsert(base->data[rightInNode], symNode);
    return base;
}


static struct object *fixSymbols(void)
{
    struct object *t;
    int i;

    t = newTree();
    for (i = 0; i < symbolTop; i++)
        t->data[0] = symbolTreeInsert(t->data[0],
                                      newNode(oldSymbols[i], nilObject,
                                              nilObject));
    return t;
}


static void fixGlobals(void)
{
    struct object *t;
    int i;

    t = globalValues;
    t->class = lookupGlobalName("Dictionary", 0);
    t->data[0] = newOrderedArray();
    t->data[1] = newArray(0);

    /*
     * Insert each class name as a reference to the class
     * object itself.
     */
    for (i = 0; i < globalTop; i++) {
        if (strncmp(globalNames[i], "Meta", 4) == 0) {
            continue;
        }

        dictionaryInsert(t, newSymbol(globalNames[i]), globals[i]);
    }

    /*
     * Insert this object itself under the name "Smalltalk"
     */
    dictionaryInsert(t, newSymbol("Smalltalk"), t);
}

/* ------------------------------------------------------------- */
/*	checkGlobals                                                 */
/* ------------------------------------------------------------- */

static void checkGlobals(void)
{
    int i;
    struct object *o;

    for (i = 0; i < globalTop; i++) {
        o = globals[i];
        if (!o->class) {
            error("checkGlobals(): Class %s never defined!", globalNames[i]);
        }
    }
}
