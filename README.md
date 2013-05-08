<HTML>
<TITLE>New Little Smalltalk README</TITLE>
<BODY bgcolor="#FFFFFF">
<H1>1. New Features and Changes</H1>
This file explains the differences between Dr. Tim Budd's original
Little Smalltalk and this version.
<H2>1.1 Copyright Changes</H2>
Dr. Budd has indicated to me that he is abandoning the restrictions of the
original copyright.  The message follows:
<p>
<pre>
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
</pre>
<p>
I have put my changes under the GNU General Public License or GPL.   See
the included file COPYING for details.  I felt that this was the closest license
to the spirit of Dr. Budd's original license.
<p>
I have attempted to preserve the original goal of Little Smalltalk as a very
simple system
that provides a significant part of the functionality of a Smalltalk-80
compliant system
without the overhead and complexity.  My original goal in using LST was to add a
specific X Windows widget set to it to allow GUI programming.  Instead, I
conceived the
idea of using a web browser as the front-end and making LST into a tiny web
server that
would serve pages that implemented a simple code browser.
<p>
I modified the LST VM to support rudimentary sockets and added a very basic
class browser (but not editor).  That was in early 2001.  Then LST languished
again for over a year.  In mid 2002, I found references to a Tiny
Smalltalk by Andy Valencia (of VSTa fame).
<p>
I found Tiny Smalltalk and discovered that it was actually a modified
version of Tim Budd's original LST 4.0.  I do not know who did the
modifications for certain, but I think it was mostly Andy Valencia.
<p>
Andy's version (or those he borrowed from) had a several nice features that
I had seriously thought about, but had not persued.  Andy had converted
LST to work under VSTa (or so it seemed from the code).  I removed most
of the OS-specific changes in order to make it work under Linux.
<p>
In addition, Andy was working on both a disassembler for bytecode and a
command line debugger.  I have not used either and do not know if they
work.
<p>
Some very ridimentary benchmarks showed that Andy's version was nearly
an order of magnitude faster than the one I'd been hacking on (I did not
do a comparison against the original LST 4.0).  I suspect that I slowed
my version down somewhat compared to the original, but Andy's non-boxed
integers certainly helped things.  I'm not sure about the change in
Dictionary implementation.  The method cache seems to work fairly well
with a high hit rate (I don't remember the exact figure, but better than
95% I think...).  So, method lookup is generally not that much of the
overhead.
<p>
I took my changes to the original LST and merged them with those that
Andy's version had.  The result I called LST 4.5.  It isn't done or
even half-baked, but it is an amusing little toy now.
<p>
Dr. Budd's original system came in with an image of just under 4000 objects.  I
have not been able to keep under that limit, but the current image is still less than
4100 objects!
<H2>1.2 New Features</H2>
<H3>1.2.1 Increase Range for SmallInts</H3>
The range of SmallInts from 0-1000 to 0 to 2^31-1.
<H3>1.2.2 New Image Format</H3>
The image format has been changed to support larger SmallInts and to attempt to
be platform independent.  See below for more information.
<H3>1.2.3 Class Browser</H3>
LST now supports both TCP sockets and simple class/method browser using a web
browser.  You
can look at class methods and edit them (not well tested).
<H1>2. Code Changes</H1>
This section describes bug fixes and changes that have been made to the
original
source and image source.
<H2>2.1 Code Reorganization</H2>
I reorganized the code so that main.c was much smaller, added several other
files etc.
Most of this was to split the functionality of the system into smaller units so
that
the image builder code would compile without being linked to the whole thing.
I had originally changed some internal structure names and other things to
get the code to compile with a C++ compiler (G++), but Andy's source did
not have that and it was easier to leave that alone and merge the rest.
<p>
Due to the reorganization, I also changed the makefiles for the
vm (the "st" program) and the image builder.
<H2>2.2 Primitives added</H2>
I added a primitive to implement the new #position: method for String.
<p>
I added several primitives for the socket operations.  These are only
sketched in at this point.  For instance, I still do not have the
setsockopt() call correctly setting SO_REUSEADDR on the socket.
<H2>2.4 Bug Fixes</H2>
Most of these came from Andy's code.
<H1>3. Major changes and additions</H1>
This section describes the major changes and additions done so far.
<p>
<H2>3.1 Andy's changes</H2>
Andy Valencia did several changes that I found (and perhaps more that
I missed).  Again, I am assuming that Andy did these changes as I did not
find any evidence to the contrary.
<UL>
<LI>He converted integers into
non-boxed objects and expanded the size to 2^31 (from 0-1000 in LST 4.0).
<LI>He
fixed several small bugs in the class library and changed the Smalltalk-based
parser to correctly handle some cases that the old one didn't handle.
<LI>He
changed the implementation of the Dictionary class to use an array instead of a
tree.   The entries in the array are sorted so that a binary search can be
used.  I am not sure why he did this.  It does appear to result in fewer
objects in the image.
<LI>He implemented a much better solution to the method cache problem than I
had.  Originally, when you changed a method, if the old one had already been
called and was in the cache, you got the old one. When a method is compiled
for a class, a special primitive is called and that method is flushed from
the cache.
<LI>He converted many primitives to use the more familiar ST-80 style of
code where the primitive returns from the method if it does not fail.  This
is not complete.  I have adopted this for some of the primitives I added.
</UL>
<H2>3.2 Internet TCP socket support</H2>
<H3>3.2.1 Primitives</H3>
There are several new primitives that implement various operations on
a BSD-style TCP socket.
<H3>3.2.2 Classes</H3>
The TCPSocket class has been added.  It doesn't do much more than wrap the
primitives.  It needs some clean up, but seems to work.
<H2>3.3 New General Classes</H2>
<H3>3.3.1 StringBuffer</H3>
This class provides a buffer in which to build strings.  When I was
experimenting with
the HTTPBrowserWindow code (see below), it became clear that constructing HTTP
responses
by concatenating strings was far too slow to be usable.  So, I created this
class as a subclass of List.
<p>
<TABLE WIDTH="95%" BORDER=1>
<TR><TD ALIGN=LEFT><B>Method</B></TD><TD><B>Functionality</B></TD></TR>
<TR><TD ALIGN=LEFT>add:</TD><TD>Forces the passed object into a string via
printString.</TD></TR>
<TR><TD ALIGN=LEFT>addLast:</TD><TD>Forces the passed object into a string via
printString.</TD></TR>
<TR><TD ALIGN=LEFT>printString</TD><TD>This constructs and returns a single
string representing all the strings in the buffer.</TD></TR>
<TR><TD ALIGN=LEFT>size</TD><TD>This returns the combined size of all the
strings in the buffer.</TD></TR>
</TABLE>
<p>
This class is used heavily in the HTTP class browser.
<H2>3.4 Class browser GUI</H2>
<B>The following is from the previous implementation of the class browser and
should only used as an idea of where I was going.  The actual code is in
classbrowser.st.  Look there before assuming anything!  You can do some
editing of methods with the current system.  The information below is
incorrect.
</B>
<p>
I added a new way to view classes.  This is via a web browser.  I did not want
to add
all the GUI code to LST (yet) via some widget set, so I added a set of classes
to handle
HTTP requests, dispatching and displaying lists and code to the browser.
<H2>3.5 New Classes</H2>
I added the following classes:
<TABLE WIDTH="95%" BORDER=1>
<TR><TD ALIGN=LEFT><B>Class</B></TD><TD><B>Functionality</B></TD></TR>
<TR><TD ALIGN=LEFT>HTTPRequest</TD><TD>This class provides access to a standard
HTTP request
somewhat like that provided by the mod_perl Apache request object, only much
more primitive.
You can get the path, action (GET or POST), or URL arguments from a request.
You return
your response to the request object.</TD></TR>
<TR><TD ALIGN=LEFT>HTTPDispatcher</TD><TD>This class provides a mechanism to
associate
URL paths with specific blocks of code.  When a request comes in, a dispatcher
will look up
the path of the URL request (via HTTPRequest) and dispatch it to a block.
Blocks can be
registered by outside entities on any URL path.</TD></TR>
<TR><TD ALIGN=LEFT>HTTPBrowserWindow</TD><TD>This class implements a very simple

