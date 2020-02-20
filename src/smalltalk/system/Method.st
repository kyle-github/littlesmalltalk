" class definition for Method "
+Object subclass: #Method variables: #( name byteCodes literals stackSize temporarySize class text ) classVariables: #( )
" class methods for Method "
=Method
flushCache
    <34>.
    self primitiveFailed

!
=Method
name: n byteCodes: b literals: l stackSize: s temporarySize: ts class: c text: t
    | newMethod |
    newMethod <- self new.
    super in: newMethod at: 1 put: n.
    super in: newMethod at: 2 put: b.
    super in: newMethod at: 3 put: l.
    super in: newMethod at: 4 put: s.
    super in: newMethod at: 5 put: ts.
    super in: newMethod at: 6 put: c.
    super in: newMethod at: 7 put: t.
    ^ newMethod

!
" instance methods for Method "
!Method
args: argNames inst: instNames temp: tempNames
    " Hook for recording symbolic debug "

!
!Method
byteCodes
    ^ byteCodes

!
!Method
containingClass
    ^ class

!
!Method
literals
    ^ literals

!
!Method
name
    ^ name

!
!Method
stackSize
    ^ stackSize

!
!Method
temporarySize
    ^temporarySize

!
!Method
text
    ^ text

!
