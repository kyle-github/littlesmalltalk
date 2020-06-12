" class definition for Node "
+Object subclass: #Node variables: #( value left right ) classVariables: #( )
" class methods for Node "
=Node
new: value
    " creation, left left and right empty "
    ^ self in: self new at: 1 put: value

!
" instance methods for Node "
!Node
add: anElement
    value < anElement
        ifTrue: [ right notNil
            ifTrue: [ right add: anElement ]
            ifFalse: [ right <- Node new: anElement ] ]
        ifFalse: [ left notNil
            ifTrue: [ left add: anElement ]
            ifFalse: [ left <- Node new: anElement ] ]

!
!Node
at: key ifAbsent: exceptionBlock
    value = key ifTrue: [ ^ value ].
    value < key
        ifTrue: [ right notNil
            ifTrue: [ ^ right at: key ifAbsent: exceptionBlock ]
            ifFalse: [ ^ exceptionBlock value ] ]
        ifFalse: [ left notNil
            ifTrue: [ ^ left at: key ifAbsent: exceptionBlock ]
            ifFalse: [ ^ exceptionBlock value ] ]

!
!Node
do: aBlock
    left notNil ifTrue: [ left do: aBlock ].
    aBlock value: value.
    ^ right notNil ifTrue: [ right do: aBlock ]

!
!Node
first
    left notNil
        ifTrue: [ ^ left first ]
        ifFalse: [ ^ value ]

!
!Node
remove: key ifAbsent: exceptionBlock
    value = key
        ifTrue: [ right notNil
            ifTrue: [ value <- right first.
            right <- right removeFirst.
            ^ self ]
            ifFalse: [ ^ left ] ].
    value < key
        ifTrue: [ right notNil
            ifTrue: [ right <- right remove: key ifAbsent: exceptionBlock ]
            ifFalse: [ ^ exceptionBlock value ] ]
        ifFalse: [ left notNil
            ifTrue: [ left <- left removeL key ifAbsent: exceptionBlock ]
            ifFalse: [ ^ exceptionBlock value ] ]

!
!Node
removeFirst
    left notNil
        ifTrue: [ left <- left removeFirst. ^ self ]
        ifFalse: [ ^ right ]

!
!Node
reverseDo: aBlock
    right notNil ifTrue: [ right do: aBlock ].
    aBlock value: value.
    left notNil ifTrue: [ left do: aBlock ]

!
!Node
value
    ^ value

!
