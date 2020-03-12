#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <alloca.h>
#include "prim.h"
#include "err.h"
#include "interp.h"
#include "memory.h"
#include "globals.h"


/* temporary directory is shared. */
//class getUnixString;
//class getUnixString;
char *tmpdir = NULL;


#define SOCK_BUF_SIZE 1024
static uint8_t socketReadBuffer[SOCK_BUF_SIZE] = {0,};

#define FILEMAX 200
static FILE *filePointers[FILEMAX] = {0,};

/*
These static character strings are used for URL conversion
*/

static char * badURLChars = ";/?:@&=+!*'(),$-_.<>#%\"\t\n\r";
static char * hexDigits = "0123456789ABCDEF";


/* forward refs for helper functions. */
static void getUnixString(char * to, int size, struct object * from);
static struct object * stringToUrl(struct byteObject * from);
static struct object * urlToString(struct byteObject * from);



/*
    primitive handler
    (note that many primitives are handled in the interpreter)
*/

struct object *primitive(int primitiveNumber, struct object *args, int *failed)
{
    struct object *returnedValue = nilObject;
    int i, j;
    int rc;
    FILE *fp;
    uint8_t *p;
    struct byteObject *stringReturn;
    //char nameBuffer[80], modeBuffer[80];
    int subPrim=0;
    char netBuffer[18];
    struct sockaddr myAddr;
    struct sockaddr_in sin;
    struct in_addr iaddr;
    socklen_t myAddrSize;
    int sock;
    int sock_opt;
    int port;
    struct byteObject *ba;


