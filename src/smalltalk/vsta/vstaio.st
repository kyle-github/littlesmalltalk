+File subclass: #SysFile variables: #(format nested)
=SysFile
openRead: nm format: fmt | ret |
	ret <- self openRead: nm.
	ret format: fmt.
	^ret
!
=SysFile
openWrite: nm
	self error: 'Writing not supported'
!
!SysFile
format: fmt
	format <- fmt
!
!SysFile
format
	^format
!
!SysFile
convert: field as: fmt
	(fmt = $s) ifTrue: [ ^field ].
	(fmt = $d) ifTrue: [ ^field asNumber ].
	self error: ('Unknown format: ' + fmt asString)
!
!SysFile
directive: l | name |
	" Nesting to an included file "
	((l from: 1 to: 8) = '#include') ifTrue: [
		" Get name of file to include "
		name <- (l break: ' 	') at: 2.

		" Nest down to its fileID "
		nested isNil ifTrue: [
			nested <- List new
		].
		nested add: fileID.
		fileID <- self class doOpen: name mode: 'r'.
		fileID isNil ifTrue: [
			self error: ('Cannot open file ' + name)
		].
		^nil
	].
	self error: ('Unknown directive: ' + l)
!
!SysFile
unnest
	" Close the current file and make the next most recent active "
	self close: fileID.
	fileID <- nested first.
	nested removeFirst
!
!SysFile
readLine | l c |
	" Get the next line "
	l <- super readLine.
	l isNil ifTrue: [
		" If it's EOF, see about un-nesting "
		((nested notNil) and: [ nested isEmpty not ]) ifTrue: [
			self unnest.
			^self readLine
		].
		^nil
	].

	" #include and any other directives "
	((l at: 1) = $#) ifTrue: [
		" Process the directive "
		self directive: l.

		" and then try over to get a line of input "
		^self readLine
	].

	" By default, just break into strings at the colon separators "
	l <- l break: ':'.
	format isNil ifTrue: [ ^l ].

	" If we have a format, convert fields "
	1 to: format size do: [:i|
		c <- format at: i.
		l at: i put: (self convert: (l at: i) as: c)
	].
	^l
!
