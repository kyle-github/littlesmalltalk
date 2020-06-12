" class definition for Encoder "
+Object subclass: #Encoder variables: #( name byteCodes index literals stackSize maxStack ) classVariables: #( )
" class methods for Encoder "
" instance methods for Encoder "
!Encoder
backUp
    " back up one instruction "
    index <- index - 1

!
!Encoder
currentLocation
    ^ index

!
!Encoder
expandByteCodes	| newarray size |
    size <- byteCodes size.
    newarray <- ByteArray new: size + 8.
    1 to: size do: [:i | newarray at: i put: (byteCodes at: i)].
    byteCodes <- newarray

!
!Encoder
genCode: byte
    index <- index + 1.
    (index >= byteCodes size)
        ifTrue: [ self expandByteCodes].
    byteCodes at: index put: byte.
    ^ index

!
!Encoder
genHigh: high low: low
    (low >= 16)
        ifTrue: [ self genHigh: 0 low: high. self genCode: low ]
        ifFalse: [ self genCode: high * 16 + low ]

!
!Encoder
genLiteral: aValue | idx |
    idx <- literals indexOf: aValue.
    idx notNil ifTrue: [ ^ idx - 1 ].
    literals <- literals with: aValue.
    ^ literals size - 1

!
!Encoder
genVal: byte
    self genCode: (byte rem: 256).
    self genCode: (byte quo: 256).
    ^ index-1

!
!Encoder
lineNum: l
    " Don't care, except in DebugEncoder subclass "

!
!Encoder
method: maxTemps class: c text: text
    ^ Method name: name byteCodes: byteCodes literals: literals
        stackSize: maxStack temporarySize: maxTemps class: c
        text: text

!
!Encoder
name: n
    name <- n asSymbol.
    byteCodes <- ByteArray new: 20.
    index <- 0.
    literals <- Array new: 0.
    stackSize <- 0.
    maxStack <- 1.

!
!Encoder
patch: loc
        " patch a goto from a block "
    byteCodes at: loc put: (index rem: 256).
    byteCodes at: (loc + 1) put: (index quo: 256)

!
!Encoder
popArgs: n
    stackSize <- stackSize - n.

!
!Encoder
pushArgs: n
    stackSize <- stackSize + n.
    maxStack <- stackSize max: maxStack

!
