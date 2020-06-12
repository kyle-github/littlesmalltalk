" class definition for Parser "
+Object subclass: #Parser variables: #( text index tokenType token argNames tempNames instNames maxTemps errBlock lineNum ) classVariables: #( )
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
    ^ token = '<-'

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
