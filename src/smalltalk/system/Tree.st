" class definition for Tree "
+Collection subclass: #Tree variables: #( root ) classVariables: #( )
" class methods for Tree "
" instance methods for Tree "
!Tree
add: anElement
    root isNil
        ifTrue: [ root <- Node new: anElement ]
        ifFalse: [ root add: anElement ].
    ^anElement

!
!Tree
addAll: aCollection
    aCollection do: [:element| self add: element ]

!
!Tree
at: key ifAbsent: exceptionBlock
    root isNil
        ifTrue: [ ^ exceptionBlock value ]
        ifFalse: [ ^ root at: key ifAbsent: exceptionBlock ]

!
!Tree
collect: transformBlock | newTree |
    newTree <- Tree new.
    self do: [:element| newTree add: (transformBlock value: element)]
    ^newTree

!
!Tree
copy
    ^Tree new addAll: self

!
!Tree
do: aBlock
    root notNil ifTrue: [ root do: aBlock ]

!
!Tree
first
    root notNil
        ifTrue: [ ^root first ]
        ifFalse: [ self emptyCollection ]

!
!Tree
isEmpty
    ^ root isNil

!
!Tree
remove: key ifAbsent: exceptionBlock
    root isNil
        ifTrue: [ exceptionBlock value ]
        ifFalse: [ root <- root remove: key ifAbsent: exceptionBlock ]

!
!Tree
removeFirst
    root isNIl ifTrue: [ self emptyCollection ].
    root <- root removeFirst

!
!Tree
reverseDo: aBlock
    root notNil ifTrue: [ root reverseDo: aBlock ]

!
!Tree
select: testBlock | newTree |
    newTree <- Tree new.
    self do: [:element|
        (testBlock value: element)
            ifTrue: [newTree add: element]
    ].
    ^newTree

!
