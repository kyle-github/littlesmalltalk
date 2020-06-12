" class definition for OrderedArray "
+Array subclass: #OrderedArray variables: #( ) classVariables: #( )
" class methods for OrderedArray "
" instance methods for OrderedArray "
!OrderedArray
add: value
    ^ self insert: value at: (self location: value)

!
!OrderedArray
includes: value | position |
    position <- self location: value.
    ^ (position <= self size) and: [ value = (self at: position)]

!
!OrderedArray
location: value | low high mid |
    low <- 1.
    high <- self size + 1.
    [ low < high ] whileTrue:
        [ mid <- (low + high) quo: 2.
        (self at: mid) < value
            ifTrue: [ low <- mid + 1 ]
            ifFalse: [ high <- mid ] ].
    ^ low

!