    *failed = 0;
    switch(primitiveNumber) {
    case 100:
    {
        /* open a file */
        int pathSize = SIZE(args->data[0]) + 1;
        char *pathBuffer = (char *)alloca((size_t)pathSize);
        int modeSize = SIZE(args->data[1]) + 1;
        char *modeBuffer = (char *)alloca((size_t)modeSize);

        getUnixString(pathBuffer, pathSize, args->data[0]);
        getUnixString(modeBuffer, modeSize, args->data[1]);

        fp = fopen(pathBuffer, modeBuffer);
        if (fp != NULL) {
            for (i = 0; i < FILEMAX; ++i) {
                if (filePointers[i] == NULL) {
                    break;
                }
            }
            if (i >= FILEMAX) {
                error("too many open files");
            } else {
                returnedValue = newInteger(i);
                filePointers[i] = fp;
            }
        } else {
            *failed = 1;
        }
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

    case 105:
    {
        /* edit a string */
        char *tmpFileName = NULL;
        size_t tmpFileNameSize = 0;

        /* first get the size needed for the temporary path */
        tmpFileNameSize = (size_t)snprintf(tmpFileName, 0, "%s/lsteditXXXXXX", tmpdir) + 1;

        info("temporary file name size is %d", (int)tmpFileNameSize);

        /* allocate it on the stack and write the string into it. */
        tmpFileName = (char *)alloca((size_t)tmpFileNameSize + 1);
        memset(tmpFileName, 0, tmpFileNameSize + 1);
        snprintf(tmpFileName, tmpFileNameSize + 1, "%s/lsteditXXXXXX", tmpdir);

        info("DEBUG: temp file name pattern: %s",tmpFileName);

        rc = mkstemp(tmpFileName);
        /* copy string to file */
        if(rc == -1) {
            error("error making temporary file name: errno=%d!", errno);
        }

        fp = fopen(tmpFileName, "w");
        if (fp == NULL) {
            error("cannot open temp edit file %s!", tmpFileName);
        }

        j = SIZE(args->data[0]);
        p = ((struct byteObject *) args->data[0])->bytes;

        for (i = 0; i < j; i++) {
            fputc(*p++, fp);
        }
        fputc('\n', fp);

        fclose(fp);

        /* call the editor */
        size_t cmdBufSize = tmpFileNameSize + strlen("vi ");
        char *cmdBuf = (char*)alloca(cmdBufSize + 1);
        memset(cmdBuf, 0, cmdBufSize + 1);

        strcpy(cmdBuf,"vi ");
        strcat(cmdBuf, tmpFileName);
        rc = system(cmdBuf);

        if(rc == -1) {
            error("error starting editor: %d!",(int)errno);
        }

        /* copy back to new string */
        fp = fopen(tmpFileName, "r");
        if (fp == NULL) {
            error("cannot open temp edit file %s!", tmpFileName);
        }

        /* get length of file */
        fseek(fp, 0, 2);
        j = (int) ftell(fp);

        returnedValue = (struct object *)(stringReturn = (struct byteObject *)gcialloc(j));
        returnedValue->class = args->data[0]->class;

        /* reset to beginning, and read values */
        fseek(fp, 0, 0);

        /* FIXME - check for EOF! */
        for (i = 0; i < j; i++) {
            stringReturn->bytes[i] = (uint8_t)fgetc(fp);
        }
        /* now clean up files */
        fclose(fp);
        unlink(tmpFileName);
    }

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
        if (!IS_BINOBJ(returnedValue)) {
            *failed = 1;
            break;
        }

        /* Sanity check on I/O count */
        i = integerValue(args->data[2]);
        if ((i < 0) || (i > (int)SIZE(returnedValue))) {
            *failed = 1;
            break;
        }

        /* Do the I/O */
        i = (int)fread(bytePtr(returnedValue), sizeof(char), (size_t)i, fp);
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
        if (!IS_BINOBJ(returnedValue)) {
            *failed = 1;
            break;
        }

        /* Sanity check on I/O count */
        i = integerValue(args->data[2]);
        if ((i < 0) || (i > (int)SIZE(returnedValue))) {
            *failed = 1;
            break;
        }

        /* Do the I/O */
        i = (int)fwrite(bytePtr(returnedValue), sizeof(char), (size_t)i, fp);
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
        if(!IS_BINOBJ(args->data[0])) {
            printf("#position: failed, first arg is not a binary object.\n");
            *failed = 1;
            break;
        }

        if(!IS_BINOBJ(args->data[1])) {
            printf("#position: failed, second arg is not a binary object.\n");
            *failed = 1;
            break;
        }

        /* get the sizes of the strings */
        i = SIZE(args->data[0]);
        j = SIZE(args->data[1]);

        /*
         * don't bother to compare if either string has a zero length
         * or the second string is longer than the first
         */

        if((i > 0) && (j > 0) && (i>=j)) {
            /* using alloca to make sure that the memory is freed automatically */
            char *p = (char *)alloca((size_t)i+1);
            char *q = (char *)alloca((size_t)j+1);
            char *r = (char *)0;

            getUnixString(p,i+1,args->data[0]);
            getUnixString(q,j+1,args->data[1]);

            /* find the pointer to the substring */
            r = strstr(p,q);

            if(r != NULL) {
                /* the string was found */
                returnedValue = newInteger((r-p)+1);
            } else {
                returnedValue = nilObject;
            }

            /* success */
            break;
        } else {
            if((i<0) || (j<0)) {
                printf("#position: failed due to unusable string sizes, string 1 size %d, string 2 size %d.\n", i, j);
            } else {
                /* i==0 or j==0 or i<j in which case we have no match but no error. */
                returnedValue = nilObject;
                break;
            }
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

    /* large timestamps */
    case 160: /* print out a microsecond timestamp and message string. */
        {
            struct byteObject *msg = (struct byteObject *)(args->data[0]);
            int64_t ticks = time_usec();

            printf("Log: %.*s\n", SIZE(msg), bytePtr(msg));

            *failed = 0;

            returnedValue = nilObject;
        }

        break;

    case 161: /* return an Integer with the microsecond timestamp. */
        returnedValue = newLInteger(time_usec());
        *failed = 0;
        break;

    case 170: /* get argv strings as an Array of Strings. */
        {
            struct object *argv_array = NULL;

            /* allocate enough space for the result Array. */
            argv_array = gcalloc(prog_argc);
            argv_array->class = ArrayClass;

            /* we are going to allocate Strings and that could cause GC. */
            PUSH_ROOT(argv_array);

            for(int index = 0; index < prog_argc; index++) {
                struct object *argv_entry = NULL;
                const char *argv_str = prog_argv[index];
                int str_len = (int)strlen(argv_str);

                /* could cause GC */
                argv_entry = gcialloc(str_len);
                argv_entry->class = StringClass;

                /* copy the bytes. */
                for(int i=0; i < str_len; i++) {
                    bytePtr(argv_entry)[i] = (uint8_t)argv_str[i];
                }

                /* get the pointer to the array again.   Could have changed due to GC. */
                argv_array = PEEK_ROOT();

                argv_array->data[index] = argv_entry;
            }

            argv_array = POP_ROOT();

            returnedValue = argv_array;

            *failed = 0;
        }

        break;

    case 200: /* this is a set of primitives for socket handling */
        subPrim = integerValue(args->data[0]);

        /* 200-250 socket handling */
        switch(subPrim) {
        case 0: /* open a TCP socket */
            sock = socket(PF_INET,SOCK_STREAM,0);

            if(sock == -1) {
                error("Cannot open TCP socket.");
            }

            sock_opt = 1;

            if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&sock_opt, sizeof(sock_opt))) {
                close(sock);
                error("Error setting socket reuse option!");
            }

            /* return the value anyway */

            info("opened socket %d.", sock);

            returnedValue = newInteger(sock);

            break;

        case 1: /* accept on a socket */
            sock = integerValue(args->data[1]);

            if(listen(sock, 10) == -1) {
                error("Error listening on TCP socket.");
            }

            /* set the maximum size */
            myAddrSize = sizeof(myAddr);

            /* printf("accept(%d, %p, %d)\n", sock, &myAddr, myAddrSize); */

            sock = accept(sock, &myAddr, &myAddrSize);
            if(sock == -1) {
                error("Error accepting on TCP socket.  Errno=%d", errno);
            }

            returnedValue = newInteger(sock);

            break;

        case 2: /* close a socket */
            sock = integerValue(args->data[1]);

            info("closing socket %d.", sock);

            close(sock);

            returnedValue = trueObject;

            break;

        case 3: /* bind a socket to an address and port */
            /* this takes three arguments, the socket fd,
            the address (as a dotted-notation string) and
            the port as an integer */
            sock = integerValue(args->data[1]);
            getUnixString(netBuffer, sizeof(netBuffer)-1, args->data[2]);
            port = integerValue(args->data[3]);

            info("Socket: %d",sock);
            info("IP: %s",netBuffer);
            info("Port: %d",port);

            /* convert the string IP to a network representation */
            if(inet_aton((const char *)netBuffer,&iaddr) == 0) {
                error("Illegal address passed to bind primitive.");
            }

            /* build a sockaddr_in struct */
            sin.sin_family = AF_INET;
            sin.sin_port = htons((u_int16_t)port);
            /*			sin.sin_port = (u_int16_t)0;*/
            sin.sin_addr = iaddr;

            if(bind(sock,(struct sockaddr *)&sin,sizeof(sin)) == -1) {
                error("Cannot bind TCP socket to address.");
            }

            returnedValue = trueObject;

            break;

        case 7: /* read from a TCP socket.  This returns a byte array. */
            sock = integerValue(args->data[1]);

            for(i=0; i<SOCK_BUF_SIZE; i++) {
                socketReadBuffer[i]=(char)0;
            }

            i = (int)read(sock,(void *)socketReadBuffer,(size_t)SOCK_BUF_SIZE);
            if((i < 0) && (errno != EAGAIN) && (errno != EWOULDBLOCK)) {
                info("socket read returned an error: %d (%d)!", i, errno);
                *failed = 1;
                break;
            }

//            printf("Read: %s\n",socketReadBuffer);

            ba = (struct byteObject *)gcialloc(i);
            ba->class = ByteArrayClass;

            /* copy data into the new ByteArray */
            for(j=0; j<i; j++) {
                bytePtr(ba)[j] = socketReadBuffer[j];
            }

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

            j = (int)write(sock,(void *)p,(size_t)i);

            if(j>0)
                returnedValue = newInteger(j);
            else
                returnedValue = nilObject;

            break;

        default: /* unknown socket primitive */
            error("Unknown socket primitive operation: %d!",subPrim);
            break;
        }

        break;

    default:
        error("unknown primitive %d!", primitiveNumber);
    }
    return(returnedValue);
}




