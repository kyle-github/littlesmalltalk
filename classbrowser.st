"*********************************************************"
"*  Class class update.  Create a visible MetaXXX class. *"
"*********************************************************"
!Class
subclass: nm variables: v classVariables: cv | meta metaName |
	" create the meta class and the class.  Add both to the globals. "
	metaName <- ('Meta' + nm asString) asSymbol.
	meta <- Class new name: metaName
		parent: self class
		variables: cv.
	globals at: metaName put: meta.

	" make the actual class "
	globals at: nm put: ( meta new name: nm
		parent: self
		variables: v ).
	^ 'subclass created: ' + nm printString
!
"******************************************************
   String class additions.  These help parsing and
   handling URLs and HTML.
******************************************************"
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
position: aString
	" find arg as substring and return position "

	<150 self aString>.

	self primitiveFailed
!
!String
toUrl
	" convert to URL encoding "
	<151 self>.

	self primitiveFailed
!
!String
fromUrl
	" convert from URL encoding "
	<152 self>.

	self primitiveFailed
!
!String
encodeHTML | transStr lt gt |
	" encode < and > so that HTML can show correctly in HTML "

	transStr <- ''.
	lt <- Char new: 60.  " < character"
	gt <- Char new: 62.  " > character"



	" transform the string into something that can be put into HTML. "
	self do: [ :c | ( c = lt )
				ifTrue: [ c <- '&lt;' ]
				ifFalse: [( c = gt )
						ifTrue: [ c <- '&gt;' ]
				].

			transStr <- transStr + (c asString).
	].

	^ transStr
!
"**********************************************
  StringBuffer class.  Copied idea from Java.
**********************************************"
+List subclass: #StringBuffer variables: #()
!StringBuffer
add: anObj
	^ super add: (anObj printString).
!
!StringBuffer
addLast: anObj
	^ super addLast: (anObj printString).
!
!StringBuffer
size | tempSize |
	tempSize <- 0.
	self do: [:entry | tempSize <- tempSize + (entry size) ].

	^ tempSize
!
!StringBuffer
printString | size index result |
	size <- self size.

	result <- String new: size.

	index <- 1.

	self do: [ :entry | entry do: [ :char | result at: index put: char. index <- index + 1 ] ].

	^ result.
!
"******************"
"  Socket classes  "
"******************"
+Object subclass: #Socket variables: #(fd)
+Socket subclass: #TCPSocket variables: #()
=Socket
open: sockType
	" return a file descriptor (small int) for the new socket or fail "
	" sockType: 1 = TCP, 2 = UDP "
	<200 0 sockType>

	self primitiveFailed
!
=Socket
newType: sockType
	^ self in: (super new) at: 1 put: (self open: sockType).
!
=Socket
newFD: anFD
	^ self in: (super new) at: 1 put: anFD.
!
=Socket
acceptOn: fd
	<200 1 fd>

	self primitiveFailed
!
!Socket
close
	<200 2 fd>.

	self primitiveFailed
!
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
hasError
	<200 6 fd self>.

	self primitiveFailed
!
!Socket
getFD
	^ fd
!
"**************************************"
" TCP Class.  This adds a few methods. "
"**************************************"
=TCPSocket
new
	^ self newType: 1
!
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
"****************************************"
"           HTTPRequest class            "
"****************************************"
+Object subclass: #HTTPRequest variables: #(sock reqPath reqAction reqArgs reqRawData reqPathAndArgs reqError reqLength)
!HTTPRequest
read: aSock
	sock <- aSock.

	 ^ (self rawData size > 0).
