#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "memory.h"
#include "globals.h"


#define OK (1)


typedef struct {
    FILE *src_file;
    char *class_name;
    char *parent_class_name;
    char *instance_var_str;
    char *class_var_str;
} class_source;


static class_source **get_sources(char **source_files, int num_sources)
static void open_source_file(class_source *source, const char *class_file_name);
static void sort_source_files(class_source **st_sources, int num_sources);

static char *get_delimited_token(char **str, char start_char, char end_char);
static void error(const char *templ, ...);
static void info(const char *templ, ...);






int main(int argc, char **argv)
{
    class_source **st_sources = NULL;
    int num_sources = argc-1;
    char **source_files = argv;

    info("Starting up....");

    st_sources = get_sources(argc - 1, argv++);


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

