" class definition for Block "
+Context subclass: #Block variables: #( argumentLocation creatingContext oldBytePointer ) classVariables: #( )
" class methods for Block "
" instance methods for Block "
!Block
argCount
    self error: 'Incorrect argument passing to Block'

!
!Block
backtrace | narg |
        " backtrace context calls "
    'block from ' print. method name print.
    '(' print.
    narg <- 0.
    arguments do: [:a |
        (narg > 0) ifTrue: [', ' print ].
        a class print.
        narg <- narg+1
    ].
    ')' printNl.
    previousContext notNil
        ifTrue: [ previousContext backtrace ]

!
!Block
value
    " start block execution "
    <8 self>
    (self argCount)

!
!Block
value: a
    " start block execution "
    <8 a self>
    (self argCount)

!
!Block
value: a value: b
    " start block execution "
    <8 a b self>
    (self argCount)

!
!Block
whileFalse: aBlock
    self value ifFalse: [ aBlock value. ^ self whileFalse: aBlock ]

!
!Block
whileTrue: aBlock
    self value ifTrue: [ aBlock value. ^ self whileTrue: aBlock ]

!