!
!HTTPRequest
response: aResp  | responseSize tmpResponse lineTerm index |
	tmpResponse <- StringBuffer new.

	" create the line termination string, note carriage return and linefeed "
	lineTerm <- ((Char new: 13) asString) + (Char newline asString).

	" get the response size "
	responseSize <- aResp size.

	" make HTTP headers"
	tmpResponse addLast: ('HTTP/1.0 200 LittleSmalltalk' + lineTerm).
	tmpResponse addLast: ('Content-Type: text/html' + lineTerm).
	tmpResponse addLast: ('Content-Length: ' + (responseSize printString) + lineTerm).
	tmpResponse addLast: lineTerm.

	" add the response text "
	tmpResponse addLast: (aResp printString).

	sock write: (tmpResponse printString).

	" close the connection now. "
	sock close.
	^ self.
!
!HTTPRequest
rawData		| i termString doubleTerm tempData contentLength |
	" read the request raw data.  This does some parsing. "

	reqRawData isNil ifFalse: [ ^ reqRawData ].

	" the line terminator for HTTP headers is CRLF "
	termString <- ((Char new: 13) asString) + (Char newline asString).

	" the terminator between the HTTP headers and body is CRLF CRLF "
	doubleTerm <- termString + termString.

	" is the socket nil? or not open? "

	(sock isNil) ifTrue: [ ^ nil ].

	" get the data from the socket until we see the header/body delimiter "

	tempData <- sock read asString.

	[ (tempData position: doubleTerm) isNil]
		whileTrue: [ tempData <- tempData + (sock read asString) ].

  	" OK, we have all the headers, what kind of request is it? "

	reqRawData <- tempData.
	reqLength <- tempData size.

	" if this is a POST, we need to get the length and read the data "
	((self action) = 'POST') ifTrue: [

'Processing POST action.' printNl.

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

			" the total length is the length of the header plus separator plus body, -1 for zero start. "
			contentLength <- contentLength + (tempData position: doubleTerm) + (doubleTerm size) - 1.

			" read until we have all the data "
			[ (tempData size) < contentLength ] whileTrue: [tempData <- tempData + (sock read asString)].
		]
		ifFalse: [ reqLength <- 0 ].

 	" we have all the raw data. We've set reqAction, reqLength already, so set reqRawData "
	reqRawData <- tempData.

	^ reqRawData.
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
action
	" if it was set once, return it. "
	reqAction isNil ifFalse: [ ^ reqAction.].

'reqAction before parse: ' print.
reqAction printString printNl.

'Position of GET: ' print.
((self rawData) position: 'GET') printString printNl.

'Position of POST: ' print.
((self rawData) position: 'POST') printString printNl.

	((self rawData) position: 'GET') = 1 ifTrue: [ reqAction <- 'GET'. ].
	((self rawData) position: 'POST') = 1 ifTrue: [ reqAction <- 'POST'. ].

	reqAction isNil ifTrue: [ reqAction <- 'UNKNOWN' ].

'reqAction: ' print.
reqAction printString printNl.

	^ reqAction.
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
args	| i pathArgField argsData keyValList key val argList|
	" get args for both URL and POST data "

	reqArgs isNil ifFalse: [ ^ reqArgs ].

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
			]
   		].

     	" copy data from the form data if this is a POST "
	(self action) = 'POST' ifTrue: [
		i <- ((self rawData size) + 1) - reqLength.

		argsData <- argsData + ((self rawData) from: i to: (self rawData size))
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
		" handle case where key indicates a flag "
		(argList size) = 0 ifTrue: [ val <- true asString ]
				   ifFalse: [ val <- argList first asString ].
		val isNil ifTrue: [ val <- 'no value' ].
		reqArgs at: (key fromUrl asSymbol) put: (val fromUrl).

       		].

	^ reqArgs.
!
!HTTPRequest
at: aSymbol
	^ (self args) at: aSymbol ifAbsent: [ nil ].
!
"*******************************"
"     HTTPDispatcher class      "
"*******************************"
+Object subclass: #HTTPDispatcher variables: #(map env runFlag sock request errorHandler)
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
		tmpRequest read: clientSock.

		aBlock <- map at: (tmpRequest path) ifAbsent: [ nil ].

		( aBlock isNil )
			ifTrue: [ errorHandler value: tmpRequest value: env]
			ifFalse: [ aBlock value: tmpRequest value: env ].

		clientSock close.
	].