void getUnixString(char * to, int size, struct object * from)
{
    int i;
    int fsize = SIZE(from);
    struct byteObject * bobj = (struct byteObject *) from;

    if (fsize >= size) {
        error("getUnixString(): String too long, %d, for buffer, %d bytes!", fsize, size);
    }

    for (i = 0; i < fsize; i++) {
        to[i] = (char)bobj->bytes[i];
    }

    to[i] = '\0';	/* put null terminator at end */
}




/*
 * newLInteger()
 *  Create new Integer (64-bit)
 */
struct object *newLInteger(int64_t val)
{
    struct object *res;
    int64_t *tmp;

    res = gcialloc(sizeof(int64_t));
    res->class = IntegerClass;
    tmp = (int64_t *)bytePtr(res);
    *tmp = val;
    return(res);
}

/*
 * do_Integer()
 *  Implement the appropriate 64-bit Integer operation
 *
 * Returns NULL on error, otherwise the resulting Integer or
 * Boolean (for comparisons) object.
 */
struct object *do_Integer(int op, struct object *low, struct object *high)
{
    int64_t l, h;
    int64_t *tmp;

    tmp = (int64_t *)bytePtr(low);
    l = *tmp;
    tmp = (int64_t *)bytePtr(high);
    h = *tmp;
    switch (op) {
    case 25:    /* Integer division */
        if (h == 0LL) {
            return(NULL);
        }
        return(newLInteger(l/h));

    case 26:    /* Integer remainder */
        if (h == 0LL) {
            return(NULL);
        }
        return(newLInteger(l%h));

    case 27:    /* Integer addition */
        return(newLInteger(l+h));

    case 28:    /* Integer multiplication */
        return(newLInteger(l*h));

    case 29:    /* Integer subtraction */
        return(newLInteger(l-h));

    case 30:    /* Integer less than */
        return((l < h) ? trueObject : falseObject);

    case 31:    /* Integer equality */
        return((l == h) ? trueObject : falseObject);

    default:
        error("do_Integer(): Invalid primitive integer operation %d!", op);
    }
    return(NULL);
}



