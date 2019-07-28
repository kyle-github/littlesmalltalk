" class definition for SmallInt "
+Number subclass: #SmallInt variables: #( ) classVariables: #( seed )
" class methods for SmallInt "
=SmallInt
atRandom
    " Set up seed one time.  TBD: init from something external;
      getpid() or time() "
    seed isNil ifTrue: [ seed <- 17 ].

    " Rotate the random number generator. "
    seed <- ((seed * 1103515245 + 12345) truncSmallInt)
        bitAnd: 268435455.
    ^ seed

!
" instance methods for SmallInt "
!SmallInt
* arg
    <15 self arg>
    (arg isMemberOf: SmallInt) ifFalse: [^self * arg asSmallInt].
    self primitiveFailed

!
!SmallInt
+ arg
    <10 self arg>
    (arg isMemberOf: SmallInt) ifFalse: [^self + arg asSmallInt].
    self primitiveFailed

!
!SmallInt
- arg
    <16 self arg>
    (arg isMemberOf: SmallInt) ifFalse: [^self - arg asSmallInt].
    self primitiveFailed

!
!SmallInt
< arg
    <13 self arg>
    (arg isMemberOf: SmallInt) ifFalse: [^self < arg asSmallInt].
    self primitiveFailed

!
!SmallInt
= arg
    <14 self arg>
    (arg isMemberOf: SmallInt) ifFalse: [^self = arg asSmallInt].
    self primitiveFailed

!
!SmallInt
asInteger
    ^Integer new: self

!
!SmallInt
asSmallInt
    ^self

!
!SmallInt
bitAnd: arg
    <37 self arg>.
    ^ (self bitAnd: arg asSmallInt)

!
!SmallInt
bitOr: arg
    <36 self arg>.
    ^ (self bitOr: arg asSmallInt)

!
!SmallInt
bitShift: arg
    <39 self arg>.
    (arg isKindOf: SmallInt) ifTrue: [ self overflow ].
    ^ (self bitShift: arg asSmallInt)

!
!SmallInt
hash
    ^ self

!
!SmallInt
quo: arg
    <11 self arg>
    (arg isMemberOf: SmallInt) ifFalse: [^self quo: arg asSmallInt].
    (0 = arg) ifTrue: [^ self error: 'division by zero'].
    self primitiveFailed

!
!SmallInt
rem: arg
    <12 self arg>
    (arg isMemberOf: SmallInt) ifFalse: [^self rem: arg asSmallInt].
    (0 = arg) ifTrue: [^ self error: 'division by zero'].
    self primitiveFailed

!
!SmallInt
truncSmallInt
    ^self

!
