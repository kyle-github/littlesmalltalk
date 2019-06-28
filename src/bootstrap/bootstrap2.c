#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>


#define OK (1)

struct method_def {
    struct method_def *sibling;

    char *name;
    char *text;
};

struct class_def {
    struct class_def *parent;
    struct class_def *sibling;
    struct class_def *children;

    char *filename;

    char *name;
    char *superclass;
    char *inst_vars;
    char *class_vars;
};


static void error(const char *templ, ...);
static void info(const char *templ, ...);
static int init_classes(int num_classes, char **class_filenames, struct class_def **classes);
static int load_class_files(struct class_def **classes);
static int read_class_file(FILE *class_source, struct class_def *def);



int main(int argc, char **argv)
{
    int rc = 0;
    struct class_def **classes = NULL;

    if(!init_classes(argc, argv, classes)) {
        error("Cannot initialize class information!");
    }

    if((rc = load_class_files(classes)) != OK) {
        error("Cannot load source files!");
    }

    return 0;
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




int init_classes(int num_classes, char **class_filenames, struct class_def **classes)
{

    if((*classes = calloc(sizeof (struct class_def *), (size_t)num_classes + 1)) == NULL) {
        error("Cannot allocate class array!");
    }

    for(int i = 0; i < num_classes; i++) {
        char name_buf[128] = {0,};
        sscanf(class_filenames[i], "%s.st", name_buf);

        classes[i] = calloc(1, sizeof (struct class_def));

        if(!(classes[i])) {
            error("Unable to allocate memory for a class def struct!");
        }

        classes[i]->name = strdup(name_buf);
        classes[i]->filename = strdup(class_filenames[i]);

        if(!(classes[i])->name) {
            error("Unable to allocate memory for class name!");
        }

        info("Created class def entry for class %s.", name_buf);
    }

    return OK;
}



int load_class_files(struct class_def **classes)
{
    for(int i=0; classes[i] != NULL; i++) {
        FILE *class_source = fopen(classes[i]->filename,"r");
        if(!class_source) {
            error("Cannot open class source file %s!", classes[i]->filename);
        }

        if(!read_class_file(class_source, classes[i])) {
            error("Cannot process class source file %s!", classes[i]->filename);
        }

        fclose(class_source);
    }

    return OK;
}




int read_class_file(FILE *class_source, struct class_def *def)
{
    if(!read_class_def_header(class_source, def)) {
        error("Cannot find/process class definition information from class source file %s!", def->filename);
    }

    while(!feof(class_source)) {
        if(!read_method_def(class_source, def)) {
            error("Cannot process method from class source file %s!", def->filename);
        }
    }

    return OK;
}



int read_class_def_header(FILE *class_source, struct class_def *def)
{
    char parent_class[128] = {0,};
    char class_name[128] = {0,};
    char inst_vars[300] = {0, };
    char
    /* skip whitespace/comments until we find the + part.
     *
     * Example:
     *   +Collection subclass: #Array variables: #( ) classVariables: #( )
     */
    line = get_line(class_source);
    while(line && line[0] != '+') {
        line = get_line(class_source);
    }

    if(sscanf(line, "+%s subclass: #%s variables: #(%s) classVariables: #(%s)", parent_class, class_name, inst_vars, class_vars) != 4) {
        error("Incomplete class definition header in class source file %s!", def->filename);
    }

    if(strcmp(def->name, class_name) != 0) {
        error("Class source file %s contains definition for class %s!", def->filename, class_name);
    }

    def->superclass = strdup(parent_class);
    def->inst_vars = strdup(inst_vars);
    def->class_vars = strdup(class_vars);

}