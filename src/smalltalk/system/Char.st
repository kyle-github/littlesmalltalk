" class definition for Char "
+Magnitude subclass: #Char variables: #( value ) classVariables: #( )
" class methods for Char "
=Char
doInput
    <9>

!
=Char
eof
        " return an EOF indication--not a true Char, but polymorphic "
    ^ self new: 256

!
=Char
input	| c |
    " read a single char from input stream "
    c <- self doInput.
    (c notNil)
        ifTrue: [ ^self new: c ]
        ifFalse: [ ^nil ]

!
=Char
new: value
    " create and initialize a new char "
    ^ self in: self new at: 1 put: value

!
=Char
newline
        " return newline character "
    ^ self new: 10

!
=Char
tab
        " return tab character "
    ^ self new: 9

!
" instance methods for Char "
!Char
< aChar
    ^ value < aChar value

!
!Char
= aChar
    ^ value = aChar value

!
!Char
asString
    " return char as a string value "
    ^ String new: 1; at: 1 put: self

!
!Char
hash
    ^ value

!
!Char
isAlphabetic
    ^ self isLowerCase or: [ self isUpperCase ]

!
!Char
isAlphanumeric
        " are we a letter or a digit? "
    ^ self isAlphabetic or: [ self isDigit ]

!
!Char
isBlank
        "spaces, tabs and newlines are all blank"
    ^ value = 32 or: [ value = 9 or: [ value = 10 ] ]

!
!Char
isDigit
    ^ self between: $0 and: $9

!
!Char
isEOF
    ^ value = 256

!
!Char
isLowerCase
    ^ self between: $a and: $z

!
!Char
isUpperCase
    ^ self between: $A and: $Z

!
!Char
lowerCase
    self isUpperCase
        ifTrue: [ ^ Char new: (value - 65) + 97 ]

!
!Char
print
    <3 value>

!
!Char
printString
    ^ String new: 2; at: 1 put: $$ ; at: 2 put: self

!
!Char
upperCase
    self isLowerCase
        ifTrue: [ ^ Char new: (value - 97) + 65 ]

!
!Char
value
        " return our ascii value as an integer "
    ^ value

!
