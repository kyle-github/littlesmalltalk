" class definition for BlockNode "
+ParserNode subclass: #BlockNode variables: #( statements temporaryLocation ) classVariables: #( )
" class methods for BlockNode "
" instance methods for BlockNode "
!BlockNode
compile: encoder block: inBlock | patchLocation |
    super compile: encoder.
    encoder genHigh: 12 low: temporaryLocation.
    patchLocation <- encoder genVal: 0.
    self compileInLine: encoder block: true.
    encoder genHigh: 15 low: 2. " return top of stack "
    encoder patch: patchLocation

!
!BlockNode
compileInLine: encoder block: inBlock
    statements reverseDo:
        [ :stmt | stmt compile: encoder block: inBlock.
            encoder genHigh: 15 low: 5 " pop top " ].
    encoder backUp

!
!BlockNode
isBlock
    ^ true

!
!BlockNode
statements: s temporaryLocation: t
    statements <- s.
    temporaryLocation <- t

!
