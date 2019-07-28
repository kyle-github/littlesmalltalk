" class definition for Symbol "
+Magnitude subclass: #Symbol variables: #( ) classVariables: #( symbols )
" class methods for Symbol "
=Symbol
intern: string
    <23 string Symbol>

!
=Symbol
new: fromString | sym |
    ^ symbols at: fromString
        ifAbsent: [ symbols add: (self intern: fromString) ]

!
" instance methods for Symbol "
!Symbol
< arg
        " works with either symbol or string arguments "
    ^ self printString < arg printString

!
!Symbol
= aString
        " works with either symbol or string arguments "
    ^ self printString = aString printString

!
!Symbol
asString
    ^self printString

!
!Symbol
asSymbol
    ^self

!
!Symbol
hash
    ^self printString hash

!
!Symbol
printString
    <23 self String>

!
