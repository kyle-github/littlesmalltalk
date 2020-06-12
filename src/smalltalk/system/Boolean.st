" class definition for Boolean "
+Object subclass: #Boolean variables: #( ) classVariables: #( )
" class methods for Boolean "
" instance methods for Boolean "
!Boolean
and: aBlock
    ^ self
        ifTrue: [ aBlock value ]
        ifFalse: [ false ]

!
!Boolean
ifFalse: aBlock
    ^ self ifTrue: [ nil ] ifFalse: [ aBlock value ]

!
!Boolean
ifFalse: falseBlock ifTrue: trueBlock
    ^ self ifTrue: [ trueBlock  value ] ifFalse: [ falseBlock value ]

!
!Boolean
ifTrue: aBlock
    ^ self ifTrue: [ aBlock value ] ifFalse: [ nil ]

!
!Boolean
not
    ^ self
        ifTrue: [ false ]
        ifFalse: [ true ]

!
!Boolean
or: aBlock
    ^ self
        ifTrue: [ true ]
        ifFalse: [ aBlock value ]

!
