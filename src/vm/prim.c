#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
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
#include "globs.h"


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

        printf("temporary file name size is %d\n", (int)tmpFileNameSize);

        /* allocate it on the stack and write the string into it. */
        tmpFileName = (char *)alloca((size_t)tmpFileNameSize + 1);
        memset(tmpFileName, 0, tmpFileNameSize + 1);
        snprintf(tmpFileName, tmpFileNameSize + 1, "%s/lsteditXXXXXX", tmpdir);

        printf("DEBUG: temp file name pattern: %s\n",tmpFileName);

        rc = mkstemp(tmpFileName);
        /* copy string to file */
        if(rc == -1) {
            sysErrorInt("error making temporary file name: errno=", errno);
        }

        fp = fopen(tmpFileName, "w");
        if (fp == NULL) {
            sysError("cannot open temp edit file");
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
            sysErrorInt("error starting editor: %ld",(intptr_t)rc);
        }

        /* copy back to new string */
        fp = fopen(tmpFileName, "r");
        if (fp == NULL) {
            sysError("cannot open temp edit file");
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
        if((args->data[0]->size & FLAG_BIN) == 0) {
            printf("#position: failed, first arg is not a binary object.\n");
            *failed = 1;
            break;
        }

        if((args->data[1]->size & FLAG_BIN) == 0) {
            printf("#position: failed, second arg is not a binary object.\n");
            *failed = 1;
            break;
        }

        /* get the sizes of the strings */
        i = SIZE(args->data[0]);
        j = SIZE(args->data[1]);

        /* don't bother to compare if either string has a zero length
        or the second string is longer than the first */

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
            if((i>0) && (j>0)) {
                returnedValue = nilObject;
                break;
            } else {
                printf("#position: failed due to unusable string sizes, string 1 size %d, string 2 size %d.\n", i, j);
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

    case 160: /* print out a microsecond timestamp and message string. */
    {
        struct byteObject *msg = (struct byteObject *)(args->data[0]);
        char *tmpMsg = NULL;
        size_t tmpMsgSize = (size_t)SIZE(msg)+1;
        int64_t ticks = time_usec();

        tmpMsg = (char *)alloca(tmpMsgSize);

        memset(tmpMsg, 0, tmpMsgSize);

        memcpy(tmpMsg, msg->bytes, tmpMsgSize-1);

        printf("%ld: (%ld) %s\n", ticks, tmpMsgSize, tmpMsg);

        *failed = 0;

        returnedValue = nilObject;
    }

        break;

    case 200: /* this is a set of primitives for socket handling */
        subPrim = integerValue(args->data[0]);

        /* 200-250 socket handling */
        switch(subPrim) {
        case 0: /* open a TCP socket */
            sock = socket(PF_INET,SOCK_STREAM,0);

            if(sock == -1) {
                sysError("Cannot open TCP socket.");
            }

            sock_opt = 1;

            if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&sock_opt, sizeof(sock_opt))) {
                close(sock);
                sysError("Error setting socket reuse option!");
            }

            /* return the value anyway */
            returnedValue = newInteger(sock);

            break;

        case 1: /* accept on a socket */
            sock = integerValue(args->data[1]);

            if(listen(sock, 10) == -1) {
                sysError("Error listening on TCP socket.");
            }

            /* set the maximum size */
            myAddrSize = sizeof(myAddr);

            /* printf("accept(%d, %p, %d)\n", sock, &myAddr, myAddrSize); */

            sock = accept(sock, &myAddr, &myAddrSize);
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
            getUnixString(netBuffer, sizeof(netBuffer)-1, args->data[2]);
            port = integerValue(args->data[3]);

            printf("Socket: %d\n",sock);
            printf("IP: %s\n",netBuffer);
            printf("Port: %d\n",port);

            /* convert the string IP to a network representation */
            if(inet_aton((const char *)netBuffer,&iaddr) == 0) {
                sysError("Illegal address passed to bind primitive.");
            }

            /* build a sockaddr_in struct */
            sin.sin_family = AF_INET;
            sin.sin_port = htons((u_int16_t)port);
            /*			sin.sin_port = (u_int16_t)0;*/
            sin.sin_addr = iaddr;

            if(bind(sock,(struct sockaddr *)&sin,sizeof(sin)) == -1) {
                sysError("Cannot bind TCP socket to address.");
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
                printf("socket read returned an error: %d (%d)!\n", i, errno);
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
            sysErrorInt("Unknown socket primitive operation: ",subPrim);
            break;
        }

        break;

    default:
        sysErrorInt("unknown primitive", primitiveNumber);
    }
    return(returnedValue);
}




void getUnixString(char * to, int size, struct object * from)
{
    int i;
    int fsize = SIZE(from);
    struct byteObject * bobj = (struct byteObject *) from;

    if (fsize >= size) {
        sysErrorInt("getUnixString(): String too long for buffer!", fsize);
    }

    for (i = 0; i < fsize; i++) {
        to[i] = (char)bobj->bytes[i];
    }

    to[i] = '\0';	/* put null terminator at end */
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
    newStr->class = StringClass;

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

struct object * urlToString(struct byteObject * from)
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
    newStr->class = StringClass;

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
