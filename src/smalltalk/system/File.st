" class definition for File "
+Object subclass: #File variables: #( fileID ) classVariables: #( )
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
writeImage
    " save the current image in a file "
    <104 fileID>
    fileID isNil ifTrue: [ self notOpened ].
    self primitiveFailed

!
