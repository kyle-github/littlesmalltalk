# Little Smalltalk

- [Little Smalltalk](#little-smalltalk)
  - [Platforms](#platforms)
  - [Releases](#releases)
  - [Copyright/License](#copyrightlicense)
  - [History](#history)
    - [Archive](#archive)
  - [Performance](#performance)
  - [Size](#size)
  - [New Features and Changes from Version 4.0](#new-features-and-changes-from-version-40)
  - [Class Browser/Editor](#class-browsereditor)
  - [Building Little Smalltalk](#building-little-smalltalk)
  - [Running Little Smalltalk](#running-little-smalltalk)
    - [Command Line REPL](#command-line-repl)
    - [Web-based IDE](#web-based-ide)
  - [End Notes](#end-notes)

Little Smalltalk is a version, or dialect, of Smalltalk that does not conform
to the Smalltalk-80 standard.  Little Smalltalk has been around for about 35 years.

In 1984, Dr. Timothy Budd of Oregon State University
released the first version of Little Smalltalk.  It was a very simple, command-line
system.  It ran well in small amounts of memory.  Though it is not fast, it is
very simple and it is complete.

Over the next ten years Dr. Budd updated and upgraded Little Smalltalk.  The final
standalone version was version 4.0 released in about 1994.  He went on to create
Small World, a version based on the Java VM.

The version here is based on changes I made in 2000-2003 and combines some changes
made by Andy Valencia.  I added some primitives to do basic
TCP socket operations and wrote a very minimal class browser/editor web IDE.

## Platforms

This version of Little Smalltalk is only tested with Linux!   It will need some C source code additions to allow compilation on Windows using Microsoft's compiler.  It might work with WLS.   It might work on macOS.   Eventually the goal is to make it work better cross platform.

All development is done on 64-bit x86 Ubuntu.   Some testing has been done on 32-bit x86 Debian.  Very basic testing was done using a big-endian (MIPS) version of Debian.

## Releases

v4.7.0 - released 20200612 - By Kyle Hayes. Many changes:

- Changed image format to work on 32 and 64-bit systems.
- Simplified the garbage collector to a pure Baker-style two-space GC.  It takes a little longer for GC passes, but all garbage is collected.
- Attempts at platform-neutral image format.   Minimal testing on Debian on MIPS (big endian).
- Fixed latent bug with constant too large for SmallInt in original source.
- Redid the bootstrap program to input the same source format as output by source file out for classes.
- Redid the web IDE build process to build the image automatically.
- Changed how the initial method is found to have multiple fall-backs for older images.
- Redid how the image is saved (saves a single object now).
- Many, many changes to the web IDE interface.
- Made the web interface work with FireFox and Chrome (it did not work on modern versions of FireFox).

v4.6.1 - released 20181207 - by Kyle Hayes.  Made into CMake project.

v4.6.0 - released 20130515 - by Kyle Hayes, ported to 64-bit, clean ups etc.

v4.5.0 - released 2002? - by Kyle Hayes

v???? - released by Andy Valencia (found in 2002 but done earlier than that)

v4.0.0 - released (not fully released) 1994 - by Dr. Timothy Budd, OSU.

v3.0 - released ? - by Dr. Timothy Budd, OSU. (The book, "A Little Smalltalk" is based on this version.)

v2.0 - released ? - by Dr. Timothy Budd, OSU.

v1.0 - released ? - by Dr. Timoth Budd, OSU.

## Copyright/License

Dr. Budd has indicated to me that he is abandoning the restrictions of the
original copyright.  The message follows:

```text
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
```

My version from 2001 was under the GPL.  I felt that this was the closest
license to the spirit of Dr. Budd's original.  However, it became apparent
that the license was a problem when I look back over the last decade.  There
have been other forks, but none based on my original version 4.5.

I have relicensed the code under the 3-clause BSD license (see the file
LICENSE).

## History

I have attempted to preserve the original goal of Little Smalltalk as a very simple system that provides a part of the functionality of a Smalltalk-80 system without the overhead and complexity.  My original goal in using LST was to add a specific X Windows widget set to it to allow GUI programming.  Instead, I conceived the idea of using a web browser as the front-end and making LST into a tiny web server that would serve pages that implemented a simple code browser and editor.

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
Andy's version had.  The result I called LST 4.5.  That is the base for this release.

It isn't done or even half-baked, but it is an amusing little toy now.

### Archive

I have included an archive of all the versions I could find.   These were collected by
Danny or Charles over time.   Please see the `archive` directory and the `README` therein for
more information and individual versions.

Russell Allen has been updating SmallWorld, Budd's Java port of Little Smalltalk.   Please visit his
GitHub repository [SmallWorld](https://github.com/russellallen/SmallWorld) for the most recent version.
As of the last update to this README, that was 2015.

*Note:* if you have an original copy of Parla and can share it, please let me know!   The version
I had in the archive was a copy of the Wayback Machine web page and not the original source.  I have
not been able to find a copy on the Internet.

## Performance

Little Smalltalk is not fast.  Not fast at all compared to modern JIT-powered
VMs for Java and Smalltalk.  However, it is very small and very accessible.  It is
a good tool to learn how to do a complete language implementation including
garbage collection, compiling, interpretation, bootstrapping etc.

It is a basic bytecoded VM with bytecodes much like those used in other languages.
The VM implements a simple stack-based execution environment for compiled Smalltalk
code.

## Size

The base image has about 3400 objects.  The web IDE has about 5200 objects.  The interpreter executable is
smaller than L2 cache on most recent CPUs and smaller than L1 on some.

## New Features and Changes from Version 4.0

There have been several new features added since Dr. Budd's original 4.0 release.

- SmallInt range increased from 0-999 to +/- 2^31 (approximately). (Andy Valencia)
- Integer operations are done via 64-bit boxed integers in most cases. (Andy Valencia)
- Dictionary now uses a sorted array instead of a tree internally. (Andy Valencia)
- Method cache changes to enable cache flushing when methods are updated. (Andy Valencia)
  - change the method cache hash calculation to have fewer collisions.   (Kyle Hayes)
- The image format was changed to be more compact and to be platform-neutral (Kyle Hayes)
- A web-based class browser/editor has been added.   This still needs a lot of work as multiple attempts at reasonable, simple HTML handling all are present. (Kyle Hayes)
- The code has been moved around and split and several changes to the directory structure have been done. Ongoing. (Kyle Hayes)
- The bootstrapping code shares as much as possible with the VM code to reduce duplication.   Ongoing. (Kyle Hayes)
- Conversion to CMake for the build system. (Kyle Hayes)
- Several primitives were added:
  - For string methods. (Kyle Hayes)
  - For TCP socket support. (Kyle Hayes)
  - and others I have forgotten.
- Many small bug fixes.   (Andy Valencia, Kyle Hayes)
- Fixed a problem with the bootstrapper and Smalltalk compiler where the search for a matching instance variable was done in the wrong order.  (Kyle Hayes)
- Implemented some debugging output in C code that shows line numbers and functions.  (Kyle Hayes)
- Refactor of some primitives to work more like those in ST-80 with fallthrough.  (Andy Valencia)
- Various new classes:
  - StringBuffer for string building. (Kyle Hayes)
  - Many iterations on HTML building classes. (Kyle Hayes)
  - Debugging classes. (Andy Valencia)
  - Log class with log levels. (Kyle Hayes)
  - Transcript. (Kyle Hayes)
  - Socket classes. (Kyle Hayes)
- The bootstrapping process now uses the same input format as the class `fileIn` and `fileOut` methods. This is a major change from earlier versions. (Kyle Hayes)
  - The web IDE can output a valid single Smalltalk file containing the entire source.
  - The bootstrapper can use that single file to bootstrap an image.

## Class Browser/Editor

I added a basic class browser initially in about 2001.   Later I started adding more and more functionality to the browser including buttons to save the image automatically to a numbered image file.

The class browser is roughly functional.  The following are implemented (though usually in a very basic way):

- A basic UI at [http://localhost:6789](http://localhost:6789)
- Add classes and meta-classes.
- Add methods to classes and meta-classes.
- Edit methods on classes and meta-classes.
- Save images.
- File out the source for a class.
- Basic "do it" page at [http://localhost:6789/do_it](http://localhost:6789/do_it)
  
It is ugly!   No attempt at any CSS or other styling has been done.   Many parts are very rough.  Debugging code is still enabled.

The long term goal is to implement a StringTemplate class that support something like Mustache templates.   This is underway but not at all ready to use to replace the existing HTML builder classes.

## Building Little Smalltalk

The build is based on CMake.

- make a build directory in the root:
  
  ```sh
  $> mkdir build
  ```

- decend into the build directory and run CMake:

  ```sh
  $> cd build; cmake ..
  ```

- watch the pretty output.   If all goes well, you should end up with several files.
  - `lst` - this is the main Little Smalltalk binary.
  - `bootstrap` - this program is used to build the initial images.
  - `lst.img` - this is the command line REPL image.   It contains about 3400 objects.
  - `webide.img` - this is the web-based IDE image.   It contains about 5200 objects.

## Running Little Smalltalk

Copy the executable `lst` and at least one of the images to a directory where you want to work.

### Command Line REPL

```sh
$> ./lst ./lst.img
```

### Web-based IDE

```sh
$> ./lst ./webide.img
```

If you run the web IDE, you will need to start your browser and point it at [http://localhost:6789](http://localhost:6789).   There is a basic `Do It` implementation at [http://localhost:6789/do_it](http://localhost:6789/do_it)

## End Notes

Little Smalltalk is very basic.   It is a toy.  It is not suitable for production use.   All that said, it is a great way to understand how a self-hosted object oriented language is make.

If you run across any bugs, please let me know by opening a GitHub issue.   If you determine how to fix the bug, even better!

Have fun!

Kyle Hayes
kyle.hayes at gmail.com
