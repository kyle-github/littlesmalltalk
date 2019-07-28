" class definition for Magnitude "
+Object subclass: #Magnitude variables: #( ) classVariables: #( )
" class methods for Magnitude "
" instance methods for Magnitude "
!Magnitude
<= arg
    ^ self < arg or: [ self = arg ]

!
!Magnitude
> arg
    ^ arg < self

!
!Magnitude
>= arg
    ^ (self > arg) or: [ self = arg ]

!
!Magnitude
between: low and: high
    ^ low <= self and: [ self <= high ]

!
!Magnitude
max: arg
    ^ self < arg ifTrue: [ arg ] ifFalse: [ self ]

!
!Magnitude
min: arg
    ^ self < arg ifTrue: [ self ] ifFalse: [ arg ]

!
