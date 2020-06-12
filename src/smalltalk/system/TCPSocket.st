" class definition for TCPSocket "
+Socket subclass: #TCPSocket variables: #( ) classVariables: #( )
" class methods for TCPSocket "
=TCPSocket
new
    ^ self newType: 1

!
" instance methods for TCPSocket "
!TCPSocket
accept | newFD |
    newFD <- (self class) acceptOn: (self getFD).
    ^ (self class) newFD: newFD.

!
!TCPSocket
read
    <200 7 (self getFD)>.

    self primitiveFailed

!
!TCPSocket
write: str
    <200 8 (self getFD) str>.

    self primitiveFailed

!
