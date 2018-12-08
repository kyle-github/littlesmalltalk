/*
	Little Smalltalk main program, unix version
	Written by Tim Budd, budd@cs.orst.edu
	All rights reserved, no guarantees given whatsoever.
	May be freely redistributed if not for profit.

	starting point, primitive handler for unix
	version of the little smalltalk system
*/
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#if defined(VSTA) && defined(PROFILE)
# include <mcount.h>
#endif
#include "globs.h"

/*
	the following defaults must be set

*/
# define DefaultImageFile "lst.img"
# define DefaultStaticSize 100000
# define DefaultDynamicSize 100000
# define DefaultTmpdir "/tmp"

/*
--------------------
*/

#include "memory.h"
#include "interp.h"
#include <stdio.h>

/* # define COUNTTEMPS */

unsigned int debugging = 0, cacheHit = 0, cacheMiss = 0, gccount = 0;
static char *tmpdir = DefaultTmpdir;

extern int errno;

void
sysError(char * a)
{
    fprintf(stderr,"unrecoverable system error: %s\n", a);
    exit(1);
}


void
sysErrorInt(char * a, intptr_t b)
{
    fprintf(stderr,"unrecoverable system error: %s %ld\n", a, b);
    exit(1);
}


void
sysErrorStr(char * a, char * b)
{
    fprintf(stderr,"unrecoverable system error: %s %s\n", a, b);
    exit(1);
}


static void
backTrace(struct object * aContext)
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

# ifdef COUNTTEMPS
FILE * tempFile;
# endif

int
main(int argc, char ** argv)
{
    struct object *aProcess, *aContext, *o;
    int size, i, staticSize, dynamicSize;
    FILE *fp;
    char imageFileName[120], *p;

    strcpy(imageFileName, DefaultImageFile);
    staticSize = DefaultStaticSize;
    dynamicSize = DefaultDynamicSize;

    /*
     * See if our environment tells us what TMPDIR to use
     */
    p = getenv("TMPDIR");
    if (p) {
        tmpdir = strdup(p);
    }

    /* first parse arguments */
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            printf("Little Smalltalk, version 4.6.1\n");
        } else if (strcmp(argv[i], "-s") == 0) {
            staticSize = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-d") == 0) {
            dynamicSize = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-g") == 0) {
            debugging = 1;
        } else {
            strcpy(imageFileName, argv[i]);
        }
    }

# ifdef COUNTTEMPS
    tempFile = fopen("/usr/tmp/counts", "w");
# endif

    gcinit(staticSize, dynamicSize);

    /* read in the method from the image file */
    fp = fopen(imageFileName, "r");
    if (! fp) {
        fprintf(stderr,"cannot open image file: %s\n", imageFileName);
        exit(1);
    }

    printf("%d objects in image\n", fileIn(fp));
    fclose(fp);

    /* build a context around it */

    aProcess = staticAllocate(3);
    /* context should be dynamic */
    aContext = gcalloc(contextSize);
    aContext->class = ContextClass;


    aProcess->data[contextInProcess] = aContext;
    size = integerValue(initialMethod->data[stackSizeInMethod]);
    aContext->data[stackInContext] = staticAllocate(size);
    aContext->data[argumentsInContext] = nilObject;

    aContext->data[temporariesInContext] = staticAllocate(19);
    aContext->data[bytePointerInContext] = newInteger(0);
    aContext->data[stackTopInContext] = newInteger(0);
    aContext->data[previousContextInContext] = nilObject;
    aContext->data[methodInContext] = initialMethod;

    /* now go do it */
    rootStack[rootTop++] = aProcess;

#if defined(VSTA) && defined(PROFILE)
    take_samples(1);
#endif
    switch(execute(aProcess, 0)) {
    case 2:
        printf("User defined return\n");
        break;

    case 3:
        printf("can't find method in call\n");
        aProcess = rootStack[--rootTop];
        o = aProcess->data[resultInProcess];
        printf("Unknown method: %s\n", bytePtr(o));
        aContext = aProcess->data[contextInProcess];
        backTrace(aContext);
        break;

    case 4:
        printf("\nnormal return\n");
        break;

    case 5:
        printf("time out\n");
        break;

    default:
        printf("unknown return code\n");
        break;
    }
#if defined(VSTA) && defined(PROFILE)
    dump_samples();
#endif
    printf("cache hit %u miss %u", cacheHit, cacheMiss);