class browser.
You can only look at the methods of classes and at the method text.  Currently
there is no
provision to allow you to add methods or classe, or to modify methods.  Given
what is there
now, this should not be a hard thing to add.</TD></TR>
</TABLE>
<H2>3.6 Image format changes</H2>
See the document <B>imageformat.txt</B> for an explanation of the new image
format.  This
format is slightly more concise and allows for SmallInts that are much, much
larger
than before.  It is designed to work across big-endian and little-endian
platforms as
well as 32 or 64-bit word sizes.  Since I only have Intel-based machines, I
cannot test
this yet.
<H1>4. Running LST 4.5</H1>
To run the LST command line, do the following steps:
<UL>
<LI>Untar the source into a directory.  It will untar into a subdirectory
called lst4.5.  Descend into that directory.
<LI>Descend into the ImageBuilder directory.  Run 'make image'.  This will
build the imageBuilder program and make the initial image from the
imagesource.st file.  The image will be in the file 'image' in the ImageBuilder
directory.
<LI>return up one level and run 'make st'.  This will make the VM.
<LI>in the directory in which you did the previous, run './st'.  That will
start LST.  You'll see a prompt like:
<PRE>
kyle@nahuatl:~/lang/lst/lst4.5> ./st
3227 objects in image
->
</PRE>
<LI>have fun!  That's the running LST interpreter (read-eval loop).
</UL>
To run the class browser, a few more steps are needed.  I'll assume that
you are at the LST prompt.
<UL>
<LI>Load the class browser source:
<PRE>
kyle@nahuatl:~/lang/lst/lst4.5> ./st
3227 objects in image
-> File fileIn: 'classbrowser.st'.
</PRE>
This will spew a lot of stuff to the screen as it reads in the source in
classbrowser.st.
<LI>Start the class browser:
<PRE>
-> HTTPClassBrowser new start.
Socket: 3
IP: 127.0.0.1
Port: 6789
</PRE>
LST will print out the socket file descriptor number, the IP it bound to
and the port it is using.
<LI>Fire up your favorite browser (I haven't tried a text-only browser, but you need frames) and
point it to 'http://localhost:6789'.
<LI>Be patient.  It takes a couple of seconds to get all the frames loaded on my P-II 400.
<LI>LST will ignore the command line prompt unless you click the button to stop the
class browser in the lower left corner of the browser window.
</UL>
<H1>5. Known Bugs</H1>
None!  Well, given the minimal amount of use of LST 4.5, I believe that there
are many bugs.  They just have not been found yet.
<p>
Actually, the largest two bugs are that I haven't written any C socket code for about a
decade and what I have written is fairly Linux specific.  I do not have access to any
other OSes to test.  It would be wonderful if someone could make this code POSIX 
compliant and also add in #define's and the rest to make it compile under that
OS from Redmond.
</BODY>
</HTML>
