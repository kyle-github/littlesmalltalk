" class definition for List "
+Collection subclass: #List variables: #( elements ) classVariables: #( )
" class methods for List "
=List
with: firstElement	| newList |
    newList <- self new.
    newList add: firstElement.
    ^ newList

!
" instance methods for List "
!List
add: anElement
    elements <- Link value: anElement next: elements.
    ^ anElement

!
!List
addAll: aCollection
    aCollection do: [ :element | self addLast: element ]

!
!List
addLast: anElement
    elements isNil
        ifTrue: [ self add: anElement]
        ifFalse: [ elements addLast: anElement ].
    ^ anElement

!
!List
at: index | link |
    link <- self findLink: index ifAbsent: [ self badIndex ].
    ^ link value

!
!List
at: index ifAbsent: aBlock | link |
    link <- self findLink: index ifAbsent: [nil].
    link isNil ifTrue: [ ^ aBlock value ].
    ^ link value

!
!List
at: index put: value | link |
    link <- self findLink: index.
    link value: value

!
!List
badIndex
    self error: 'Invalid List index'

!
!List
copy
    ^ self asList

!
!List
do: aBlock
    ^ elements notNil ifTrue: [ elements do: aBlock ]

!
!List
findLink: index ifAbsent: aBlock | idx link |
    link <- elements.
    idx <- index.
    link isNil ifTrue: [ ^ aBlock value ].
    [ link notNil ] whileTrue: [
        idx <- idx-1.
        (idx = 0) ifTrue: [ ^ link ].
        link <- link next
    ].
    ^ aBlock value

!
!List
first
    ^ self at: 1

!
!List
isEmpty
    ^ elements isNil

!
!List
remove: anElement
    self remove: anElement
        ifAbsent: [ self emptyCollection ]

!
!List
remove: anElement ifAbsent: exceptionBlock
    elements isNil
        ifTrue: [ exceptionBlock value ]
        ifFalse: [ elements remove: anElement ifAbsent: exceptionBlock ]

!
!List
removeFirst
    elements isNil
        ifTrue: [ self emptyCollection ]
        ifFalse: [ elements <- elements next ]

!
!List
reverse | newList |
    newList <- List new.
    self do: [ :element | newList add: element ].
    ^ newList

!
!List
reverseDo: aBlock
    ^ elements notNil ifTrue: [ elements reverseDo: aBlock ]

!
!List
select: testBlock | newList |
    newList <- List new.
    self reverseDo: [:element | (testBlock value: element)
        ifTrue: [ newList add: element ] ].
    ^ newList

!
