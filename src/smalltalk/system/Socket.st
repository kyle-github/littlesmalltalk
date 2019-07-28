" class definition for Socket "
+Object subclass: #Socket variables: #( fd ) classVariables: #( )
" class methods for Socket "
=Socket
acceptOn: fd
    <200 1 fd>

    self primitiveFailed

!
=Socket
newFD: anFD
    ^ self in: (super new) at: 1 put: anFD.

!
=Socket
newType: sockType
    ^ self in: (super new) at: 1 put: (self open: sockType).

!
=Socket
open: sockType
    " return a file descriptor (small int) for the new socket or fail "
    " sockType: 1 = TCP, 2 = UDP "
    <200 0 sockType>

    self primitiveFailed

!
" instance methods for Socket "
!Socket
bindTo: host onPort: port
    <200 3 fd host port>.

    self primitiveFailed

!
!Socket
canRead
    <200 4 fd>.

    self primitiveFailed

!
!Socket
canWrite
    <200 5 fd>.

    self primitiveFailed

!
!Socket
close
    <200 2 fd>.

    self primitiveFailed

!
!Socket
getFD
    ^ fd

!
!Socket
hasError
    <200 6 fd self>.

    self primitiveFailed

!
