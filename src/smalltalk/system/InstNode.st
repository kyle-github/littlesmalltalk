" class definition for InstNode "
+ParserNode subclass: #InstNode variables: #( position ) classVariables: #( )
" class methods for InstNode "
" instance methods for InstNode "
!InstNode
assign: encoder
    encoder genHigh: 6 low: position - 1

!
!InstNode
assignable
    ^ true

!
!InstNode
compile: encoder block: inBlock
    super compile: encoder.
    encoder genHigh: 1 low: position - 1

!
!InstNode
position: p
    position <- p

!
