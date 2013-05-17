Older Versions of Little Smalltalk
==================================

This directory includes older versions of Little Smalltalk.

These were gathered by Charles Childers and others over the years and
hosted on www.littlesmalltalk.org until that site went away.  These
copies are curtesy of the Internet Archive.

I have duplicated the text that www.littlesmalltalk.org had for each version below.

Note that "I" below is not me, Kyle Hayes, but the owner of littlesmalltalk.org, Danny Reinhold.

Little Smalltalk v1
===================

*file:* lst-1.tar.gz

This is the version described in the book "A Little Smalltalk" by Timothy A. Budd.

In this version most interesting stuff is done directly in the C source code.
When you compare the different versions of the system you'll see that in later versions
more and more functionality has been moved into the Smalltalk image and that
the C source code turned into a smaller and smaller virtual machine.

Due to a complete loss of my archives I was unable to provide this version for quite a long time.
Thanks to Charles Childers (CRC) I am able to bring it back to the public now.

On most modern operating systems you will probably have some problems when you
try to compile it. Some changes must be done.
If you have any patches for Windows, Linux or any other operating system, please let me now!
I would be glad to provide them here.

Little Smalltalk v2
===================

*file:* lst-2.tar.gz

For a long time I was unable to find an archive of version 2.
This has changed now:

Leif Strand found such an archive - and I am really, really happy that he sent it to me for inclusion to the archive!

Here is Leif's mail (from 15th of June 2007):

```
Hello,

I notice on your web site that Little Smalltalk v2 is missing from your 
collection.  By pure chance, I happen to have a file called 
"little-st.tar.gz" which I downloaded in August of 1999.  Within the 
archive, there are four files of particular interest:

-rw-r----- bbraun/10035   8437 1998-05-22 11:20 little-st/READ_ME
-rw-r----- bbraun/10035  60268 1987-10-04 11:56 little-st/part01
-rw-r----- bbraun/10035  59474 1987-10-04 11:56 little-st/part02
-rw-r----- bbraun/10035  30123 1987-10-04 11:56 little-st/part03

According to the "READ_ME" file, it is a beta of v2 (?).  The 
part files appear to be Tim Budd's post of v2 to 
comp.sources.unix.  The other files in the archive have a timestamp of 
1998-05-22.  I'm guessing this is when "bbraun" packaged it.

See attached file "little-st.tar.gz".  I hope you find it useful!

--Leif
```

Of course I found it useful - and I added it immediately to the archive!

Leif, thank you very much!


Little Smalltalk v3
===================

(Note from Kyle Hayes: I think this might actually be the version described in the book, but I am not sure.)

*file:* lst-3.tar.gz

*file:* lst-3-crc.tar.gz

*file:* lst-3-patched.tar.gz

Version 3 was the basis for several other developments (PDST was a successor of version 3).
This may be due to the fact that the system is very easy to understand, simple to extend
and absolutely free. Timothy A. Budd originally changed the license terms with version 4 so that
for example commercial usage was not allowed. Now these restrictions have been removed.
All my current developments are based on version 4 instead of version 3.

But anyway - extremly instructive and simply cool.

Thanks to Charles Childers I can provide these copies.
He also gracefully provided some modifications he worked out 

Here are some comments by Charles Childers (crc):

I have a copy of Little Smalltalk, version 3.04. 

My copies include an original distribution that I picked up some time ago, and a slightly modified version that compiles with warnings, but no errors, on SuSE Linux 10.1 and NetBSD 3.0. It does not compile under FreeBSD 6.1, or OpenBSD 3.9. It will also compile under BeOS R5 with some makefile tweaks (remove -lm from the library list), but not all of the objects can be built into the systemImage yet.

If you are interested in either of these distributions, please let me know. I can (and intend to) do further work on my distribution, to allow it to compile cleanly on the modern BSD, Linux, and BeOS systems.

Well, of course I was interested.


Little Smalltalk v4
===================


This was the last version done by Timothy A. Budd before he developed something similar
to LittleSmalltalk based on Java technologies.
(It is called SmallWorld and can be found here.)

Version 4 contained lots of changes - in fact it was a complete rewrite of the LittleSmalltalk
system. The C source code basically consists of a virtual machine only.
All the real functionality is contained in the Smalltalk image.
This is very similar to what the original inventors of Smalltalk had in mind and realized in their systems.

Unfortunately with version 4 Timothy also introduces license terms that forbid the usage
of the LittleSmalltalk system in commercial projects etc.
But fortunately Timothy allowed me (Danny Reinhold) to completely remove those license
terms and define my own license for the LittleSmalltalk system. So it is now as free as
possible in terms of beer and speach...

Version 4 is quite advanced and harder to understand than earlier versions since most
functionality is not readable in the C source code.
But it is the most powerful version of LittleSmalltalk and once you understood the virtual
machine you can also simply understand the functionality that is 'hidden' in the Smalltalk image.
Hey - it is Smalltalk - so it is no problem to read and understand everything!

*file:* lst-4-original.tar.gz - the original version!

*file:* lst-4.1-windows.zip - a modified version to allow compilation on Windows systems!

*file:* lst-4-bootstrap.tar.gz - another variation of version 4 found by Charles Childers. This version comes with source code of the Smalltalk image and a tool that compiles that source and creates an initial image file. So this version allows bootstrapping of the complete LittleSmalltalk system with only a C compiler at hand and without magic in the image file. This version is the basis for our further work on LittleSmalltalk system 5 since April 2007.

From version 4.1 upwards all versions are developed and maintained by
Danny Reinhold / Reinhold Software Services and other contributors to the
LittleSmalltalk project. Please don't bother Timothy Budd with errors in those versions. 





