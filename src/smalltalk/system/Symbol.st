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
implementors | result classes literals |
    result <- List new.

    classes <- globals select: [ :o | o isKindOf: Class ].

    classes do: [ :c |
        ((c methods) at: self ifAbsent: [nil])  notNil ifTrue: [ result add: c ].
    ].

    ^ result

        
    
!
!Symbol
printString
    <23 self String>

!
!Symbol
senders | result classes literals |
    result <- List new.

    classes <- globals select: [ :o | o isKindOf: Class ].

    classes do: [ :c |
        (c methods) binaryDo: [ :n :m |
            " transcript put: ('Checking method ' + (c printString) + '#' + (n printString)). "

            ((m literals) notNil) ifTrue: [
                ((m literals) includes: self) ifTrue: [ result add: m ].
            ].
        ].
    ].

    ^ result

        
    
!
