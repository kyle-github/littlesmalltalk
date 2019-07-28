" class definition for PrimitiveNode "
+ParserNode subclass: #PrimitiveNode variables: #( number arguments ) classVariables: #( )
" class methods for PrimitiveNode "
" instance methods for PrimitiveNode "
!PrimitiveNode
compile: encoder block: inBlock | argsize |
    argsize <- arguments size.
    super compile: encoder.
    encoder pushArgs: argsize.
    arguments reverseDo: [ :a | a compile: encoder block: inBlock ].
    encoder genHigh: 13 low: argsize.
    encoder genCode: number.
    encoder popArgs: argsize

!
!PrimitiveNode
number: n arguments: a
    number <- n.
    arguments <- a.

!
