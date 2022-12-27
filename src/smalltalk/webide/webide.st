" class definition for Object "
+nil subclass: #Object variables: #( ) classVariables: #( )
" class definition for Application "
+Object subclass: #Application variables: #( ) classVariables: #( )
" class definition for REPL "
+Application subclass: #REPL variables: #( ) classVariables: #( )
" class definition for WebIDE "
+Application subclass: #WebIDE variables: #( ) classVariables: #( )
" class definition for Boolean "
+Object subclass: #Boolean variables: #( ) classVariables: #( )
" class definition for False "
+Boolean subclass: #False variables: #( ) classVariables: #( )
" class definition for True "
+Boolean subclass: #True variables: #( ) classVariables: #( )
" class definition for Class "
+Object subclass: #Class variables: #( name parentClass methods size variables ) classVariables: #( )
" class definition for Context "
+Object subclass: #Context variables: #( method arguments temporaries stack bytePointer stackTop previousContext ) classVariables: #( )
" class definition for Block "
+Context subclass: #Block variables: #( argumentLocation creatingContext oldBytePointer ) classVariables: #( )
" class definition for DOMElement "
+Object subclass: #DOMElement variables: #( startTag attribs endTag sibling child ) classVariables: #( )
" class definition for DOMBody "
+DOMElement subclass: #DOMBody variables: #( ) classVariables: #( )
" class definition for DOMBold "
+DOMElement subclass: #DOMBold variables: #( ) classVariables: #( )
" class definition for DOMBreak "
+DOMElement subclass: #DOMBreak variables: #( ) classVariables: #( )
" class definition for DOMDiv "
+DOMElement subclass: #DOMDiv variables: #( ) classVariables: #( )
" class definition for DOMForm "
+DOMElement subclass: #DOMForm variables: #( ) classVariables: #( )
" class definition for DOMHead "
+DOMElement subclass: #DOMHead variables: #( ) classVariables: #( )
" class definition for DOMItalic "
+DOMElement subclass: #DOMItalic variables: #( ) classVariables: #( )
" class definition for DOMPage "
+DOMElement subclass: #DOMPage variables: #( head body ) classVariables: #( )
" class definition for DOMSpan "
+DOMElement subclass: #DOMSpan variables: #( ) classVariables: #( )
" class definition for DOMSubmitButton "
+DOMElement subclass: #DOMSubmitButton variables: #( ) classVariables: #( )
" class definition for DOMTextInput "
+DOMElement subclass: #DOMTextInput variables: #( ) classVariables: #( )
" class definition for Encoder "
+Object subclass: #Encoder variables: #( name byteCodes index literals stackSize maxStack ) classVariables: #( )
" class definition for File "
+Object subclass: #File variables: #( fileID ) classVariables: #( )
" class definition for HTMLElement "
+Object subclass: #HTMLElement variables: #( attributes ) classVariables: #( )
" class definition for HTMLBody "
+HTMLElement subclass: #HTMLBody variables: #( content ) classVariables: #( )
" class definition for HTMLBreak "
+HTMLElement subclass: #HTMLBreak variables: #( ) classVariables: #( )
" class definition for HTMLDiv "
+HTMLElement subclass: #HTMLDiv variables: #( content ) classVariables: #( )
" class definition for HTMLText "
+HTMLElement subclass: #HTMLText variables: #( content ) classVariables: #( )
" class definition for HTMLTag "
+Object subclass: #HTMLTag variables: #( attributes content preamble epilogue ) classVariables: #( )
" class definition for HTMLAnchor "
+HTMLTag subclass: #HTMLAnchor variables: #( href target content ) classVariables: #( )
" class definition for HTMLHead "
+HTMLTag subclass: #HTMLHead variables: #( content ) classVariables: #( )
" class definition for HTMLPage "
+HTMLTag subclass: #HTMLPage variables: #( head body ) classVariables: #( )
" class definition for HTTPClassBrowser "
+Object subclass: #HTTPClassBrowser variables: #( ) classVariables: #( )
" class definition for HTTPDispatcher "
+Object subclass: #HTTPDispatcher variables: #( map env runFlag sock request errorHandler ) classVariables: #( )
" class definition for HTTPRequest "
+Object subclass: #HTTPRequest variables: #( sock reqPath reqAction reqArgs reqRawData reqPathAndArgs reqError reqLength ) classVariables: #( )
" class definition for Link "
+Object subclass: #Link variables: #( value next ) classVariables: #( )
" class definition for Log "
+Object subclass: #Log variables: #( ) classVariables: #( level )
" class definition for Magnitude "
+Object subclass: #Magnitude variables: #( ) classVariables: #( )
" class definition for Association "
+Magnitude subclass: #Association variables: #( key value ) classVariables: #( )
" class definition for Char "
+Magnitude subclass: #Char variables: #( value ) classVariables: #( )
" class definition for Collection "
+Magnitude subclass: #Collection variables: #( ) classVariables: #( )
" class definition for Array "
+Collection subclass: #Array variables: #( ) classVariables: #( )
" class definition for ByteArray "
+Array subclass: #ByteArray variables: #( ) classVariables: #( )
" class definition for OrderedArray "
+Array subclass: #OrderedArray variables: #( ) classVariables: #( )
" class definition for String "
+Array subclass: #String variables: #( ) classVariables: #( )
" class definition for Dictionary "
+Collection subclass: #Dictionary variables: #( keys values ) classVariables: #( )
" class definition for Interval "
+Collection subclass: #Interval variables: #( low high step ) classVariables: #( )
" class definition for List "
+Collection subclass: #List variables: #( elements ) classVariables: #( )
" class definition for StringBuffer "
+List subclass: #StringBuffer variables: #( ) classVariables: #( )
" class definition for Set "
+Collection subclass: #Set variables: #( members growth ) classVariables: #( )
" class definition for IdentitySet "
+Set subclass: #IdentitySet variables: #( ) classVariables: #( )
" class definition for Tree "
+Collection subclass: #Tree variables: #( root ) classVariables: #( )
" class definition for Number "
+Magnitude subclass: #Number variables: #( ) classVariables: #( )
" class definition for Integer "
+Number subclass: #Integer variables: #( ) classVariables: #( )
" class definition for SmallInt "
+Number subclass: #SmallInt variables: #( ) classVariables: #( seed )
" class definition for Symbol "
+Magnitude subclass: #Symbol variables: #( ) classVariables: #( symbols )
" class definition for Method "
+Object subclass: #Method variables: #( name byteCodes literals stackSize temporarySize class text ) classVariables: #( )
" class definition for Node "
+Object subclass: #Node variables: #( value left right ) classVariables: #( )
" class definition for Parser "
+Object subclass: #Parser variables: #( text index tokenType token argNames tempNames instNames maxTemps errBlock lineNum ) classVariables: #( )
" class definition for ParserNode "
+Object subclass: #ParserNode variables: #( lineNum ) classVariables: #( )
" class definition for ArgumentNode "
+ParserNode subclass: #ArgumentNode variables: #( position ) classVariables: #( )
" class definition for AssignNode "
+ParserNode subclass: #AssignNode variables: #( target expression ) classVariables: #( )
" class definition for BlockNode "
+ParserNode subclass: #BlockNode variables: #( statements temporaryLocation ) classVariables: #( )
" class definition for BodyNode "
+ParserNode subclass: #BodyNode variables: #( statements ) classVariables: #( )
" class definition for CascadeNode "
+ParserNode subclass: #CascadeNode variables: #( head list ) classVariables: #( )
" class definition for InstNode "
+ParserNode subclass: #InstNode variables: #( position ) classVariables: #( )
" class definition for LiteralNode "
+ParserNode subclass: #LiteralNode variables: #( value ) classVariables: #( )
" class definition for MessageNode "
+ParserNode subclass: #MessageNode variables: #( receiver name arguments ) classVariables: #( )
" class definition for PrimitiveNode "
+ParserNode subclass: #PrimitiveNode variables: #( number arguments ) classVariables: #( )
" class definition for ReturnNode "
+ParserNode subclass: #ReturnNode variables: #( expression ) classVariables: #( )
" class definition for TemporaryNode "
+ParserNode subclass: #TemporaryNode variables: #( position ) classVariables: #( )
" class definition for Process "
+Object subclass: #Process variables: #( context state result ) classVariables: #( )
" class definition for Socket "
+Object subclass: #Socket variables: #( fd ) classVariables: #( )
" class definition for TCPSocket "
+Socket subclass: #TCPSocket variables: #( ) classVariables: #( )
" class definition for StringTemplate "
+Object subclass: #StringTemplate variables: #( parts values header footer ) classVariables: #( )
" class definition for Template "
+Object subclass: #Template variables: #( parts ) classVariables: #( )
" class definition for Transcript "
+Object subclass: #Transcript variables: #( history ) classVariables: #( )
" class definition for Undefined "
+Object subclass: #Undefined variables: #( ) classVariables: #( )
" class definition for WWWElement "
+Object subclass: #WWWElement variables: #( content style ) classVariables: #( )
" class definition for WWWBody "
+WWWElement subclass: #WWWBody variables: #( ) classVariables: #( )
" class definition for WWWPage "
+WWWElement subclass: #WWWPage variables: #( ) classVariables: #( )
" class methods for Object "
=Object
in: object at: index put: value
    " change data field in object, used during initialization "
    " returns the intialized object "
    <5 value object index>



!
" instance methods for Object "
!Object
= arg
    ^ self == arg



!
!Object
== arg
    <1 self arg>



!
!Object
asString
    ^ (self printString)


!
!Object
basicDo: aBlock
    ^ self do: aBlock



!
!Object
basicSize
    <4 self>.
    self primitiveFailed



!
!Object
become: other
    " Exchange identity with another object "
    (Array with: self) elementsExchangeIdentityWith: (Array with: other)



!
!Object
class
    <2 self>



!
!Object
debug
    <18>



!
!Object
doesNotUnderstand: aSel
    self error: (self printString + ' (class '+
        (self class printString) +
        '): does not understand ' + aSel printString)



!
!Object
error: str
        " print the message "
    str printNl.
        " then halt "
    <19>



!
!Object
hash
    " Most objects should generate something based on their value "
    ^ self class printString hash



!
!Object
in: object at: index
    " browse instance variable via debugger "
    <24 object index>.
    self primitiveFailed



!
!Object
isKindOf: aClass	| clas |
    clas <- self class.
    [ clas notNil ] whileTrue:
        [ clas == aClass ifTrue: [ ^ true ].
          clas <- clas superclass ].
    ^ false



!
!Object
isMemberOf: aClass
    ^ self class == aClass



!
!Object
isNil
    ^ false



!
!Object
notNil
    ^ true



!
!Object
primitiveFailed
    self error: 'Primitive failed'



!
!Object
print | aStr |
    aStr <- (self printString).
    Transcript put: aStr.
    aStr do: [ :c | c print ]



!
!Object
printNl
    self print. Char newline print



!
!Object
printString
    ^ self class printString



!
!Object
question: text	| answer |
    text print.
    answer <- String input.
    (answer notNil)
        ifTrue: [ answer <- answer at: 1 ifAbsent: [ $n ] ].
    ^ answer = $y or: [ answer = $Y]



!
!Object
respondsTo: aMessage
    ^ self class allMethods includes: aMessage



!
!Object
species
    " By default, we re-instantiate ourselves as our own Class "
    ^ self class



!
!Object
subclassResponsibility
    self error: 'Subclass responsibility'



!
!Object
~= arg
    ^ (self = arg) not



!
!Object
~~ arg
    ^ (self == arg) not



!
" class methods for Application "
" instance methods for Application "
!Application
args
    " get the command line args as an Array of Strings. "
    ^ <170>


!
" class methods for REPL "
" instance methods for REPL "
!REPL
start	| command |
    " main execution loop "
    " 'ABCDEF' replaceFrom: 2 to: 4 with: 'abcdef' startingAt: 3. "
    [ '-> ' print. command <- String input. command notNil ]
        whileTrue: [ command isEmpty
            ifFalse: [ command doIt printNl ] ]



!
" class methods for WebIDE "
" instance methods for WebIDE "
!WebIDE
start	| command |
    " Start HTTP-based class browser. "
    'Starting class browser/IDE on http://localhost:6789.' printNl.
    HTTPClassBrowser start.



!
" class methods for Boolean "
" instance methods for Boolean "
!Boolean
and: aBlock
    ^ self
        ifTrue: [ aBlock value ]
        ifFalse: [ false ]



!
!Boolean
ifFalse: aBlock
    ^ self ifTrue: [ nil ] ifFalse: [ aBlock value ]



!
!Boolean
ifFalse: falseBlock ifTrue: trueBlock
    ^ self ifTrue: [ trueBlock  value ] ifFalse: [ falseBlock value ]



!
!Boolean
ifTrue: aBlock
    ^ self ifTrue: [ aBlock value ] ifFalse: [ nil ]



!
!Boolean
not
    ^ self
        ifTrue: [ false ]
        ifFalse: [ true ]



!
!Boolean
or: aBlock
    ^ self
        ifTrue: [ true ]
        ifFalse: [ aBlock value ]



!
" class methods for False "
=False
new
    " there is only one false value "
    ^ false



!
" instance methods for False "
!False
and: aBlock
    ^ false



!
!False
ifTrue: trueBlock ifFalse: falseBlock
    ^ falseBlock value



!
!False
not
    ^ true



!
!False
or: aBlock
    ^ aBlock value



!
!False
printString
    ^ 'false'



!
" class methods for True "
=True
new
    " there is only one true value "
    ^ true



!
" instance methods for True "
!True
and: aBlock
    ^ aBlock value



!
!True
ifTrue: trueBlock ifFalse: falseBlock
    ^ trueBlock value



!
!True
not
    ^ false



!
!True
or: aBlock
    ^ true



!
!True
printString
    ^ 'true'



