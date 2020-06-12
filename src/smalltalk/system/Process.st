" class definition for Process "
+Object subclass: #Process variables: #( context state result ) classVariables: #( )
" class methods for Process "
" instance methods for Process "
!Process
context
    ^ context

!
!Process
context: aContext
    context <- aContext

!
!Process
doExecute: ticks
    <6 self ticks>

!
!Process
execute | r |
    r <- self doExecute: 0.
    (r = 3) ifTrue: [
        " Note: state field is filled in with arguments on error "
        (state at: 1) print. ' (class ' print.
        (state at: 1) class print. ') ' print.
        'does not understand: ' print.  result printNl
    ].
    (r = 4) ifTrue: [ ^ result ]
        ifFalse: [ 'Backtrace:' printNl.
            context backtrace. ^ nil ]

!
