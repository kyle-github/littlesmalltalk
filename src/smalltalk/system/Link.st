" class definition for Link "
+Object subclass: #Link variables: #( value next ) classVariables: #( )
" class methods for Link "
=Link
value: v
        " return a new link with given value field "
        " and empty link field "
    ^ self in: self new at: 1 put: v

!
=Link
value: v next: n	| new |
        " return a new link with the given fields "
    new <- self new.
    self in: new at: 1 put: v.
    self in: new at: 2 put: n.
    ^ new

!
" instance methods for Link "
!Link
addLast: anElement
    next notNil
        ifTrue: [ ^ next addLast: anElement ]
        ifFalse: [ next <- Link value: anElement ]

!
!Link
do: aBlock
    aBlock value: value.
    next notNil ifTrue: [ ^ next do: aBlock ]

!
!Link
next
    ^ next

!
!Link
remove: anElement ifAbsent: exceptionBlock
    value = anElement
        ifTrue: [ ^ next ]
        ifFalse: [ next notNil
            ifTrue: [ next <- next remove: anElement
                ifAbsent: exceptionBlock. ^ self ]
            ifFalse: [ ^ exceptionBlock value ] ]

!
!Link
reverseDo: aBlock
    next notNil ifTrue: [ next reverseDo: aBlock ].
    aBlock value: value

!
!Link
value
    ^ value

!
!Link
value: val
    value <- val

!
