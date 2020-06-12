" class definition for Log "
+Object subclass: #Log variables: #( ) classVariables: #( level )
" class methods for Log "
=Log
detail: msg
    " (level = #error) ifTrue: [ transcript put: msg. self log: msg]. "
    " (level = #warn) ifTrue: [ transcript put: msg. self log: msg]. "
    " (level = #info) ifTrue: [ transcript put: msg. self log: msg]. "
    (level = #detail) ifTrue: [ transcript put: msg. self log: msg].
    (level = #spew) ifTrue: [ transcript put: msg. self log: msg].

!
=Log
error: msg
    (level = #error) ifTrue: [ transcript put: msg. self log: msg].
    (level = #warn) ifTrue: [ transcript put: msg. self log: msg].
    (level = #info) ifTrue: [ transcript put: msg. self log: msg].
    (level = #detail) ifTrue: [ transcript put: msg. self log: msg].
    (level = #spew) ifTrue: [ transcript put: msg. self log: msg].

!
=Log
info: msg
    " (level = #error) ifTrue: [ transcript put: msg. self log: msg]. "
    " (level = #warn) ifTrue: [ transcript put: msg. self log: msg]. "
    (level = #info) ifTrue: [ transcript put: msg. self log: msg].
    (level = #detail) ifTrue: [ transcript put: msg. self log: msg].
    (level = #spew) ifTrue: [ transcript put: msg. self log: msg].

!
=Log
level
    ^ level
!
=Log
level: lvl
   level <- lvl
   ^ self
!
=Log
log: msg
   <160 msg>

!
=Log
spew: msg
    " (level = #error) ifTrue: [ transcript put: msg. self log: msg]. "
    " (level = #warn) ifTrue: [ transcript put: msg. self log: msg]. "
    " (level = #info) ifTrue: [ transcript put: msg. self log: msg]. "
    " (level = #detail) ifTrue: [ transcript put: msg. self log: msg]. "
    (level = #spew) ifTrue: [ transcript put: msg. self log: msg].

!
=Log
warn: msg
    " (level = #error) ifTrue: [ transcript put: msg. self log: msg]. "
    (level = #warn) ifTrue: [ transcript put: msg. self log: msg].
    (level = #info) ifTrue: [ transcript put: msg. self log: msg].
    (level = #detail) ifTrue: [ transcript put: msg. self log: msg].
    (level = #spew) ifTrue: [ transcript put: msg. self log: msg].

!
" instance methods for Log "
