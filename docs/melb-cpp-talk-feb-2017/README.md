# What's This?

This directory contains slides and code for my talk "Compile-Time Method-Dispatch
Using Or-Combinable Value Types" given at Melbourne C++ Meetup, February 14, 2017.
I've made a few tweaks since the talk to make things clearer, and updated
commentary at the top of each file to reflect what was discussed during the talk.

**START HERE:** [`00-intro.md`](00-intro.md)

This is the complete content of the talk. You can follow along by reading through each file in
numbered order. Each source file has a comment at the top describing what it
does and/or how it steps forward from the previous file. Some files also have
comments in-line describing details. In many cases the talk proceeds by
iteratively refactoring from one file to the next. You may find it useful to
diff adjacent-numbered files to see what's been added/changed.

Note: the files are ordered by the leading numbers, but there are gaps in the
numbering scheme. Roughly, the leading number represents sections of the talk. 40 is the
final section. 80 are extras, and 90 is reference material you probably don't need
to worry about.

The final version of the code that was presented during the talk is
[`35-typechecking.cpp`](35-typechecking.cpp). This is similar to the code
that is now available as `StaticMode.h` and `examples/two-modes-1.cpp`.

### Compiler Compatibility

With the exception of [`24a-cpp17-auto-template-parameter.cpp`](24a-cpp17-auto-template-parameter.cpp),
which requires
a C++17 compiler, you should be able to compile and run each `.cpp` file stand-alone
with any C++11 compiler (I used clang++ 3.8.0-2ubuntu4 during the talk).
If you open this repo (from the root directory) with VS Code, and have clang installed,
you can hit Ctrl-Shift-B to compile and run the currently open source file
(see [`02-hello-world.cpp`](02-hello-world.cpp) for more details).

#### MSVC 2015 SP3

All C++ code from the talk compiles with MSVC 2015 SP3, with one exception:
the final `#ifdef`ed-out variant in `27-two-modes-alt-impls.cpp` will not
compile as-is. This code uses `decltype` in a way that is broken in
MSVC 2015. A workaround is documented in `docs/MSVC-2015.md`.

Please let me know if anything is not working, or if you find errors/typos.
Pull requests and issues will be happily accepted.

Thanks,

Ross.
Melbourne, February 20, 2017.

---
All files in this repository are Copyright &copy; 2017 Ross Bencina,
except where otherwise indicated.
For the license, please see the LICENSE file in the repository root (MIT license).
