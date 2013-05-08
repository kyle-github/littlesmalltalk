
The image format is described here.  This is different from
the image format of the original LST 4 for several reasons.
The primary reason was that the old image format, while simple
and fairly clean, did not allow SmallInts to be very large.
The new format uses a more compact direct binary representation
of integer values such as sizes or the value of a SmallInt.

Each object starts with a header.  This is a type tag followed by
a size or value parameter.  If the size or value is between
0 and 15 (inclusive), then the whole header is one byte. The
type section is the top three bits.  If the size or value is
greater than 15, then the number of bytes needed to express
it is stored in the lower four bits of the header and the
following bytes are the size or value.  This allows for very
long fields in practice.

	Header:

	ttt0vvvv	if 0<=val<=15

	ttt1ssss	if val>15, then header is followed
			by ssss bytes that contain the value.

Examples:

A string object that contains 12 bytes with "hello, world"
in them would be this in the image:

Header	011  0  1010
	|   |    |
	|   |    +--- length = 12
	|   |
	|   +-------- short header
	|
	+------------ byte array type

Data 'hello, world'

A string that was longer, like 'hello there, world' would
be stored as follows in the image:

Header	011  1  0001	00010010
	|   |    |         |
	|   |    |         +-- length = 18
	|   |    |
	|   |    +------------ length of length = 1 byte
	|   |
	|   +----------------- long header
	|
	+--------------------- byte array type

Data 'hello there, world'

Use of this format made images about 10% smaller.  Integers
are stored by storing the value of the integer as the value
in the header.  If the integer is less than 16, then it will
be stored in one byte.  An integer between 16 and 255 will be
stored in two bytes.  One that is between 256 and 65535 will
be stored in three bytes etc.

For byte arrays, the value is used as the size of the array.
For "normal" objects, the value is used as the number of
instance variables.

For objects that reference an object that has been filed out
already, the value is used as the index of the object in the
image.  I.e. if the value in an instance variable is an object
that was the fourth one filed out, the value 4 will be used
with a type tag that indicates it was a "previous" object.

See the values defined near the end of lst_memory.h.

The current types are:

0	(0<<5)	error
32	(1<<5)	Standard object
64	(2<<5)	Small integer
96	(3<<5)	byte array
128	(4<<5)	previously seen object
160	(5<<5)	nil

Note that the last one is not necessarily useful anymore since
the nil object is the first one filed out.  In that case, the
previously seen object tag with an index of zero would be fine.
This may be worth optimizing later, but since it currently takes
only one byte either way, it probably isn't worth it.

Note that the code in lst_image_builder.c to write out images replicates
that in lst_memory.c in large part.  The differences are somewhat small,
but profound.  First, the array that is used to note the positions of
object that have been filed out is a statically allocated array, not
one of the object spaces.  Second, and much more difficult to deal with
is the fact that lst_image_builder does _NOT_ use the same bit flags
in the flags field in the object size.  The runtime uses one of the bits
to indicate that the object is a binary object (i.e. it does not contain
pointers) and the other for garbage collection.  The lst_image_builder
code does not do GC during image building.  It uses the two-bit field
to indicate more precisely the type of the object.  A value of 1 in this
field indicates a byte array (such as a ByteArray or String object).  A
value of 3 in this field indicates an integer (SmallInt to be precise).

The difference between the meanings of the type field is a real pain
as it requires the image saving code to be basically duplicated in
both lst_memory.c and lst_image_builder.c.





