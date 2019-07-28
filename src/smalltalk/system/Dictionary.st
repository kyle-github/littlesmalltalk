" class definition for Dictionary "
+Collection subclass: #Dictionary variables: #( keys values ) classVariables: #( )
" class methods for Dictionary "
=Dictionary
new | newDict |
    newDict <- super new.
    self in: newDict at: 1 put: (OrderedArray new: 0).
    self in: newDict at: 2 put: (Array new: 0).
    ^ newDict

!
" instance methods for Dictionary "
!Dictionary
at: key
    ^ self at: key ifAbsent: [ self noKey ]

!
!Dictionary
at: key ifAbsent: exceptionBlock | position |
    position <- keys location: key.
    ((position <= keys size) and: [ key = (keys at: position)])
        ifTrue: [ ^ values at: position ]
        ifFalse: [ ^ exceptionBlock value ]

!
!Dictionary
at: key put: value | position |
    position <- keys location: key.
    (position <= keys size and: [ key = (keys at: position)])
        ifTrue: [ values at: position put: value ]
        ifFalse: [ keys <- keys insert: key at: position.
            values <- values insert: value at: position ].
    ^ value

!
!Dictionary
binaryDo: aBlock
    1 to: keys size do:
        [:i | aBlock value: (keys at: i) value: (values at: i) ]

!
!Dictionary
do: aBlock
    values do: aBlock

!
!Dictionary
isEmpty
    ^ keys isEmpty

!
!Dictionary
keysAsArray | i ret |
    ret <- Array new: keys size.
    1 to: keys size do: [:i| ret at: i put: (keys at: i)].
    ^ ret

!
!Dictionary
keysDo: aBlock
    1 to: keys size do: [:i| aBlock value: (keys at: i)]

!
!Dictionary
noKey
    self error: 'key not found in dictionary lookup'

!
!Dictionary
printString | count res |
    res <- self class printString + ' ('.
    count <- 0.
    self binaryDo: [:k :elem|
        (count = 0) ifFalse: [ res <- res + ', ' ].
        res <- res + (k printString + ' -> ' + elem printString).
        count <- count + 1.
        (count >= 20) ifTrue: [ ^ res + ', ...)' ]
    ].
    ^ res + ')'

!
!Dictionary
removeKey: key
    ^ self removeKey: key ifAbsent: [ self noKey ]

!
!Dictionary
removeKey: key ifAbsent: exceptionBlock | position |
    position <- keys location: key.
    (position <= keys size and: [ key = (keys at: position) ])
        ifTrue: [ keys <- keys removeIndex: position.
            values <- values removeIndex: position]
        ifFalse: [ ^ exceptionBlock value ]

!
