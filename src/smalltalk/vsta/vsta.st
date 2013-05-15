+Object subclass: #VSTa variables: (Array new: 0) classVariables: #(hz)
=VSTa
showCPU: vals | tmhz tm |
	" Get as numbers "
	tmhz <- vals asNumber.
	tm <- tmhz quo: hz.

	" Below one second "
	(tmhz < hz) ifTrue: [
		^ '.' + (((tmhz*100) quo: hz) printWidth: 2)
	].

	" Less than an hour, show in minutes and seconds "
	(tm < (60*60)) ifTrue: [
		^ ((tm quo: 60) printWidth: 2) + ':' +
		 ((tm rem: 60) printWidth: 2)
	].

	" Less than a day, show in hours and minutes "
	(tm < (24*60*60)) ifTrue: [
		^  ((tm quo: (60*60)) printWidth: 2) + 'h' +
		 ((tm rem: (60*60)) printWidth: 2) + 'm'
	].

	" Otherwise days and hours "
	^ ((tm quo: (24*60*60)) printString) + 'd' +
		((tm rem: (24*60*60)) printString) + 'h'

!
=VSTa
showps: l | fields times |
	" Burst status columns into an Array "
	fields <- l break: ' '.

	" Command name "
	((fields at: 2) printWidth: 8) print.

	" State "
	((fields at: 3) printWidth: 8) print.

	" Number of threads "
	((fields at: 4) printWidth: 8) print.

	" User and system CPU times.  Convert based on range. "
	times <- (fields at: 5) break: '/'.
	(((self showCPU: (times at: 1)) printWidth: -5) + '/' +
		((self showCPU: (times at: 2)) printWidth: 8)) print.

	" ID "
	(fields at: 6) print
!
=VSTa
ps | f l pids |
	" Banner "
	'PID     CMD     STATE   NTHREAD   USR/SYS     ID' printNl.

	" Get list of process ID's in system "
	f <- File openRead: '//fs/proc'.
	pids <- OrderedArray new.
	[ l <- f readLine. l notNil ] whileTrue: [
		l first isDigit ifTrue: [
			pids <- pids add: l asNumber
		]
	].
	f close.

	" Display status for each PID "
	pids do: [:pid|
		" PID "
		pid print.
		Char tab print.

		" Access the /proc node for this PID "
		f <- File openRead: '//fs/proc:' + pid printString +
			'/status'.

		" If it's still there, display its status "
		f notNil ifTrue: [
			l <- f readLine.
			f close.
			l notNil ifTrue: [ self showps: l ].
			Char newline print
		]
	]
!
=VSTa
initialize
	hz <- 20
!
+VSTa initialize
