# Wrap Up

We've created a kind of compile-time configuration language allowing
construction and inspection of sets of disjoint modes using a convenient syntax.
We looked at:

* Representing mode selectors using empty structs (later templates).
* Parameter-dependent dispatch using function overloading and value types
  (type-level values) representing specific modes.
* Building a mode set using variadic templates and `constexpr` overloading of `operator|`.
* Extracting specific mode types from the mode set (`get_mode`)
* Making the whole construction type safe using `static_assert` to enforce
  invariants on `ModeSet` and parameters passed to `drawLine()`.

## Final Thoughts

Syntactically it might be considered unintuitive to use flag syntax
for compile-time function selection, but in my view it's better than the runtime
alternatives when the functionality does lend itself to compile-time
optimization. For a client-facing API it's more usable than using
a separate function parameter for each mode.

The compile-time type checking is something that you can't do
with ordinary or-able value flags. This is a benefit.

C++11 makes it possible to write this kind of code in a compact and relatively
understandable way, without having to depend on a metaprogramming library such as
`boost::mpl` or `boos::hana`. The "library" that I demonstrated in this talk is less than 250
lines of code including the type checking utilities (excluding comments).
In C++14 or C++17 it would probably be under 200 lines, since some of the
type checking code is there to emulate C++17 library functions.

## Thanks

Thanks to the kind people of Stack Overflow and the `##C++` and `##C++-general` channels on freenode IRC for
answering my questions about the material in this talk.

In particular, here are links to my relevant stack overflow questions:

* [Using tag-dispatch to implement or-combinable flags](http://stackoverflow.com/questions/40977757/using-tag-dispatch-to-implement-or-combinable-flags)
-- This was before I worked out that I was going to need to implement type checking.

* [Is static_assert supposed to work when invoked via decltype expression?](http://stackoverflow.com/questions/41010924/is-static-assert-supposed-to-work-when-invoked-via-decltype-expression)
-- This is the reason that I check for invariants such as duplicate modes inside the `ModeSet` type rather than inside `operator|`.
A `static_assert` inside `operator|` will not fire inside an unevaluated context such as `decltype(solid|dotted`).

* [Inheritance or composition when recursing over a parameter pack?](http://stackoverflow.com/questions/41301168/inheritance-or-composition-when-recursing-over-a-parameter-pack)
-- pertains to how we implement searching in `get_mode` and in the type checking code.

* [Infer enum class type from enum class value template parameter?](http://stackoverflow.com/questions/40960936/infer-enum-class-type-from-enum-class-value-template-parameter)
-- The C++17 auto template parameter business.

* [MSVC2015 decltype parameter type in overloaded template function](http://stackoverflow.com/questions/41001482/msvc2015-decltype-parameter-type-in-overloaded-template-function)
-- An MSVC2015 bug when using `decltype` to deduce a parameter type for a template function like we do in some cases. Discusses the workaround.

* [Will we ever get partial specialization of member functions?](http://stackoverflow.com/questions/41096587/will-we-ever-get-partial-specialization-of-member-functions)
-- The reason that I am confident to use function-overload based dispatch (tag dispatch) instead of partial specialization.

Thanks also to the audience at Melbourne C++ Meetup for great questions and discussion.
I've integrated some of the points that were raised into this version of the slides and code.

## Questions?

## Extras (time permitting)

* Type erasure example: [`82-extra-type-erasure.cpp`](82-extra-type-erasure.cpp) demonstrates using mode expressions
  to build a class that implements a base-class interface.
* Idea about using the same scheme for type-checking flags that use runtime values.
* Could look at how I'm using this in my actual code.

---
rossb@audiomulch.com

@RossBencina

[github.com/RossBencina](https://github.com/RossBencina)

[rossbencina.com](http://rossbencina.com)

[audiomulch.com](http://audiomulch.com)
