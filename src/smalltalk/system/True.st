" class definition for True "
+Boolean subclass: #True variables: #( ) classVariables: #( )
" class methods for True "
=True
new
    " there is only one true value "
    ^ true

!
" instance methods for True "
!True
and: aBlock
    ^ aBlock value

!
!True
ifTrue: trueBlock ifFalse: falseBlock
    ^ trueBlock value

!
!True
not
    ^ false

!
!True
or: aBlock
    ^ true

!
!True
printString
    ^ 'true'

!
