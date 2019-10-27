/*
  Copyright (c) Kyle Hayes, 2019.

*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "globals.h"

/*
 * We use signalling NaNs.   Non-signalling NaNs can be actual values produced when doing
 * things like dividing by zero.   So we use signalling.
 *
 * NaN format:
 *                               Bit Position
 * 6 6665555 5555 5 544 44444444 33333333 33222222 22221111 11111100 00000000
 * 3 2109876 5432 1 098 76543210 98765432 10987654 32109876 54321098 76543210
 * s eeeeeee|eeee m mmm|mmmmmmmm|mmmmmmmm|mmmmmmmm|mmmmmmmm|mmmmmmmm|mmmmmmmm
 *
 * s = sign bit (1 bit)
 * e = exponent (11 bits)
 * m = mantissa (52 bits)
 *
 * bit 51 being zero makes this a signalling NaN.  The rest of the mantissa must not be all zero bits!
 *
 * break into two groups:
 *
 * object headers - always what an oop points to.
 * field values - the contents of an object.
 *
 * Headers carrier info about the object:
 *   - class ID
 *   - object size.
 *
 * Field values are NaN encoded values.
 *   - a non-signalling NaN
 *   - a valid double floating IEEE 754 64-bit floating point value
 *   - a NaN encoded value:
 *      - bit 63, bit 50
 *           0,      0  - 50 bits of pointer to an object, must not be NULL.
 *           0,      1  - 32-bit SmallInt.
 *           1,      0  - 50 bits of pointer to an external object, must not be NULL.
 *           1,      1  - 32-bit Symbol.
 *
 */




#define FIELD_MASK      (0xFFFA000000000000ULL)
#define DOUBLE_VAL      (0x0000000000000000ULL)
#define OOP_VAL         (0x7FF8000000000000ULL)
#define SMALLINT_VAL    (0x7FFA000000000000ULL)
#define EXTERN_PTR_VAL  (0xFFF8000000000000ULL)
#define SYMBOL_VAL      (0xFFFA000000000000ULL)
#define PTR_MASK       (~0xFFFA000000000000ULL)
#define INT_MASK        (0x00000000FFFFFFFFULL)

typedef enum { DOUBLE_TYPE, OOP_TYPE, SMALLINT_TYPE, EXTERN_PTR_TYPE, SYMBOL_TYPE } field_type_t;


typedef int32_t smallint;
typedef int32_t symbol;


/*
 * all allocated objects have a header that
 * describes what kind it is, a class, and
 * how big it is.
 */

typedef struct {
    uint32_t class_id;
    uint32_t size;
} obj_header;


/*
 * A cell is the basic unit of memory.  All of
 * object memory is composed of cells.
 */

typedef union {
    uint8_t b[8];
    double d;
    uint64_t u;
    void *p;
    obj_header h;
} cell;


/*
 * basic object type.
 *
 * All objects have a header, a relocation pointer,
 * and optional fields.
 *
 * This defines the incomplete type in globals.h.
 */

struct oop_t {
    cell header;
    cell relocation_ptr;
    cell fields[];
};



inline static oop get_cell_oop(cell c)
{
    return (oop)(uintptr_t)(c.u & PTR_MASK);
}


inline static oop get_oop_class(oop obj)
{
    /* get the object that the oop points to and return the class from that. */
    return get_cell_oop(Classes->fields[0x7FFFFFFFU & obj->header.h.class_id]);
}

inline static bool is_bytearray(oop obj)
{
    return ((0x10000000 & obj->header.h.class_id) != 0);
}

inline static uint32_t get_oop_size(oop obj)
{
    return obj->header.h.size;
}

inline static void *get_oop_redirect(oop obj)
{
    return obj->relocation_ptr.p; // not encoded.
}

inline static cell get_oop_field(oop obj, int field_index)
{
    return obj->fields[field_index];
}


inline static field_type_t get_cell_type(cell c)
{
    uint64_t val = c.u & FIELD_MASK;
    field_type_t type = DOUBLE_TYPE;

    switch(val) {
        case OOP_VAL: type = OOP_TYPE; break;
        case SMALLINT_VAL: type = SMALLINT_TYPE; break;
        case EXTERN_PTR_VAL: type = EXTERN_PTR_TYPE; break;
        case SYMBOL_VAL: type = SYMBOL_TYPE; break;
        default: type = DOUBLE_TYPE; break;
    }

    return type;
}

inline static oop get_cell_class(cell c)
{
    oop class_result = NULL;

    switch(get_cell_type(c)) {
        case DOUBLE_TYPE: class_result = DoubleClass; break;
        case OOP_TYPE: class_result = get_oop_class(get_cell_oop(c)); break;
        case SMALLINT_TYPE: class_result = SmallIntClass; break;
        case SYMBOL_TYPE: class_result = SymbolClass; break;
        default: class_result = NULL; break;
    }

    return class_result;
}

inline static smallint get_cell_smallint(cell c)
{
    return (smallint)(uint32_t)(c.u & INT_MASK);
}

inline static symbol get_cell_symbol(cell c)
{
    return (symbol)(uint32_t)(c.u & INT_MASK);
}

inline static double get_cell_double(cell c)
{
    return c.d;
}




inline static cell new_smallint_cell(smallint s)
{
    cell c;

    c.u = (SMALLINT_VAL | (uint64_t)(uint32_t)s);

    return c;
}

inline static cell new_oop_cell(oop p)
{
    cell c;

    c.u = (OOP_VAL | (uint64_t)(uintptr_t)p);

    return c;
}

inline static cell new_symbol_cell(symbol s)
{
    cell c;

    c.u = (SYMBOL_VAL | (uint64_t)(uint32_t)s);

    return c;
}

inline static cell new_extern_ptr_cell(void *p)
{
    cell c;

    c.u = (EXTERN_PTR_VAL | (uint64_t)(uintptr_t)p);

    return c;
}

inline static cell new_double_cell(double d)
{
    cell c;

    c.d = d;

    return c;
}
