# LittleSmalltalk VM Overview

This document provides an overview of the major flow of control, common data types, and a detailed explanation of the code in the `src/vm` directory, which implements a small version of the Smalltalk programming language.

## Major Flow of Control

The VM is responsible for loading, saving, and managing Smalltalk objects in memory. The main flow of control involves:

1. **Image File Operations**
   - **Loading (fileIn)**: Reads a saved image file, reconstructs objects, and sets up the runtime environment.
   - **Saving (fileOut)**: Serializes the current state of objects to an image file for persistence.

2. **Object Management**
   - Objects are created, linked, and managed in memory. The VM handles both ordinary objects and special cases (e.g., SmallInt, ByteArray).
   - Indirection arrays (`indirArray`) are used to track objects during serialization/deserialization.

3. **Garbage Collection and Memory Management**
   - Memory is managed in contiguous spaces. The VM uses pointer arithmetic and offset calculations to relocate objects when loading images.

4. **Class and Global Object Initialization**
   - Core classes and global objects (e.g., `nil`, `true`, `false`, `ArrayClass`, etc.) are initialized and added to the static root set for easy access.

## Common Data Types

- **struct object**: The fundamental type representing Smalltalk objects. Contains pointers to class and instance data.
- **struct byteObject**: A subtype of `struct object` for byte arrays, used for objects with raw byte data.
- **Object Arrays**: Arrays of pointers to `struct object`, used for indirection and tracking during image operations.
- **SmallInt**: Special representation for small integers, handled differently from pointer-based objects.
- **Global Objects**: Pointers to core objects and classes (e.g., `nilObject`, `trueObject`, `globalsObject`, etc.).

## Detailed Code Overview

### Image File Operations

- **fileIn(FILE *fp)**: Reads the image version and dispatches to the appropriate loader (`fileIn_version_0`, `fileIn_version_1`, etc.). Initializes indirection arrays and reconstructs objects.
- **fileOut(FILE *fp)**: Serializes the current state of the VM to an image file using the latest supported format.
- **objectRead(FILE *fp)**: Reads an object from the image file, handling different types (ordinary, SmallInt, ByteArray, previous object, nil).
- **objectWrite(FILE *fp, struct object *obj)**: Writes an object to the image file, handling special cases and tracking written objects.

### Indirection and Object Tracking

- **indirArray**: An array of pointers to objects used to track objects during serialization/deserialization. Prevents duplicate writes and supports reference resolution.
- **indirtop**: Index into `indirArray`, tracks the number of objects processed.

### Tagging and Serialization

- **writeTag/readTag**: Functions for writing and reading object tags, which encode type and value/size information in a compact format.
- **getIntSize**: Determines the number of bytes needed to store an integer value.

### Memory Management

- **fix_offset**: Adjusts object pointers when loading an image to account for differences in memory layout.
- **object_fix_up**: Recursively fixes up pointers within objects after loading raw image data.

### Class and Global Initialization

- **lookupGlobal**: Retrieves core objects and classes from the global dictionary after loading.
- **addStaticRoot**: Adds objects to the static root set to prevent them from being garbage collected.

### Error Handling and Debugging

- **error/info**: Macros or functions for reporting errors and informational messages.
- **Debug Output**: Extensive use of `fprintf` and `printf` for tracing image operations and object initialization.

## Summary of Key Files in `src/vm`

- **image.c**: Implements image file loading/saving, object serialization/deserialization, and memory fix-up routines.
- **memory.c/memory.h**: Manages memory allocation, garbage collection, and pointer arithmetic.
- **interp.c/interp.h**: Implements the Smalltalk bytecode interpreter and execution engine.
- **globals.c/globals.h**: Defines and manages global objects and classes.
- **err.c/err.h**: Error handling and reporting utilities.

## Example: Object Serialization Flow

1. **Saving an Object**
   - `objectWrite(fp, obj)`
     - Checks if the object is a SmallInt or ByteArray.
     - Writes a tag and serializes the object/class/data.
     - Tracks written objects in `indirArray`.

2. **Loading an Object**
   - `objectRead(fp)`
     - Reads a tag to determine type and size.
     - Allocates and reconstructs the object.
     - Resolves references using `indirArray`.

3. **Fixing Up Pointers**
   - After loading raw image data, `object_fix_up` and `fix_offset` adjust pointers to match the current memory layout.

## ASCII Diagram: Object Serialization

```
+-------------------+
| Smalltalk Object  |
+-------------------+
| class pointer     |
| instance data     |
+-------------------+
        |
        v
+-------------------+
| objectWrite/read  |
+-------------------+
| Tag (type/size)   |
| Data/References   |
+-------------------+
        |
        v
+-------------------+
| Image File        |
+-------------------+
```

---

This overview summarizes the architecture and flow of the LittleSmalltalk VM as implemented in the `src/vm` directory. For further details, refer to the source code and comments within each file.

## Function Reference: C Implementation

