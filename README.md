What is Little Smalltalk?
=========================

Little Smalltalk is a version, or dialect, of Smalltalk that does not conform
to the Smalltalk-80 standard.  Little Smalltalk has been around for about 30 years.  
In 1984, Dr. Timothy Budd of Oregon State University
released the first version of Little Smalltalk.  It was a very simple, command-line
system.  It ran well in small amounts of memory.  Though it is not fast, it is
very simple and it is complete.

Over the next ten years Dr. Budd updated and upgraded Little Smalltalk.  The final
standalone version was version 4.0 released in about 1994.  He went on to create
Small World, a version based on the Java VM.

The version here is based on changes I made in 2000-2003 and combines some changes
made by Andy Valencia.  I added some primitive to do basic
TCP socket operations and wrote a very minimal class browser/editor web IDE.

Version History
===============

v4.6.0 - released 20130515 - by Kyle Hayes, ported to 64-bit, clean ups etc.

v4.5.0 - released 2001? - by Kyle Hayes

v4.0.0 - released (not fully released) 1994 - by Dr. Timothy Budd, OSU.

v3.0 - released ? - by Dr. Timothy Budd, OSU. (The book, "A Little Smalltalk" is based on this version.)

v2.0 - released ? - by Dr. Timothy Budd, OSU.

v1.0 - released ? - by Dr. Timoth Budd, OSU.

Copyright Changes
=================

Dr. Budd has indicated to me that he is abandoning the restrictions of the
original copyright.  The message follows:

'''
Return-path: <budd@ghost.CS.ORST.EDU>
Received: from mta2.snfc21.pbi.net (mta2-pr.snfc21.pbi.net)
 by sims1.snfc21.pbi.net
 (Sun Internet Mail Server sims.3.5.1999.07.30.00.05.p8) with ESMTP id
 <0FOD00GRP4FHP6@sims1.snfc21.pbi.net> for kyroha@sims-ms-daemon; Fri,
 14 Jan 2000 21:55:00 -0800 (PST)
Received: from ghost.CS.ORST.EDU ([128.193.38.105])
 by mta2.snfc21.pbi.net (Sun Internet Mail Server sims.3.5.1999.09.16.21.57.p8)
 with ESMTP id <0FOD007LF46V36@mta2.snfc21.pbi.net> for
 kyroha@sims1.snfc21.pbi.net; Fri, 14 Jan 2000 21:49:43 -0800 (PST)
Received: from oops.CS.ORST.EDU (oops.CS.ORST.EDU [128.193.38.26])
 by ghost.CS.ORST.EDU (8.9.3/8.9.3) with ESMTP id VAA15600 for
 <kyroha@pacbell.net>; Fri, 14 Jan 2000 21:49:43 -0800 (PST)
Received: by oops.CS.ORST.EDU (8.8.5/CS-Client) id FAA16044; Sat,
 15 Jan 2000 05:49:42 +0000 (GMT)
Date: Sat, 15 Jan 2000 05:49:42 +0000 (GMT)
From: Tim Budd <budd@CS.ORST.EDU>
Subject: Re: Little smalltalk
To: kyroha@pacbell.net
Message-id: <200001150549.FAA16044@oops.CS.ORST.EDU>
Status: RO
X-Status: A


well, I haven't touched LST since 94, you are welcome to any sources to
do whatever you wish with it.
'''

My version from 2001 was under the GPL.  I felt that this was the closest 
license to the spirit of Dr. Budd's original.  However, it became apparent
that the license was a problem when I look back over the last decade.  There
have been other forks, but none based on my original version 4.5.  

I have relicensed the code under the 3-clause BSD license (see the file
LICENSE).  

History
=======

I have attempted to preserve the original goal of Little Smalltalk as a very
simple system
that provides a significant part of the functionality of a Smalltalk-80
compliant system
without the overhead and complexity.  My original goal in using LST was to add a
specific X Windows widget set to it to allow GUI programming.  Instead, I
conceived the
idea of using a web browser as the front-end and making LST into a tiny web
server that
would serve pages that implemented a simple code browser and editor.

