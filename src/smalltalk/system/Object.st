" class definition for Object "
+nil subclass: #Object variables: #( ) classVariables: #( )
" class methods for Object "
=Object
in: object at: index put: value
    " change data field in object, used during initialization "
    " returns the intialized object "
    <5 value object index>

!
" instance methods for Object "
!Object
= arg
    ^ self == arg

!
!Object
== arg
    <1 self arg>

!
!Object
basicDo: aBlock
    ^ self do: aBlock

!
!Object
basicSize
    <4 self>.
    self primitiveFailed

!
!Object
become: other
    " Exchange identity with another object "
    (Array with: self) elementsExchangeIdentityWith: (Array with: other)

!
!Object
class
    <2 self>

!
!Object
debug
    <18>

!
!Object
doesNotUnderstand: aSel
    self error: (self printString + ' (class '+
        (self class printString) +
        '): does not understand ' + aSel printString)

!
!Object
error: str
        " print the message "
    str printNl.
        " then halt "
    <19>

!
!Object
hash
    " Most objects should generate something based on their value "
    ^ self class printString hash

!
!Object
in: object at: index
    " browse instance variable via debugger "
    <24 object index>.
    self primitiveFailed

!
!Object
isKindOf: aClass	| clas |
    clas <- self class.
    [ clas notNil ] whileTrue:
        [ clas == aClass ifTrue: [ ^ true ].
          clas <- clas superclass ].
    ^ false

!
!Object
isMemberOf: aClass
    ^ self class == aClass

!
!Object
isNil
    ^ false

!
!Object
notNil
    ^ true

!
!Object
primitiveFailed
    self error: 'Primitive failed'

!
!Object
print
    self printString do: [ :c | c print ]

!
!Object
printNl
    self print. Char newline print

!
!Object
printString
    ^ self class printString

!
!Object
question: text	| answer |
    text print.
    answer <- String input.
    (answer notNil)
        ifTrue: [ answer <- answer at: 1 ifAbsent: [ $n ] ].
    ^ answer = $y or: [ answer = $Y]

!
!Object
respondsTo: aMessage
    ^ self class allMethods includes: aMessage

!
!Object
species
    " By default, we re-instantiate ourselves as our own Class "
    ^ self class

!
!Object
subclassResponsibility
    self error: 'Subclass responsibility'

!
!Object
~= arg
    ^ (self = arg) not

!
!Object
~~ arg
    ^ (self == arg) not

!
