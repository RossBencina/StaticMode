# StaticMode

[![Build Status](https://travis-ci.org/RossBencina/StaticMode.svg?branch=master)](https://travis-ci.org/RossBencina/StaticMode)

StaticMode is a small header-only C++11 library that can help you to
implement overload-based function selection (sometimes called tag dispatch).
Unlike typical uses of tag dispatch, StaticMode is intended for use in
public APIs.

The key type in StaticMode is the`Mode` template, which you use to define mode
constants (analogous to `enum` members or `constexpr` `std::integral_constant`
instances). The defining features of StaticMode are:

1. Modes from different categories can be combined using the `|` operator,
allowing multiple modes to be passed using a familiar flag-like syntax.
2. StaticMode uses the type system to enforce constraints
on mode expressions, yielding compile-time errors for incorrect mode usage,
such as combining mutually exclusive modes. StaticMode also supports
default modes, and checking that functions are called with only supported modes.
3. StaticMode executes at compile-time. `Mode` and related types have
no runtime representation (they are empty structs). All features are
implemented at the type level, or with `constexpr`.

StaticMode is defined in a single header file:
[`include/StaticMode.h`](include/StaticMode.h). To use StaticMode, simply
include this header file in your project.

StaticMode has no dependencies and requires only
a working C++11 compiler. StaticMode is compatible with MSVC 2015 SP3
(but see [`docs/MSVC-2015.md`](docs/MSVC-2015.md) for information about
client code compatibility).

> **Status:** So far the code has one known user: me.
I'm happy to evolve StaticMode to be more useful for other people.
If you can see ways to improve the API, or have additional requirements,
please open a ticket and we can discuss. -- Ross

## Minimal Example

A minimal example of how StaticMode can be used. Compilable source code for this example is here:
[`examples/minimal.cpp`](examples/minimal.cpp). )

```c++
#include "StaticMode.h"
```


Use an `enum class` to represent each mode category. Each member
of the `enum` represents a mode in that category.
Then use `constexpr Mode<>` to declare instances for each mode -- these are the
constant "mode flags" that you'll use in your code.

```c++
enum class LineStyle { dotted, dashed };

constexpr staticmode::Mode<LineStyle, LineStyle::dotted> dotted;
constexpr staticmode::Mode<LineStyle, LineStyle::dashed> dashed;

enum class EndStyle { arrow, circle };

constexpr staticmode::Mode<EndStyle, EndStyle::arrow> arrow;
constexpr staticmode::Mode<EndStyle, EndStyle::circle> circle;
```

Note that StaticMode treats modes within a mode category as mutually exclusive.
(e.g. a line can't be both dotted and dashed.) Keep this in mind when
defining your modes.

To use the modes you defined, create function overloads for each mode's type.
Here we use `decltype` to save repeating the full `Mode<...>` type of
each mode.

```c++
inline void drawLine_(decltype(dotted)) { std::cout << "....."; }
inline void drawLine_(decltype(dashed)) { std::cout << "-----"; }
inline void drawRightEnd_(decltype(arrow)) { std::cout << ">"; }
inline void drawRightEnd_(decltype(circle)) { std::cout << "o"; }
```

With this arrangement you can invoke the function overload
that applies to a particular mode by passing `Mode` instances as arguments.

```c++
drawLine_(dotted);
drawLine_(dashed);
drawRightEnd_(arrow);
drawRightEnd_(circle);
```

StaticMode implements `operator|` on `Mode` instances.
This lets you combine modes into *mode expressions* such as:

```c++
dotted|arrow;
dashed|circle;
dashed|dotted; // correctly fails to compile. can't be dashed *and* dotted
```

The last line will not compile because StaticMode enforces
mutual exclusivity within a mode category: A mode expression can contain
at most one mode from each category, but it can contain modes from any
number of separate mode categories. (Although this example shows only two
mode categories, any number of categories are supported.)

To write a function that accepts a mode expression as a parameter,
write a template a follows. Use the `get_mode_t` template to extract
individual modes from the mode expression:

```c++
template<typename ModeExpr=staticmode::ModeSet<> > // default to empty ModeSet
void drawLine(ModeExpr /*modes*/={}) {
    using lineStyle_t = staticmode::get_mode_t<LineStyle, ModeExpr, /*default:*/decltype(dashed)>;
    using endStyle_t = staticmode::get_mode_t<EndStyle, ModeExpr, /*default:*/decltype(arrow)>;

    drawLine_(lineStyle_t{});
    drawRightEnd_(endStyle_t{});
    std::cout << "\n";
}
```

This implementation of `drawLine()` can be called with mode expressions that
include both a line style and an end style. Either or both modes can be
omitted, in which case the default modes passed to `get_mode_t` will be used.

```
int main() {
    // example usage:
    drawLine(dotted|circle);
    drawLine(dotted); // defaults to arrow end style
    drawLine(circle); // defaults to dashed line style
    drawLine();       // defaults to dashed|arrow
}
```

StaticMode also includes type checking templates that you can use to
check whether a mode expression satisfies certain constraints such as
only containing specific mode categories, or requiring that certain
mode categories are present. See below for more details.


## More Examples and Documentation

The [`examples/`](examples) directory contains examples of how StaticMode can
be used. Each example has a descriptive comment at the top of the file.

For background and a detailed walk-through of the implementation techniques
used in StaticMode, the slides and code from Ross' talk at Melbourne C++
Meetup are available here:
[`docs/melb-cpp-talk-feb-2017/README.md`](docs/melb-cpp-talk-feb-2017/README.md).


## Additional Features

In addition to the `Mode` type, mode expressions, and the `get_type_t`
template, StaticMode provides the following features:

### `ModeType<T>` base class for each mode category

Each `Mode<T,X>` derives from a `ModeType<T>` base class. Continuing the
above example, this means that you could write a `drawLine_()` method that
matches any line style:

```c++
void drawLine_(staticmode::ModeType<LineStyle>) { std::cout << "/////"; }
```

This is useful when you want to implement a general case, and only write
specific overloads for a few modes.

### `Mode<T,X>` implements the `std::integral_constant` interface

`Mode<T,X>` implements all of the same members as `std::integral_constant`.
`Mode<T,X>` instances have a conversion operator that automatically
converts them to the underlying `enum class` value if needed.
`Mode` also implements `operator()` that returns the `enum class` value,
so that `Mode` instances can be used as function objects.

### `is_mode_expression` template

The `is_mode_expression` template can be used to check whether a function
argument is a mode expression. This can be used in combination with
`static_assert` to emit a compilation error (see examples).

### `has_no_other_modes` template

The `has_no_other_modes` template can be used to check that a function
argument doesn't reference any unsupported mode categories.
This can be used it in combination with `static_assert` to emit a
compilation error (see examples).

### `has_mode` template

The `has_mode` template can be used to test whether a mode expression
contains a representative of a particular mode category. This can be
used to implement required mode categories
(see [`examples/required-mode.cpp`](examples/required-mode.cpp)).


## Building and Running Tests and Examples (Requires Cmake)

This section covers building and running the tests and examples.
To use StaticMode you don't need to build anything, just include `StaticMode.h`.

Unit test source code is located at:
[`test/StaticMode_test.cpp`](test/StaticMode_test.cpp).

### On Unix

To build and run the tests and examples on a Unix system, enter the
following commands at a shell prompt from the project root:

```
$ mkdir build
$ cd build
$ cmake ..
$ make
$ make test
```

If tests fail, the following command will re-run the tests with detailed output:

```
$ make CTEST_OUTPUT_ON_FAILURE=1 test
```

After running make you will find the example executables in `build/examples`.

### On Windows

From a Visual Studio command prompt in the project root directory, the following
commands can be used to create a Visual Studio Solution in the `build\`
directory.

```
> mkdir build
> cd build
> cmake ..
```

---
All files in this repository are Copyright &copy; 2017 Ross Bencina,
except where otherwise indicated.
For the license, please see the LICENSE file in the repository root (MIT license).
