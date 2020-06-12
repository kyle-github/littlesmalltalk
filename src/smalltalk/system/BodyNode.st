" class definition for BodyNode "
+ParserNode subclass: #BodyNode variables: #( statements ) classVariables: #( )
" class methods for BodyNode "
" instance methods for BodyNode "
!BodyNode
compile: encoder block: inBlock
    super compile: encoder.
    statements reverseDo:
        [ :stmt | stmt compile: encoder block: inBlock.
            encoder genHigh: 15 low: 5 " pop "].
    encoder genHigh: 15 low: 1 " return self "

!
!BodyNode
statements: s
    statements <- s

!