!
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
classDefSource | nl outBuf instVars metaClass classVars |
    nl <- (Char new: 10) asString.
    outBuf <- StringBuffer new.

    " instance information. "
    instVars <- self variables.

    " class information "
    metaClass <- self class.
    classVars <- metaClass variables.

    " cover strange Class is the meta class of Class case. "
    (self = metaClass) ifTrue: [
        classVars <- Array new: 0.
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

    ^ outBuf asString



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
fileOutSource | nl outFile classDef methodDefs |
    " get the class definition header. "
    classDef <- self classDefSource.

    " get the method definitions. "
    methodDefs <- self methodDefSource.

    " output class source. "
    outFile <- File openWrite: ( (self name printString) + '.st').
    outFile write: (classDef printString) size: (classDef size).
    outFile write: (methodDefs printString) size: (methodDefs size).
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
methodDefSource | nl outFile outBuf instMethods metaClass classMethods |
    nl <- (Char new: 10) asString.
    outBuf <- StringBuffer new.

    " instance information. "
    instMethods <- self methods.

    " class information "
    metaClass <- self class.
    classMethods <- metaClass methods.

    " cover strange Class is the meta class of Class case. "
    (self = metaClass) ifTrue: [
        classMethods <- Dictionary new.
    ].

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

    ^ outBuf asString








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
    ^ (globals select: [ :o | (o isKindOf: Class) and: [ (o superclass) = self ] ])



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
" class methods for Context "
" instance methods for Context "
!Context
arguments
    ^ arguments



!
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
method
    ^ method



!
!Context
perform: aMethod withArguments: a | proc |
    self setup: aMethod withArguments: a.
    proc <- Process new.
    proc context: self.
    ^ proc execute



!
!Context
previousContext
    ^ previousContext



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
" class methods for Block "
" instance methods for Block "
!Block
argCount
    self error: 'Incorrect argument passing to Block'



!
!Block
backtrace | narg |
        " backtrace context calls "
    'block from ' print. self method name print.
    '(' print.
    narg <- 0.
    self arguments do: [:a |
        (narg > 0) ifTrue: [', ' print ].
        a class print.
        narg <- narg+1
    ].
    ')' printNl.
    self previousContext notNil
        ifTrue: [ self previousContext backtrace ]



!
!Block
value
    " start block execution "
    <8 self>
    (self argCount)



!
!Block
value: a
    " start block execution "
    <8 a self>
    (self argCount)



!
!Block
value: a value: b
    " start block execution "
    <8 a b self>
    (self argCount)



!
!Block
whileFalse: aBlock
    self value ifFalse: [ aBlock value. ^ self whileFalse: aBlock ]



!
!Block
whileTrue: aBlock
    self value ifTrue: [ aBlock value. ^ self whileTrue: aBlock ]



!
" class methods for DOMElement "
=DOMElement
new | anElem |
    anElem <- super new.

    self in: anElem at: ((self superclass size) + 1) put: nil.
    self in: anElem at: ((self superclass size) + 2) put: nil.
    self in: anElem at: ((self superclass size) + 3) put: nil.
    self in: anElem at: ((self superclass size) + 4) put: nil.
    self in: anElem at: ((self superclass size) + 5) put: nil.

    ^ anElem



!
" instance methods for DOMElement "
!DOMElement
addChild: anElem
    child isNil ifTrue: [
        child <- anElem.
    ] ifFalse: [
        child addSibling: anElem.
    ].

    ^ self




!
!DOMElement
addSibling: anElem
    sibling isNil ifTrue: [
        sibling <- anElem.
    ] ifFalse: [
        sibling addSibling: anElem.
    ].

    ^ self




!
!DOMElement
attrAt: aKey
    ^ self attrAt: aKey ifAbsent: [ nil ].




!
!DOMElement
attrAt: aKey ifAbsent: aBlock
    attribs isNil ifTrue: [ ^ (aBlock value) ].

    ^ attribs at: aKey ifAbsent: aBlock.



!
!DOMElement
attrAt: aKey put: aVal
    attribs isNil ifTrue: [ attribs <- Dictionary new ].
    
    attribs at: aKey put: aVal.

    ^ self




!
!DOMElement
endTag: aStr
    endTag <- aStr.

    ^ self




!
!DOMElement
renderOn: aStream | total |
    " handle opening tag. "
    startTag notNil ifTrue: [ 
        aStream write: '<'. 
        aStream write: startTag.

        attribs notNil ifTrue: [
            attribs binaryDo: [ :name :value |
                " <name>=<quote>value<quote>"
                aStream write: ' '.
                aStream write: (name printString).
                aStream write: '="'.
                aStream write: (value printString).
                aStream write: '"'.
            ].
        ].

        aStream write: '>'.
    ].

    child notNil ifTrue: [
        (child isKindOf: DOMElement) ifTrue: [
            child renderOn: aStream.
        ] ifFalse: [
            aStream write: (child printString).
        ].
    ].

    endTag notNil ifTrue: [ 
        aStream write: '</'.
        aStream write: endTag.
        aStream write: '>' 
    ].

    sibling notNil ifTrue: [
        (sibling isKindOf: DOMElement) ifTrue: [
            sibling renderOn: aStream.
        ] ifFalse: [
            aStream write: (sibling printString).
        ].
    ].

    ^ aStream



!
!DOMElement
size | total |
    total <- 0.
    startTag notNil ifTrue: [ total <- total + 1 + (startTag size) ].
    attribs notNil ifTrue: [
        attribs binaryDo: [ :name :value |
            " <name>=<quote>value<quote> "
            total <- total + 1 + (name printString size) + 2 + (value printString size) + 1.
        ].

        " one more for the closing > "
        total <- total + 1.
    ].

    " handle children "
    child notNil ifTrue: [total <- total + (child size) ].

    " </tag> "
    endTag notNil ifTrue: [ total <- total + 2 + (endTag size) + 1 ].

    " handle siblings. "
    sibling notNil ifTrue: [total <- total + (sibling size) ].

    ^ total

   
            
            


!
!DOMElement
startTag: aStr
    startTag <- aStr

    ^ self



!
!DOMElement
style: aStr
    self attrAt: #style put: aStr.

    ^ self




!
" class methods for DOMBody "
=DOMBody
new | aBody |
    aBody <- super new.

    aBody startTag: 'body'.
    aBody endTag: 'body'.

    ^ aBody




!
" instance methods for DOMBody "
" class methods for DOMBold "
=DOMBold
new | aB |
    aB <- super new.

    aB startTag: 'b'.
    aB endTag: 'b'.

    ^ aB



!
=DOMBold
new: someText | aB |
    aB <- self new.

    aB addChild: someText.

    ^ aB



!
" instance methods for DOMBold "
" class methods for DOMBreak "
=DOMBreak
new | aBr |
    aBr <- super new.

    aBr startTag: 'br'.

    ^ aBr




!
" instance methods for DOMBreak "
" class methods for DOMDiv "
=DOMDiv
new | aDiv |
    aDiv <- super new.

    aDiv startTag: 'div'.
    aDiv endTag: 'div'.

    ^ aDiv




!
=DOMDiv
new: aChild | aDiv |
    aDiv <- self new.

    aDiv addChild: aChild.

    ^ aDiv




!
" instance methods for DOMDiv "
" class methods for DOMForm "
=DOMForm
action: anUrl | aForm |
    aForm <- self new.

    aForm action: anUrl.

    ^ aForm




!
=DOMForm
action: anUrl method: aHTTPMethod | aForm |
    aForm <- self new.

    aForm action: anUrl.
    aForm method: aHTTPMethod.

    ^ aForm




!
=DOMForm
new | aForm |
    aForm <- super new.

    aForm startTag: 'form'.
    aForm endTag: 'form'.

    ^ aForm




!
" instance methods for DOMForm "
!DOMForm
action: anUrl 
    self attrAt: #action put: anUrl.

    ^ self



!
!DOMForm
method: mthd
    self attrAt: #method put: mthd.

    ^ self




!
" class methods for DOMHead "
=DOMHead
new | aHead |
    aHead <- super new.

    aHead startTag: 'head'.
    aHead endTag: 'head'.

    ^ aHead




!
" instance methods for DOMHead "
" class methods for DOMItalic "
=DOMItalic
new | anI |
    anI <- super new.

    anI startTag: 'i'.
    anI endTag: 'i'.

    ^ anI




!
=DOMItalic
new: someText | anI |
    anI <- self new.

    anI addChild: someText.

    ^ anI




!
" instance methods for DOMItalic "
" class methods for DOMPage "
=DOMPage
new | aPage head body |
    aPage <- super new.

    aPage startTag: 'html'.
    aPage endTag: 'html'.

    head <- DOMHead new.
    aPage addChild: head.

    body <- DOMBody new.
    aPage addChild: body. 

    self in: aPage at: ((self superclass size) + 1) put: head. 
    self in: aPage at: ((self superclass size) + 2) put: body. 

    ^ aPage




!
" instance methods for DOMPage "
!DOMPage
body 
    ^ body




!
!DOMPage
head
    ^ head




!
" class methods for DOMSpan "
=DOMSpan
new | aSpan |
    aSpan <- super new.

    aSpan startTag: 'span'.
    aSpan endTag: 'span'.

    ^ aSpan




!
=DOMSpan
new: aChild | aSpan |
    aSpan <- self new.

    aSpan addChild: aChild.

    ^ aSpan




!
" instance methods for DOMSpan "
" class methods for DOMSubmitButton "
=DOMSubmitButton
name: nm value: val  | aButton |
    aButton <- self new.

    aButton name: nm.
    aButton value: val.

    ^ aButton




!
=DOMSubmitButton
new | aButton |
    aButton <- super new.

    aButton startTag: 'input'.
    aButton attrAt: #type put: 'submit'.

    ^ aButton




!
" instance methods for DOMSubmitButton "
!DOMSubmitButton
name: nm
    self attrAt: #name put: nm.
    self attrAt: #id put: nm.

    ^ self



!
!DOMSubmitButton
value: val
    self attrAt: #value put: val.

    ^ self




!
" class methods for DOMTextInput "
=DOMTextInput
name: nm   | anInput |
    anInput <- self new.

    anInput name: nm.

    ^ anInput




!
=DOMTextInput
name: nm value: val  | anInput |
    anInput <- self new.

    anInput name: nm.
    anInput value: val.

    ^ anInput




!
=DOMTextInput
new | anInput |
    anInput <- super new.

    anInput startTag: 'input'.
    anInput attrAt: #type put: 'text'.

    ^ anInput




!
" instance methods for DOMTextInput "
!DOMTextInput
name: nm
    self attrAt: #name put: nm.
    self attrAt: #id put: nm.

    ^ self



!
!DOMTextInput
value: val
    self attrAt: #value put: val.

    ^ self




!
" class methods for Encoder "
" instance methods for Encoder "
!Encoder
backUp
    " back up one instruction "
    index <- index - 1



!
!Encoder
currentLocation
    ^ index



!
!Encoder
expandByteCodes	| newarray size |
    size <- byteCodes size.
    newarray <- ByteArray new: size + 8.
    1 to: size do: [:i | newarray at: i put: (byteCodes at: i)].
    byteCodes <- newarray



!
!Encoder
genCode: byte
    index <- index + 1.
    (index >= byteCodes size)
        ifTrue: [ self expandByteCodes].
    byteCodes at: index put: byte.
    ^ index



!
!Encoder
genHigh: high low: low
    (low >= 16)
        ifTrue: [ self genHigh: 0 low: high. self genCode: low ]
        ifFalse: [ self genCode: high * 16 + low ]



!
!Encoder
genLiteral: aValue | idx |
    idx <- literals indexOf: aValue.
    idx notNil ifTrue: [ ^ idx - 1 ].
    literals <- literals with: aValue.
    ^ literals size - 1



!
!Encoder
genVal: byte
    self genCode: (byte rem: 256).
    self genCode: (byte quo: 256).
    ^ index-1



!
!Encoder
lineNum: l
    " Don't care, except in DebugEncoder subclass "



!
!Encoder
method: maxTemps class: c text: text
    ^ Method name: name byteCodes: byteCodes literals: literals
        stackSize: maxStack temporarySize: maxTemps class: c
        text: text



!
!Encoder
name: n
    name <- n asSymbol.
    byteCodes <- ByteArray new: 20.
    index <- 0.
    literals <- Array new: 0.
    stackSize <- 0.
    maxStack <- 1.



!
!Encoder
patch: loc
        " patch a goto from a block "
    byteCodes at: loc put: (index rem: 256).
    byteCodes at: (loc + 1) put: (index quo: 256)



!
!Encoder
popArgs: n
    stackSize <- stackSize - n.



!
!Encoder
pushArgs: n
    stackSize <- stackSize + n.
    maxStack <- stackSize max: maxStack



!
" class methods for File "
=File
doOpen: nm mode: mode
    <100 nm mode>



!
=File
fileIn: nm | file |
    file <- self openRead: nm.
    file opened ifFalse: [ ^ self error: 'cannot open file ' + nm ].
    file fileIn.
    file close.
    ^ 'file in completed'



!
=File
image: nm | file |
        " open a file, write the image, then close "
    file <- self openWrite: nm.
    file opened ifFalse: [ ^ self error: 'cannot open file ' + nm ].
    file writeImage.
    file close



!
=File
openRead: nm
        " open new file for reading "
    ^ self in: (self new) at: 1 put: (self doOpen: nm mode: 'r')



!
=File
openUpdate: nm
        " open new file for reading and writing "
    ^ self in: (self new) at: 1 put: (self doOpen: nm mode: 'r+')



!
=File
openWrite: nm
        " open new file for writing "
    ^ self in: (self new) at: 1 put: (self doOpen: nm mode: 'w')



!
" instance methods for File "
!File
at: idx
    <108 fileID idx>.
    self primitiveFailed



!
!File
at: idx get: buf | size |
    self at: idx.
    size <- buf size.
    <106 fileID buf size>



!
!File
at: idx put: buf
    self at: idx.
    self write: buf size: buf size



!
!File
at: idx size: count | buf res |
    buf <- ByteArray new: count.
    res <- self at: idx get: buf.
    (res < count) ifTrue: [ buf <- buf from: 1 to: res ].
    ^ buf



!
!File
close
        " close file, return file descriptor "
    fileID notNil ifTrue: [
        self close: fileID.
        fileID <- nil
    ]



!
!File
close: id
    <103 id>



!
!File
doRead
    <101 fileID>.
    fileID isNil ifTrue: [ self notOpened ].
    self primitiveFailed



!
!File
fileIn		| cmd |
    [ cmd <- self readChar. cmd notNil ] whileTrue: [
        self fileInDispatch: cmd
    ]



!
!File
fileInDispatch: cmd | c |
    " Immediate execution "
    cmd = $+ ifTrue: [
        self readLine doIt printNl.
        ^ self
    ].

    " Method definition, '!' -> instance method, '=' -> class method "
    (cmd = $! or: [ cmd = $=]) ifTrue: [
        self methodCommand: (cmd = $!).
        ^ self
    ].

    " Comment enclosed in quotes... find matching quote "
    (cmd = $") ifTrue: [
        [ c <- self readChar. c ~= $" ] whileTrue: [
            " Consume chars until closing quote "
            nil
        ].
        ^ self
    ].

    " Blank line, just return to process next line "
    (cmd = Char newline) ifTrue: [
        ^ self
    ].

    " It is random chars (treat as comment--discard) "
    self readLine



!
!File
methodCommand: classCmd | name aClass text line |
    name <- self readLine asSymbol.

    aClass <- globals at: name ifAbsent: [
        ^ self error: 'unknown class name in file-in: ' + name printString 
    ].

    text <- ''.
    [ line <- self readLine. line isNil ifTrue: [ ^ self error: 'unexpected end of input during fileIn' ]. line ~= '!' ] whileTrue: [ 
          text <- text + line + Char newline asString 
    ].
    classCmd
        ifTrue: [ (aClass addMethod: text) printNl ]
        ifFalse: [ (aClass class addMethod: text) printNl ]



!
!File
notOpened
    self error: 'file is not open'



!
!File
opened
    ^ fileID notNil



!
!File
readChar	| c |
        " read a single character from a file "
    c <- self doRead.
    c notNil ifTrue: [ ^ Char new: c ].
    ^ c



!
!File
readLine	| value  c nl |
    " read a line from input "
    fileID isNil ifTrue: [ self error: 'cannot read from unopened file' ].
    value <- ''.
    nl <- Char newline.
    [ c <- self doRead.
      c isNil ifTrue: [ ^ nil ].
      c <- Char new: c.
      c ~= nl ] whileTrue:
        [ value <- value + c asString ].
    ^ value



!
!File
write: buf size: count
    <107 fileID buf count>.
    self primitiveFailed



!
!File
writeCharValue: n
    <102 fileID n>.
    fileID isNil ifTrue: [ self notOpened ].
    self primitiveFailed



!
!File
writeClassDefFor: cls | src subclasses |
    " write the class definition header. "
    src <- cls classDefSource.
    self write: (src) size: (src size).

    " recurse down into the subclasses. "
    subclasses <- cls subclasses.
    (subclasses notNil) ifTrue: [
        subclasses do: [ :subCls |
            self writeClassDefFor: subCls.
        ]
    ].



!
!File
writeImage
        " save the current image in a file "
    fileID notNil
        ifTrue: [ <104 fileID> ]



!
!File
writeImageSource
    " write out the entire image as a set of sources ordered by parent/child with all the class defs first. "

    " write all the class definition headers. "
    self writeClassDefFor: Object.

    " write all the methods. "
    self writeMethodsFor: Object.



!
!File
writeMethodsFor: cls | src subclasses |
    " write the class definition header. "
    src <- cls methodDefSource.
    self write: (src) size: (src size).

    " recurse down into the subclasses. "
    subclasses <- cls subclasses.
    (subclasses notNil) ifTrue: [
        subclasses do: [ :subCls |
            self writeMethodsFor: subCls.
        ]
    ].



!
" class methods for HTMLElement "
=HTMLElement
new  | tag offset |
    tag <- super new.

    (self superclass isNil)
        ifTrue: [ offset <- 0 ]
        ifFalse: [ offset <- (self superclass) size ].

    " set up dictionary for attributes. "
    self in: tag at: (offset + 1) put: (Dictionary new).

    ^ tag



!
" instance methods for HTMLElement "
!HTMLElement
asHTML | buf |
    buf <- StringBuffer new.

    self asHTMLOn: buf.

    ^ (buf asString)





!
!HTMLElement
asHTMLOn: aStringBuffer
    ^ self subclassResponsibility



!
!HTMLElement
atAttr: aSymbol
    " get the attribute or nil "
    ^ (self atAttr: aSymbol ifAbsent: [ nil ]).



!
!HTMLElement
atAttr: aSymbol ifAbsent: aBlock
    " get the value from the attributes dictionary and run aBock if it is missing. "
    ^ (attributes at: aSymbol ifAbsent: aBlock).



!
!HTMLElement
attrSize | total |
    " return the size the encoded attributes would be. "

    total <- 0.

    attributes binaryDo: [ :k :v |
        total <- total + (' ' size).
        total <- total + ((k asString) size).
        total <- total + ('="' size).
        total <- total + ((v printString) size).
        total <- total + ('" ' size).
    ].

    ^ total



!
!HTMLElement
attributes
    " return the current attributes. "
    ^ attributes



!
!HTMLElement
encodeAttrsOn: aString at: anIndex | i kStr vStr |
    " encode the attributes in the passed string at the passed index. "

    i <- anIndex.

    attributes binaryDo: [ :k :v |
        kStr <- (k asString).

        kStr copyInto: aString at: i.
        i <- i + (kStr size).

        vStr <- (v asString).


        (k asString) do: [ :c |
             aString at: i put: c.
             i <- i + 1.
        ].
    ].

    ^ i



!
!HTMLElement
encodeHTML: anHTMLString | transStr lt gt newSize targIndex |
    " encode < and > so that HTML can show correctly in HTML "

    lt <- Char new: 60.  " < character "
    gt <- Char new: 62.  " > character "

    newSize <- 0.

    " get the new size. "
    anHTMLString do: [ :c |
         (( c = lt ) or: [ c = gt ]) ifTrue: [ newSize <- newSize + 4. ]
                                    ifFalse: [ newSize <- newSize + 1. ].
    ].

    " allocate a new string of the right size, 3 = 4 chars for replacement minus the one that is there now. "
    transStr <- String new: newSize.

    targIndex <- 1.

    anHTMLString do: [ :c |
        ( c = lt ) ifTrue: [
            transStr at: targIndex put: $&.
            transStr at: (targIndex + 1) put: $l.
            transStr at: (targIndex + 2) put: $t.
            transStr at: (targIndex + 3) put: $;.
            targIndex <- targIndex + 4.
        ] ifFalse: [
            ( c = gt ) ifTrue: [
                transStr at: targIndex put: $&.
                transStr at: (targIndex + 1) put: $g.
                transStr at: (targIndex + 2) put: $t.
                transStr at: (targIndex + 3) put: $;.
                targIndex <- targIndex + 4.
            ] ifFalse: [
                transStr at: targIndex put: c.
                targIndex <- targIndex + 1.
            ].
        ].
    ].

    ^ transStr



!
!HTMLElement
fromUrl: aString
    " convert from URL encoding "
    <152 aString>.

    self primitiveFailed



!
!HTMLElement
size
    " subclasses need to implement this. "
    self subclassResponsibility




!
!HTMLElement
toUrl: aString
    " convert to URL encoding "
    <151 aString>.

    self primitiveFailed



!
" class methods for HTMLBody "
=HTMLBody
new | body offset |
    body <- super new.

    (self superclass isNil)
        ifTrue: [ offset <- 0 ]
        ifFalse: [ offset <- (self superclass) size ].

    " set content with new list "
    self in: body at: (offset + 1) put: (List new).

    ^ body



!
" instance methods for HTMLBody "
!HTMLBody
add: aTag
   content addLast: aTag.

   ^ self



!
!HTMLBody
asHTMLOn: buf
    buf addLast: '<body>'.
    content do: [ :t | (t notNil) ifTrue: [t asHTMLOn: buf] ].
    buf addLast: '</body>'.



!
!HTMLBody
content
    ^ content



!
!HTMLBody
size | total |
    total <- 0.

    total <- total + ('<body>' size).
    content do: [ :t | t notNil ifTrue: [total <- total + (t size)] ].
    total <- total + ('</body>' size).

    ^ total



!
" class methods for HTMLBreak "
" instance methods for HTMLBreak "
!HTMLBreak
asHTMLOn: aList
    aList addLast: '<br>'.



!
!HTMLBreak
size
    ^ ('<br>' size)



!
" class methods for HTMLDiv "
=HTMLDiv
new | div offset |
    div <- super new.

    (self superclass isNil)
        ifTrue: [ offset <- 0 ]
        ifFalse: [ offset <- (self superclass) size ].

    " set content with new list "
    self in: div at: (offset + 1) put: (List new).

    ^ div



!
" instance methods for HTMLDiv "
!HTMLDiv
add: aTag
   content addLast: aTag.

   ^ self


!
!HTMLDiv
asHTMLOn: buf
    buf addLast: '<div>'.
    content do: [ :t | (t notNil) ifTrue: [t asHTMLOn: buf] ].
    buf addLast: '</div>'.



!
!HTMLDiv
content
    ^ content



!
!HTMLDiv
size | total |
    total <- 0.

    total <- total + ('<div>' size).
    content do: [ :t | t notNil ifTrue: [total <- total + (t size)] ].
    total <- total + ('</div>' size).

    ^ total



!
" class methods for HTMLText "
=HTMLText
new: aStr | elem offset |
    elem <- super new.

    (self superclass isNil)
        ifTrue: [ offset <- 0 ]
        ifFalse: [ offset <- (self superclass) size ].

    " set content with the string "
    self in: elem at: (offset + 1) put: aStr.

    ^ elem


!
" instance methods for HTMLText "
!HTMLText
asHTMLOn: buf
    content notNil ifTrue: [ buf addLast: content ].


!
!HTMLText
content
    ^ content


!
!HTMLText
content: aStr
    content <- aStr.

    ^ self


!
!HTMLText
size
   ^ (content notNil ifTrue: [content size] ifFalse: [ 0 ])


!
" class methods for HTMLTag "
=HTMLTag
new  | tag |
    tag <- super new.

    " set up dictionary for attributes. "
    self in: tag at: 1 put: (Dictionary new).

    " set up list for content/children. "
    self in: tag at: 2 put: (List new).

    " set initial preamble and epilogue "
    self in: tag at: 3 put: (String new: 0).
    self in: tag at: 4 put: (String new: 0).

    ^ tag




!
" instance methods for HTMLTag "
!HTMLTag
add: aTag
    " add new content. "
    content addLast: aTag.

    ^ self




!
!HTMLTag
asHTML | buf |
    buf <- StringBuffer new.

    self asHTMLOn: buf.

    ^ (buf asString)





!
!HTMLTag
asHTMLOn: aStringBuffer
    ^ self subclassResponsibility



!
!HTMLTag
atAttr: aSymbol
    " get the attribute or nil "
    ^ (self atAttr: aSymbol ifAbsent: [ nil ]).



!
!HTMLTag
atAttr: aSymbol ifAbsent: aBlock
    " get the value from the attributes dictionary and run aBock if it is missing. "
    ^ (attributes at: aSymbol ifAbsent: aBlock).


!
!HTMLTag
attrSize | total |
    " return the size the encoded attributes would be. "

    total <- 0.

    attributes binaryDo: [ :k :v |
        total <- total + (' ' size).
        total <- total + (k asString) size.
        total <- total + ('="' size).
        total <- total + (v printString) size.
        total <- total + ('" ' size).
    ].

    ^ total



!
!HTMLTag
attributes
    " return the current attributes. "
    ^ attributes


!
!HTMLTag
content
    " return the current content. "
    ^ content



!
!HTMLTag
encodeAttrsOn: aString at: anIndex | i kStr vStr |
    " encode the attributes in the passed string at the passed index. "

    i <- anIndex.

    attributes binaryDo: [ :k :v |
        kStr <- (k asString).

        kStr copyInto: aString at: i.
        i <- i + (kStr size).

        vStr <- (v asString).

        
        (k asString) do: [ :c |
             aString at: i put: c.
             i <- i + 1.
        ].
    ].

    ^ i





!
!HTMLTag
encodeHTML: anHTMLString | transStr lt gt newSize targIndex |
    " encode < and > so that HTML can show correctly in HTML "

    lt <- Char new: 60.  " < character "
    gt <- Char new: 62.  " > character "

    newSize <- 0.

    " get the new size. "
    anHTMLString do: [ :c |
         (( c = lt ) or: [ c = gt ]) ifTrue: [ newSize <- newSize + 4. ]
                                    ifFalse: [ newSize <- newSize + 1. ].
    ].

    " allocate a new string of the right size, 3 = 4 chars for replacement minus the one that is there now. "
    transStr <- String new: newSize.

    targIndex <- 1.

    anHTMLString do: [ :c |
        ( c = lt ) ifTrue: [
            transStr at: targIndex put: $&.
            transStr at: (targIndex + 1) put: $l.
            transStr at: (targIndex + 2) put: $t.
            transStr at: (targIndex + 3) put: $;.
            targIndex <- targIndex + 4.
        ] ifFalse: [
            ( c = gt ) ifTrue: [
                transStr at: targIndex put: $&.
                transStr at: (targIndex + 1) put: $g.
                transStr at: (targIndex + 2) put: $t.
                transStr at: (targIndex + 3) put: $;.
                targIndex <- targIndex + 4.
            ] ifFalse: [
                transStr at: targIndex put: c.
                targIndex <- targIndex + 1.
            ].
        ].
    ].


    ^ transStr




!
!HTMLTag
fromUrl: aString
    " convert from URL encoding "
    <152 aString>.

    self primitiveFailed



!
!HTMLTag
outputOn: aString at: anIndex
    " encode the tag into a string starting at the index, return the new index. "
    self subclassResponsibility



!
!HTMLTag
size 
    " subclasses need to implement this. "
    self subclassResponsibility




!
!HTMLTag
toUrl: aString
    " convert to URL encoding "
    <151 aString>.

    self primitiveFailed



!
" class methods for HTMLAnchor "
=HTMLAnchor
new | a |

    a <- self new: '' target: '' content: ''.

    ^ a
    


!
=HTMLAnchor
new: hStr content: cStr | a |

    a <- self new: hStr target: '' content: cStr.

    ^ a
    


!
=HTMLAnchor
new: hStr target: tStr content: cStr | a offset |

    a <- super new.

    " debugging "
    " 'Before HTMLAnchor new: ' print. hStr print.  "
    " ' target: ' print. tStr print.  "
    " ' content: ' print. cStr printNl. " 

    " self in: a at: ((self superclass size) + 1) put: hStr.
    self in: a at: ((self superclass size) + 2) put: tStr.
    self in: a at: ((self superclass size) + 3) put: cStr. "

    a href: hStr.
    a target: tStr.
    a content: cStr.

    " debugging "
    " 'After HTMLAnchor new: ' print. (a href notNil ifTrue: [ a href ] ifFalse: [ 'NIL' ]) print. "
    " ' target: ' print. (a target notNil ifTrue: [ a target ] ifFalse: [ 'NIL' ]) print. "
    " ' content: ' print. (a content notNil ifTrue: [ a content ] ifFalse: [ 'NIL' ]) printNl. "

    ^ a
    


!
" instance methods for HTMLAnchor "
!HTMLAnchor
asHTMLOn: aBuf 

    aBuf addLast: '<a href="'.
    aBuf addLast: href.
    aBuf addLast: '"'.

    (target notNil and: [target size > 0]) ifTrue: [
        aBuf addLast: ' target="'.
        aBuf addLast: target.
        aBuf addLast: '"'
    ].

    aBuf addLast: '>'.

    (content notNil and: [(content asString size) > 0]) ifTrue: [
        aBuf addLast: content.
    ].

    aBuf addLast: '</a>'.

   


!
!HTMLAnchor
content
    ^ content



!
!HTMLAnchor
content: aStr
    content <- aStr.

    ^ self



!
!HTMLAnchor
href
    ^ href


!
!HTMLAnchor
href: aUrlStr
    href <- aUrlStr

    ^ self


!
!HTMLAnchor
size | total | 
    total <- 0.

    total <- total + ('<a href="' size) 
                   + (href size)
                   + ('"' size).

    (target notNil and: [target size > 0]) ifTrue: [
        total <- total + (' target="' size)
                       + (target size)
                       + ('"' size)
    ].

    total <- total + '>'.

    (target notNil and: [content size > 0]) ifTrue: [
        total <- total + (content size).
    ].

    total <- total + ('</a>' size).

    ^ total

   


!
!HTMLAnchor
target
    ^ target


!
!HTMLAnchor
target: aStr
    target <- aStr

    ^ self



!
" class methods for HTMLHead "
=HTMLHead
new | head offset |
    head <- super new.
    
    (self superclass isNil) 
        ifTrue: [ offset <- 0 ]
        ifFalse: [ offset <- (self superclass) size ].

    " set content with new list "
    self in: head at: (offset + 1) put: (List new).

    ^ head



!
" instance methods for HTMLHead "
!HTMLHead
add: aTag
   content addLast: aTag.

   ^ self



!
!HTMLHead
asHTMLOn: buf
    buf addLast: '<head>'.
    content do: [ :t | t notNil ifTrue: [t asHTMLOn: buf] ].
    buf addLast: '</head>'.



!
!HTMLHead
content
    ^ content



!
!HTMLHead
content: aList
    " set the content of the HEAD element. "
    content <- aList

    ^ self


!
!HTMLHead
size | total | 
    total <- 0.

    total <- total + ('<head>' size).
    
    content do: [ :t | t notNil ifTrue: [total <- total + (t size)] ].

    total <- total + ('</head>' size).

    ^ total

    


!
" class methods for HTMLPage "
=HTMLPage
new | page offset |
    page <- super new.
    
    (self superclass isNil) 
        ifTrue: [ offset <- 0 ]
        ifFalse: [ offset <- (self superclass) size ].

    " set head and body defaults. "
    self in: page at: (offset + 1) put: (HTMLHead new).
    self in: page at: (offset + 2) put: (HTMLBody new).

    ^ page


!
" instance methods for HTMLPage "
!HTMLPage
asHTMLOn: buf
    buf addLast: '<html>'.
    head isNil ifFalse: [ head asHTMLOn: buf ].
    body isNil ifFalse: [ body asHTMLOn: buf ].
    buf addLast: '</html>'.



!
!HTMLPage
body
    ^ body


!
!HTMLPage
body: aBody
    body <- aBody.

    ^ self




!
!HTMLPage
head
    ^ head


!
!HTMLPage
head: aHead
    head <- aHead

    ^ self



!
!HTMLPage
size | total | 
    total <- 0.

    total <- total + ('<html>' size).
    
    total <- total + (head size).
    total <- total + (body size).

    total <- total + ('</html>' size).

    ^ total

    


!
" class methods for HTTPClassBrowser "
=HTTPClassBrowser
new | browser sock |
    " create a default socket on which to listen "
    browser <- super new.

    sock <- TCPSocket new.
    sock bindTo: '127.0.0.1' onPort: 6789.

    ^ browser startOn: sock



!
=HTTPClassBrowser
start | browser sock |
    " create a default socket on which to listen "
    browser <- super new.

    sock <- TCPSocket new.
    sock bindTo: '127.0.0.1' onPort: 6789.

    ^ browser startOn: sock



!
" instance methods for HTTPClassBrowser "
!HTTPClassBrowser
class: aReq | errorPage page superLink methodTmpl variableTmpl cls |
    " we need an error page in case there is no such class. "
    errorPage <- StringTemplate new: '
        <HTML><TITLE>{error}</TITLE>
            <BODY>
                <H1>{error}</H1>
            </BODY>
        </HTML>'.

    " the superclass might be empty in the case of Object. "
    superLink <- StringTemplate new: '<a href="/class?class={superclass}" target="Class {superclass}">{superclass}</a>'.

    " set up the superlink rendering block. "
    superLink values at: #superclass put: [ :c | c asString].

    " methods are in a table with a template for the rows. "
    methodTmpl <- StringTemplate new: '<tr><td><a href="/class?class={class}&method={methodEsc}" target="Class {class}-{method}">{method}</a></td></tr>'.

    " set up the variable table rendering. "
    methodTmpl header: '<table><tr><th>Instance Methods</th></tr>'.
    methodTmpl values at: #class put: [ :m | (m containingClass asString) ].
    methodTmpl values at: #method put: [ :m | (m name asString) ].
    methodTmpl values at: #methodEsc put: [ :m | ((m name asString) toUrl) ].
    methodTmpl footer: '</table>'.

    " variables are in a table with a template for the rows. "
    variableTmpl <- StringTemplate new: '<tr><td>{name}</td></tr>'.

    " set up the variable table rendering. "
    variableTmpl header: '<table><tr><th>Instance Variables</th></tr>'.
    variableTmpl values at: #name put: [ :vStr | vStr asString].
    variableTmpl footer: '</table>'.

    " we need the normal page to display the class. "
    page <- StringTemplate new: '
        <HTML><TITLE>Class {class}</TITLE>
            <BODY>
                <H1>{superLink} >> {class}</H1>
                <div>
                    <table>
                        <tr><td>{variables}</td><td>{methods}</td></tr>
                    </table>
                </div>
            </BODY>
        </HTML>'.

    " set up the page rendering blocks. "
    page values at: #class put: [ :c | c asString ].
    page values at: #superLink put: [ :c |
        (c superclass) notNil ifTrue: [
            superLink render: (c superclass)
        ] ifFalse: [
            'nil'
        ]
    ].
    page values at: #methods put: [ :c | 
        (c methods) notNil ifTrue: [ 
            ((c methods size) > 0) ifTrue: [
                " return the methods as a Collection. "
                methodTmpl renderObjs: (c methods collect: [ :m | m ])
            ] ifFalse: [ 'No instance methods' ]
        ] ifFalse: [ 'No instance methods' ] 
    ].
    page values at: #variables put: [ :c | 
        (c variables) notNil ifTrue: [ 
            ((c variables size) > 0) ifTrue: [
                variableTmpl renderObjs: (c variables)
            ] ifFalse: [ 'No instance variables' ]
        ] ifFalse: [ 'No instance variables' ] 
    ].

    " get the class, if possible. "
    (aReq at: #class) notNil ifTrue: [
        cls <- globals at: ((aReq at: #class) asSymbol) ifAbsent: [ nil ].

        cls isNil ifTrue: [
            errorPage values at: #error put: ((aReq at: #class) + ' not found!').
            ^ aReq response: (errorPage render: aReq).
        ] 
    ] ifFalse: [
        errorPage values at: #error put: 'No class passed!'.
        
        ^ aReq response: (errorPage render: aReq).
    ].

    " render the page. "
    ^ aReq response: (page render: cls)


!
!HTTPClassBrowser
classPage: aReq | page classList |
    ^ 'not supported'


!
!HTTPClassBrowser
classes: aReq | page classItem |
    " set up the rendering blocks. "

    " an item in the class list tree. "
    classItem <- StringTemplate new: '<li><a href="/class?class={class}" target="Class {class}">{class}</a>{subclasses}</li>'.

    classItem header: '<ul>'.
    classItem footer: '</ul>'.
    classItem values at: #class put: [ :c | c asString].
    classItem values at: #subclasses put: [ :c |
        classItem renderObjs: (c subclasses)
    ].

    page <- StringTemplate new: '
        <HTML><TITLE>Class List</TITLE>
            <BODY>
                <H1>Classes</H1>
                    {classes}
            </BODY>
        </HTML>'.

    page values at: #classes put: [ :c | classItem renderObjs: ((Array new: 1) at: 1 put: c) ].

    ^aReq response: (page render: Object)


!
!HTTPClassBrowser
compileMethodOn: aReq | outBuf classStr class methSrc action meth isMeta |
    outBuf <- StringBuffer new.
    outBuf addLast: '<HTML><BODY bgcolor="#FFFFFF">'.

    " check to make sure this is a POST "
    action <- aReq action.

    " DEBUG "
    " Log log: '#compileMethodOn: starting'. "

    action = 'POST' ifFalse: [
        " DEBUG "
        " Log log: ('#compileMethodOn: failed, need POST method, but had ' + action). "
        outBuf addLast: '<B>POST form submission required.</B></BODY></HTML> '.
        ^ aReq response: outBuf.
    ].

    " DEBUG "
    " Log log: '#compileMethodOn: get the class name.'. "

    " get the class "
    classStr <- aReq at: #class.

    classStr isNil ifTrue: [ outBuf addLast: '<B>No class chosen.</B></BODY></HTML>'.
                 ^ aReq response: outBuf ].

    " DEBUG "
    " Log log: '#compileMethodOn: get the class.'. "

    class <- globals at: (classStr asSymbol) ifAbsent: [ nil ].

    class isNil ifTrue: [
        " DEBUG "
        " Log log: ('compileMethodOn: no such class ' + classStr). "
        outBuf addLast: '<B>No such class: '.
        outBuf addLast: classStr.
        outBuf addLast: '!</B></BODY></HTML>'.
        ^ aReq response: outBuf 
    ].

    " DEBUG "
    " Log log: '#compileMethodOn: get the isMeta attribute.'. "

    isMeta <- aReq at: #ismeta.

    isMeta isNil ifFalse: [ class <- class class. classStr <- class printString ].

    " DEBUG "
    " Log log: '#compileMethodOn: get the method source.'. "

    " get the method source. "
    methSrc <- aReq at: #methsrc.

    methSrc isNil ifTrue: [ 
        " DEBUG "
        " Log log: ('#compileMethodOn: no source for method!'). "
        outBuf addLast: '<B>No method source!</B></BODY></HTML>'.
        ^ aReq response: outBuf 
    ].

    " DEBUG "
    " Log log: ('#compileMethodOn: filtering out carriage returns.'). "

    " filter out carriage returns, the parser sees those as weird literals! "
    methSrc <- (methSrc printString) reject: [ :c | (c value) = 13 ].

    " DEBUG "
    " Log log: ('#compileMethodOn: Carriage returns filtered out.'). "
    " Log log: ('#compileMethodOn: Parsing method source.'). "

    " compile the method source "
    meth <- class parseMethod: methSrc.

    " DEBUG "
    " Log log: ('#compileMethodOn: Method source parsed.'). "

    meth isNil ifTrue: [ outBuf addLast: '<B>Parse error!.</B></BODY></HTML>'. ^ aReq response: outBuf ].

    " store the new method. "
    class methods at: meth name put: meth.

    " flush the lookup cache so that the new method gets called. "

    " DEBUG "
    " Log log: ('#compileMethodOn: Flushing method cache.'). "

    Method flushCache.

    outBuf addLast: meth name printString.
    outBuf addLast: ' added to class '.
    outBuf addLast: classStr.
    outBuf addLast: '</BODY></HTML>'.

    ^ aReq response: outBuf.



!
!HTTPClassBrowser
doIt: aReq	| page doItStr meth result outBuf txt doItDiv resultDiv |
    page <- HTMLPage new.

    doItStr <- aReq at: #cmd.
    (doItStr notNil and: [ doItStr size > 0 ]) ifTrue: [
        " execute the command "
        result <- doItStr doIt printString.
        Transcript put: ('=> ' + (result encodeHTML)).
        Transcript put: ('-> ' + doItStr).
    ].

    outBuf <- StringBuffer new.

    outBuf addLast: '<FORM METHOD="GET" ACTION="/do_it">'.
    outBuf addLast: '<input type="text" id="cmd" name="cmd" required size="70" value="">'.
    outBuf addLast: ' <input TYPE=SUBMIT NAME="do_it" VALUE="Do It!">'.
    outBuf addLast: '</FORM>'.

    txt <- HTMLText new: (outBuf asString).

    doItDiv <- HTMLDiv new.
    doItDiv add: txt.

    resultDiv <- HTMLDiv new.

    Transcript history reverseDo: [ :entry |
        resultDiv add: (HTMLText new: ('<pre>' + entry + '</pre>')).
    ].

    page body add: doItDiv.
    page body add: resultDiv.

    aReq response: (page asHTML).


    


!
!HTTPClassBrowser
editMethodOn: aReq | outBuf classStr class methStr method methodText isMeta |
    outBuf <- StringBuffer new.

    Log info: '#editMethodOn: starting.'.

    outBuf addLast: '<HTML><BODY bgcolor="#FFFFFF">'.

    "debugging" 
    "(aReq args) binaryDo: [ :key :val | outBuf addLast: ((key printString) + ' = ' + (val printString) + '<br>') ]."

    " get the class name from the arguments. "
    classStr <- aReq at: #class.
    isMeta <- aReq at: #ismeta.

    " if there isn't a class string chosen "
    classStr isNil ifTrue: [
        Log info: ('#editMethodOn: No class name passed.').
        outBuf addLast: '<B>No class chosen.</B></BODY></HTML>'.
        ^ aReq response: outBuf.
    ].

    isMeta isNil ifTrue: [ isMeta <- false. ]
                 ifFalse: [ isMeta <- true. ].

    class <- globals at: (classStr asSymbol) ifAbsent: [ nil ].
    class isNil ifTrue: [ 
        Log warn: ('#editMethodOn: No class ' + classStr + ' found!').
        outBuf addLast: '<B>No such class, ' + classStr + '!</B></BODY></HTML>'.
        ^ aReq response: outBuf 
    ].

    " if this is a meta class, then it won't be in the globals. "
    isMeta ifTrue: [
        class <- class class.
    ].

    " set up default method text. "
    methodText <- 'aNewMethod: anArg' + (Char newline asString) + '    ^ nil.'.

    " get the method name, or blank/nil if none. "
    methStr <- aReq at: #method.

    " look up the method and get the source. "
    methStr isNil ifFalse: [
        method <- (class methods) at: (methStr asSymbol) ifAbsent: [ nil ].
        method isNil ifTrue: [
            Log warn: ('#editMethodOn: No method ' + methStr + ' found in class ' + classStr + '!').
            outBuf addLast: '<B>No such method, ' + methStr + ', in class ' + classStr + '!</B></BODY></HTML>'.
            ^ aReq response: outBuf 
        ] ifFalse: [
            Log detail: ('#editMethodOn: encoding method text as HTML.').

            methodText <- (method text encodeHTML).

            Log detail: ('#editMethodOn: method text encoded.').
        ].
    ].

    outBuf addLast: '<div style="display: flex; flex-flow: column; height: 100%;">'.

    outBuf addLast: '<FORM ACTION="/compile_method?class='.
    outBuf addLast:    classStr.
    isMeta ifTrue: [
        outBuf addLast: '&ismeta=true'.
    ].
    outBuf addLast:    '" ENCTYPE="application/x-www-form-urlencoded" METHOD="POST">'.
    outBuf addLast:    '<div style="flex: 0 1 auto;">'.
    outBuf addLast:       '<INPUT TYPE=SUBMIT NAME=compile VALUE="Compile">'.
    outBuf addLast:    '</div>'.
    outBuf addLast:    '<div style="flex: 1 1 auto; overflow-y: auto;">'.
    outBuf addLast:       '<TEXTAREA style="width: 100%; height: 95%; overflow-y: auto;" NAME="methsrc" WRAP="OFF">'.
    outBuf addLast:          methodText.
    outBuf addLast:       '</TEXTAREA>'.
    outBuf addLast:    '</div>'.
    outBuf addLast:    '</FORM>'.
    outBuf addLast: '</div>'.
    outBuf addLast: '</BODY></HTML>'.

    Log info: '#editMethodOn: Done.'.

    ^ aReq response: outBuf.



!
!HTTPClassBrowser
generateClassTree: cls | anchor subclasses subclassList |
    ^ ' not supported. '


!
!HTTPClassBrowser
listClassesOn: aReq | outBuf objName thePage theBody theList theUrl a |
    outBuf <- StringBuffer new.

    " debugging "
    " 'listClassesOn: building the page and body.' printNl. "

    thePage <- HTMLPage new.
    theBody <- HTMLBody new.
 
    thePage body: theBody.

    globals do: [ :obj |
        (obj isKindOf: Class) ifTrue: [
            objName <- obj printString.

            ((objName position: 'Meta') = 1) ifFalse: [ 
                " debugging "
                " ('listClassesOn: adding class ' + objName + ' to list of links.') printNl. "

                theUrl <- '/method_list_frame?class=' + (objName toUrl).

                theBody add: (HTMLAnchor new: theUrl target: 'method_list_frame' content: objName).
                theBody add: (HTMLBreak new)
            ]. 
        ].
    ].

    outBuf addLast: (thePage asHTML).

    " debugging "
    " 'print out the class listing page' printNl. "
    " (thePage asHTML) printNl. "

    ^ aReq response: outBuf.



!
!HTTPClassBrowser
listMethodsOn: aReq | outBuf classStr class variables |
    outBuf <- StringBuffer new.

    " header for page "
    outBuf addLast:  '<HTML><BODY bgcolor="#FFFFFF">'.

    classStr <- aReq at: #class.

    " if there isn't a class string chosen "
    classStr isNil ifTrue: [ outBuf addLast: '<B>No class chosen.</B></BODY></HTML>'.
                 ^ aReq response: outBuf ].

    outBuf addLast: '<p><b><i>'.
    outBuf addLast: classStr.
    outBuf addLast: '</i></b> Class</p>'.

    class <- globals at: (classStr asSymbol) ifAbsent: [ nil ].

    class isNil ifTrue: [ outBuf addLast: '<B>No such class!</B></BODY></HTML>'.
                 ^ aReq response: outBuf ].

    " get the variable list "
    variables <- class variables.

    (variables isNil or: [ (variables size) = 0 ]) ifFalse: [
         outBuf addLast: '<p><table border=0><tr><td border=0>Instance&nbsp;Variables&nbsp;</td><td><i>'.
         variables do: [ :var |
             outBuf addLast: var printString.
             outBuf addLast: ' '.
         ].
         outBuf addLast: '</td></tr></table></p>'.
    ] ifTrue: [
         outBuf addLast: '<p><span><i>No Instance Variables</i></span></p>'.
    ].


    " some classes have no methods "
    (class methods size) = 0 ifTrue: [
            outBuf addLast: '<B>No methods in class</B>'
        ] ifFalse: [
            class methods binaryDo: [ :name :meth |
                    " HTML doesn't like < signs "
                    outBuf addLast: '<A HREF="/edit_frame?class='.
                    outBuf addLast: classStr.
                    outBuf addLast: '&method='.
                    outBuf addLast: (name printString toUrl).
                    outBuf addLast: '" target="edit_frame">'.
                    outBuf addLast: (name printString encodeHTML).
                    outBuf addLast: '</A><BR>' ]
            ].

    outBuf addLast: '</BODY></HTML>'.

    ^ aReq response: outBuf.



!
!HTTPClassBrowser
showBaseFrameOn: aReq | outBuf |
    outBuf <- StringBuffer new.

    outBuf addLast: '<HTML><FRAMESET COLS="40%,60%" FRAMEBORDER="YES">'.
    outBuf addLast: '<FRAME SRC="/control_list_frame" NAME="control_list_frame">'.
    outBuf addLast: '<FRAME SRC="/edit_frame" NAME="edit_frame">'.
    outBuf addLast: '</FRAMESET></HTML>'.

    ^ aReq response: outBuf.



!
!HTTPClassBrowser
showClassOn: aReq | outBuf classStr class variables metaName metaClass classVars createClass newClassName |
    outBuf <- StringBuffer new.

    " header for page "
    outBuf addLast:  '<HTML><BODY bgcolor="#FFFFFF">'.

    classStr <- aReq at: #class.

    " if there isn't a class string chosen "
    classStr isNil ifTrue: [ outBuf addLast: '<B>No class chosen.</B></BODY></HTML>'.
                 ^ aReq response: outBuf ].

    outBuf addLast: '<p><b><i>'.
    outBuf addLast: classStr.
    outBuf addLast: '</i></b></p>'.

    class <- globals at: (classStr asSymbol) ifAbsent: [ nil ].

    class isNil ifTrue: [ outBuf addLast: '<B>No such class!</B></BODY></HTML>'.
                 ^ aReq response: outBuf ].

    outBuf addLast: '<p>Subclass of <b><i>'.
    outBuf addLast: (class superclass printString).
    outBuf addLast: '</i></b></p>'.



    " get the variable list "
    variables <- class variables.

    (variables isNil or: [ (variables size) = 0 ]) ifFalse: [
         outBuf addLast: '<p><table border=0><tr><td border=1>Instance Variables</td><td><i>'.
         variables do: [ :var |
             outBuf addLast: var printString.
             outBuf addLast: ' '.
         ].
         outBuf addLast: '</td></tr></table></p>'.
    ] ifTrue: [
         outBuf addLast: '<p><span><i>No Instance Variables</i></span></p>'.
    ].


    " some classes have no methods "
    outBuf addLast: '<p><table border=1><tr><th>Instance methods</th><tr><td>'.
    class methods binaryDo: [ :name :meth |
        outBuf addLast: '<A HREF="/edit_frame?class='.
        outBuf addLast: classStr.
        outBuf addLast: '&method='.
        outBuf addLast: (name printString toUrl).
        outBuf addLast: '" target="edit_frame">'.
        outBuf addLast: (name printString encodeHTML).
        outBuf addLast: '</A><br>' 
    ].
    outBuf addLast: '</td></tr>'.

    " add the link to create a new method. "
    outBuf addLast: '<tr><td><a href="/edit_frame?class='.
    outBuf addLast: classStr.
    outBuf addLast: '" target="edit_frame">Create new instance method.</a></td></tr>'.

    " close off the table. "
    outBuf addLast: '</table></p>'.

    " get metaclass info "

    metaClass <- class class.
    metaName <- metaClass printString.

    metaClass isNil ifFalse: [
        outBuf addLast: '<p>Metaclass: <b>'.
        outBuf addLast: metaName.
        outBuf addLast: '</b></p>'.

        variables <- metaClass variables.
        (variables isNil or: [ (variables size) = 0 ]) ifFalse: [
             outBuf addLast: '<p><table border=0><tr><td border=1>Class Variables</td><td><i>'.
             variables do: [ :var |
                 outBuf addLast: var printString.
                 outBuf addLast: ' '.
             ].
             outBuf addLast: '</td></tr></table></p>'.
        ] ifTrue: [
             outBuf addLast: '<p><span><i>No Class Variables</i></span></p>'.
        ].

        outBuf addLast: '<p><table border=1><tr><th>Class methods</th><tr><td>'.
        metaClass methods binaryDo: [ :name :meth |
            outBuf addLast: '<A HREF="/edit_frame?ismeta=true&class='.
            outBuf addLast: classStr.
            outBuf addLast: '&method='.
            outBuf addLast: (name printString toUrl).
            outBuf addLast: '" target="edit_frame">'.
            outBuf addLast: (name printString encodeHTML).
            outBuf addLast: '</A><br>' 
        ].   
        outBuf addLast: '</td></tr>'.

        " add the link to create a new method. "
        outBuf addLast: '<tr><td><a href="/edit_frame?ismeta=true&class='.
        outBuf addLast: classStr.
        outBuf addLast: '" target="edit_frame">Create new class method.</a></td></tr>'.

        " close off the table. "
        outBuf addLast: '</table></p>'.
    ].


    (aReq args) binaryDo: [ :key :val | outBuf addLast: ((key printString) + ' = ' + (val printString) + '<BR>') ].

    " see if we are building a new class. "
    createClass <- aReq at: #createclass.

    (createClass isNil) ifFalse: [
        newClassName <- aReq at: #newclassname.

        (newClassName isNil) ifTrue: [
            outBuf addLast: '<b>new class name was blank!<b><br>'.
        ] ifFalse: [
            variables <- aReq at: #instvars.
            classVars <- aReq at: #classvars.

            (variables isNil or: [ (variables printString size) = 0 ]) 
                ifTrue: [ variables <- Array new: 0 ]
                ifFalse: [ variables <- ((variables printString break: ' ') collect: [ :v | v asSymbol]) asArray].

            (classVars isNil or: [ (classVars printString size) = 0 ]) 
                ifTrue: [ classVars <- Array new: 0 ]
                ifFalse: [ classVars <- ((classVars printString break: ' ') collect: [ :v | v asSymbol]) asArray].

            outBuf addLast: 'instance variables: '.
            outBuf addLast: (variables printString).
            outBuf addLast: '<br>class variables: '.
            outBuf addLast: (classVars printString).

            " create the new class "
            class subclass: (newClassName asSymbol) variables: variables classVariables: classVars.

            outBuf addLast: '<br><i>Created new subclass '.
            outBuf addLast: newClassName.
            outBuf addLast: '</i><br>'.
        ].
    ].

    outBuf addLast: '<FORM METHOD="GET" ACTION="/method_list_frame">'.
    outBuf addLast: '<input type="hidden" id="class" name="class" value="'.
    outBuf addLast: classStr.
    outBuf addLast: '">'.
    outBuf addLast: '<label for="newclassname">New class name: </label>'.
    outBuf addLast: '<input type="text" id="newclassname" name="newclassname" required minlength="1" size="15" value=""><br>'.
    outBuf addLast: '<label for="instvars">Instance variables: </label>'.
    outBuf addLast: '<input type="text" id="instvars" name="instvars" size="15" value=""><br>'.
    outBuf addLast: '<label for="classvars">Class variables: </label>'.
    outBuf addLast: '<input type="text" id="classvars" name="classvars" size="15" value=""><br>'.
    outBuf addLast: '<INPUT TYPE=SUBMIT NAME="createclass" VALUE="Create new class">'.
    outBuf addLast: '</FORM>'.
    outBuf addLast: '<FORM METHOD="GET" ACTION="/method_list_frame">'.
    outBuf addLast: '<input type="hidden" id="class" name="class" value="'.
    outBuf addLast: classStr.
    outBuf addLast: '">'.
    outBuf addLast: '<INPUT TYPE=SUBMIT NAME="savesource" VALUE="Save class source">'.
    outBuf addLast: '</FORM>'.

    ((aReq at: #savesource) isNil) ifFalse: [
        class fileOutSource.
        outBuf addLast: '<br><i>Saved class source to '.
        outBuf addLast: classStr.
        outBuf addLast: '.st'.
        outBuf addLast: '</i><br>'.
    ].

    outBuf addLast: '</BODY></HTML>'.

    ^ aReq response: outBuf.



!
!HTTPClassBrowser
showControlFrameOn: aReq | outBuf imageName imageFile aPage mainDiv aForm aDiv saveStatus|
    outBuf <- StringBuffer new.

    "(aReq args) binaryDo: [ :key :val | outBuf addLast: ((key printString) + ' = ' + (val printString) + '<BR>') ]."

    saveStatus <- ''.

    imageName <- aReq at: #imagename.

    (imageName isNil) ifFalse: [
        imageFile <- File openWrite: imageName.

        (imageFile isNil) ifFalse: [
            " update the counter before we save the image otherwise we'll overwrite the last one. "

            globals at: #nextImageNum put: ((globals at: #nextImageNum ifAbsent: [ 0 ]) + 1).

            imageFile writeImage.
            imageFile close.

            saveStatus <- ('Image written to file ' + imageName + '.').
        ] ifTrue: [
            saveStatus <- ('Error writing image to file ' + imageName + '!').
        ].
    ].

    " construct imageName for next save. "
    imageName <- ('lst' + ((globals at: #nextImageNum ifAbsent: [ 0 ]) printString) + '.img').

    " set up the page "
    aPage <- DOMPage new.
    mainDiv <- DOMDiv new style: 'width: 100%;border: 1px solid black;'.
    aPage body addChild: mainDiv.

    " set up image saving form. "
    aForm <- (DOMForm action: '/control_frame' method: 'GET').
    aForm addChild: ((DOMSubmitButton name: 'save' value: 'Save Image') style: 'margin: 0.5em 0.5em 0.5em 0.5em; border: 1px solid black;').
    aForm addChild: ((DOMTextInput name: 'imagename' value: imageName) style: 'margin: 0.5em 0.5em 0.5em 0.5em; border: 1px solid black;').
    (saveStatus size > 0) ifTrue: [aForm addChild: ((DOMDiv new: (DOMItalic new: saveStatus)) style: 'margin: 0.5em 0.5em 0.5em 0.5em; border: 1px solid black;') ].
    mainDiv addChild: ((DOMDiv new: aForm) style: 'display: inline-block; margin: 0.5em 0.5em 0.5em 0.5em; width: 95%; border: 1px solid black;').



    " set up button to stop browser. "
    aForm <- (DOMForm action: '/stop' method: 'GET').
    aForm attrAt: #target put: '_top'.
    aForm addChild: ((DOMSubmitButton name: 'stop' value: 'Stop Browser') style: 'margin: 0.5em 0.5em 0.5em 0.5em; border: 1px solid black;').
    mainDiv addChild: ((DOMDiv new: aForm) style: 'display: inline-block; margin: 0em 0.5em 0.5em 0.5em; width: 95%; border: 1px solid black;').


    " outBuf addLast: '<html><body>'. "

    " render the page. "
    aPage renderOn: outBuf.
    " outBuf addLast: ((aPage renderOn: (StringBuffer new)) printString encodeHTML). "

    " outBuf addLast: '</body></html>'. "

    ^ aReq response: outBuf.



!
!HTTPClassBrowser
showControlListFrameOn: aReq | outBuf |
    outBuf <- StringBuffer new.

    outBuf addLast: '<HTML><FRAMESET ROWS="80%,20%" FRAMEBORDER="YES">'.
    outBuf addLast: '<FRAME SRC="/list_frame" NAME="list_frame">'.
    outBuf addLast: '<FRAME SRC="/control_frame" NAME="control_frame">'.
    outBuf addLast: '</FRAMESET></HTML>'.

    ^ aReq response: outBuf.



!
!HTTPClassBrowser
showErrorOn: aReq | outBuf |
    " outBuf <- StringBuffer new. "

    " outBuf addLast: '<HTML><BODY bgcolor="#FFFFFF"><B>Path not recognized!</B><BR>'. "
    " outBuf addLast: '<PRE>'. "
    " outBuf addLast: ('path: ' + (aReq path) + (Char newline asString)). "
    " aReq args isNil ifFalse: [ (aReq args) binaryDo:
                    [ :key :val | outBuf addLast: ((key printString) + '=' + (val printString) + (Char newline asString)) ] ]. "

    " outBuf addLast: '</PRE></BODY></HTML>'. "

    " ^ aReq responseErr: 404 withMessage: outBuf. "

    ^ aReq responseErr: 404 withMessage: ('Unknown path ' + (aReq path) + '!') .



!
!HTTPClassBrowser
showListFrameOn: aReq | outBuf |
    outBuf <- StringBuffer new.

    outBuf addLast: '<HTML><FRAMESET COLS="40%,60%" FRAMEBORDER="YES">'.
    outBuf addLast: '<FRAME SRC="/class_list_frame" NAME="class_list_frame">'.
    outBuf addLast: '<FRAME SRC="/method_list_frame" NAME="method_list_frame">'.
    outBuf addLast: '</FRAMESET></HTML>'.

    ^ aReq response: outBuf.



!
!HTTPClassBrowser
start | sock acceptSock |
    " create a default socket on which to listen "
    sock <- TCPSocket new.
    sock bindTo: '127.0.0.1' onPort: 6789.

    ^ self startOn: sock



!
!HTTPClassBrowser
startOn: aSock | dispatcher |
    dispatcher <- HTTPDispatcher new.

    dispatcher register: [:aReq :anEnv | self showBaseFrameOn: aReq. nil]
            at: '/'.
    dispatcher register: [:aReq :anEnv | self showControlListFrameOn: aReq. nil]
            at: '/control_list_frame'.
    dispatcher register: [:aReq :anEnv | self showListFrameOn: aReq. nil]
            at: '/list_frame'.
    dispatcher register: [:aReq :anEnv | self showControlFrameOn: aReq. nil]
            at: '/control_frame'.
    dispatcher register: [:aReq :anEnv | self listClassesOn: aReq. nil]
            at: '/class_list_frame'.
    dispatcher register: [:aReq :anEnv | self showClassOn: aReq. nil]
            at: '/method_list_frame'.
    dispatcher register: [:aReq :anEnv | self editMethodOn: aReq. nil]
            at: '/edit_frame'.
    dispatcher register: [:aReq :anEnv | self compileMethodOn: aReq. nil]
            at: '/compile_method'.
    dispatcher register: [:aReq :anEnv | self doIt: aReq. nil]
            at: '/do_it'.

    dispatcher register: [:aReq :anEnv | aReq response: '<HTML><BODY><B>Class browser stopped.</B></BODY></HTML>'. dispatcher stop.  aSock close. nil]
            at: '/stop'.


    dispatcher register: [ :aReq :anEnv | self classes: aReq. nil ] 
            at: '/classes'.

    dispatcher register: [ :aReq :anEnv | self class: aReq. nil ] 
            at: '/class'.



    dispatcher registerErrorHandler: [ :aReq :anEnv | self showErrorOn: aReq. nil].

    dispatcher startOn: aSock.

    ^ nil.



!
" class methods for HTTPDispatcher "
" instance methods for HTTPDispatcher "
!HTTPDispatcher
register: aBlock at: aPath
    map isNil ifTrue: [ map <- Dictionary new ].

    map at: aPath put: aBlock.
    ^ self.



!
!HTTPDispatcher
registerErrorHandler: anObj
    errorHandler <- anObj.
    ^ self.



!
!HTTPDispatcher
startOn: aSock | tmpRequest aBlock clientSock |
    runFlag <- true.
    env <- Dictionary new.
    [ runFlag = true ] whileTrue: [
        " get a request from the socket and dispatch it "
        clientSock <- aSock accept.

        tmpRequest <- HTTPRequest new.
        (tmpRequest read: clientSock) ifTrue: [
            aBlock <- map at: (tmpRequest path) ifAbsent: [ nil ].

            ( aBlock isNil )
                ifTrue: [ errorHandler value: tmpRequest value: env]
                ifFalse: [ aBlock value: tmpRequest value: env ].
        ] ifFalse: [
            Log detail: 'No request found on TCP socket! Closing connection.'.
        ].

        clientSock close.
    ].



!
!HTTPDispatcher
stop
    runFlag <- false.



!
" class methods for HTTPRequest "
" instance methods for HTTPRequest "
!HTTPRequest
action
    " if it was set once, return it. "
    reqAction isNil ifFalse: [ ^ reqAction.].

    " 'reqAction before parse: ' print. "
    " reqAction printString printNl. "

    " 'Position of GET: ' print. "
    " ((self rawData) position: 'GET') printString printNl. "

    " 'Position of POST: ' print. "
    " ((self rawData) position: 'POST') printString printNl. "

    ((self rawData) position: 'GET') = 1 ifTrue: [ reqAction <- 'GET'. ].
    ((self rawData) position: 'POST') = 1 ifTrue: [ reqAction <- 'POST'. ].

    reqAction isNil ifTrue: [ reqAction <- 'UNKNOWN' ].

    " 'reqAction: ' print. "
    " reqAction printString printNl. "

    ^ reqAction.



!
!HTTPRequest
args	| i pathArgField argsData keyValList key val argList|
    " get args for both URL and POST data "

    " if we already got then, just return "
    reqArgs isNil ifFalse: [ ^ reqArgs ].

    " we have not already gotten the args, so get them now. "

    reqArgs <- Dictionary new.

    " concatenate args "
    pathArgField <- self pathAndArgs.

    (pathArgField isNil) ifFalse: [
        i <- pathArgField position: '?'.

        i isNil ifFalse: [
            " copy the data "
            argsData <- pathArgField from: (i+1) to: (pathArgField size).

            " append a & to make sure that we break correctly "
            argsData <- argsData + '&'.
        ].
    ].

    " copy data from the form data if this is a POST "
    (self action) = 'POST' ifTrue: [
        i <- ((self rawData size) + 1) - reqLength.

        argsData <- argsData + ((self rawData) from: i to: (self rawData size)).
    ].

    " do a little error checking "
    argsData isNil ifTrue: [ ^reqArgs ].

    (argsData size) = 0 ifTrue: [ ^ reqArgs ].

    " split up the key value pairs "
    keyValList <- argsData break: '&'.

    keyValList do: [ :keyValField |
        argList <- keyValField break: '='.

        key <- argList first.
        argList removeFirst.

        " skip key if it has no value "
        (argList size) = 0 
            ifTrue: [ val <- nil ]
            ifFalse: [ val <- argList first asString ].

        val isNil ifFalse: [ reqArgs at: (key fromUrl asSymbol) put: (val fromUrl) ].
    ].

    ^ reqArgs.




!
!HTTPRequest
at: aSymbol
    ^ (self args) at: aSymbol ifAbsent: [ nil ].



!
!HTTPRequest
path	| i pathArgField |

    reqPath isNil ifFalse: [ ^ reqPath ].

    reqPath = '' ifTrue: [ ^ nil ].

    pathArgField <- self pathAndArgs.

    pathArgField isNil ifTrue: [ reqPath <- ''. ^ nil ].

    i <- pathArgField position: '?'.

    i isNil ifTrue: [ reqPath <- pathArgField. ^ reqPath ].

    reqPath <- pathArgField from: 1 to: (i - 1).

    ^ reqPath.



!
!HTTPRequest
pathAndArgs	| i lines firstLine fields pathArgField |

    reqPathAndArgs isNil ifFalse: [ ^ reqPathAndArgs ].

    " break raw data into lines "
    lines <- (self rawData) break: (((Char new: 13) asString) +(Char newline asString)).

    firstLine <- lines first.

    " break on spaces "
    fields <- firstLine break: ' '.

    " path plus arguments is second field "
    fields removeFirst.

    reqPathAndArgs <- fields first.

    ^ reqPathAndArgs.



!
!HTTPRequest
rawData		| i termStringCR termStringNL doubleTermCR doubleTermNL tempData contentLength sepUsesCR |
    " read the request raw data.  This does some parsing. "

    " return the data if we already have it. "
    reqRawData isNil ifFalse: [ ^ reqRawData ].

    " is the socket nil? or not open? "
    (sock isNil) ifTrue: [ ^ nil ].

    " the line terminator for HTTP headers is CRLF "
    termStringCR <- ((Char new: 13) asString) + (Char newline asString).
    termStringNL <- (Char newline asString).

    " the terminator between the HTTP headers and body is CRLF CRLF "
    doubleTermCR <- termStringCR + termStringCR.
    doubleTermNL <- termStringNL + termStringNL.

    " get the data from the socket until we see the header/body delimiter "
    " DEBUG "
    "'About to call #asString on result of socket read.' printNl."
    tempData <- sock read asString.

    " if we get no data perhaps the browser closed the connection? "
    ((tempData size) = 0) ifTrue: [ ^ nil ].

    " DEBUG "
    " ('tempData size == ' + ((tempData size) printString)) printNl."
    " 'position of double CR/LN == ' print. (tempData position: doubleTermCR) printString printNl. "

    [ ((tempData position: doubleTermCR) isNil) and: [(tempData position: doubleTermNL) isNil] ] whileTrue: [ 
        " DEBUG "
        " 'Waiting for header separator.' printNl. "
        " 'about to call #asString on result of socket read while waiting for more data.' printNl. "

        tempData <- tempData + (sock read asString) 
    ].

    " OK, we have all the headers, what kind of request is it? "
    reqRawData <- tempData.
    reqLength <- tempData size.

    " DEBUG "
    " '------------------------------------------------------------------' printNl. "
    " reqRawData printNl. "
    " '------------------------------------------------------------------' printNl. "

    " if this is a POST, we need to get the length and read the data "
    ((self action) = 'POST') ifTrue: [
        " DEBUG "
        " 'Processing POST action.' printNl. "

        i <- tempData position: 'Content-Length:' .
        i isNil ifTrue: [ reqError <- '400 POST without Content-Length header'. ^ nil ].

        " find the first digit character. "
        i <- i + ('Content-Length:' size).

        [ (tempData at: i) isBlank ] whileTrue: [ i <- i+1 ].

        contentLength <- 0.

        " convert the size into an integer while reading it in "
        [ (tempData at: i) isDigit ] whileTrue:
            [ contentLength <- (contentLength * 10) + (((tempData at: i) value) - ($0 value)). i <- i+1 ].

        " store the length for later "
        reqLength <- contentLength.

        " what kind of separator is used? "
        sepUsesCR <- ((tempData position: doubleTermNL) isNil). 

        " the total length is the length of the header plus separator plus body, -1 for zero start. "
        sepUsesCR ifTrue: [ contentLength <- contentLength + (tempData position: doubleTermCR ) + (doubleTermCR size) - 1 ]
                  ifFalse: [ contentLength <- contentLength + (tempData position: doubleTermNL ) + (doubleTermNL size) - 1 ].

        " read until we have all the data "

        " DEBUG "
        " 'calling #asString on result of socket read while getting all POST data.' printNl. "
        [ (tempData size) < contentLength ] whileTrue: [tempData <- tempData + (sock read asString)].
    ] ifFalse: [ 
        reqLength <- 0 
    ].

    " we have all the raw data. We've set reqAction, reqLength already, so set reqRawData "
    reqRawData <- tempData.

    ^ reqRawData.



!
!HTTPRequest
read: aSock  | data |
    sock <- aSock.

    data <- self rawData.

    (data isNil) ifTrue: [ ^ false ] ifFalse: [ ^ true ].



!
!HTTPRequest
response: aResp  | responseSize tmpResponse lineTerm index |
    tmpResponse <- StringBuffer new.

    " create the line termination string, note carriage return and linefeed "
    lineTerm <- ((Char new: 13) asString) + (Char newline asString).

    " get the response size "
    responseSize <- aResp size.

    " make HTTP headers, we are dumb and only talk HTTP 1.0 so far. "
    tmpResponse addLast: ('HTTP/1.0 200 OK' + lineTerm).
    tmpResponse addLast: ('Content-Type: text/html' + lineTerm).
    tmpResponse addLast: ('Content-Length: ' + (responseSize printString) + lineTerm).
    tmpResponse addLast: ('Server: LittleSmalltalk' + lineTerm).
    tmpResponse addLast: ('Connection: close' + lineTerm).
    tmpResponse addLast: lineTerm.

    " add the response text "
    tmpResponse addLast: (aResp printString).

    " 'Sending response:' printNl. "
    " tmpResponse printString printNl. "

    sock write: (tmpResponse printString).

    " close the connection now. "
    " sock close. "
    ^ self.



!
!HTTPRequest
responseErr: aCode withMessage: aResp  | responseSize tmpResponse lineTerm index |
    tmpResponse <- StringBuffer new.

    " create the line termination string, note carriage return and linefeed "
    lineTerm <- ((Char new: 13) asString) + (Char newline asString).

    " get the response size "
    responseSize <- aResp size.

    " make HTTP headers, we are dumb and only talk HTTP 1.0 so far. "
    tmpResponse addLast: ('HTTP/1.0 ' + (aCode printString) + ' ' + aResp + lineTerm).
    tmpResponse addLast: ('Content-Type: text/html' + lineTerm).
    tmpResponse addLast: ('Content-Length: ' + (responseSize printString) + lineTerm).
    tmpResponse addLast: ('Server: LittleSmalltalk' + lineTerm).
    tmpResponse addLast: ('Connection: close' + lineTerm).
    tmpResponse addLast: lineTerm.

    " add the response text "
    tmpResponse addLast: (aResp printString).

    'Sending response:' printNl.
    tmpResponse printString printNl.

    sock write: (tmpResponse printString).

    " close the connection now. "
    " sock close. "
    ^ self.



!
" class methods for Link "
=Link
value: v
        " return a new link with given value field "
        " and empty link field "
    ^ self in: self new at: 1 put: v



!
=Link
value: v next: n	| new |
        " return a new link with the given fields "
    new <- self new.
    self in: new at: 1 put: v.
    self in: new at: 2 put: n.
    ^ new



!
" instance methods for Link "
!Link
addLast: anElement
    next notNil
        ifTrue: [ ^ next addLast: anElement ]
        ifFalse: [ next <- Link value: anElement ]



!
!Link
do: aBlock
    aBlock value: value.
    next notNil ifTrue: [ ^ next do: aBlock ]



!
!Link
next
    ^ next



!
!Link
remove: anElement ifAbsent: exceptionBlock
    value = anElement
        ifTrue: [ ^ next ]
        ifFalse: [ next notNil
            ifTrue: [ next <- next remove: anElement
                ifAbsent: exceptionBlock. ^ self ]
            ifFalse: [ ^ exceptionBlock value ] ]



!
!Link
reverseDo: aBlock
    next notNil ifTrue: [ next reverseDo: aBlock ].
    aBlock value: value



!
!Link
value
    ^ value



!
!Link
value: val
    value <- val



!
" class methods for Log "
=Log
detail: msg
    " (level = #error) ifTrue: [ Transcript put: msg. self log: msg]. "
    " (level = #warn) ifTrue: [ Transcript put: msg. self log: msg]. "
    " (level = #info) ifTrue: [ Transcript put: msg. self log: msg]. "
    (level = #detail) ifTrue: [ Transcript put: msg. self log: msg].
    (level = #spew) ifTrue: [ Transcript put: msg. self log: msg].



!
=Log
error: msg
    (level = #error) ifTrue: [ Transcript put: msg. self log: msg].
    (level = #warn) ifTrue: [ Transcript put: msg. self log: msg].
    (level = #info) ifTrue: [ Transcript put: msg. self log: msg].
    (level = #detail) ifTrue: [ Transcript put: msg. self log: msg].
    (level = #spew) ifTrue: [ Transcript put: msg. self log: msg].



!
=Log
info: msg
    " (level = #error) ifTrue: [ Transcript put: msg. self log: msg]. "
    " (level = #warn) ifTrue: [ Transcript put: msg. self log: msg]. "
    (level = #info) ifTrue: [ Transcript put: msg. self log: msg].
    (level = #detail) ifTrue: [ Transcript put: msg. self log: msg].
    (level = #spew) ifTrue: [ Transcript put: msg. self log: msg].



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
    " (level = #error) ifTrue: [ Transcript put: msg. self log: msg]. "
    " (level = #warn) ifTrue: [ Transcript put: msg. self log: msg]. "
    " (level = #info) ifTrue: [ Transcript put: msg. self log: msg]. "
    " (level = #detail) ifTrue: [ Transcript put: msg. self log: msg]. "
    (level = #spew) ifTrue: [ Transcript put: msg. self log: msg].



!
=Log
warn: msg
    " (level = #error) ifTrue: [ Transcript put: msg. self log: msg]. "
    (level = #warn) ifTrue: [ Transcript put: msg. self log: msg].
    (level = #info) ifTrue: [ Transcript put: msg. self log: msg].
    (level = #detail) ifTrue: [ Transcript put: msg. self log: msg].
    (level = #spew) ifTrue: [ Transcript put: msg. self log: msg].



!
" instance methods for Log "
" class methods for Magnitude "
" instance methods for Magnitude "
!Magnitude
<= arg
    ^ self < arg or: [ self = arg ]



!
!Magnitude
> arg
    ^ arg < self



!
!Magnitude
>= arg
    ^ (self > arg) or: [ self = arg ]



!
!Magnitude
between: low and: high
    ^ low <= self and: [ self <= high ]



!
!Magnitude
max: arg
    ^ self < arg ifTrue: [ arg ] ifFalse: [ self ]



!
!Magnitude
min: arg
    ^ self < arg ifTrue: [ self ] ifFalse: [ arg ]



!
" class methods for Association "
=Association
key: k
        "key is set once, value is resettable"
    ^ self in: self new at: 1 put: k



!
=Association
key: k value: v | ret |
        "key is set once, value is resettable"
    ret <- self new.
    self in: ret at: 1 put: k.
    self in: ret at: 2 put: v.
    ^ ret



!
" instance methods for Association "
!Association
< k
        "compare both with keys and associations"
    (k class == Association)
        ifTrue: [ ^ key < k key ]
        ifFalse: [ ^ key < k ]



!
!Association
= k
        "compare both with keys and associations"
    (k class == Association)
        ifTrue: [ ^ key = k key ]
        ifFalse: [ ^ key = k ]



!
!Association
hash
    ^ key hash



!
!Association
key
    ^ key



!
!Association
printString
    ^ '(' + key printString + ' -> ' + value printString + ')'



!
!Association
value
    ^ value



!
!Association
value: v
    value <- v



!
" class methods for Char "
=Char
doInput
    <9>



!
=Char
eof
        " return an EOF indication--not a true Char, but polymorphic "
    ^ self new: 256



!
=Char
input	| c |
    " read a single char from input stream "
    c <- self doInput.
    (c notNil)
        ifTrue: [ ^self new: c ]
        ifFalse: [ ^nil ]



!
=Char
new: value
    " create and initialize a new char "
    ^ self in: self new at: 1 put: value



!
=Char
newline
        " return newline character "
    ^ self new: 10



!
=Char
tab
        " return tab character "
    ^ self new: 9



!
" instance methods for Char "
!Char
< aChar
    ^ value < aChar value



!
!Char
= aChar
    ^ value = aChar value



!
!Char
asString
    " return char as a string value "
    ^ String new: 1; at: 1 put: self



!
!Char
hash
    ^ value



!
!Char
isAlphabetic
    ^ self isLowerCase or: [ self isUpperCase ]



!
!Char
isAlphanumeric
        " are we a letter or a digit? "
    ^ self isAlphabetic or: [ self isDigit ]



!
!Char
isBlank
    " spaces, tabs, carriage returns and newlines are all blank. "
    " These are ordered in guessed most to least common order. "
    (value = 32) ifTrue: [ ^ true ].
    (value = 10) ifTrue: [ ^ true ].
    (value = 9) ifTrue: [ ^ true ].
    (value = 13) ifTrue: [ ^ true ].

    ^ false.



!
!Char
isDigit
    ^ self between: $0 and: $9



!
!Char
isEOF
    ^ value = 256



!
!Char
isLowerCase
    ^ self between: $a and: $z



!
!Char
isUpperCase
    ^ self between: $A and: $Z



!
!Char
lowerCase
    self isUpperCase
        ifTrue: [ ^ Char new: (value - 65) + 97 ]



!
!Char
print
    <3 value>



!
!Char
printString
    ^ String new: 2; at: 1 put: $$ ; at: 2 put: self



!
!Char
upperCase
    self isLowerCase
        ifTrue: [ ^ Char new: (value - 97) + 65 ]



!
!Char
value
        " return our ascii value as an integer "
    ^ value



!
" class methods for Collection "
" instance methods for Collection "
!Collection
< aCollection
    self do: [ :element | (aCollection includes: element)
        ifFalse: [ ^ false ] ].
    ^ true



!
!Collection
= aCollection
    ^ self < aCollection and: [ aCollection < self ]



!
!Collection
anyOne
    self do: [:it| ^ it].
    self emptyCollection



!
!Collection
asArray		| newArray index |
    newArray <- Array new: self size.
    index <- 1.
    self do: [ :element | newArray at: index put: element.
        index <- index + 1 ].
    ^ newArray



!
!Collection
asList
    ^ List new addAll: self



!
!Collection
asString	| newString index |
    newString <- String new: self size.
    index <- 1.
    self do: [ :element | newString at: index put: element.
        index <- index + 1 ].
    ^ newString



!
!Collection
at: value
    ^ self at: value ifAbsent: [ self noElement ]



!
!Collection
at: value ifAbsent: exceptionBlock
    self do: [ :element | 
        ((element class) = (value class)) ifTrue: [
            (element = value) ifTrue: [ ^ element ].
        ].
    ].

    ^ exceptionBlock value



!
!Collection
collect: transformBlock	| newList |
    newList <- List new.
    self do: [:element | newList addLast: (transformBlock value: element)].
    ^ newList



!
!Collection
do: aBlock
    self subclassResponsibility



!
!Collection
emptyCollection
    self error: (self class printString + ' is empty')



!
!Collection
from: argLow to: argHigh | ret idx size base low high |
    low <- argLow max: 1.
    high <- argHigh min: self size.
    size <- (high - low) + 1.
    (size < 1) ifTrue: [ ^ Array new: 0 ].
    ret <- Array new: size.
    base <- idx <- 1.
    self do: [:elem|
        ((idx >= low) and: [idx <= high]) ifTrue: [
            ret at: base put: elem.
            base <- base + 1.
            (base > size) ifTrue: [ ^ ret ]
        ].
        idx <- idx + 1.
    ].
    ^ ret



!
!Collection
includes: value
    self at: value ifAbsent: [ ^ false ].
    ^ true



!
!Collection
isEmpty
        " empty if there are no elements "
    ^ self size = 0



!
!Collection
noElement
    self error: 'Element not present'



!
!Collection
occurencesOf: obj | count |
    count <- 0.
    self do: [:o| (o = obj) ifTrue: [ count <- count + 1]].
    ^ count



!
!Collection
printString | count res |
    res <- super printString.
    (self respondsTo: #do:) ifFalse: [ ^ res ].
    count <- 0.
    res <- res + ' ('.
    self basicDo: [:elem|
        (count = 0) ifFalse: [ res <- res + ' ' ].
        res <- res + elem printString.
        count <- count + 1.
        (count >= 20) ifTrue: [ ^ res + ' ...)' ]
    ].
    ^ res + ')'



!
!Collection
reject: testBlock
        " select the things that do not match predicate "
    ^ self select: [:x | (testBlock value: x) not ]



!
!Collection
select: testBlock	| newList |
    newList <- List new.
    self do: [:x | (testBlock value: x) ifTrue: [newList addLast: x]].
    ^ newList



!
!Collection
size	| tally |
    tally <- 0.
    self do: [:i | tally <- tally + 1].
    ^ tally



!
" class methods for Array "
=Array
new
    ^ self new: 0



!
=Array
new: sz
    <7 self sz>



!
=Array
with: elemA
    ^ self in: (self new: 1) at: 1 put: elemA



!
=Array
with: elemA with: elemB | ret |
    ret <- self new: 2.
    self in: ret at: 1 put: elemA.
    self in: ret at: 2 put: elemB.
    ^ ret



!
=Array
with: elemA with: elemB with: elemC | ret |
    ret <- self new: 3.
    self in: ret at: 1 put: elemA.
    self in: ret at: 2 put: elemB.
    self in: ret at: 3 put: elemC.
    ^ ret



!
" instance methods for Array "
!Array
+ aValue	| sizeA sizeB newValue |
    " concatenate two arrays together "
    " FIXME - should check to make sure the arrays are the same class. "
    sizeA <- self size.
    sizeB <- aValue size.
    newValue <- self class new: (sizeA + sizeB).
    newValue replaceFrom: 1 to: sizeA with: self.
    newValue replaceFrom: (sizeA + 1) to: (sizeA + sizeB) with: aValue.
    ^ newValue



!
!Array
< arg		| selfsize argsize |
    selfsize <- self size. argsize <- arg size.
    1 to: (selfsize min: argsize)
        do: [:i | (self at: i) ~= (arg at: i)
            ifTrue: [ ^ (self at: i) < (arg at: i) ]].
    ^ selfsize < argsize



!
!Array
= anArray
    (anArray isKindOf: Collection) ifFalse: [ ^ false ].
    self size = anArray size ifFalse: [ ^ false ].
    1 to: self size do:
        [:i | (self at: i) = (anArray at: i)
            ifFalse: [ ^ false ]].
    ^ true



!
!Array
at: index
    <24 self index>
    (self includesKey: index) ifFalse: [ self badIndex ].
    self primitiveFailed



!
!Array
at: index ifAbsent: exceptionBlock
    <24 self index>
    exceptionBlock value



!
!Array
at: index put: value
    <5 value self index>
    (self includesKey: index) ifFalse: [ self badIndex ].
    self primitiveFailed



!
!Array
badIndex
    self error: 'array indexing error'



!
!Array
copy
    ^ self asArray



!
!Array
do: aBlock
    1 to: self size do: [:i | aBlock value: (self at: i)]



!
!Array
elementsExchangeIdentityWith: otherArray
    <35 self otherArray>.
    self primitiveFailed



!
!Array
first
    ^self at: 1



!
!Array
from: low to: high | start stop size obj |
    start <- low max: 0.
    stop <- high min: self size.
    size <- (stop + 1 - start) max: 0.
    obj <- (self species) new: size.
    1 to: size do: [ :i |
        obj at: i put: (self at: start).
        start <- start + 1 ].
    ^ obj



!
!Array
hash | sz |
    sz <- self size.
    (sz < 2) ifTrue: [
        (sz = 1) ifTrue: [ ^ (self at: 1) hash + sz ].
        ^ 0
    ].
    ^ (self at: 1) hash + (self at: sz) hash



!
!Array
includes: aValue
    self do: [ :element | 
        " only compare elements of the same type. "
        ((element class) = (aValue class)) ifTrue: [
            element = aValue ifTrue: [ ^ true ].
        ].
    ].

    ^ false



!
!Array
includesKey: index
    ^ index between: 1 and: self size



!
!Array
indexOf: aValue
    1 to: self size do: [:idx|
        ((self at: idx) == aValue) ifTrue: [ ^ idx ]
    ].
    ^ nil



!
!Array
indexOfVal: aValue
    1 to: self size do: [:idx|
        ((self at: idx) = aValue) ifTrue: [ ^ idx ]
    ].
    ^ nil



!
!Array
insert: value at: position | newArray newSize |
    newSize <- self size + 1.
    newArray <- self class new: newSize.
    newArray replaceFrom: 1 to: position-1 with: self.
    newArray at: position put: value.
    newArray replaceFrom: position+1 to: newSize with:
        self startingAt: position.
    ^ newArray



!
!Array
removeIndex: position  | newArray newSize |
    newSize <- self size - 1.
    newArray <- self class new: newSize.
    newArray replaceFrom: 1 to: position-1 with: self.
    newArray replaceFrom: position to: newSize with: self
        startingAt: position+1.
    ^ newArray



!
!Array
replaceFrom: start to: stop with: replacement
    ^ self replaceFrom: start to: stop with: replacement startingAt: 1



!
!Array
replaceFrom: start to: stop with: replacement startingAt: repStart | base |
    <38 start stop replacement repStart self>.
    base <- repStart-1.
    0 to: (stop - start) do: [:idx|
        self at: (idx + start) put:
            (replacement at: (idx + repStart))
    ]



!
!Array
size
    " compute number of elements "
    <4 self>



!
!Array
startsWith: prefix
    " Return true if we start with the passed prefix string, false if not. "

    " if we are smaller than the prefix, then we do not start with the prefix! "
    ((self size) < (prefix size)) ifTrue: [ ^ false ].

    " compare all elements up to the prefix size "
    (1 to: (prefix size)) do: [ :i |  ((self at: i) = (prefix at: i)) ifFalse: [ ^ false ] ].

    ^ true

   


!
!Array
with: newItem	| newArray size |
    size <- self size.
    newArray <- self class new: size + 1.
    newArray replaceFrom: 1 to: size with: self.
    newArray at: size + 1 put: newItem
    ^ newArray



!
" class methods for ByteArray "
=ByteArray
new: size
    <20 self size>



!
" instance methods for ByteArray "
!ByteArray
asString | str sz |
    sz <- self size.
    str <- String new: sz.
    1 to: sz do: [:i| str at: i put: ((self at: i) asChar)].
    ^ str



!
!ByteArray
at: index
    <21 self index>
    (self includesKey: index) ifFalse: [ self badIndex ].
    self primitiveFailed



!
!ByteArray
at: index ifAbsent: exceptionBlock
    <21 self index>
    exceptionBlock value



!
!ByteArray
at: index put: aValue
    <22 aValue self index>
    (self includesKey: index) ifFalse: [ self badIndex ].
    self primitiveFailed



!
!ByteArray
basicAt: index
    <21 self index>
    ^nil



!
" class methods for OrderedArray "
" instance methods for OrderedArray "
!OrderedArray
add: value
    ^ self insert: value at: (self location: value)



!
!OrderedArray
includes: value | position |
    position <- self location: value.
    ^ (position <= self size) and: [ value = (self at: position)]



!
!OrderedArray
location: value | low high mid |
    low <- 1.
    high <- self size + 1.
    [ low < high ] whileTrue:
        [ mid <- (low + high) quo: 2.
        (self at: mid) < value
            ifTrue: [ low <- mid + 1 ]
            ifFalse: [ high <- mid ] ].
    ^ low



!
" class methods for String "
=String
input	| value c nl |
    " read a line from input "
    value <- ''. nl <- Char newline.
    [ c <- Char input.
      c isNil ifTrue: [ ^ nil ]. c ~= nl ] whileTrue:
        [ value <- value + c asString ].
    ^ value



!
=String
new: size
    <20 self size>



!
" instance methods for String "
!String
asNumber | val |
    " parse a base-10 ASCII number, return nil on failure "
    val <- 0.
    self do: [:c|
        c isDigit ifFalse: [^nil].
        val <- (val * 10) + (c value - 48)
    ].
    ^val



!
!String
asString " for symmetry with Symbol. "
    ^ self



!
!String
asSymbol
    ^ Symbol new: self



!
!String
at: index
    ^self at: index ifAbsent: [ self badIndex ]



!
!String
at: index ifAbsent: exceptionBlock | c |
    c <- self basicAt: index.
    (c isNil)
         ifTrue: [ ^ exceptionBlock value ]
         ifFalse: [ ^ Char new: c ]



!
!String
at: index put: aValue
    (self basicAt: index put: aValue value) isNil ifTrue: [
        self badIndex
    ]



!
!String
basicAt: index
    <21 self index>
    ^nil



!
!String
basicAt: index put: value
    <22 value self index>
    ^nil



!
!String
break: separators  | wordStart wordEnd words word |
    " break string into words, using separators "
    word <- ''.
    words <- List new.

    " get the word boundaries. "

    wordStart <- 1.
    wordEnd <- 0.
    
    (1 to: (self size)) do: [ :i |
         (separators includes: (self at: i)) ifTrue: [
              (wordEnd >= wordStart) ifTrue: [
                  word <- self from: wordStart to: wordEnd.
                  words addLast: word.
              ].

              wordStart <- (i + 1).
         ] ifFalse: [ wordEnd <- i ].
    ].

    " if there is any remaining word or there were no separators. "
    (wordEnd >= wordStart) ifTrue: [
        word <- self from: wordStart to: wordEnd.
        words addLast: word.
    ].

    ^ words



!
!String
collect: transformationBlock
    ^ (super collect: transformationBlock) asString



!
!String
copy
    " make a clone of ourself "
    <23 self String>



!
!String
copyInto: aString at: anIndex | i |
    " copy ourselves into another string starting at anIndex "

    i <- anIndex.

    self do: [ :c |
        aString at: i put: c.
        i <- i + 1.
    ]

    ^ self




!
!String
doIt	| meth |
    meth <- Undefined parseMethod: 'doItCommand ^' + self.
    ^ meth notNil
        ifTrue: [ ^ Context new
              perform: meth withArguments: (Array new: 1) ]



!
!String
doIt: aReq	
     " REMOVE "
    ^ nil



!
!String
edit
    <105 self>



!
!String
encodeHTML | transStr lt gt newSize targIndex |
    " encode < and > so that HTML can show correctly in HTML "

    lt <- Char new: 60.  " < character "
    gt <- Char new: 62.  " > character "

    newSize <- 0.

    " get the new size. "
    self do: [ :c |
         (( c = lt ) or: [ c = gt ]) ifTrue: [ newSize <- newSize + 4. ]
                                    ifFalse: [ newSize <- newSize + 1. ].
    ].

    " allocate a new string of the right size, 3 = 4 chars for replacement minus the one that is there now. "
    transStr <- String new: newSize.

    targIndex <- 1.

    self do: [ :c |
        ( c = lt ) ifTrue: [
            transStr at: targIndex put: $&.
            transStr at: (targIndex + 1) put: $l.
            transStr at: (targIndex + 2) put: $t.
            transStr at: (targIndex + 3) put: $;.
            targIndex <- targIndex + 4.
        ] ifFalse: [
            ( c = gt ) ifTrue: [
                transStr at: targIndex put: $&.
                transStr at: (targIndex + 1) put: $g.
                transStr at: (targIndex + 2) put: $t.
                transStr at: (targIndex + 3) put: $;.
                targIndex <- targIndex + 4.
            ] ifFalse: [
                transStr at: targIndex put: c.
                targIndex <- targIndex + 1.
            ].
        ].
    ].



    " transform the string into something that can be put into HTML. "
    " self do: [ :c | 
        ( c = lt )
            ifTrue: [ c <- '<' ]
            ifFalse: [ 
                ( c = gt )
                     ifTrue: [ c <- '>' ]
            ].

            transStr <- transStr + (c asString).
    ]."

    ^ transStr




!
!String
from: low to: high | start stop size newString |
    start <- low max: 0.
    stop <- high min: self size.
    size <- (stop + 1 - start) max: 0.
    newString <- String new: size.
    1 to: size do: [ :i |
        newString at: i put: (self at: start).
        start <- start + 1 ].
    ^ newString



!
!String
fromUrl
    " convert from URL encoding "
    <152 self>.

    self primitiveFailed



!
!String
hash | sz |
    sz <- self size.
    (sz < 2) ifTrue: [
        (sz = 1) ifTrue: [ ^ (self at: 1) value ].
        ^ 0
    ].
    ^ (self at: 1) value + (self at: sz) value



!
!String
position: aString
    " find arg as substring and return position "

    <150 self aString>.

    self primitiveFailed



!
!String
printString
    ^ self



!
!String
printWidth: width | ret |
    (self size >= width absolute) ifTrue: [ ^ self ].
    ret <- self.
    (width negative) ifTrue: [
            (self size + 1) to: (width negated) do:
                [:ignore| ret <- ' ' + ret].
        ]
        ifFalse: [
            (self size + 1) to: width do:
                [:ignore| ret <- ret + ' ' ].
        ].
    ^ret



!
!String
reverse
    ^ self asList reverse asString



!
!String
select: testBlock
    ^ (super select: testBlock) asString



!
!String
toUrl
    " convert to URL encoding "
    <151 self>.

    self primitiveFailed



!
" class methods for Dictionary "
=Dictionary
new | newDict |
    newDict <- super new.
    self in: newDict at: 1 put: (OrderedArray new: 0).
    self in: newDict at: 2 put: (Array new: 0).
    ^ newDict



!
" instance methods for Dictionary "
!Dictionary
at: key
    ^ self at: key ifAbsent: [ self noKey: key ]



!
!Dictionary
at: key ifAbsent: exceptionBlock | position |
    position <- keys location: key.
    ((position <= keys size) and: [ key = (keys at: position)])
        ifTrue: [ ^ values at: position ]
        ifFalse: [ ^ exceptionBlock value ]



!
!Dictionary
at: key put: value | position |
    position <- keys location: key.
    (position <= keys size and: [ key = (keys at: position)])
        ifTrue: [ values at: position put: value ]
        ifFalse: [ keys <- keys insert: key at: position.
            values <- values insert: value at: position ].
    ^ value



!
!Dictionary
binaryDo: aBlock
    1 to: keys size do:
        [:i | aBlock value: (keys at: i) value: (values at: i) ]



!
!Dictionary
do: aBlock
    values do: aBlock



!
!Dictionary
isEmpty
    ^ keys isEmpty



!
!Dictionary
keysAsArray | i ret |
    ret <- Array new: keys size.
    1 to: keys size do: [:i| ret at: i put: (keys at: i)].
    ^ ret



!
!Dictionary
keysDo: aBlock
    1 to: keys size do: [:i| aBlock value: (keys at: i)]



!
!Dictionary
noKey: key
    self error: ('key #' + (key asString) + ' not found in dictionary!')



!
!Dictionary
printString | count res |
    res <- self class printString + ' ('.
    count <- 0.
    self binaryDo: [:k :elem|
        (count = 0) ifFalse: [ res <- res + ', ' ].
        res <- res + (k printString + ' -> ' + elem printString).
        count <- count + 1.
        (count >= 20) ifTrue: [ ^ res + ', ...)' ]
    ].
    ^ res + ')'



!
!Dictionary
removeKey: key
    ^ self removeKey: key ifAbsent: [ self noKey: key ]



!
!Dictionary
removeKey: key ifAbsent: exceptionBlock | position |
    position <- keys location: key.
    (position <= keys size and: [ key = (keys at: position) ])
        ifTrue: [ keys <- keys removeIndex: position.
            values <- values removeIndex: position]
        ifFalse: [ ^ exceptionBlock value ]



!
" class methods for Interval "
=Interval
from: l to: h step: s | newInterval |
    newInterval <- self in: self new at: 1 put: l.
    self in: newInterval at: 2 put: h.
    self in: newInterval at: 3 put: s.
    ^ newInterval



!
" instance methods for Interval "
!Interval
atRandom | ret |
    " Return a random element from our sequence "
    ret <- (SmallInt atRandom) rem: ((high - low + 1) quo: step).
    ^ low + (ret * step)



!
!Interval
do: aBlock	| current |
    current <- low.
    (step < 0)
        ifTrue: [
            [ current >= high ] whileTrue:
                [ aBlock value: current.
                current <- current + step ] ]
        ifFalse: [
            [ current <= high ] whileTrue:
                [ aBlock value: current.
                current <- current + step ] ]



!
!Interval
high
    ^ high



!
!Interval
high: h
    high <- h



!
!Interval
includes: val
    " Check within range first "
    ((val < low) or: [val > high]) ifTrue: [ ^ false ].
    " Then check if in equivalence class of interval "
    ^ ((val - low) rem: step) = 0



!
!Interval
low
    ^ low



!
!Interval
low: l
    low <- l



!
!Interval
printString | s |
    s <- (self class printString) + ' <' +
        low printString + '..' + high printString.
    (step ~= 1) ifTrue: [ s <- s + ' by ' + step printString ].
    ^ s + '>'



!
" class methods for List "
=List
with: firstElement	| newList |
    newList <- self new.
    newList add: firstElement.
    ^ newList



!
" instance methods for List "
!List
add: anElement
    elements <- Link value: anElement next: elements.
    ^ anElement



!
!List
addAll: aCollection
    aCollection do: [ :element | self addLast: element ]



!
!List
addLast: anElement
    elements isNil
        ifTrue: [ self add: anElement]
        ifFalse: [ elements addLast: anElement ].
    ^ anElement



!
!List
at: index | link |
    link <- self findLink: index ifAbsent: [ self badIndex ].
    ^ link value



!
!List
at: index ifAbsent: aBlock | link |
    link <- self findLink: index ifAbsent: [nil].
    link isNil ifTrue: [ ^ aBlock value ].
    ^ link value



!
!List
at: index put: value | link |
    link <- self findLink: index.
    link value: value



!
!List
badIndex
    self error: 'Invalid List index'



!
!List
copy
    ^ self asList



!
!List
do: aBlock
    ^ elements notNil ifTrue: [ elements do: aBlock ]



!
!List
findLink: index ifAbsent: aBlock | idx link |
    link <- elements.
    idx <- index.
    link isNil ifTrue: [ ^ aBlock value ].
    [ link notNil ] whileTrue: [
        idx <- idx-1.
        (idx = 0) ifTrue: [ ^ link ].
        link <- link next
    ].
    ^ aBlock value



!
!List
first
    ^ self at: 1



!
!List
isEmpty
    ^ elements isNil



!
!List
remove: anElement
    self remove: anElement
        ifAbsent: [ self emptyCollection ]



!
!List
remove: anElement ifAbsent: exceptionBlock
    elements isNil
        ifTrue: [ exceptionBlock value ]
        ifFalse: [ elements remove: anElement ifAbsent: exceptionBlock ]



!
!List
removeFirst
    elements isNil
        ifTrue: [ self emptyCollection ]
        ifFalse: [ elements <- elements next ]



!
!List
reverse | newList |
    newList <- List new.
    self do: [ :element | newList add: element ].
    ^ newList



!
!List
reverseDo: aBlock
    ^ elements notNil ifTrue: [ elements reverseDo: aBlock ]



!
!List
select: testBlock | newList |
    newList <- List new.
    self reverseDo: [:element | (testBlock value: element)
        ifTrue: [ newList add: element ] ].
    ^ newList



!
" class methods for StringBuffer "
" instance methods for StringBuffer "
!StringBuffer
add: anObj
    ^ super add: (anObj printString).



!
!StringBuffer
addLast: anObj
    ^ super addLast: (anObj printString).



!
!StringBuffer
asString | size index result |
    size <- self size.

    result <- String new: size.

    index <- 1.

    self do: [ :entry | (entry asString) do: [ :char | result at: index put: char. index <- index + 1 ] ].

    ^ result.



!
!StringBuffer
printString
    ^ (self asString)
    


!
!StringBuffer
size | tempSize |
    tempSize <- 0.
    self do: [:entry | tempSize <- tempSize + (entry size) ].

    ^ tempSize



!
!StringBuffer
write: anObj
    self addLast: anObj.

    ^ self



!
" class methods for Set "
=Set
new
    ^ self new: 10



!
=Set
new: size | ret |
    ret <- super new.
    self in: ret at: 1 put: (Array new: size).
    self in: ret at: 2 put: size.
    ^ ret



!
" instance methods for Set "
!Set
add: elem | pos |
    " Find the appropriate slot... if none, need to grow the Set "
    pos <- self location: elem.
    pos isNil ifTrue: [
        self grow.
        ^ self add: elem
    ].

    " If the slot is nil, this is a new entry which we put in place now.
      If it wasn't nil, we still re-store it so that if it's an
      Association, the value portion will be updated. "
    members at: pos put: elem.
    ^ elem



!
!Set
at: value ifAbsent: aBlock | pos |
    pos <- self location: value.
    ((pos isNil) or: [ (members at: pos) isNil ]) ifTrue: [
        ^ aBlock value
    ].
    ^ value



!
!Set
compare: t and: e
    ^ t = e



!
!Set
do: aBlock
    members do: [:elem| elem notNil ifTrue: [ aBlock value: elem ]]



!
!Set
grow | bigger old oldsize |
    " Re-create ourselves in place with a new, bigger storage "
    old <- members.
    members <- Array new: (old size + growth).

    " Re-insert each existing Set member "
    old do: [:elem| self add: elem]



!
!Set
indexOf: value
    ^ self at: value ifAbsent: [ nil ]



!
!Set
location: elem | pos start t |
    start <- pos <- (elem hash rem: members size) + 1.
    [ true ] whileTrue: [
        " Return this position if we match, or have reached
          a nil slot. "
        t <- members at: pos.
        ((t isNil) or: [self compare: t and: elem]) ifTrue: [
            ^ pos
        ].

        " Advance to next slot, circularly "
        pos <- pos + 1.
        (pos > members size) ifTrue: [
            pos <- 1
        ].

        " Return nil if we have scanned the whole Set "
        (pos = start) ifTrue: [ ^ nil ]
    ]



!
!Set
rehash: start | pos elem |
    pos <- start.
    [ true ] whileTrue: [
        " Advance to next slot, ceasing when we reach our start "
        pos <- pos + 1.
        (pos > members size) ifTrue: [ pos <- 1 ].
        (pos = start) ifTrue: [ ^ self ]

        " If we reach a nil slot, there are no further rehash
          worries. "
        elem <- members at: pos.
        elem isNil ifTrue: [ ^ self ].

        " Nil out the slot, and then re-insert the element "
        members at: pos put: nil.
        self add: elem
    ]



!
!Set
remove: elem
    ^ self remove: elem ifAbsent: [self noElement ]



!
!Set
remove: elem ifAbsent: aBlock | pos |
    " If not found, return error "
    pos <- self location: elem.
    ((pos isNil) or: [(members at: pos) isNil]) ifTrue: [
        aBlock value
    ].

    " Remove our element from the Set "
    members at: pos put: nil.

    " Re-hash all that follow "
    self rehash: pos.

    ^ elem



!
!Set
size | tally |
    tally <- 0.
    members do: [:elem| elem notNil ifTrue: [ tally <- tally + 1 ] ].
    ^ tally



!
" class methods for IdentitySet "
" instance methods for IdentitySet "
!IdentitySet
compare: t and: e
    ^ t == e



!
" class methods for Tree "
" instance methods for Tree "
!Tree
add: anElement
    root isNil
        ifTrue: [ root <- Node new: anElement ]
        ifFalse: [ root add: anElement ].
    ^anElement



!
!Tree
addAll: aCollection
    aCollection do: [:element| self add: element ]



!
!Tree
at: key ifAbsent: exceptionBlock
    root isNil
        ifTrue: [ ^ exceptionBlock value ]
        ifFalse: [ ^ root at: key ifAbsent: exceptionBlock ]



!
!Tree
collect: transformBlock | newTree |
    newTree <- Tree new.
    self do: [:element| newTree add: (transformBlock value: element)]
    ^newTree



!
!Tree
copy
    ^Tree new addAll: self



!
!Tree
do: aBlock
    root notNil ifTrue: [ root do: aBlock ]



!
!Tree
first
    root notNil
        ifTrue: [ ^root first ]
        ifFalse: [ self emptyCollection ]



!
!Tree
isEmpty
    ^ root isNil



!
!Tree
remove: key ifAbsent: exceptionBlock
    root isNil
        ifTrue: [ exceptionBlock value ]
        ifFalse: [ root <- root remove: key ifAbsent: exceptionBlock ]



!
!Tree
removeFirst
    root isNIl ifTrue: [ self emptyCollection ].
    root <- root removeFirst



!
!Tree
reverseDo: aBlock
    root notNil ifTrue: [ root reverseDo: aBlock ]



!
!Tree
select: testBlock | newTree |
    newTree <- Tree new.
    self do: [:element|
        (testBlock value: element)
            ifTrue: [newTree add: element]
    ].
    ^newTree



!
" class methods for Number "
=Number
new
    " can't create this way, return zero "
    ^ 0



!
" instance methods for Number "
!Number
absolute
    (self negative) ifTrue: [ ^ self negated ]



!
!Number
asChar
    ^ Char new: (self asSmallInt)



!
!Number
asDigit
    (self < 10) ifTrue:
        [ ^(Char new: (self asSmallInt + 48)) asString ].
    ^(Char new: (self asSmallInt + 55)) asString



!
!Number
atRandom
    " Return random number from 1 to self "
    (self < 2) ifTrue: [ ^ self ].
    ^ ((1 to: self) atRandom)



!
!Number
bitAnd: arg
    ^ (self asSmallInt bitAnd: arg)



!
!Number
bitOr: arg
    ^ (self asSmallInt bitOr: arg)



!
!Number
bitShift: arg
    ^ (self asSmallInt bitShift: arg)



!
!Number
factorial
    self <= 1 ifTrue: [ ^ 1 ]
    ifFalse: [ ^ (self - 1) factorial * self ]



!
!Number
negated
    ^0-self



!
!Number
negative
    ^self < 0



!
!Number
overflow
    self error: 'Numeric overflow'



!
!Number
printString
    ^self printWidth: 1 base: 10



!
!Number
printWidth: width
    ^self printWidth: width base: 10



!
!Number
printWidth: width base: base | res n dig wasNeg wide |
    res <- ''.
    (self negative) ifTrue: [
        wasNeg <- true.
        wide <- width-1.
        n <- self negated
    ] ifFalse: [
        wasNeg <- false.
        wide <- width.
        n <- self
    ].
    [true] whileTrue: [
        res <- ((n rem: base) asDigit) + res.
        n <- n quo: base.
        (n = 0) ifTrue: [
            ((res size)+1) to: wide do: [:ignore|
                res <- '0' + res
            ].
            wasNeg ifTrue: [ res <- '-' + res ].
            ^res
        ]
    ]



!
!Number
to: limit
    ^ Interval from: self to: limit step: 1



!
!Number
to: limit by: step
    ^ Interval from: self to: limit step: step



!
!Number
to: limit by: step do: aBlock  | i |
    i <- self.
    [ i <= limit ] whileTrue: [ aBlock value: i. i <- i + step ]



!
!Number
to: limit do: aBlock  | i |
        " optimize arithmetic loops "
    i <- self.
    [ i <= limit ] whileTrue: [ aBlock value: i. i <- i + 1 ]



!
" class methods for Integer "
=Integer
new: low
    <32 low>
    low <- low asSmallInt.
    <32 low>
    self primitiveFailed



!
" instance methods for Integer "
!Integer
* arg
    <28 self arg>
    (arg isMemberOf: Integer) ifFalse: [^self * arg asInteger].
    self primitiveFailed



!
!Integer
+ arg
    <27 self arg>
    (arg isMemberOf: Integer) ifFalse: [^self + arg asInteger].
    self primitiveFailed



!
!Integer
- arg
    <29 self arg>
    (arg isMemberOf: Integer) ifFalse: [^self - arg asInteger].
    self primitiveFailed



!
!Integer
< arg
    <30 self arg>
    (arg isMemberOf: Integer) ifFalse: [^self < arg asInteger].
    self primitiveFailed



!
!Integer
= arg
    <31 self arg>
    (arg isMemberOf: Integer) ifFalse: [^self = arg asInteger].
    self primitiveFailed



!
!Integer
asInteger
    ^self



!
!Integer
asSmallInt
    <33 self>.
    self overflow



!
!Integer
hash
    <33 self>.
    ^ (self rem: 65536) asSmallInt



!
!Integer
quo: arg
    <25 self arg>
    (arg isMemberOf: Integer) ifFalse: [^self quo: arg asInteger].
    (0 = arg) ifTrue: [^ self error: 'division by zero'].
    self primitiveFailed



!
!Integer
rem: arg
    <26 self arg>
    (arg isMemberOf: Integer) ifFalse: [^self rem: arg asInteger].
    (0 = arg) ifTrue: [^ self error: 'division by zero'].
    self primitiveFailed



!
!Integer
truncSmallInt
    <40 self>.
    self primitiveFailed



!
" class methods for SmallInt "
=SmallInt
atRandom
    " Set up seed one time.  TBD: init from something external; getpid() or time() "

    seed isNil ifTrue: [ seed <- 17 ].

    " Rotate the random number generator. From Wikipedia. "
    seed <- (((seed * 8121) + 28411) rem: 134456) truncSmallInt.

    ^ seed.



!
" instance methods for SmallInt "
!SmallInt
* arg
    <15 self arg>
    (arg isMemberOf: SmallInt) ifFalse: [^self * arg asSmallInt].
    self primitiveFailed



!
!SmallInt
+ arg
    <10 self arg>
    (arg isMemberOf: SmallInt) ifFalse: [^self + arg asSmallInt].
    self primitiveFailed



!
!SmallInt
- arg
    <16 self arg>
    (arg isMemberOf: SmallInt) ifFalse: [^self - arg asSmallInt].
    self primitiveFailed



!
!SmallInt
< arg
    <13 self arg>
    (arg isMemberOf: SmallInt) ifFalse: [^self < arg asSmallInt].
    self primitiveFailed



!
!SmallInt
= arg
    <14 self arg>
    (arg isMemberOf: SmallInt) ifFalse: [^self = arg asSmallInt].
    self primitiveFailed



!
!SmallInt
asInteger
    ^Integer new: self



!
!SmallInt
asSmallInt
    ^self



!
!SmallInt
bitAnd: arg
    <37 self arg>.
    ^ (self bitAnd: arg asSmallInt)



!
!SmallInt
bitOr: arg
    <36 self arg>.
    ^ (self bitOr: arg asSmallInt)



!
!SmallInt
bitShift: arg
    <39 self arg>.
    (arg isKindOf: SmallInt) ifTrue: [ self overflow ].
    ^ (self bitShift: arg asSmallInt)



!
!SmallInt
hash
    ^ self



!
!SmallInt
quo: arg
    <11 self arg>
    (arg isMemberOf: SmallInt) ifFalse: [^self quo: arg asSmallInt].
    (0 = arg) ifTrue: [^ self error: 'division by zero'].
    self primitiveFailed



!
!SmallInt
rem: arg
    <12 self arg>
    (arg isMemberOf: SmallInt) ifFalse: [^self rem: arg asSmallInt].
    (0 = arg) ifTrue: [^ self error: 'division by zero'].
    self primitiveFailed



!
!SmallInt
truncSmallInt
    ^self



!
" class methods for Symbol "
=Symbol
intern: string
    <23 string Symbol>



!
=Symbol
new: fromString | sym |
    ^ symbols at: fromString
        ifAbsent: [ symbols add: (self intern: fromString) ]



!
" instance methods for Symbol "
!Symbol
< arg
        " works with either symbol or string arguments "
    ^ self printString < arg printString



!
!Symbol
= aString
        " works with either symbol or string arguments "
    ^ self printString = aString printString



!
!Symbol
asString
    ^self printString



!
!Symbol
asSymbol
    ^self



!
!Symbol
hash
    ^self printString hash



!
!Symbol
implementors | result classes literals |
    result <- List new.

    classes <- globals select: [ :o | o isKindOf: Class ].

    classes do: [ :c |
        ((c methods) at: self ifAbsent: [nil])  notNil ifTrue: [ result add: c ].
    ].

    ^ result

        
    


!
!Symbol
printString
    <23 self String>



!
!Symbol
senders | result classes literals |
    result <- List new.

    classes <- globals select: [ :o | o isKindOf: Class ].

    classes do: [ :c |
        (c methods) binaryDo: [ :n :m |
            " Transcript put: ('Checking method ' + (c printString) + '#' + (n printString)). "

            ((m literals) notNil) ifTrue: [
                ((m literals) includes: self) ifTrue: [ result add: m ].
            ].
        ].
    ].

    ^ result

        
    


!
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
" class methods for Node "
=Node
new: value
    " creation, left left and right empty "
    ^ self in: self new at: 1 put: value



!
" instance methods for Node "
!Node
add: anElement
    value < anElement
        ifTrue: [ right notNil
            ifTrue: [ right add: anElement ]
            ifFalse: [ right <- Node new: anElement ] ]
        ifFalse: [ left notNil
            ifTrue: [ left add: anElement ]
            ifFalse: [ left <- Node new: anElement ] ]



!
!Node
at: key ifAbsent: exceptionBlock
    value = key ifTrue: [ ^ value ].
    value < key
        ifTrue: [ right notNil
            ifTrue: [ ^ right at: key ifAbsent: exceptionBlock ]
            ifFalse: [ ^ exceptionBlock value ] ]
        ifFalse: [ left notNil
            ifTrue: [ ^ left at: key ifAbsent: exceptionBlock ]
            ifFalse: [ ^ exceptionBlock value ] ]



!
!Node
do: aBlock
    left notNil ifTrue: [ left do: aBlock ].
    aBlock value: value.
    ^ right notNil ifTrue: [ right do: aBlock ]



!
!Node
first
    left notNil
        ifTrue: [ ^ left first ]
        ifFalse: [ ^ value ]



!
!Node
remove: key ifAbsent: exceptionBlock
    value = key
        ifTrue: [ right notNil
            ifTrue: [ value <- right first.
            right <- right removeFirst.
            ^ self ]
            ifFalse: [ ^ left ] ].
    value < key
        ifTrue: [ right notNil
            ifTrue: [ right <- right remove: key ifAbsent: exceptionBlock ]
            ifFalse: [ ^ exceptionBlock value ] ]
        ifFalse: [ left notNil
            ifTrue: [ left <- left removeL key ifAbsent: exceptionBlock ]
            ifFalse: [ ^ exceptionBlock value ] ]



!
!Node
removeFirst
    left notNil
        ifTrue: [ left <- left removeFirst. ^ self ]
        ifFalse: [ ^ right ]



!
!Node
reverseDo: aBlock
    right notNil ifTrue: [ right do: aBlock ].
    aBlock value: value.
    left notNil ifTrue: [ left do: aBlock ]



!
!Node
value
    ^ value



!
" class methods for Parser "
" instance methods for Parser "
!Parser
addArgName: name
    ((instNames includes: name)
        or: [ argNames includes: name ])
        ifTrue: [ self error: 'doubly defined argument name: ' +
            name asString].
    argNames <- argNames with: name



!
!Parser
addTempName: name
    (((argNames includes: name)
        or: [ instNames includes: name ] )
        or: [ tempNames includes: name ] )
        ifTrue: [ self error: 'doubly defined name '].
    tempNames <- tempNames with: name.
    maxTemps <- maxTemps max: tempNames size



!
!Parser
arrayLiteral	| node |
    tokenType isAlphabetic
        ifTrue: [ node <- Symbol new: token. self nextLex. ^ node ].
    ^ self readLiteral



!
!Parser
binaryContinuation: base | receiver name lnum |
    receiver <- self unaryContinuation: base.
    [ self tokenIsBinary]
        whileTrue: [ lnum <- lineNum.
            name <- token asSymbol. self nextLex.
            receiver <- (MessageNode at: lnum)
                receiver: receiver name: name arguments:
                    (List with:
                        (self unaryContinuation: self readTerm)) ].
    ^ receiver



!
!Parser
charIsSyntax: c
    ^ ('.()[]#^$;' includes: c) or: [ c = $' ]



!
!Parser
currentChar
    ^ text at: index ifAbsent: [ Char eof ]



!
!Parser
error: aString
    'Compile error near line ' print.
    lineNum printString print.
    ': ' print.
    aString printNl.
    errBlock value



!
!Parser
keywordContinuation: base  | receiver name args lnum |
    receiver <- self binaryContinuation: base.
    self tokenIsKeyword
        ifFalse: [ ^ receiver ].
    name <- ''.
    args <- List new.
    lnum <- lineNum.
    [ self tokenIsKeyword ]
        whileTrue: [ name <- name + token. self nextLex.
            args add:
                (self binaryContinuation: self readTerm) ].
    ^ (MessageNode at: lnum) receiver:
        receiver name: name asSymbol arguments: args



!
!Parser
lexAlphabetic | cc start |
    start <- index.
    [ ((cc <- self nextChar) isAlphabetic) or: [ cc = $: ] ]
            whileTrue: [ nil ].
        " add any trailing colons "
    token <- text from: start to: index - 1



!
!Parser
lexBinary	| c d |
    c <- self currentChar.
    token <- c asString.
    d <- self nextChar.
    (self charIsSyntax: c) ifTrue: [ ^ token ].
    (((d isBlank
        or: [ d isDigit])
        or: [ d isAlphabetic ])
        or: [ self charIsSyntax: d])
            ifTrue: [ ^ token ].
    token <- token + d asString.
    self nextChar



!
!Parser
lexInteger	| start |
    start <- index.
    [ self nextChar isDigit ]
        whileTrue: [ nil ].
    token <- text from: start to: index - 1



!
!Parser
nameNode: name
    " make a new name node "

    name == #super
        ifTrue: [ ^ (ArgumentNode at: lineNum) position: 0 ].

    (1 to: tempNames size) do: [:i |
        (name == (tempNames at: i))
            ifTrue: [ ^ (TemporaryNode at: lineNum)
                position: i ] ].

    (1 to: argNames size) do: [:i |
        (name == (argNames at: i))
            ifTrue: [ ^ (ArgumentNode at: lineNum) position: i ] ].

    " newer names shadow older ones; lookup in reverse to get newer ones first. "
    ((instNames size) to: 1 by: -1) do: [:i |
        (name == (instNames at: i)) ifTrue: [ 
            " debug "
            " ('Parser#nameNode found instance var ' + (name printString) + ' at offset ' + (i printString)) printNl. " 
            ^ (InstNode at: lineNum) position: i 
        ].
    ].

    ^ (LiteralNode at: lineNum);
        value: (globals at: name
            ifAbsent: [ ^ self error:
                'unrecognized name: ' + name printString ])



!
!Parser
nextChar
    (self currentChar = Char newline) ifTrue: [
        lineNum <- lineNum + 1
    ].
    index <- index + 1.
    ^ self currentChar



!
!Parser
nextLex
    self skipBlanks.
    tokenType <- self currentChar.
    tokenType isEOF   " end of input "
        ifTrue: [ tokenType <- $  . token <- nil. ^ nil ].
    tokenType isDigit ifTrue: [ ^ self lexInteger ].
    tokenType isAlphabetic ifTrue: [ ^ self lexAlphabetic ].
    ^ self lexBinary



!
!Parser
parse: c
    ^ self parse: c with: Encoder



!
!Parser
parse: c with: encoderClass	| encoder meth |
    " note -- must call text:instanceVars: first "
    errBlock <- [ ^ nil ].
    self nextLex.
    encoder <- encoderClass new.
    encoder name: self readMethodName.
    self readMethodVariables.
    self readBody compile: encoder block: false.
    meth <- encoder method: maxTemps class: c text: text.
    meth args: argNames inst: instNames temp: tempNames.
    ^ meth



!
!Parser
readArray	| value |
    self nextChar. self nextLex. value <- Array new: 0.
    [ tokenType ~= $) ]
        whileTrue: [ value <- value with: self arrayLiteral ].
    self nextLex.
    ^ value



!
!Parser
readBlock    | stmts saveTemps lnum |
    saveTemps <- tempNames.
    lnum <- lineNum.
    self nextLex.
    tokenType = $:
        ifTrue: [ self readBlockTemporaries ].
    stmts <- self readStatementList.
    tempNames <- saveTemps.
    tokenType = $]
        ifTrue: [ self nextLex.
            ^ (BlockNode at: lnum) statements: stmts
                temporaryLocation: saveTemps size ]
        ifFalse: [ self error: 'unterminated block']



!
!Parser
readBlockTemporaries
    [ tokenType = $: ]
        whileTrue: [ self currentChar isAlphabetic
            ifFalse: [ self error: 'ill formed block argument'].
            self nextLex.
            self tokenIsName
                ifTrue: [ self addTempName: token asSymbol ]
                ifFalse: [ self error: 'invalid block argument list '].
            self nextLex ].
    tokenType = $|
        ifTrue: [ self nextLex ]
        ifFalse: [ self error: 'invalid block argument list ']



!
!Parser
readBody | lnum |
    lnum <- lineNum.
    ^ (BodyNode at: lnum) statements: self readStatementList



!
!Parser
readCascade: base   | node list |
    node <- self keywordContinuation: base.
    tokenType = $;
        ifTrue: [ node <- (CascadeNode at: lineNum) head: node.
            list <- List new.
            [ tokenType = $; ]
                whileTrue: [ self nextLex.
                    list add:
                        (self keywordContinuation: nil ) ].
            node list: list ].
    ^ node



!
!Parser
readExpression   | node lnum |
    self tokenIsName ifFalse: [ ^ self readCascade: self readTerm ].
    node <- self nameNode: token asSymbol. self nextLex.
    self tokenIsArrow
        ifTrue: [ node assignable
                ifFalse: [ self error: 'illegal assignment'].
            lnum <- lineNum.
            self nextLex.
            ^ (AssignNode at: lnum) target:
                node expression: self readExpression ].
    ^ self readCascade: node



!
!Parser
readInteger  | value |
    value <- token asNumber.
    value isNil ifTrue: [ self error: 'integer expected' ].
    self nextLex.
    ^ value



!
!Parser
readLiteral   | node |
    tokenType = $$
        ifTrue: [ node <- self currentChar.
            self nextChar. self nextLex. ^ node ].
    tokenType isDigit
        ifTrue: [ ^ self readInteger ].
    token = '-'
        ifTrue: [ self nextLex. ^ self readInteger negated ].
    tokenType = $'
        ifTrue: [ ^ self readString ].
    tokenType = $#
        ifTrue: [ ^ self readSymbol ].
    self error: 'invalid literal: ' + token



!
!Parser
readMethodName   | name |
    self tokenIsName	" unary method "
        ifTrue: [ name <- token. self nextLex. ^ name ].
    self tokenIsBinary	" binary method "
        ifTrue: [ name <- token. self nextLex.
            self tokenIsName
                ifFalse: [ self error: 'missing argument'].
                self addArgName: token asSymbol.
                self nextLex. ^ name ].
    self tokenIsKeyword
        ifFalse: [ self error: 'invalid method header'].
    name <- ''.
    [ self tokenIsKeyword ]
        whileTrue: [ name <- name + token. self nextLex.
            self tokenIsName
                ifFalse: [ self error: 'missing argument'].
                self addArgName: token asSymbol.
                self nextLex ].
    ^ name



!
!Parser
readMethodVariables
    tokenType = $| ifFalse: [ ^ nil ].
    self nextLex.
    [ self tokenIsName ]
        whileTrue: [ self addTempName: token asSymbol. self nextLex ].
    tokenType = $|
        ifTrue: [ self nextLex ]
        ifFalse: [ self error: 'illegal method variable declaration']



!
!Parser
readPrimitive  | num args lnum |
    lnum <- lineNum.
    self nextLex.
    num <- self readInteger.
    args <- List new.
    [ tokenType ~= $> ]
        whileTrue: [ args add: self readTerm ].
    self nextLex.
    ^ (PrimitiveNode at: lnum) number: num arguments: args



!
!Parser
readStatement | lnum |
    tokenType = $^
        ifTrue: [ lnum <- lineNum. self nextLex.
            ^ (ReturnNode at: lnum)
                expression: self readExpression ].
    ^ self readExpression



!
!Parser
readStatementList   | list |
    list <- List new.
    [ list add: self readStatement.
      tokenType notNil and: [ tokenType = $. ] ]
        whileTrue: [ self nextLex.
            (token isNil or: [ tokenType = $] ] )
                ifTrue: [ ^ list ] ].
    ^ list



!
!Parser
readString  | first last cc |
    first <- index.
    [ cc <- self currentChar.
      cc isNil ifTrue: [ self error: 'unterminated string constant'].
      cc ~= $' ] whileTrue: [ index <- index + 1 ].
    last <- index - 1.
    self nextChar = $'
        ifTrue: [ self nextChar.
            ^ (text from: first to: index - 2) + self readString ].
    self nextLex.
    ^ text from: first to: last



!
!Parser
readSymbol   | cc |
    cc <- self currentChar.
    (cc isEOF or: [ cc isBlank])
        ifTrue: [ self error: 'invalid symbol'].
    cc = $( ifTrue: [ ^ self readArray ].
    (self charIsSyntax: cc)
        ifTrue: [ self error: 'invalid symbol'].
    self nextLex.
    cc <- Symbol new: token. self nextLex.
    ^ cc



!
!Parser
readTerm   | node lnum |
    token isNil
        ifTrue: [ self error: 'unexpected end of input' ].
    tokenType = $(
        ifTrue: [ self nextLex. node <- self readExpression.
            tokenType = $)
                ifFalse: [ self error: 'unbalanced parenthesis' ].
            self nextLex. ^ node ].
    tokenType = $[ ifTrue: [ ^ self readBlock ].
    tokenType = $< ifTrue: [ ^ self readPrimitive ].
    self tokenIsName
        ifTrue: [ node <- self nameNode: token asSymbol.
            self nextLex. ^ node ].
    lnum <- lineNum.
    ^ (LiteralNode at: lnum) value: self readLiteral



!
!Parser
skipBlanks  | cc |
    cc <- self currentChar.
    [ cc isBlank ] whileTrue: [ cc <- self nextChar ].
    ( cc = $" ) ifTrue: [ self skipComment ]



!
!Parser
skipComment  | cc |
    [ cc <- self nextChar.
      cc isEOF ifTrue: [ ^ self error: 'unterminated comment'].
      cc ~= $" ] whileTrue: [ nil ].
    self nextChar. self skipBlanks



!
!Parser
text: aString instanceVars: anArray
    text <- aString.
    index <- 1.
    lineNum <- 1.
    argNames <- Array new: 1.
    argNames at: 1 put: #self.
    instNames <- anArray.
    tempNames <- Array new: 0.
    maxTemps <- 0



!
!Parser
tokenIsArrow
    (token isKindOf: String) ifFalse: [ ^ false ].
    ^ token = '<-' or: [token = ':=']



!
!Parser
tokenIsBinary
    (((token isNil
        or: [ self tokenIsName])
        or: [ self tokenIsKeyword])
        or: [ self charIsSyntax: tokenType ]) ifTrue: [ ^ false ].
    ^ true



!
!Parser
tokenIsKeyword
    tokenType isAlphabetic ifFalse: [ ^ false ].
    ^ (token at: token size) = $:



!
!Parser
tokenIsName
    tokenType isAlphabetic ifFalse: [ ^ false ].
    ^ (token at: token size) isAlphanumeric



!
!Parser
unaryContinuation: base | receiver lnum |
    receiver <- base.
    [ self tokenIsName ]
        whileTrue: [ lnum <- lineNum.
            receiver <- (MessageNode at: lnum)
                receiver: receiver name: token asSymbol
                    arguments: (List new).
                self nextLex ].
    ^ receiver



!
" class methods for ParserNode "
=ParserNode
at: l | ret |
    ret <- super new.
    self in: ret at: 1 put: l.
    ^ ret



!
=ParserNode
new
    self error: 'Must use at: for creation'



!
" instance methods for ParserNode "
!ParserNode
assignable
    ^ false



!
!ParserNode
compile: encoder
    encoder lineNum: lineNum



!
!ParserNode
isBlock
    ^ false



!
!ParserNode
isSuper
    ^ false



!
" class methods for ArgumentNode "
" instance methods for ArgumentNode "
!ArgumentNode
compile: encoder block: inBlock
    super compile: encoder.
    position = 0
        ifTrue: [ encoder genHigh: 2 low: 0 ]
        ifFalse: [ encoder genHigh: 2 low: position - 1 ]



!
!ArgumentNode
isSuper
    ^ position = 0



!
!ArgumentNode
position: p
    position <- p



!
" class methods for AssignNode "
" instance methods for AssignNode "
!AssignNode
compile: encoder block: inBlock
    super compile: encoder.
    expression compile: encoder block: inBlock.
    target assign: encoder



!
!AssignNode
target: t expression: e
    target <- t.
    expression <- e



!
" class methods for BlockNode "
" instance methods for BlockNode "
!BlockNode
compile: encoder block: inBlock | patchLocation |
    super compile: encoder.
    encoder genHigh: 12 low: temporaryLocation.
    patchLocation <- encoder genVal: 0.
    self compileInLine: encoder block: true.
    encoder genHigh: 15 low: 2. " return top of stack "
    encoder patch: patchLocation



!
!BlockNode
compileInLine: encoder block: inBlock
    statements reverseDo:
        [ :stmt | stmt compile: encoder block: inBlock.
            encoder genHigh: 15 low: 5 " pop top " ].
    encoder backUp



!
!BlockNode
isBlock
    ^ true



!
!BlockNode
statements: s temporaryLocation: t
    statements <- s.
    temporaryLocation <- t



!
" class methods for BodyNode "
" instance methods for BodyNode "
!BodyNode
compile: encoder block: inBlock
    super compile: encoder.
    statements reverseDo:
        [ :stmt | stmt compile: encoder block: inBlock.
            encoder genHigh: 15 low: 5 " pop "].
    encoder genHigh: 15 low: 1 " return self "



!
!BodyNode
statements: s
    statements <- s



!
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
" class methods for InstNode "
" instance methods for InstNode "
!InstNode
assign: encoder
    encoder genHigh: 6 low: position - 1



!
!InstNode
assignable
    ^ true



!
!InstNode
compile: encoder block: inBlock
    super compile: encoder.
    encoder genHigh: 1 low: position - 1



!
!InstNode
position: p
    position <- p



!
" class methods for LiteralNode "
" instance methods for LiteralNode "
!LiteralNode
compile: encoder block: inBlock
    super compile: encoder.
    value == nil ifTrue: [ ^ encoder genHigh: 5 low: 10 ].
    value == true ifTrue: [ ^ encoder genHigh: 5 low: 11 ].
    value == false ifTrue: [ ^ encoder genHigh: 5 low: 12 ].
    (((value class == SmallInt) and:
     [value < 10]) and: [value negative not])
        ifTrue: [ ^ encoder genHigh: 5 low: value ].
    encoder genHigh: 4 low: (encoder genLiteral: value)



!
!LiteralNode
value: v
    value <- v



!
" class methods for MessageNode "
" instance methods for MessageNode "
!MessageNode
argumentsAreBlock
    arguments do: [ :arg | arg isBlock ifFalse: [ ^ false ]].
    ^ true



!
!MessageNode
cascade: encoder block: inBlock
    self evaluateArguments: encoder block: inBlock.
    self sendMessage: encoder block: inBlock



!
!MessageNode
compile2: encoder block: inBlock
    self argumentsAreBlock ifTrue: [
        name = #ifTrue: ifTrue: [ ^ self compile: encoder
                test: 8 constant: 10 block: inBlock ].
        name = #ifFalse: ifTrue: [ ^ self compile: encoder
                test: 7 constant: 10 block: inBlock ].
        name = #and: ifTrue: [ ^ self compile: encoder
                test: 8 constant: 12 block: inBlock ].
        name = #or: ifTrue: [ ^ self compile: encoder
                test: 7 constant: 11 block: inBlock ].
        name = #ifTrue:ifFalse:
            ifTrue: [ ^ self optimizeIf: encoder block: inBlock ].
        ].
    self evaluateArguments: encoder block: inBlock.
    name = '<' asSymbol ifTrue: [ ^ encoder genHigh: 11 low: 0].
    name = '<=' asSymbol ifTrue: [ ^ encoder genHigh: 11 low: 1].
    name = '+' asSymbol ifTrue: [ ^ encoder genHigh: 11 low: 2].
    self sendMessage: encoder block: inBlock



!
!MessageNode
compile: encoder block: inBlock
    super compile: encoder.
    receiver isNil
        ifTrue: [ ^ self cascade: encoder block: inBlock ].
    ((receiver isBlock and: [ self argumentsAreBlock ])
        and: [name = #whileTrue: or: [ name = #whileFalse ] ] )
        ifTrue: [ ^ self optimizeWhile: encoder block: inBlock ].
    receiver compile: encoder block: inBlock.
    receiver isSuper
        ifTrue: [ ^ self sendToSuper: encoder block: inBlock ].
    name = #isNil ifTrue: [ ^ encoder genHigh: 10 low: 0 ].
    name = #notNil ifTrue: [ ^ encoder genHigh: 10 low: 1 ].
    self compile2: encoder block: inBlock



!
!MessageNode
compile: encoder test: t constant: c block: inBlock | save ssave |
    super compile: encoder.
    encoder genHigh: 15 low: t.  " branch test "
    save <- encoder genVal: 0.
    arguments first compileInLine: encoder block: inBlock.
    encoder genHigh: 15 low: 6.  " branch "
    ssave <- encoder genVal: 0.
    encoder patch: save.
    encoder genHigh: 5 low: c.  " push constant "
    encoder patch: ssave



!
!MessageNode
evaluateArguments: encoder block: inBlock
    encoder pushArgs: 1 + arguments size.
    arguments reverseDo: [ :arg |
        arg compile: encoder block: inBlock ]



!
!MessageNode
optimizeIf: encoder block: inBlock | save ssave |
    encoder genHigh: 15 low: 7.  " branch if true test "
    save <- encoder genVal: 0.
    arguments first compileInLine: encoder block: inBlock.
    arguments removeFirst.
    encoder genHigh: 15 low: 6.  " branch "
    ssave <- encoder genVal: 0.
    encoder patch: save.
    arguments first compileInLine: encoder block: inBlock.
    encoder patch: ssave



!
!MessageNode
optimizeWhile: encoder block: inBlock | start save |
    start <- encoder currentLocation.
    receiver compileInLine: encoder block: inBlock.
    name = #whileTrue:	" branch if false/true "
        ifTrue: [ encoder genHigh: 15 low: 8 ]
        ifFalse: [ encoder genHigh: 15 low: 7 ].
    save <- encoder genVal: 0.
    arguments first compileInLine: encoder block: inBlock.
    encoder genHigh: 15 low: 5. " pop from stack "
    encoder genHigh: 15 low: 6. " branch "
    encoder genVal: start. " branch target "
    encoder patch: save.
    encoder genHigh: 5 low: 10  " push nil "



!
!MessageNode
receiver: r name: n arguments: a
    receiver <- r.
    name <- n.
    arguments <- a



!
!MessageNode
sendMessage: encoder block: inBlock
    encoder popArgs: arguments size.
        " mark arguments, then send message "
    encoder genHigh: 8 low: 1 + arguments size.
    encoder genHigh: 9 low: (encoder genLiteral: name)



!
!MessageNode
sendToSuper: encoder block: inBlock
    self evaluateArguments: encoder block: inBlock.
    encoder genHigh: 8 low: 1 + arguments size.
    encoder genHigh: 15 low: 11.
    encoder genCode: (encoder genLiteral: name)



!
" class methods for PrimitiveNode "
" instance methods for PrimitiveNode "
!PrimitiveNode
compile: encoder block: inBlock | argsize |
    argsize <- arguments size.
    super compile: encoder.
    encoder pushArgs: argsize.
    arguments reverseDo: [ :a | a compile: encoder block: inBlock ].
    encoder genHigh: 13 low: argsize.
    encoder genCode: number.
    encoder popArgs: argsize



!
!PrimitiveNode
number: n arguments: a
    number <- n.
    arguments <- a.



!
" class methods for ReturnNode "
" instance methods for ReturnNode "
!ReturnNode
compile: encoder block: inBlock
    super compile: encoder.
    expression compile: encoder block: inBlock.
    inBlock
        ifTrue: [ encoder genHigh: 15 low: 3 " block return " ]
        ifFalse: [ encoder genHigh: 15 low: 2 " stack return " ]



!
!ReturnNode
expression: e
    expression <- e



!
" class methods for TemporaryNode "
" instance methods for TemporaryNode "
!TemporaryNode
assign: encoder
    encoder genHigh: 7 low: position - 1



!
!TemporaryNode
assignable
    ^ true



!
!TemporaryNode
compile: encoder block: inBlock
    super compile: encoder.
    encoder genHigh: 3 low: position - 1



!
!TemporaryNode
position: p
    position <- p



!
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
" class methods for StringTemplate "
=StringTemplate
new: tmplString | tmpl |
    tmpl <- super new.

    " set up the parts. "
    self in: tmpl at: ((self superclass size) + 1) put: (List new).

    " set up the values. "
    self in: tmpl at: ((self superclass size) + 2) put: (Dictionary new).

    " split up template string "
    tmpl parse: tmplString.

    ^ tmpl





!
" instance methods for StringTemplate "
!StringTemplate
footer: f
    footer <- f.

    ^ self


!
!StringTemplate
header: h
    header <- h.

    ^ self


!
!StringTemplate
parse: aStr 
    " search for {varName} in the string. "
    (aStr size > 0) ifTrue: [
        ( (aStr at: 1) = ${ ) ifTrue: [
            self parseValue: (aStr from: 2 to: (aStr size)).
        ] ifFalse: [
            self parseText: aStr.
        ].
    ].

    ^ self





!
!StringTemplate
parseText: aStr | endIndex |
    " gather the text part between value parts. "

    endIndex <- 1.
    [(endIndex <= (aStr size)) and: [(aStr at: endIndex) ~= ${ ] ] whileTrue: [ endIndex <- endIndex + 1].

   " save the part without any values. "
    parts addLast: (aStr from: 1 to: (endIndex - 1)).

    " now parse the value part. "
    (endIndex < (aStr size)) ifTrue: [ self parseValue: (aStr from: (endIndex + 1) to: (aStr size)) ].

    ^ self




!
!StringTemplate
parseValue: aStr | endIndex val |
    " gather the value part between { and } markers. "

    " The leading { character has been skipped. "
    endIndex <- 1.
    [(endIndex <= (aStr size)) and: [(aStr at: endIndex) ~= $} ] ] whileTrue: [ endIndex <- endIndex + 1].

    " did we match anything? "
    (endIndex <= (aStr size)) ifTrue: [
        " save the string as a value Symbol. "
        val <- (Symbol new: (aStr from: 1 to: (endIndex - 1))).
        values at: val put: nil.
        parts addLast: val.

        " now parse the remaining part. "
        (endIndex < (aStr size)) ifTrue: [ self parseText: (aStr from: (endIndex + 1) to: (aStr size)) ].
   ].




!
!StringTemplate
parts
    ^ parts



!
!StringTemplate
render | buf val |
    ^ 'not supported.'


!
!StringTemplate
render: anObj | buf val |
    " render the template using this object as input to the blocks. "

    " skip if the input is nil. "
    anObj isNil ifTrue: [ ^ '' ].

    buf <- StringBuffer new.

    " go through all the parts and render. "
    parts do: [ :p |
        (p isKindOf: Symbol) ifTrue: [
            (val <- (values at: p ifAbsent: [nil])) notNil ifTrue: [
                " if the value is a Block, evaluated it with the anObj argument as input. "
                (val isKindOf: Block) ifTrue: [ buf write: (val value: anObj) ]
                                     ifFalse: [ buf write: (val asString) ]
            ]
        ] ifFalse: [
            buf write: p.
        ].
    ].

    ^ (buf asString)





!
!StringTemplate
renderObjs: aCollection | buf val |
    " render the template using the passed collection, one object per instance. "

    " skip if nil "
    aCollection isNil ifTrue: [ ^ '' ].

    buf <- StringBuffer new.

    " write out header. "
    header notNil ifTrue: [ 
        (header isKindOf: Block) ifTrue: [ buf write: (header value: aCollection) ]
                                ifFalse: [ buf write: (header asString) ]
    ].

    " go through the list of inputs and render. "
    aCollection do: [ :elem | buf write: (self render: elem) ]. 

    " write out footer. "
    footer notNil ifTrue: [ 
        (footer isKindOf: Block) ifTrue: [ buf write: (footer value: aCollection) ]
                             ifFalse: [ buf write: (footer asString) ]
    ].

    ^ (buf asString)





!
!StringTemplate
values
    ^ values



!
" class methods for Template "
" instance methods for Template "
" class methods for Transcript "
=Transcript
do: aBlock
    " iterate over the history of the global transcript object "

    (self new) do: aBlock.

    ^ self


!
=Transcript
history
    " get history from the global transcript object "

    ^ ((self new) history)


!
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
=Transcript
put: anObj
    " put an object into the global transcript object "

    (self new) put: anObj


!
=Transcript
reset
    " get history from the global transcript object "

    (self new) reset


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
" class methods for Undefined "
=Undefined
new
    " there is only one nil object "
    ^ nil



!
" instance methods for Undefined "
!Undefined
isNil
    " yes, we are nil "
    ^ true



!
!Undefined
notNil
    " no, we are not not-nil "
    ^ false



!
!Undefined
printString
    ^ 'nil'



!
" class methods for WWWElement "
=WWWElement
new: content  | elem |
    elem <- super new.

    " store content. "
    self in: elem at: 1 put: content.

    " no style. "
    self in: elem at: 2 put: nil.

    ^ elem


!
=WWWElement
new: content style: style  | elem |
    elem <- super new.

    " store content. "
    self in: elem at: 1 put: content.

    " store style. "
    self in: elem at: 2 put: style.

    ^ elem


!
" instance methods for WWWElement "
!WWWElement
content
    ^ content


!
!WWWElement
evaluateContent: buf

    " content must be a Block, WWWElement or an Array "
    ((content class) = Array) 
        ifTrue: [
            content do: [ :elem | (elem isNil) ifFalse: [elem value: buf ] ]
        ] ifFalse: [ 
            (content isNil) ifFalse: [content value: buf ]
        ].

    ^ buf.



!
!WWWElement
style
    ^ style.


!
!WWWElement
value | buf |
    buf <- StringBuffer new.

    ^ self value: buf



!
!WWWElement
value: buf

    " content must be a Block, WWWElement or an Array "
    self evaluateContent: buf.

    ^ buf.



!
" class methods for WWWBody "
" instance methods for WWWBody "
!WWWBody
value: buf
    buf addLast: '<BODY>'.

    self evaluateContent: buf.

    buf addLast: '</BODY>'.

    ^ buf



!
" class methods for WWWPage "
" instance methods for WWWPage "
!WWWPage
value: buf
    buf addLast: '<HTML>'.

    self evaluateContent: buf.

    buf addLast: '</HTML>'.

    ^ buf




!
