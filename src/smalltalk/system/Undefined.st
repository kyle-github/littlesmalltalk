" class definition for Undefined "
+Object subclass: #Undefined variables: #( ) classVariables: #( )
" class methods for Undefined "
=Undefined
new
    " there is only one nil object "
    ^ nil

!
" instance methods for Undefined "
!Undefined
isNil
    " yes, we are nil "
    ^ true

!
!Undefined
main	| command |
        " main execution loop "
    'ABCDEF' replaceFrom: 2 to: 4 with: 'abcdef' startingAt: 3.
    [ '-> ' print. command <- String input. command notNil ]
        whileTrue: [ command isEmpty
            ifFalse: [ command doIt printNl ] ]

!
!Undefined
notNil
    " no, we are not not-nil "
    ^ false

!
!Undefined
printString
    ^ 'nil'

!