#define SCALE (1000)
    while ((cacheHit > INT_MAX/SCALE) || (cacheMiss > INT_MAX/SCALE)) {
        cacheHit /= 10;
        cacheMiss /= 10;
    }
    i = (SCALE * cacheHit) / (cacheHit + cacheMiss);
    printf(" ratio %u.%u%%\n", i / 10, i % 10);
    printf("%u garbage collections\n", gccount);
    return(0);
}

/*
	primitive handler
	(note that many primitives are handled in the interpreter)
*/

#define FILEMAX 20
static FILE *filePointers[FILEMAX];

static void
getUnixString(char * to, int size, struct object * from)
{
    int i;
    int fsize = SIZE(from);
    struct byteObject * bobj = (struct byteObject *) from;

    if (fsize > size) {
        sysErrorInt("error converting text into unix string", fsize);
    }
    for (i = 0; i < fsize; i++) {
        to[i] = bobj->bytes[i];
    }
    to[i] = '\0';	/* put null terminator at end */
}


/*
These static character strings are used for URL conversion
*/

static char * badURLChars = ";/?:@&=+!*'(),$-_.<>#%\"\t\n\r";
static char * hexDigits = "0123456789ABCDEF";



/**
 * stringToUrl
 *
 * convert the passed string object to a URL safe set of characters.
 * Returns a String object.
 */

static struct object * stringToUrl(struct byteObject * from)
{
    int i,j;
    struct byteObject *newStr = (struct byteObject *)0;
    int new_size = 0;
    int bad_chars = 0;
    int fsize = from->size >> 2;
    char *from_ptr = (char *)from->bytes;
    char *to_ptr = (char *)0;
    char c;

    /* count bad chars */
    for(i=0; i<fsize; i++) {
        if(NULL != strchr(badURLChars,from_ptr[i])) {
            bad_chars++;
        }
    }

    /* we're going to allocate, so we need to make sure that nothing can
    move out from underneath us. */
    rootStack[rootTop++] = (struct object *)from;

    /* allocate enough space for the new string.  Note that
    the size is increased by two for every bad char.  This is
    because a URL-safe replacement is a "%" char followed by two
    hex digits. The "%" replaces the original character and the two
    hex digits are added. */

    new_size = fsize + (bad_chars * 2);
    newStr = (struct byteObject *)gcialloc(new_size);
    newStr->class = lookupGlobal("String");

    /* OK, now done with allocation, get the from string back */
    from = (struct byteObject *)rootStack[--rootTop];
    from_ptr = (char *)from->bytes;

    /* did allocation succeed? */
    if(NULL == newStr) {
        sysError("stringToUrl(): unable to allocate string!");
        return nilObject;
    }

    /* copy the characters */
    j = 0;
    to_ptr = (char *)newStr->bytes;

    for(i=0; i<fsize && j<new_size; i++) {
        if(strchr(badURLChars,from_ptr[i])) {
            to_ptr[j++] = '%';
            to_ptr[j++] = hexDigits[0x0F & ((from_ptr[i])>>4)];
            to_ptr[j++] = hexDigits[0x0F & (from_ptr[i])];
        } else {
            /* convert spaces to plusses too, but this doesn't change the string size. */
            c = from_ptr[i];

            if(c == ' ')
                c = '+';
            to_ptr[j++] = c;
        }
    }

    return (struct object *)newStr;
}




/**
 * urlToString
 *
 * convert the passed string object from a URL safe set of characters.
 * Returns a String object.
 */

static struct object * urlToString(struct byteObject * from)
{
    int i,j;
    struct byteObject *newStr = (struct byteObject *)0;
    int new_size = 0;
    int url_chars = 0;
    int fsize = from->size >> 2;
    char *from_ptr = (char *)from->bytes;
    char *to_ptr = (char *)0;
    int is_hex=0;
    int hex_val;
    char c;

    /* count unpacked chars */
    for(i=0; i<fsize; i++) {
        if('%' == from_ptr[i]) {
            url_chars++;
        }
    }

    /* we're going to allocate, so we need to make sure that nothing can
    move out from underneath us. */
    rootStack[rootTop++] = (struct object *)from;

    /* allocate enough space for the new string.  Note that
    the size is decreased by two for every URL char.  This is
    because a URL-safe replacement is a "%" char followed by two
    hex digits. The "%" replaces the original character and the two
    hex digits are added. */

    new_size = fsize - (url_chars * 2);

