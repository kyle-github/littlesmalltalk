" class definition for TemporaryNode "
+ParserNode subclass: #TemporaryNode variables: #( position ) classVariables: #( )
" class methods for TemporaryNode "
" instance methods for TemporaryNode "
!TemporaryNode
assign: encoder
    encoder genHigh: 7 low: position - 1

!
!TemporaryNode
assignable
    ^ true

!
!TemporaryNode
compile: encoder block: inBlock
    super compile: encoder.
    encoder genHigh: 3 low: position - 1

!
!TemporaryNode
position: p
    position <- p

!