I modified the LST VM to support rudimentary sockets and added a very basic
class browser (but not editor).  That was in early 2001.  Then LST languished
again for over a year.  In mid 2002, I found references to a Tiny
Smalltalk by Andy Valencia (of VSTa fame).

I found Tiny Smalltalk and discovered that it was actually a modified
version of Tim Budd's original LST 4.0.  I do not know who did the
modifications for certain, but I think it was mostly Andy Valencia.

Andy's version (or those he borrowed from) had a several nice features that
I had seriously thought about, but had not persued.  Andy had converted
LST to work under VSTa (or so it seemed from the code).  I removed most
of the OS-specific changes in order to make it work under Linux.

In addition, Andy was working on both a disassembler for bytecode and a
command line debugger.  I have not used either and do not know if they
work.

Some very rudimentary benchmarks showed that Andy's version was nearly
an order of magnitude faster than the one I'd been hacking on (I did not
do a comparison against the original LST 4.0).  I suspect that I slowed
my version down somewhat compared to the original, but Andy's non-boxed
Integers certainly helped things.  I'm not sure about the change in
Dictionary implementation.  The method cache seems to work fairly well
with a high hit rate (I don't remember the exact figure, but better than
95% I think...).  So, method lookup is generally not that much of the
overhead.

I took my changes to the original LST and merged them with those that
Andy's version had.  The result I called LST 4.5.  It isn't done or
even half-baked, but it is an amusing little toy now.

Performance and Size
====================

Little Smalltalk is not fast.  Not fast at all compared to modern JIT-powered
VMs for Java and Smalltalk.  However, it is very small and very accessible.  It is
a good tool to learn how to do a complete language implementation including 
garbage collection, compiling, interpretation etc.  

A stripped image on a 64-bit Linux machine (Ubuntu 12.04) is about 27kB.  The base
image is about 92kB.  The whole thing will easily fit into L2 cache on most modern 
desktop CPUs.

It is a basic bytecoded VM with bytecodes much like those used in other languages.
The VM implements a simple stack-based execution environment for compiled Smalltalk
code.

The base image has about 3200 objects.  

New Features and Changes from Version 4.0
=========================================

There have been several new features added since Dr. Budd's original 4.0 release.

* SmallInt range increased from 0-999 to +/- 2^31 (approximately) (Andy Valencia).

* Integer operations are done via 64-bit boxed integers in most cases (Andy Valencia).

* The image format was changed to more compactly encode SmallInts and to attempt
to be more platform independent.

* A web-based class browser/editor has been added.

* The code has been moved around and split and several changes to the directory
structure have been done.  The eventual goal is to have the bootstrapping code share 
as much as possible with the VM code to reduce duplication.

* Makefiles were added (not yet fully complete).

* Primitives were added to implement #position: for String and several more for 
TCP socket support.

* Bug fixes (mostly from merged code from Andy Valencia)

* Dictionary now uses a sorted array instead of a tree internally (Andy Valencia).  

* Method cache changes to enable cache flushing when methods are updated (Andy Valencia).

* Many primitives were changed to work more like those in ST-80 where the body of the method
calling the primitive is called only if the primitive fails (Andy Valencia).  I adopted this
for some of the primitives I added.

* I added a StringBuffer class.  Using the VM and bytecodes to do all the string 
concatenation and minipulation needed for a web server was too slow on the systems
I had at the time without this.

Class Browser/Editor
====================

The following is from the previous implementation of the class browser and
should only used as an idea of where I was going.  The actual code is in
classbrowser.st.  Look there before assuming anything!  You can do some
editing of methods with the current system.  The information below is
incorrect.

I added a new way to view classes.  This is via a web browser.  I did not want
to add all the GUI code to LST (yet) via some widget set, so I added a set of classes
to handle HTTP requests, dispatching and displaying lists and code to the browser.

