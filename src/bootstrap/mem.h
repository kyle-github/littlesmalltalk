/*
  Copyright (c) Kyle Hayes, 2019.

*/

#pragma once

#include <stdint.h>

/*
 * We use signalling NaNs.   Non-signalling NaNs can be actual values produced when doing
 * things like dividing by zero.   So we use signalling.
 *
 * NaN format:
 *
 *                               Bit Position
 * 6 6665555 5555 5 544 44444444 33333333 33222222 22221111 11111100 00000000
 * 3 2109876 5433 2 098 76543210 98765432 10987654 32109876 54321098 76543210
 * s eeeeeee|eeee 0 mmm|mmmmmmmm|mmmmmmmm|mmmmmmmm|mmmmmmmm|mmmmmmmm|mmmmmmmm
 *
 * s = sign bit (1 bit)
 * e = exponent (11 bits)
 * m = mantissa (51 bits)
 *
 * bit 52 being zero makes this a signalling NaN.
 *
 * So we have the following valid ranges of IEEE 64-bit floats:
 *
 * So the mask to determine if we have a signalling NaN is:
 *
 * 0x7FF4000000000000ULL
 *
 * Now we get two places to decode the value encoded as a NaN.   We will use the
 * following:
 *
 *  - if the value has the high bit set, then we have a pointer:
 *
 * const uin64_t
 *
 * if(val &
 *
 */

typedef int64_t smallint;



typedef union {
    uint8_t b[8];
    double d;
    uint64_t u;
    void *p;   // FIXME make this a cell pointer.
} cell;

#define NAN_MASK    (0xFFFA000000000000ULL)

#define CELL_PTR_MASK       (0x7FF4000000000000ULL)
#define CELL_HEADER_MASK    (0xFFF4000000000000ULL)

typedef enum { PTR_CELL, HEADER_CELL, DOUBLE_CELL } cell_type_t;

inline static cell_type_t cell_type(cell val)
{
    uint64_t tmp = val.u & NAN_MASK;

    if(tmp == CELL_PTR_MASK)  { return PTR_CELL; }
    else if(tmp == CELL_HEADER_MASK) { return HEADER_CELL; }
    else {return DOUBLE_CELL; }
}

inline static cell double_to_cell(double d)
{
    cell c;

    c.d = d;

    return d;
}

inline static double cell_to_double(cell c)
{
    return c.d;
}


inline static cell ptr_to_cell(void *p)
{
    cell c;

    c.p = p;

    c.u = ((c.u & (~NAN_MASK)) | CELL_PTR_MASK);

    return c;
}

inline static void *cell_to_ptr(cell c)
{
    cell tmp = c;

    tmp.u = (c.u & (~NAN_MASK));

    return tmp.p;
}



typedef enum {
    OBJECT_HEADER = 0x00,
    CLASS_HEADER = 0x01,
    BYTESTRING_HEADER = 0x02
    UNUSED_HEADER = 0x03
} header_type_t;



typedef struct {
    uint8_t header_type;
    uint16_t class_id;
    uint32_t size;
} header;


inline static cell header_to_cell(header h)
{
    cell c;

    c.u = CELL_HEADER_MASK | ((uint64_t)h.header_type << 48) | (((uint64_t)h.class_id) << 32) | ((uint64_t)h.size);

    return cell;
}

inline static header cell_to_header(cell c)
{
    header h;

    h.header_type = (c.u >> 48) & 0x03U;
    h.class_id = (c.u >> 32) & 0xFFFFU;
    h.size = (c.u & 0xFFFFFFFFU);

    return h;
}