    if(new_size<0)
        new_size = 0;

    newStr = (struct byteObject *)gcialloc(new_size);
    newStr->class = lookupGlobal("String");

    /* OK, now done with allocation, get the from string back */
    from = (struct byteObject *)rootStack[--rootTop];
    from_ptr = (char *)from->bytes;

    /* Did allocation succeed? */
    if(NULL == newStr) {
        sysError("urlToString(): unable to allocate string!");
        return nilObject;
    }

    /* copy the characters */
    j = 0;
    to_ptr = (char *)newStr->bytes;
    is_hex = 0;

    for(i=0; i<fsize && j<new_size; i++) {
        c = from_ptr[i];

        /* first convert plusses to spaces */
        if(c == '+')
            c = ' ';

        /* is this a hex string starting? */
        if('%' == c)
            is_hex = 3;

        switch(is_hex) {
        case 3:
            /* this is the % character, ignore it */
            is_hex--;

            break;

        case 2:
            /* this is the first hex char, convert it */
            is_hex--;

            hex_val = strchr(hexDigits,toupper(c)) - hexDigits;

            if(hex_val>=0)
                to_ptr[j] = (char)(hex_val<<4);
            else
                /* this is an error! */
                return nilObject;

            break;

        case 1:
            /* this is the last hex char, convert it, note
            that we increment j here. */
            is_hex--;

            hex_val = strchr(hexDigits,toupper(c)) - hexDigits;

            if(hex_val >= 0 )
                to_ptr[j] += (char)hex_val;
            else
                /* this is an error! */
                return nilObject;

            j++;

            break;

        default:
            /* other characters */
            to_ptr[j++] = c;
            break;
        }
    }

    return (struct object *)newStr;
}





#define SOCK_BUF_SIZE 1024
static char socketReadBuffer[SOCK_BUF_SIZE];

