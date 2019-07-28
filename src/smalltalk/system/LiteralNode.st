" class definition for LiteralNode "
+ParserNode subclass: #LiteralNode variables: #( value ) classVariables: #( )
" class methods for LiteralNode "
" instance methods for LiteralNode "
!LiteralNode
compile: encoder block: inBlock
    super compile: encoder.
    value == nil ifTrue: [ ^ encoder genHigh: 5 low: 10 ].
    value == true ifTrue: [ ^ encoder genHigh: 5 low: 11 ].
    value == false ifTrue: [ ^ encoder genHigh: 5 low: 12 ].
    (((value class == SmallInt) and:
     [value < 10]) and: [value negative not])
        ifTrue: [ ^ encoder genHigh: 5 low: value ].
    encoder genHigh: 4 low: (encoder genLiteral: value)

!
!LiteralNode
value: v
    value <- v

!
