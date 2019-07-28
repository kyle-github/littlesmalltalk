" class definition for ReturnNode "
+ParserNode subclass: #ReturnNode variables: #( expression ) classVariables: #( )
" class methods for ReturnNode "
" instance methods for ReturnNode "
!ReturnNode
compile: encoder block: inBlock
    super compile: encoder.
    expression compile: encoder block: inBlock.
    inBlock
        ifTrue: [ encoder genHigh: 15 low: 3 " block return " ]
        ifFalse: [ encoder genHigh: 15 low: 2 " stack return " ]

!
!ReturnNode
expression: e
    expression <- e

!