Below is a summary of the main functions found in the C source files in `src/vm`, their locations, and their purpose:

### image.c

- **fileIn(FILE *fp)**: Entry point for loading an image file. Dispatches to the correct version loader.
- **fileOut(FILE *fp)**: Entry point for saving the current image to a file.
- **fileOut_object(FILE *fp, struct object *obj)**: Saves a specific object to the image file.
- **fileIn_version_0/1/2/3(FILE *fp)**: Loaders for different image file versions, reconstructing objects and VM state.
- **fileOut_object_version_3(FILE *fp, struct object *globs)**: Saves objects in version 3 format.
- **objectWrite(FILE *fp, struct object *obj)**: Serializes an object to the image file, handling references and types.
- **objectRead(FILE *fp)**: Deserializes an object from the image file, reconstructing its structure and references.
- **writeTag(FILE *fp, int type, int val)**: Writes a type/value tag for an object to the image file.
- **readTag(FILE *fp, int *type, int *val)**: Reads a type/value tag from the image file.
- **getIntSize(int val)**: Determines the number of bytes needed to store an integer value.
- **fix_offset(struct object *old, int64_t offset)**: Adjusts object pointers after loading an image to match current memory layout.
- **object_fix_up(struct object *obj, int64_t offset)**: Recursively fixes up pointers within objects after loading raw image data.
- **get_image_version(FILE *fp)**: Reads the image version from the file header.
- **put_image_version(FILE *fp, uint8_t version)**: Writes the image version to the file header.

### interp.c

- **execute(struct object *aProcess, int ticks)**: The main interpreter loop. Executes bytecodes for a process object, handling stack, context, message sends, primitives, and control flow.
- **flushCache(void)**: Clears the method cache, typically after garbage collection.
- **lookupMethod(struct object *selector, struct object *class)**: Finds a method in a class or its superclasses by selector.
- **symbolcomp(struct object *left, struct object *right)**: Compares two symbol objects for method lookup.
- **bulkReplace(struct object *dest, struct object *start, struct object *stop, struct object *src, struct object *repStart)**: Implements the primitive for bulk array replacement.

### memory.c / memory.h

- **gcalloc(size_t size)**: Allocates memory for a new object in the garbage-collected heap.
- **gcialloc(size_t size)**: Allocates memory for a new byte object.
- **do_gc(void)**: Performs garbage collection, reclaiming unused objects.
- **isDynamicMemory(struct object *obj)**: Checks if an object is in dynamic memory.
- **exchangeObjects(struct object *a, struct object *b, int size)**: Exchanges the contents of two arrays.
- **WORDSUP/WORDSDOWN(obj, n)**: Pointer arithmetic helpers for moving up/down in memory by n words.

### globals.c / globals.h

- **addStaticRoot(struct object **obj)**: Adds an object to the static root set to prevent garbage collection.
- **lookupGlobal(const char *name)**: Finds a global object or class by name.

### err.c / err.h

- **error(const char *msg, ...)**: Reports a fatal error and terminates execution.
- **info(const char *msg, ...)**: Prints informational/debug messages.

---

For a complete list and details, see the source files in `src/vm`. This section provides a quick reference to the main functions and their roles in the VM implementation.

## ASCII Diagram: Initial Class Hierarchy and Metaclasses

Below is an ASCII diagram showing the initial set of classes, their parent relationships, and how metaclasses fit in for Little Smalltalk. This illustrates the convoluted bootstrap state of `Object`, `Class`, and their metaclasses:

```
                +-------------------+
                |   UndefinedClass  |
                +-------------------+
                          ^
                          |
                +-------------------+
                |      Object       |
                +-------------------+
                          ^
                          |
                +-------------------+
                |      Class        |
                +-------------------+
                          ^
                          |
                +-------------------+
                |   Metaclass       |
                +-------------------+

Metaclass relationships:

  Each class (e.g., Object, Class) has a metaclass, which is itself an instance of Metaclass.

  +-------------------+      instance of      +-------------------+
  |      Object       |---------------------> |   Object class    |
  +-------------------+                      +-------------------+
           ^                                        ^
           | parentClass                            | parentClass
  +-------------------+      instance of      +-------------------+
  |      Class        |---------------------> |   Class class     |
  +-------------------+                      +-------------------+
           ^                                        ^
           | parentClass                            | parentClass
  +-------------------+      instance of      +-------------------+
  |   Metaclass       |---------------------> | Metaclass class   |
  +-------------------+                      +-------------------+

Bootstrap relationships:

  - Object's class is Object class (a metaclass)
  - Class's class is Class class (a metaclass)
  - Metaclass's class is Metaclass class (a metaclass)
  - Metaclass class's parent is Class class
  - Class class's parent is Object class
  - Object class's parent is UndefinedClass (or nil)

This forms a circular and self-referential structure typical of Smalltalk systems, where classes and metaclasses are themselves objects and instances of other classes.
```
