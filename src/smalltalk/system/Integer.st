" class definition for Integer "
+Number subclass: #Integer variables: #( ) classVariables: #( )
" class methods for Integer "
=Integer
new: low
    <32 low>
    low <- low asSmallInt.
    <32 low>
    self primitiveFailed

!
" instance methods for Integer "
!Integer
* arg
    <28 self arg>
    (arg isMemberOf: Integer) ifFalse: [^self * arg asInteger].
    self primitiveFailed

!
!Integer
+ arg
    <27 self arg>
    (arg isMemberOf: Integer) ifFalse: [^self + arg asInteger].
    self primitiveFailed

!
!Integer
- arg
    <29 self arg>
    (arg isMemberOf: Integer) ifFalse: [^self - arg asInteger].
    self primitiveFailed

!
!Integer
< arg
    <30 self arg>
    (arg isMemberOf: Integer) ifFalse: [^self < arg asInteger].
    self primitiveFailed

!
!Integer
= arg
    <31 self arg>
    (arg isMemberOf: Integer) ifFalse: [^self = arg asInteger].
    self primitiveFailed

!
!Integer
asInteger
    ^self

!
!Integer
asSmallInt
    <33 self>.
    self overflow

!
!Integer
hash
    <33 self>.
    ^ (self rem: 65536) asSmallInt

!
!Integer
quo: arg
    <25 self arg>
    (arg isMemberOf: Integer) ifFalse: [^self quo: arg asInteger].
    (0 = arg) ifTrue: [^ self error: 'division by zero'].
    self primitiveFailed

!
!Integer
rem: arg
    <26 self arg>
    (arg isMemberOf: Integer) ifFalse: [^self rem: arg asInteger].
    (0 = arg) ifTrue: [^ self error: 'division by zero'].
    self primitiveFailed

!
!Integer
truncSmallInt
    <40 self>.
    self primitiveFailed

!
