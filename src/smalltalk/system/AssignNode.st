" class definition for AssignNode "
+ParserNode subclass: #AssignNode variables: #( target expression ) classVariables: #( )
" class methods for AssignNode "
" instance methods for AssignNode "
!AssignNode
compile: encoder block: inBlock
    super compile: encoder.
    expression compile: encoder block: inBlock.
    target assign: encoder

!
!AssignNode
target: t expression: e
    target <- t.
    expression <- e

!
