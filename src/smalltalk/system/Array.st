" class definition for Array "
+Collection subclass: #Array variables: #( ) classVariables: #( )
" class methods for Array "
=Array
new
    ^ self new: 0

!
=Array
new: sz
    <7 self sz>

!
=Array
with: elemA
    ^ self in: (self new: 1) at: 1 put: elemA

!
=Array
with: elemA with: elemB | ret |
    ret <- self new: 2.
    self in: ret at: 1 put: elemA.
    self in: ret at: 2 put: elemB.
    ^ ret

!
=Array
with: elemA with: elemB with: elemC | ret |
    ret <- self new: 3.
    self in: ret at: 1 put: elemA.
    self in: ret at: 2 put: elemB.
    self in: ret at: 3 put: elemC.
    ^ ret

!
" instance methods for Array "
!Array
+ aValue	| size1 size2 newValue |
    " catenate two strings together "
    size1 <- self size.
    size2 <- aValue size.
    newValue <- self class new: (size1 + size2).
    newValue replaceFrom: 1 to: size1 with: self.
    newValue replaceFrom: size1+1 to: size1+size2 with: aValue.
    ^ newValue

!
!Array
< arg		| selfsize argsize |
    selfsize <- self size. argsize <- arg size.
    1 to: (selfsize min: argsize)
        do: [:i | (self at: i) ~= (arg at: i)
            ifTrue: [ ^ (self at: i) < (arg at: i) ]].
    ^ selfsize < argsize

!
!Array
= anArray
    (anArray isKindOf: Collection) ifFalse: [ ^ false ].
    self size = anArray size ifFalse: [ ^ false ].
    1 to: self size do:
        [:i | (self at: i) = (anArray at: i)
            ifFalse: [ ^ false ]].
    ^ true

!
!Array
at: index
    <24 self index>
    (self includesKey: index) ifFalse: [ self badIndex ].
    self primitiveFailed

!
!Array
at: index ifAbsent: exceptionBlock
    <24 self index>
    exceptionBlock value

!
!Array
at: index put: value
    <5 value self index>
    (self includesKey: index) ifFalse: [ self badIndex ].
    self primitiveFailed

!
!Array
badIndex
    self error: 'array indexing error'

!
!Array
copy
    ^ self asArray

!
!Array
do: aBlock
    1 to: self size do: [:i | aBlock value: (self at: i)]

!
!Array
elementsExchangeIdentityWith: otherArray
    <35 self otherArray>.
    self primitiveFailed

!
!Array
first
    ^self at: 1

!
!Array
from: low to: high | start stop size obj |
    start <- low max: 0.
    stop <- high min: self size.
    size <- (stop + 1 - start) max: 0.
    obj <- (self species) new: size.
    1 to: size do: [ :i |
        obj at: i put: (self at: start).
        start <- start + 1 ].
    ^ obj

!
!Array
hash | sz |
    sz <- self size.
    (sz < 2) ifTrue: [
        (sz = 1) ifTrue: [ ^ (self at: 1) hash + sz ].
        ^ 0
    ].
    ^ (self at: 1) hash + (self at: sz) hash

!
!Array
includes: aValue
    self do: [ :element | 
        " only compare elements of the same type. "
        ((element class) = (aValue class)) ifTrue: [
            element = aValue ifTrue: [ ^ true ].
        ].
    ].

    ^ false

!
!Array
includesKey: index
    ^ index between: 1 and: self size

!
!Array
indexOf: aValue
    1 to: self size do: [:idx|
        ((self at: idx) == aValue) ifTrue: [ ^ idx ]
    ].
    ^ nil

!
!Array
indexOfVal: aValue
    1 to: self size do: [:idx|
        ((self at: idx) = aValue) ifTrue: [ ^ idx ]
    ].
    ^ nil

!
!Array
insert: value at: position | newArray newSize |
    newSize <- self size + 1.
    newArray <- self class new: newSize.
    newArray replaceFrom: 1 to: position-1 with: self.
    newArray at: position put: value.
    newArray replaceFrom: position+1 to: newSize with:
        self startingAt: position.
    ^ newArray

!
!Array
removeIndex: position  | newArray newSize |
    newSize <- self size - 1.
    newArray <- self class new: newSize.
    newArray replaceFrom: 1 to: position-1 with: self.
    newArray replaceFrom: position to: newSize with: self
        startingAt: position+1.
    ^ newArray

!
!Array
replaceFrom: start to: stop with: replacement
    ^ self replaceFrom: start to: stop with: replacement startingAt: 1

!
!Array
replaceFrom: start to: stop with: replacement startingAt: repStart | base |
    <38 start stop replacement repStart self>.
    base <- repStart-1.
    0 to: (stop - start) do: [:idx|
        self at: (idx + start) put:
            (replacement at: (idx + repStart))
    ]

!
!Array
size
    " compute number of elements "
    <4 self>

!
!Array
with: newItem	| newArray size |
    size <- self size.
    newArray <- self class new: size + 1.
    newArray replaceFrom: 1 to: size with: self.
    newArray at: size + 1 put: newItem
    ^ newArray

!