I added the following classes:

*  **HTTPRequest** This class provides access to a standard HTTP request somewhat like
that in JSP or mod_perl, only much more primitive.  Methods exist for getting the
path, action (GET or POST) or URL arguments from a request.

* **HTTPDispatcher** This class provides a mechanism to associate URL paths with
specific blocks.  When a request matches the path (exact, not REGEX matching), the 
block is called with the HTTPRequest object for the request.

* **HTTPBrowserWindow** This class builds on the others to implement a very simple
class browser and editor.


Building Little Smalltalk
=========================

Decend into the "src" directory.  Run "make".  That's it.  There will be an executable
for the VM "lst" and an image to use with it, "lst.img".  Run it on the command line:

'''
littlesmalltalk/src$ ./lst ./lst.img 
3227 objects in image
-> 
'''

That's it.  You are now in the command line read/execute loop.

To run the class browser, a few more steps are needed.  I'll assume that
you are at the LST prompt.

* Load the class browser source:
'''
littlesmalltalk/src$ ./lst ./lst.img 
3227 objects in image
-> File fileIn: 'smalltalk/webui/classbrowser.st'.
method inserted: subclass:variables:classVariables:
method inserted: from:to:
method inserted: position:
method inserted: toUrl
method inserted: fromUrl
method inserted: encodeHTML
subclass created: StringBuffer
method inserted: add:
method inserted: addLast:
method inserted: size
method inserted: printString
subclass created: Socket
subclass created: TCPSocket
method inserted: open:
method inserted: newType:
method inserted: newFD:
method inserted: acceptOn:
method inserted: close
method inserted: bindTo:onPort:
method inserted: canRead
method inserted: canWrite
method inserted: hasError
method inserted: getFD
method inserted: new
method inserted: accept
method inserted: read
method inserted: write:
subclass created: HTTPRequest
method inserted: read:
method inserted: response:
method inserted: rawData
method inserted: pathAndArgs
method inserted: action
method inserted: path
method inserted: args
method inserted: at:
subclass created: HTTPDispatcher
method inserted: register:at:
method inserted: registerErrorHandler:
method inserted: startOn:
method inserted: stop
subclass created: HTTPClassBrowser
method inserted: listClassesOn:
method inserted: listMethodsOn:
method inserted: editMethodOn:
method inserted: compileMethodOn:
method inserted: showBaseFrameOn:
method inserted: showControlListFrameOn:
method inserted: showListFrameOn:
method inserted: showControlFrameOn:
method inserted: showErrorOn:
method inserted: start
method inserted: startOn:
file in completed
-> 
'''

* Now start the class browser:
'''
-> HTTPClassBrowser new start.
Socket: 3
IP: 127.0.0.1
Port: 6789
'''

LST will print out the socket file descriptor number, the IP it bound to
and the port it is using.

Fire up your favorite browser (I haven't tried a text-only browser, but you need frames) and
point it to 'http://localhost:6789'.

It takes a couple of seconds to get all the frames loaded on an old P-II 400.  On my Core i5 laptop, it is 
almost immediate.  

LST will ignore the command line prompt unless you click the button to stop the
class browser in the lower left corner of the browser window.

Known Bugs
==========

None!  Well, given the minimal amount of use of LST 4.5, I believe that there
are many bugs.  They just have not been found yet.

Actually, the largest two bugs are that I haven't written any C socket code for about a
decade and what I have written is fairly Linux specific.  I do not have access to any
other OSes to test.  It would be wonderful if someone could make this code POSIX 
compliant and also add in #define's and the rest to make it compile under that
OS from Redmond.

I recently ported LST to a 64-bit platform (Ubuntu 12.04).  It seems to run the basic operations
and I can at least browse around in the web-based class browser.  I have not tried to 
change methods.

If you find a bug, let me know!

Have fun!

Kyle Hayes
kyle.hayes at gmail.com

