" class definition for CascadeNode "
+ParserNode subclass: #CascadeNode variables: #( head list ) classVariables: #( )
" class methods for CascadeNode "
" instance methods for CascadeNode "
!CascadeNode
compile: encoder block: inBlock
    super compile: encoder.
    head compile: encoder block: inBlock.
    list reverseDo: [ :stmt |
        encoder genHigh: 15 low: 4. " duplicate "
        stmt compile: encoder block: inBlock.
        encoder genHigh: 15 low: 5 "pop from stack " ]

!
!CascadeNode
head: h
    head <- h

!
!CascadeNode
list: l
    list <- l

!