/**
 * stringToUrl
 *
 * convert the passed string object to a URL safe set of characters.
 * Returns a String object.
 */

struct object * stringToUrl(struct byteObject * from)
{
    int i,j;
    struct byteObject *newStr = NULL;
    int new_size = 0;
    int bad_chars = 0;
    int fsize = SIZE(from);
    char *from_ptr = (char *)bytePtr(from);
    char *to_ptr = NULL;
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
    newStr->class = StringClass;

    /* OK, now done with allocation, get the from string back */
    from = (struct byteObject *)rootStack[--rootTop];
    from_ptr = (char *)from->bytes;

    /* did allocation succeed? */
    if(NULL == newStr) {
        error("stringToUrl(): unable to allocate string!");
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

struct object * urlToString(struct byteObject * from)
{
    int i,j;
    struct byteObject *newStr = NULL;
    int new_size = 0;
    int url_chars = 0;
    int fsize = (int)SIZE(from);
    char *from_ptr = (char *)bytePtr(from);
    char *to_ptr = NULL;
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
    newStr->class = StringClass;

    /* OK, now done with allocation, get the from string back */
    from = (struct byteObject *)rootStack[--rootTop];
    from_ptr = (char *)bytePtr(from);

    /* Did allocation succeed? */
    if(NULL == newStr) {
        error("urlToString(): unable to allocate string!");
        return nilObject;
    }

    /* copy the characters */
    j = 0;
    to_ptr = (char *)bytePtr(newStr);
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

            hex_val = (int)(strchr(hexDigits,toupper(c)) - hexDigits);

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

            hex_val = (int)(strchr(hexDigits,toupper(c)) - hexDigits);

            if(hex_val >= 0 ) {
                to_ptr[j] = (char)((int)to_ptr[j] + hex_val);
            } else {
                /* this is an error! */
                return nilObject;
            }

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
