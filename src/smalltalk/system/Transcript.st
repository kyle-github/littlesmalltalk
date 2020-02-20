" class definition for Transcript "
+Object subclass: #Transcript variables: #( history ) classVariables: #( )
" class methods for Transcript "
=Transcript
new | t |
    " enforce the global singleton "
    t <- globals at: #transcript ifAbsent: [ nil ].

    t isNil ifTrue: [
         t <- super new.
         self in: t at: ((self superclass size) + 1) put: (List new).
         globals at: #transcript put: t.
    ].

    ^ t

!
" instance methods for Transcript "
!Transcript
do: aBlock 
    history do: aBlock.

    ^ self


!
!Transcript
history 
    ^ history
!
!Transcript
put: anObj
    history addLast: anObj.

    [ (history size) > 100 ] whileTrue: [ history removeFirst].

    ^ anObj

!
!Transcript
reset
    history <- List new.

    ^ self


!
