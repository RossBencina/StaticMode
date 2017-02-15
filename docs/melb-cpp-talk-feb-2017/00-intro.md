# Compile-Time Method-Dispatch Using Or-Combinable Value Types

**Ross Bencina**

Melbourne C++ Meetup, February 14, 2017.

## Background

This talk is about a technique that I'm working with at the moment.

The technique involves computing and type-checking mode-selector values
at compile time, so that minimal and specific code executes at runtime.
This is quite different from code that interprets its arguments by executing
logic at runtime.

The technique I'm going to discuss is only useful if the values that drive
the logic are always known at compile time. Sometimes this is
exactly what you want -- why pay for runtime overhead when you don't need it?

The inspiration for trying out this technique were two talks by Louis Dionne
(creator of `boost::hana`, a modern metaprogramming library):

* [CppCon 2015: C++ Metaprogramming: A Paradigm Shift](https://www.youtube.com/watch?v=cg1wOINjV9U)
* [Keynote Meeting C++ 2016 - C++ Metaprogramming: evolution and future direction](https://www.youtube.com/watch?v=X_p9X5RzBJE)

Those talks go way beyond what I'm discussing here, but they alerted
me to the new capabilities of C++11 and C++14. I very much recommend watching Louis' talks
for the high-level abstract picture. My talk will be much more nuts-and bolts,
focusing on a single application of value types (type-level values).

To give an idea of where I'm coming from. There are a few books that I've read
in the past that have informed my thinking around writing this kind of code:
Alexandrescu's "Modern C++ Design", Austern's "Generic Programming and the STL",
Czarnecki & Eisenecker's "Generative Programming". I'm currently reading
Stepanov & Rose's "From Mathematics to Generic Programming."

Except where noted, all code is **C++11**. The code has no dependencies.

All off the code from this talk will be at: [github.com/RossBencina](https://github.com/RossBencina)

## Goals

* Nuts-and-bolts walk-through of a technique that I think might be useful
* Show that there are some cool things that you can do with
  templates that weren't so easy before C++11.
* Stimulate discussion!

## Caveats

* I'm still learning C++11. If you see that I've done something that's not
right please let me know.
* I'm using this code in a system that will eventually be put into production,
but this shouldn't be considered "production code". Especially since I've cut
corners for the presentation.

## Questions Welcome!

* Feel free to interrupt and ask questions at any time.
* Especially: stop me if I'm not being clear.

---
rossb@audiomulch.com

@RossBencina

[github.com/RossBencina](https://github.com/RossBencina)

[rossbencina.com](http://rossbencina.com)

[audiomulch.com](http://audiomulch.com)
