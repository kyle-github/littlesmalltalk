" class definition for Class "
+Object subclass: #Class variables: #( name parentClass methods size variables ) classVariables: #( )
" class methods for Class "
" instance methods for Class "
!Class
addMethod	| text |
    text <- ' ' edit.
    (self question: 'compile method?')
        ifTrue: [ ^ self addMethod: text ]

!
!Class
addMethod: text | meth |
    meth <- self parseMethod: text.
    meth notNil
        ifTrue: [
            methods at: meth name put: meth.
            Method flushCache.
            ^ 'method inserted: ' + meth name printString
        ]

!
!Class
allMethods | allMethods |
    parentClass isNil
        ifTrue: [ allMethods <- Dictionary new ]
        ifFalse: [ allMethods <- parentClass allMethods ].
    methods binaryDo: [ :n :m | allMethods at: n put: m ].
    ^ allMethods

!
!Class
allVariables	| names |
    " return all our variable names "
    parentClass notNil
        ifTrue: [ names <- parentClass allVariables ]
        ifFalse: [ names <- Array new: 0 ].
    (variables isNil or: [ variables isEmpty ])
        ifFalse: [ names <- names + variables ].
    ^ names

!
!Class
basicNew
    " Like new "
    <7 self size>

!
!Class
editMethod: nm	| meth text |
    meth <- methods at: nm
        ifAbsent: [ ^ self error: 'no such method'].
    text <- meth text edit.
    (self question: 'compile method?')
        ifTrue: [ ^ self addMethod: text ]

!
!Class
fileOutSource | nl outFile outBuf instVars instMethods metaClass classVars classMethods |

    nl <- (Char new: 10) asString.
    outBuf <- StringBuffer new.

    " instance information. "
    instVars <- self variables.
    instMethods <- self methods.

    " class information "
    metaClass <- self class.
    classVars <- metaClass variables.
    classMethods <- metaClass methods.

    " cover strange Class is the meta class of Class case. "
    (self = metaClass) ifTrue: [
        classVars <- Array new: 0.
        classMethods <- Dictionary new.
    ].

    " output header comment. "
    outBuf addLast: '" class definition for '.
    outBuf addLast: (self name printString).
    outBuf addLast: ' "'.
    outBuf addLast: nl.


    " output the class creation code. "
    outBuf addLast: '+'.
    outBuf addLast: (self superclass printString).
    outBuf addLast: ' subclass: #'.
    outBuf addLast: (self name printString).
    outBuf addLast: ' variables: #( '.
    (instVars notNil and: [(instVars size) > 0]) ifTrue: [instVars do: [ :var | outBuf addLast: (var printString). outBuf addLast: ' '.] ].
    outBuf addLast: ') classVariables: #( '.
    (classVars notNil and: [(classVars size) > 0]) ifTrue: [classVars do: [ :var | outBuf addLast: (var printString). outBuf addLast: ' '.] ].
    outBuf addLast: ')'.
    outBuf addLast: nl.

    " output class methods. "
    outBuf addLast: '" class methods for '.
    outBuf addLast: (self name printString).
    outBuf addLast: ' "'.
    outBuf addLast: nl.
    (classMethods notNil) ifTrue: [ 
        classMethods binaryDo: [ :methName :method |
            outBuf addLast: '='.
            outBuf addLast: (self name printString).
            outBuf addLast: nl.
            outBuf addLast: (method text).
            outBuf addLast: nl.
            outBuf addLast: '!'.
            outBuf addLast: nl.
        ].
    ].

    " output instance methods. "
    outBuf addLast: '" instance methods for '.
    outBuf addLast: (self name printString).
    outBuf addLast: ' "'.
    outBuf addLast: nl.
    (instMethods notNil) ifTrue: [ 
        instMethods binaryDo: [ :methName :method |
            outBuf addLast: '!'.
            outBuf addLast: (self name printString).
            outBuf addLast: nl.
            outBuf addLast: (method text).
            outBuf addLast: nl.
            outBuf addLast: '!'.
            outBuf addLast: nl.
        ].
    ].

    " output class source. "
    outFile <- File openWrite: ( (self name printString) + '.st').
    outFile write: (outBuf printString) size: (outBuf size).
    outFile close.

    ^ self.

!
!Class
fileOutSource: fileName | nl outFile outBuf instVars instMethods metaClass classVars classMethods |
    " remove"

    ^ nil.

!
!Class
instanceVariables	| names |
        " return all our variable names "
    parentClass notNil
        ifTrue: [ names <- parentClass instanceVariables ]
        ifFalse: [ names <- Array new: 0 ].
    (variables isNil or: [ variables isEmpty ])
        ifFalse: [ names <- names + variables ].
    ^ names

!
!Class
listAllMethods
    self allMethods keysDo: [:n| n printNl ]

!
!Class
listMethods
    methods keysDo:
        [ :name | name printNl ]

!
!Class
methods
    " return the tree of methods "
    ^ methods

!
!Class
name
    ^ name.
!
!Class
name: n parent: c variables: v
    " create a new class with the given characteristics "
    name <- n.
    parentClass <- c.
    methods <- Dictionary new.
    size <- v size + c size.
    variables <- v

!
!Class
new
    " return a new instance of ourselves "
    <7 self size>

!
!Class
parseMethod: text
    ^ (Parser new
        text: text instanceVars: self instanceVariables) parse: self

!
!Class
printString
    " just return our name "
    ^ name printString

!
!Class
size
    ^ size

!
!Class
subclass: nm
    ^ self subclass: nm variables: (Array new: 0) classVariables: (Array new: 0)

!
!Class
subclass: nm variables: v
    ^ self subclass: nm variables: v classVariables: (Array new: 0)

!
!Class
subclass: nm variables: v classVariables: cv | meta metaName |
    " create the meta class and the class.  Add both to the globals. "
    metaName <- ('Meta' + nm asString) asSymbol.
    meta <- Class new name: metaName
        parent: self class
        variables: cv.
    " globals at: metaName put: meta. "

    " make the actual class "
    globals at: nm put: ( meta new name: nm
        parent: self
        variables: v ).
    ^ 'subclass created: ' + nm printString

!
!Class
subclasses
    self subclasses: 0

!
!Class
subclasses: indent
    globals do: [ :obj |
        ((obj isKindOf: Class) and: [ obj superclass == self])
            ifTrue: [
                1 to: indent do: [:ignore| $  print ].
                obj printNl.
                obj subclasses: indent + 4 ] ]

!
!Class
superclass
    ^ parentClass

!
!Class
variables
    ^ variables

!
!Class
view: methodName
        " print the text of the given method "
    (methods at: methodName
        ifAbsent: [ ^ self error: 'no such method'])
            text print

!
!Class
viewMethod: nm  | meth |
    meth <- self allMethods at: nm
        ifAbsent: [ ^ self error: 'no such method'].
    meth text print.
    ^ ''

!
