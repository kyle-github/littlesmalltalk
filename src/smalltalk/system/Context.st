" class definition for Context "
+Object subclass: #Context variables: #( method arguments temporaries stack bytePointer stackTop previousContext ) classVariables: #( )
" class methods for Context "
" instance methods for Context "
!Context
backtrace | buf |
        " backtrace context calls "
    buf <- StringBuffer new.

    self backtraceOn: buf.

    buf printString printNl.

!
!Context
backtraceOn: aStream | narg |
        " backtrace context calls "
    narg <- 0.
    aStream write: (method name printString).
    aStream write: '('.
    arguments do: [:a |
        (narg > 0) ifTrue: [ aStream write: ', ' ].
        aStream write: (a class printString).
        narg <- narg+1.
    ].
    aStream write: ')'.
    aStream write: (Char newline asString).

    previousContext notNil
        ifTrue: [ previousContext backtraceOn: aStream ]

!
!Context
perform: aMethod withArguments: a | proc |
    self setup: aMethod withArguments: a.
    proc <- Process new.
    proc context: self.
    ^ proc execute

!
!Context
setup: aMethod withArguments: a
    method <- aMethod.
    arguments <- Array new: 1.
    bytePointer <- 0.
    stack <- Array new: method stackSize.
    stackTop <- 0.
    temporaries <- Array new: method temporarySize.

!
