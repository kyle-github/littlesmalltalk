" class definition for False "
+Boolean subclass: #False variables: #( ) classVariables: #( )
" class methods for False "
=False
new
    " there is only one false value "
    ^ false

!
" instance methods for False "
!False
and: aBlock
    ^ false

!
!False
ifTrue: trueBlock ifFalse: falseBlock
    ^ falseBlock value

!
!False
not
    ^ true

!
!False
or: aBlock
    ^ aBlock value

!
!False
printString
    ^ 'false'

!
