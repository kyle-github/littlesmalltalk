" class definition for ArgumentNode "
+ParserNode subclass: #ArgumentNode variables: #( position ) classVariables: #( )
" class methods for ArgumentNode "
" instance methods for ArgumentNode "
!ArgumentNode
compile: encoder block: inBlock
    super compile: encoder.
    position = 0
        ifTrue: [ encoder genHigh: 2 low: 0 ]
        ifFalse: [ encoder genHigh: 2 low: position - 1 ]

!
!ArgumentNode
isSuper
    ^ position = 0

!
!ArgumentNode
position: p
    position <- p

!
