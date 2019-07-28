" class definition for Association "
+Magnitude subclass: #Association variables: #( key value ) classVariables: #( )
" class methods for Association "
=Association
key: k
        "key is set once, value is resettable"
    ^ self in: self new at: 1 put: k

!
=Association
key: k value: v | ret |
        "key is set once, value is resettable"
    ret <- self new.
    self in: ret at: 1 put: k.
    self in: ret at: 2 put: v.
    ^ ret

!
" instance methods for Association "
!Association
< k
        "compare both with keys and associations"
    (k class == Association)
        ifTrue: [ ^ key < k key ]
        ifFalse: [ ^ key < k ]

!
!Association
= k
        "compare both with keys and associations"
    (k class == Association)
        ifTrue: [ ^ key = k key ]
        ifFalse: [ ^ key = k ]

!
!Association
hash
    ^ key hash

!
!Association
key
    ^ key

!
!Association
printString
    ^ '(' + key printString + ' -> ' + value printString + ')'

!
!Association
value
    ^ value

!
!Association
value: v
    value <- v

!