!
!HTTPDispatcher
stop
	runFlag <- false.
!
"*********************************"
"     HTTPClassBrowser class     "
"*********************************"
+Object subclass: #HTTPClassBrowser variables: #()
!HTTPClassBrowser
listClassesOn: aReq | outBuf |
	outBuf <- StringBuffer new.
	outBuf addLast: '<HTML><BODY bgcolor="#FFFFFF">'.

	globals do: [ :obj |
		(obj isKindOf: Class)
			ifTrue: [
				outBuf addLast: '<A HREF="/method_list_frame?class='.
				outBuf addLast: (obj printString toUrl ).
				outBuf addLast: '" target="method_list_frame">'.
				outBuf addLast: (obj printString).
				outBuf addLast: '</A><BR>' ] ].

    	outBuf addLast: '</BODY></HTML>'.

	^ aReq response: outBuf
!
!HTTPClassBrowser
listMethodsOn: aReq | outBuf classStr class |
	outBuf <- StringBuffer new.

	" header for page "
	outBuf addLast:  '<HTML><BODY bgcolor="#FFFFFF">'.

	classStr <- aReq at: #class.

	" if there isn't a class string chosen "
	classStr isNil ifTrue: [ outBuf addLast: '<B>No class chosen.</B></BODY></HTML>'.
				 ^ aReq response: outBuf ].

	class <- globals at: (classStr asSymbol) ifAbsent: [ nil ].

	class isNil ifTrue: [ outBuf addLast: '<B>No such class!</B></BODY></HTML>'.
				 ^ aReq response: outBuf ].

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
editMethodOn: aReq | outBuf classStr class methStr method |
	outBuf <- StringBuffer new.

	outBuf addLast: '<HTML><BODY bgcolor="#FFFFFF">'.

	classStr <- aReq at: #class.

	" if there isn't a class string chosen "
	classStr isNil ifTrue: [ outBuf addLast: '<B>No class chosen.</B></BODY></HTML>'.
				 ^ aReq response: outBuf ].

	class <- globals at: (classStr asSymbol) ifAbsent: [ nil ].

	class isNil ifTrue: [ outBuf addLast: '<B>No such class!</B></BODY></HTML>'.
				 ^ aReq response: outBuf ].

	" if there isn't a method string chosen "
	methStr <- aReq at: #method.

"debugging (aReq args) binaryDo: [ :key :val | outBuf addLast: ((key printString) + ' = ' + (val printString) + '<BR>') ]."

  	methStr = 'no value' ifTrue: [ outBuf addLast: '<B>No Value!</B></BODY></HTML>'. ^ aReq response: outBuf ].

	methStr isNil ifTrue: [ outBuf addLast: '<B>No method chosen.</B></BODY></HTML>'.
				 ^ aReq response: outBuf ].

	method <- (class methods) at: (methStr asSymbol) ifAbsent: [ nil ].

	method isNil ifTrue: [ outBuf addLast: '<B>No such method!</B></BODY></HTML>'.
				 ^ aReq response: outBuf ].

     	outBuf addLast: '<FORM ACTION="/compile_method?class='.
	outBuf addLast: classStr.
	outBuf addLast: '&method='.
	outBuf addLast: method name printString toUrl.
	outBuf addLast: '" ENCTYPE="application/x-www-form-urlencoded" METHOD="POST">'.
	outBuf addLast: '<TEXTAREA COLS=60 ROWS=40 NAME="methsrc" WRAP="OFF">'.
	outBuf addLast: (method text encodeHTML).
	outBuf addLast: '</TEXTAREA>'.
	outBuf addLast: '<BR><INPUT TYPE=SUBMIT NAME=compile VALUE="Compile"'.
	outBuf addLast:  '</FORM></BODY></HTML>'.

	^ aReq response: outBuf.