struct object *
primitive(int primitiveNumber, struct object *args, int *failed)
{
    struct object *returnedValue = nilObject;
    int i, j;
    int rc;
    FILE *fp;
    uint8_t *p;
    struct byteObject *stringReturn;
    char nameBuffer[80], modeBuffer[80];
    int subPrim=0;
    char netBuffer[16];
    struct sockaddr myAddr;
    struct sockaddr_in sin;
    struct in_addr iaddr;
    socklen_t myAddrSize;
    int sock;
    int port;
    struct byteObject *ba;



    *failed = 0;
    switch(primitiveNumber) {
    case 100: 	/* open a file */
        getUnixString(nameBuffer, 80, args->data[0]);
        getUnixString(modeBuffer, 10, args->data[1]);
        fp = fopen(nameBuffer, modeBuffer);
        if (fp != NULL) {
            for (i = 0; i < FILEMAX; ++i) {
                if (filePointers[i] == NULL) {
                    break;
                }
            }
            if (i >= FILEMAX) {
                sysError("too many open files");
                fclose(fp);
                *failed = 1;
            } else {
                returnedValue = newInteger(i);
                filePointers[i] = fp;
            }
        } else {
            *failed = 1;
        }
        break;

    case 101:	/* read a single character from a file */
        i = integerValue(args->data[0]);
        if ((i < 0) || (i >= FILEMAX) || !(fp = filePointers[i])) {
            *failed = 1;
            break;
        }
        i = fgetc(fp);
        if (i != EOF) {
            returnedValue = newInteger(i);
        }
        break;

    case 102:	/* write a single character to a file */
        i = integerValue(args->data[0]);
        if ((i < 0) || (i >= FILEMAX) || !(fp = filePointers[i])) {
            *failed = 1;
            break;
        }
        fputc(integerValue(args->data[1]), fp);
        break;

    case 103:	/* close file */
        i = integerValue(args->data[0]);
        if ((i < 0) || (i >= FILEMAX) || !(fp = filePointers[i])) {
            *failed = 1;
            break;
        }
        fclose(fp);
        break;

    case 104:	/* file out image */
        i = integerValue(args->data[0]);
        if ((i < 0) || (i >= FILEMAX) || !(fp = filePointers[i])) {
            *failed = 1;
            break;
        }
        fileOut(fp);
        break;

    case 105:	/* edit a string */
        /* first get the name of a temp file */
        sprintf(nameBuffer, "%s/lsteditXXXXXX", tmpdir);
        rc = mkstemp(nameBuffer);
        /* copy string to file */
        if(rc == -1) {
            sysErrorInt("error making temporary file: %ld",(intptr_t)rc);
        }

        fp = fopen(nameBuffer, "w");
        if (fp == NULL)
            sysError("cannot open temp edit file");
        j = SIZE(args->data[0]);
        p = ((struct byteObject *) args->data[0])->bytes;
        for (i = 0; i < j; i++)
            fputc(*p++, fp);
        fputc('\n', fp);
        fclose(fp);
        /* edit string */
        strcpy(modeBuffer,"vi ");
        strcat(modeBuffer,nameBuffer);
        rc = system(modeBuffer);
        if(rc == -1) {
            sysErrorInt("error starting editor: %ld",(intptr_t)rc);
        }

        /* copy back to new string */
        fp = fopen(nameBuffer, "r");
        if (fp == NULL)
            sysError("cannot open temp edit file");
        /* get length of file */
        fseek(fp, 0, 2);
        j = (int) ftell(fp);
        returnedValue = (struct object *)(stringReturn = (struct byteObject *)gcialloc(j));
        returnedValue->class = args->data[0]->class;
        /* reset to beginning, and read values */
        fseek(fp, 0, 0);
        for (i = 0; i < j; i++)
            stringReturn->bytes[i] = fgetc(fp);
        /* now clean up files */
        fclose(fp);
        unlink(nameBuffer);
        break;

    case 106:	/* Read into ByteArray */
        /* File descriptor */
        i = integerValue(args->data[0]);
        if ((i < 0) || (i >= FILEMAX) || !(fp = filePointers[i])) {
            *failed = 1;
            break;
        }

        /* Make sure we're populating an array of bytes */
        returnedValue = args->data[1];
        if ((returnedValue->size & FLAG_BIN) == 0) {
            *failed = 1;
            break;
        }

        /* Sanity check on I/O count */
        i = integerValue(args->data[2]);
        if ((i < 0) || (i > SIZE(returnedValue))) {
            *failed = 1;
            break;
        }

        /* Do the I/O */
        i = fread(bytePtr(returnedValue), sizeof(char), i, fp);
        if (i < 0) {
            *failed = 1;
            break;
        }
        returnedValue = newInteger(i);
        break;

    case 107:	/* Write from ByteArray */
        /* File descriptor */
        i = integerValue(args->data[0]);
        if ((i < 0) || (i >= FILEMAX) || !(fp = filePointers[i])) {
            *failed = 1;
            break;
        }

        /* Make sure we're writing an array of bytes */
        returnedValue = args->data[1];
        if ((returnedValue->size & FLAG_BIN) == 0) {
            *failed = 1;
            break;
        }

        /* Sanity check on I/O count */
        i = integerValue(args->data[2]);
        if ((i < 0) || (i > SIZE(returnedValue))) {
            *failed = 1;
            break;
        }

        /* Do the I/O */
        i = fwrite(bytePtr(returnedValue), sizeof(char), i, fp);
        if (i < 0) {
            *failed = 1;
            break;
        }
        returnedValue = newInteger(i);
        break;

    case 108:	/* Seek to file position */
        /* File descriptor */
        i = integerValue(args->data[0]);
        if ((i < 0) || (i >= FILEMAX) || !(fp = filePointers[i])) {
            *failed = 1;
            break;
        }

        /* File position */
        i = integerValue(args->data[1]);
        if ((i < 0) || ((i = fseek(fp, i, SEEK_SET)) < 0)) {
            *failed = 1;
            break;
        }

        /* Return position as our value */
        returnedValue = newInteger(i);
        break;


    case 150:	/* match substring in a string. Return index of substring or fail. */
        /* make sure we've got strings */
        if((args->data[0]->size & FLAG_BIN) == 0) {
            *failed = 1;
            break;
        }

        if((args->data[1]->size & FLAG_BIN) == 0) {
            *failed = 1;
            break;
        }

        /* get the sizes of the strings */
        i = SIZE(args->data[0]);
        j = SIZE(args->data[1]);

        /* don't bother to compare if either string has a zero length
        or the second string is longer than the first */

        if((i > 0) && (j > 0) && (i>=j)) {
            /* using malloc is probably not a good idea here */
            char *p = (char *)malloc(i+1);
            char *q = (char *)malloc(j+1);
            char *r = (char *)0;

            getUnixString(p,i,args->data[0]);
            getUnixString(q,j,args->data[1]);

            /* find the pointer to the substring */
            r = strstr(p,q);

            if(r != NULL) {
                /* the string was found */
                returnedValue = newInteger((r-p)+1);
            }

            /* no GC for primitives on this.  free memory for p and q*/
            free((void *)p);
            free((void *)q);

            /* success */
            break;
        }

        /* if we get here, we've failed */
        *failed = 1;
        break;

    case 151: /* convert a string to URL encoding, returns nil or string */
        returnedValue = stringToUrl((struct byteObject *)args->data[0]);

        break;


    case 152: /* convert a string from URL encoding, returns nil or string */
        returnedValue = urlToString((struct byteObject *)args->data[0]);

        break;

    case 200: /* this is a set of primitives for socket handling */
        subPrim = integerValue(args->data[0]);

        switch(subPrim) {
            /* 100-150 socket handling */
        case 0: /* open a TCP socket */
            sock = socket(PF_INET,SOCK_STREAM,0);

            if(sock == -1) {
                sysError("Cannot open TCP socket.");
            }

            /* set the socket options */
            setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,(const void *)1,sizeof(int));

            /* return the value anyway */
            returnedValue = newInteger(sock);

            break;

        case 1: /* accept on a socket */
            sock = integerValue(args->data[1]);

            if(listen(sock,10) == -1)
                sysError("Error listening on TCP socket.");


            /*			sock = accept(sock,(struct sockaddr *)&saddr,(size_t *)&sinAddrLen);*/
            sock = accept(sock,&myAddr,&myAddrSize);

            if(sock == -1) {
                printf("errno: %d\nsock: %d\n",errno,sock);
                sysError("Error accepting on TCP socket.");
            }

            returnedValue = newInteger(sock);

            break;

        case 2: /* close a socket */
            sock = integerValue(args->data[1]);

            close(sock);

            returnedValue = trueObject;

            break;

        case 3: /* bind a socket to an address and port */
            /* this takes three arguments, the socket fd,
            the address (as a dotted-notation string) and
            the port as an integer */
            sock = integerValue(args->data[1]);
            getUnixString(netBuffer, 16, args->data[2]);
            port = integerValue(args->data[3]);

            printf("Socket: %d\n",sock);
            printf("IP: %s\n",netBuffer);
            printf("Port: %d\n",port);

            /* convert the string IP to a network representation */
            if(inet_aton((const char *)netBuffer,&iaddr) == INADDR_NONE)
                sysError("Illegal address passed to bind primitive.");

            /* build a sockaddr_in struct */
            sin.sin_family = AF_INET;
            sin.sin_port = htons((u_int16_t)port);
            /*			sin.sin_port = (u_int16_t)0;*/
            sin.sin_addr = iaddr;

            if(bind(sock,(struct sockaddr *)&sin,sizeof(sin)) == -1)
                sysError("Cannot bind TCP socket to address.");

            returnedValue = trueObject;

            break;

        case 7: /* read from a TCP socket.  This returns a byte array. */
            sock = integerValue(args->data[1]);

            for(i=0; i<SOCK_BUF_SIZE; i++)
                socketReadBuffer[i]=(char)0;

            i = read(sock,(void *)socketReadBuffer,(size_t)SOCK_BUF_SIZE);

            printf("Read: %s\n",socketReadBuffer);

            ba = (struct byteObject *)gcialloc(i);
            ba->class = lookupGlobal("ByteArray");

            /* copy data into the new ByteArray */
            for(j=0; j<i; j++)
                bytePtr(ba)[j] = socketReadBuffer[j];

            returnedValue = (struct object *)ba;

            break;

        case 8: /* write to a socket, args: sock, data */
            sock = integerValue(args->data[1]);
            p = (uint8_t *)bytePtr(args->data[2]);
            i = SIZE(args->data[2]);

            /*printf("Writing: ");
            snprintf(socketReadBuffer,i,"%s",p);
            socketReadBuffer[i] = (char)0;
            printf("%s\n",socketReadBuffer);
            */

            j = write(sock,(void *)p,(size_t)i);

            if(j>0)
                returnedValue = newInteger(j);
            else
                returnedValue = nilObject;

            break;

        default: /* unknown socket primitive */
            sysErrorInt("Unknown socket primitive operation: ",subPrim);
            break;
        }

        break;

    default:
        sysErrorInt("unknown primitive", primitiveNumber);
    }
    return(returnedValue);
}
