" class definition for REPL "
+Application subclass: #REPL variables: #( ) classVariables: #( )
" class methods for REPL "
" instance methods for REPL "
!REPL
start	| command |
    " main execution loop "
    [ '-> ' print. command <- String input. command notNil ]
        whileTrue: [ command isEmpty
            ifFalse: [ command doIt printNl ] ]

!