!
!HTTPClassBrowser
compileMethodOn: aReq | outBuf classStr class methSrc action meth |
	outBuf <- StringBuffer new.
	outBuf addLast: '<HTML><BODY bgcolor="#FFFFFF">'.

	" check to make sure this is a POST "
	action <- aReq action.

	action = 'POST' ifFalse: [
		outBuf addLast: '<B>POST form submission required.</B></BODY></HTML> '.
		^ aReq response: outBuf.
	].

	" if there isn't a class string chosen "
	classStr <- aReq at: #class.

	classStr isNil ifTrue: [ outBuf addLast: '<B>No class chosen.</B></BODY></HTML>'.
				 ^ aReq response: outBuf ].

	class <- globals at: (classStr asSymbol) ifAbsent: [ nil ].

	class isNil ifTrue: [ outBuf addLast: '<B>No such class!</B></BODY></HTML>'.
				 ^ aReq response: outBuf ].

	" get the method source. "
	methSrc <- aReq at: #methsrc.

	methSrc isNil ifTrue: [ outBuf addLast: '<B>No method source!</B></BODY></HTML>'.
				 ^ aReq response: outBuf ].

	" filter out carriage returns, the parser sees those as weird literals! "
	methSrc <- (methSrc printString) reject: [ :c | (c value) = 13 ].

	" compile the method source "
	meth <- class parseMethod: methSrc.

	meth isNil ifTrue: [ outBuf addLast: '<B>Parse error!.</B></BODY></HTML>'.
				^ aReq response: outBuf ].

	" store the new method. "
	class methods at: meth name put: meth.

	" flush the lookup cache so that the new method gets called. "
	Method flushCache.

	outBuf addLast: meth name printString.
	outBuf addLast: ' added.'.
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
showControlListFrameOn: aReq | outBuf |
	outBuf <- StringBuffer new.

	outBuf addLast: '<HTML><FRAMESET ROWS="80%,20%" FRAMEBORDER="YES">'.
	outBuf addLast: '<FRAME SRC="/list_frame" NAME="list_frame">'.
	outBuf addLast: '<FRAME SRC="/control_frame" NAME="control_frame">'.
	outBuf addLast: '</FRAMESET></HTML>'.

	^ aReq response: outBuf.
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
showControlFrameOn: aReq | outBuf |
	outBuf <- StringBuffer new.

	outBuf addLast: '<HTML><BODY><FORM METHOD="GET" ACTION="/stop" TARGET="_top">'.
	outBuf addLast: '<INPUT TYPE=SUBMIT NAME=stop VALUE="Stop Browser">'.
	outBuf addLast: '</FORM></BODY></HTML>'.

	^ aReq response: outBuf.
!
!HTTPClassBrowser
showErrorOn: aReq | outBuf |
	outBuf <- StringBuffer new.

	outBuf addLast: '<HTML><BODY bgcolor="#FFFFFF"><B>Path not recognized!</B><BR>'.
	outBuf addLast: '<PRE>'.
	outBuf addLast: ('path: ' + (aReq path) + (Char newline asString)).
	aReq args isNil ifFalse: [ (aReq args) binaryDo:
					[ :key :val | outBuf addLast: ((key printString) + '=' + (val printString) + (Char newline asString)) ] ].

	outBuf addLast: '</PRE></BODY></HTML>'.

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
	dispatcher register: [:aReq :anEnv | self listMethodsOn: aReq. nil]
			at: '/method_list_frame'.
	dispatcher register: [:aReq :anEnv | self editMethodOn: aReq. nil]
			at: '/edit_frame'.
	dispatcher register: [:aReq :anEnv | self compileMethodOn: aReq. nil]
			at: '/compile_method'.
	dispatcher register: [:aReq :anEnv | aReq response: '<HTML><BODY><B>Class browser stopped.</B></BODY></HTML>'. dispatcher stop.  aSock close. nil]
			at: '/stop'.

	dispatcher registerErrorHandler: [ :aReq :anEnv | self showErrorOn: aReq. nil].

	dispatcher startOn: aSock.

	^ nil.
!
"End of the file"
