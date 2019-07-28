" class definition for Set "
+Collection subclass: #Set variables: #( members growth ) classVariables: #( )
" class methods for Set "
=Set
new
    ^ self new: 10

!
=Set
new: size | ret |
    ret <- super new.
    self in: ret at: 1 put: (Array new: size).
    self in: ret at: 2 put: size.
    ^ ret

!
" instance methods for Set "
!Set
add: elem | pos |
    " Find the appropriate slot... if none, need to grow the Set "
    pos <- self location: elem.
    pos isNil ifTrue: [
        self grow.
        ^ self add: elem
    ].

    " If the slot is nil, this is a new entry which we put in place now.
      If it wasn't nil, we still re-store it so that if it's an
      Association, the value portion will be updated. "
    members at: pos put: elem.
    ^ elem

!
!Set
at: value ifAbsent: aBlock | pos |
    pos <- self location: value.
    ((pos isNil) or: [ (members at: pos) isNil ]) ifTrue: [
        ^ aBlock value
    ].
    ^ value

!
!Set
compare: t and: e
    ^ t = e

!
!Set
do: aBlock
    members do: [:elem| elem notNil ifTrue: [ aBlock value: elem ]]

!
!Set
grow | bigger old oldsize |
    " Re-create ourselves in place with a new, bigger storage "
    old <- members.
    members <- Array new: (old size + growth).

    " Re-insert each existing Set member "
    old do: [:elem| self add: elem]

!
!Set
indexOf: value
    ^ self at: value ifAbsent: [ nil ]

!
!Set
location: elem | pos start t |
    start <- pos <- (elem hash rem: members size) + 1.
    [ true ] whileTrue: [
        " Return this position if we match, or have reached
          a nil slot. "
        t <- members at: pos.
        ((t isNil) or: [self compare: t and: elem]) ifTrue: [
            ^ pos
        ].

        " Advance to next slot, circularly "
        pos <- pos + 1.
        (pos > members size) ifTrue: [
            pos <- 1
        ].

        " Return nil if we have scanned the whole Set "
        (pos = start) ifTrue: [ ^ nil ]
    ]

!
!Set
rehash: start | pos elem |
    pos <- start.
    [ true ] whileTrue: [
        " Advance to next slot, ceasing when we reach our start "
        pos <- pos + 1.
        (pos > members size) ifTrue: [ pos <- 1 ].
        (pos = start) ifTrue: [ ^ self ]

        " If we reach a nil slot, there are no further rehash
          worries. "
        elem <- members at: pos.
        elem isNil ifTrue: [ ^ self ].

        " Nil out the slot, and then re-insert the element "
        members at: pos put: nil.
        self add: elem
    ]

!
!Set
remove: elem
    ^ self remove: elem ifAbsent: [self noElement ]

!
!Set
remove: elem ifAbsent: aBlock | pos |
    " If not found, return error "
    pos <- self location: elem.
    ((pos isNil) or: [(members at: pos) isNil]) ifTrue: [
        aBlock value
    ].

    " Remove our element from the Set "
    members at: pos put: nil.

    " Re-hash all that follow "
    self rehash: pos.

    ^ elem

!
!Set
size | tally |
    tally <- 0.
    members do: [:elem| elem notNil ifTrue: [ tally <- tally + 1 ] ].
    ^ tally

!
