" class definition for Collection "
+Magnitude subclass: #Collection variables: #( ) classVariables: #( )
" class methods for Collection "
" instance methods for Collection "
!Collection
< aCollection
    self do: [ :element | (aCollection includes: element)
        ifFalse: [ ^ false ] ].
    ^ true

!
!Collection
= aCollection
    ^ self < aCollection and: [ aCollection < self ]

!
!Collection
anyOne
    self do: [:it| ^ it].
    self emptyCollection

!
!Collection
asArray		| newArray index |
    newArray <- Array new: self size.
    index <- 1.
    self do: [ :element | newArray at: index put: element.
        index <- index + 1 ].
    ^ newArray

!
!Collection
asList
    ^ List new addAll: self

!
!Collection
asString	| newString index |
    newString <- String new: self size.
    index <- 1.
    self do: [ :element | newString at: index put: element.
        index <- index + 1 ].
    ^ newString

!
!Collection
at: value
    ^ self at: value ifAbsent: [ self noElement ]

!
!Collection
at: value ifAbsent: exceptionBlock
    self do: [ :element | element = value ifTrue: [ ^ element ]].
    ^ exceptionBlock value

!
!Collection
collect: transformBlock	| newList |
    newList <- List new.
    self do: [:element | newList addLast: (transformBlock value: element)].
    ^ newList

!
!Collection
do: aBlock
    self subclassResponsibility

!
!Collection
emptyCollection
    self error: (self class printString + ' is empty')

!
!Collection
from: argLow to: argHigh | ret idx size base low high |
    low <- argLow max: 1.
    high <- argHigh min: self size.
    size <- (high - low) + 1.
    (size < 1) ifTrue: [ ^ Array new: 0 ].
    ret <- Array new: size.
    base <- idx <- 1.
    self do: [:elem|
        ((idx >= low) and: [idx <= high]) ifTrue: [
            ret at: base put: elem.
            base <- base + 1.
            (base > size) ifTrue: [ ^ ret ]
        ].
        idx <- idx + 1.
    ].
    ^ ret

!
!Collection
includes: value
    self at: value ifAbsent: [ ^ false ].
    ^ true

!
!Collection
isEmpty
        " empty if there are no elements "
    ^ self size = 0

!
!Collection
noElement
    self error: 'Element not present'

!
!Collection
occurencesOf: obj | count |
    count <- 0.
    self do: [:o| (o = obj) ifTrue: [ count <- count + 1]].
    ^ count

!
!Collection
printString | count res |
    res <- super printString.
    (self respondsTo: #do:) ifFalse: [ ^ res ].
    count <- 0.
    res <- res + ' ('.
    self basicDo: [:elem|
        (count = 0) ifFalse: [ res <- res + ' ' ].
        res <- res + elem printString.
        count <- count + 1.
        (count >= 20) ifTrue: [ ^ res + ' ...)' ]
    ].
    ^ res + ')'

!
!Collection
reject: testBlock
        " select the things that do not match predicate "
    ^ self select: [:x | (testBlock value: x) not ]

!
!Collection
select: testBlock	| newList |
    newList <- List new.
    self do: [:x | (testBlock value: x) ifTrue: [newList addLast: x]].
    ^ newList

!
!Collection
size	| tally |
    tally <- 0.
    self do: [:i | tally <- tally + 1].
    ^ tally

!
