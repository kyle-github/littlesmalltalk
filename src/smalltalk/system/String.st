" class definition for String "
+Array subclass: #String variables: #( ) classVariables: #( )
" class methods for String "
=String
input	| value c nl |
    " read a line from input "
    value <- ''. nl <- Char newline.
    [ c <- Char input.
      c isNil ifTrue: [ ^ nil ]. c ~= nl ] whileTrue:
        [ value <- value + c asString ].
    ^ value

!
=String
new: size
    <20 self size>

!
" instance methods for String "
!String
asNumber | val |
    " parse a base-10 ASCII number, return nil on failure "
    val <- 0.
    self do: [:c|
        c isDigit ifFalse: [^nil].
        val <- (val * 10) + (c value - 48)
    ].
    ^val

!
!String
asString " for symmetry with Symbol. "
    ^ self

!
!String
asSymbol
    ^ Symbol new: self

!
!String
at: index
    ^self at: index ifAbsent: [ self badIndex ]

!
!String
at: index ifAbsent: exceptionBlock | c |
    c <- self basicAt: index.
    (c isNil)
         ifTrue: [ ^ exceptionBlock value ]
         ifFalse: [ ^ Char new: c ]

!
!String
at: index put: aValue
    (self basicAt: index put: aValue value) isNil ifTrue: [
        self badIndex
    ]

!
!String
basicAt: index
    <21 self index>
    ^nil

!
!String
basicAt: index put: value
    <22 value self index>
    ^nil

!
!String
break: separators  | wordStart wordEnd words word |
    " break string into words, using separators "
    word <- ''.
    words <- List new.

    " get the word boundaries. "

    wordStart <- 1.
    wordEnd <- 0.
    
    (1 to: (self size)) do: [ :i |
         (separators includes: (self at: i)) ifTrue: [
              (wordEnd >= wordStart) ifTrue: [
                  word <- self from: wordStart to: wordEnd.
                  words addLast: word.
              ].

              wordStart <- (i + 1).
         ] ifFalse: [ wordEnd <- i ].
    ].

    " if there is any remaining word or there were no separators. "
    (wordEnd >= wordStart) ifTrue: [
        word <- self from: wordStart to: wordEnd.
        words addLast: word.
    ].

    ^ words

!
!String
collect: transformationBlock
    ^ (super collect: transformationBlock) asString

!
!String
copy
    " make a clone of ourself "
    <23 self String>

!
!String
copyInto: aString at: anIndex | i |
    " copy ourselves into another string starting at anIndex "

    i <- anIndex.

    self do: [ :c |
        aString at: i put: c.
        i <- i + 1.
    ]

    ^ self


!
!String
doIt	| meth |
    meth <- Undefined parseMethod: 'doItCommand ^' + self.
    ^ meth notNil
        ifTrue: [ ^ Context new
              perform: meth withArguments: (Array new: 1) ]

!
!String
doIt: aReq	
     " REMOVE "
    ^ nil

!
!String
edit
    <105 self>

!
!String
encodeHTML | transStr lt gt newSize targIndex |
    " encode < and > so that HTML can show correctly in HTML "

    lt <- Char new: 60.  " < character "
    gt <- Char new: 62.  " > character "

    newSize <- 0.

    " get the new size. "
    self do: [ :c |
         (( c = lt ) or: [ c = gt ]) ifTrue: [ newSize <- newSize + 4. ]
                                    ifFalse: [ newSize <- newSize + 1. ].
    ].

    " allocate a new string of the right size, 3 = 4 chars for replacement minus the one that is there now. "
    transStr <- String new: newSize.

    targIndex <- 1.

    self do: [ :c |
        ( c = lt ) ifTrue: [
            transStr at: targIndex put: $&.
            transStr at: (targIndex + 1) put: $l.
            transStr at: (targIndex + 2) put: $t.
            transStr at: (targIndex + 3) put: $;.
            targIndex <- targIndex + 4.
        ] ifFalse: [
            ( c = gt ) ifTrue: [
                transStr at: targIndex put: $&.
                transStr at: (targIndex + 1) put: $g.
                transStr at: (targIndex + 2) put: $t.
                transStr at: (targIndex + 3) put: $;.
                targIndex <- targIndex + 4.
            ] ifFalse: [
                transStr at: targIndex put: c.
                targIndex <- targIndex + 1.
            ].
        ].
    ].



    " transform the string into something that can be put into HTML. "
    " self do: [ :c | 
        ( c = lt )
            ifTrue: [ c <- '<' ]
            ifFalse: [ 
                ( c = gt )
                     ifTrue: [ c <- '>' ]
            ].

            transStr <- transStr + (c asString).
    ]."

    ^ transStr


!
!String
from: low to: high | start stop size newString |
    start <- low max: 0.
    stop <- high min: self size.
    size <- (stop + 1 - start) max: 0.
    newString <- String new: size.
    1 to: size do: [ :i |
        newString at: i put: (self at: start).
        start <- start + 1 ].
    ^ newString

!
!String
fromUrl
    " convert from URL encoding "
    <152 self>.

    self primitiveFailed

!
!String
hash | sz |
    sz <- self size.
    (sz < 2) ifTrue: [
        (sz = 1) ifTrue: [ ^ (self at: 1) value ].
        ^ 0
    ].
    ^ (self at: 1) value + (self at: sz) value

!
!String
position: aString
    " find arg as substring and return position "

    <150 self aString>.

    self primitiveFailed

!
!String
printString
    ^ self

!
!String
printWidth: width | ret |
    (self size >= width absolute) ifTrue: [ ^ self ].
    ret <- self.
    (width negative) ifTrue: [
            (self size + 1) to: (width negated) do:
                [:ignore| ret <- ' ' + ret].
        ]
        ifFalse: [
            (self size + 1) to: width do:
                [:ignore| ret <- ret + ' ' ].
        ].
    ^ret

!
!String
reverse
    ^ self asList reverse asString

!
!String
select: testBlock
    ^ (super select: testBlock) asString

!
!String
toUrl
    " convert to URL encoding "
    <151 self>.

    self primitiveFailed

!
