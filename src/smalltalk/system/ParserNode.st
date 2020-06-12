" class definition for ParserNode "
+Object subclass: #ParserNode variables: #( lineNum ) classVariables: #( )
" class methods for ParserNode "
=ParserNode
at: l | ret |
    ret <- super new.
    self in: ret at: 1 put: l.
    ^ ret

!
=ParserNode
new
    self error: 'Must use at: for creation'

!
" instance methods for ParserNode "
!ParserNode
assignable
    ^ false

!
!ParserNode
compile: encoder
    encoder lineNum: lineNum

!
!ParserNode
isBlock
    ^ false

!
!ParserNode
isSuper
    ^ false

!
