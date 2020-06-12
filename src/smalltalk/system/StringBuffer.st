" class definition for StringBuffer "
+List subclass: #StringBuffer variables: #( ) classVariables: #( )
" class methods for StringBuffer "
" instance methods for StringBuffer "
!StringBuffer
add: anObj
    ^ super add: (anObj printString).

!
!StringBuffer
addLast: anObj
    ^ super addLast: (anObj printString).

!
!StringBuffer
asString | size index result |
    size <- self size.

    result <- String new: size.

    index <- 1.

    self do: [ :entry | (entry asString) do: [ :char | result at: index put: char. index <- index + 1 ] ].

    ^ result.

!
!StringBuffer
printString
    ^ (self asString)
    
!
!StringBuffer
size | tempSize |
    tempSize <- 0.
    self do: [:entry | tempSize <- tempSize + (entry size) ].

    ^ tempSize

!
!StringBuffer
write: anObj
    self addLast: anObj.

    ^ self

!
