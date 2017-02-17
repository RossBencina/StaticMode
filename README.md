# StaticMode

*C++11 or-able type-level values for compile time mode selection.*

StaticMode is a small C++11 library for implementing compile-time
function selection (a la tag dispatch) using type-level values that you
can `or` together using familiar value syntax.

StaticMode is defined in a single header file:
[`include/StaticMode.h`](include/StaticMode.h).

`StaticMode.h` has comprehensive unit tests:
[`test/StaticMode_test.cpp`](test/StaticMode_test.cpp).

I have been using this code for a few months, but it has only recently
been open sourced. So far the code has one known user: me.
I'm happy to evolve StaticMode to be useful to more people.
If you can see ways to improve the API, or have additional requirements,
please open a ticket and we can discuss.

## Quick Example

Here is a quick walk-through of one way that StaticMode can be used.

First, declare constants representing mode categories, like this:

```c++
enum class LineStyle { dotted, dashed };

constexpr staticmode::Mode<LineStyle, LineStyle::dotted> dotted;
constexpr staticmode::Mode<LineStyle, LineStyle::dashed> dashed;

enum class EndStyle { arrow, circle };

constexpr staticmode::Mode<EndStyle, EndStyle::arrow> arrow;
constexpr staticmode::Mode<EndStyle, EndStyle::circle> circle;
```

Note that modes within a mode category are mutually exclusive.
(e.g. a line can't be both dotted and dashed.)

> (`Mode` is an empty struct -- it has no runtime representation. It is only
> used for compile-time type-based dispatch, as we will see next.)

Use the `Mode` types to create overloaded functions, e.g.:

```c++
void drawLine_(decltype(dotted)) { std::cout << "....."; }
void drawLine_(decltype(dashed)) { std::cout << "-----"; }
void drawRightEnd_(decltype(arrow)) { std::cout << ">"; }
void drawRightEnd_(decltype(circle)) { std::cout << "o"; }
```

You can select among different functions at compile time by passing the `Mode` instances as arguments.
Functions are selected using C++ function overloading.

```c++
drawLine_(dotted);
drawLine_(dashed);
drawRightEnd_(arrow);
drawRightEnd_(circle);
```

StaticMode's `Mode` template implements `operator|`, so you can write *mode expressions* such as:

```c++
dotted|arrow;
dashed|circle;
dashed|dotted; // fails to compile. can't be dashed *and* dotted
```

The last line will issue a compiler error, because StaticMode enforces
mutual exclusivity within a mode category. A mode expression can contain
at most one mode from each category. But it can contain modes from any number of
separate mode categories (e.g. given an appropriate set of mode category declarations,
`dashed|arrow|transparent|wavy|green` would be a valid expression.)

To use a mode expression as a parameter, write a template
function that accepts the expression, and uses the `get_mode_t` template to extract the individual modes
from a mode expression. `get_mode_t` accepts the `enum class` type of
the mode category to extract, the type of the mode expression, and the type
of a default mode to use if the mode category is not represented in the mode expression.

```c++
template<typename ModeExpr=staticmode::ModeSet<> > // default to empty ModeSet
void drawLine(ModeExpr /*modes*/={}) {
    using lineStyle_t = staticmode::get_mode_t<LineStyle, ModeExpr, /*default:*/decltype(dashed)>;
    using endStyle_t = staticmode::get_mode_t<EndStyle, ModeExpr, /*default:*/decltype(arrow)>;

    drawLine_(lineStyle_t{});
    drawRightEnd_(endStyle_t{});
}

// example usage:

drawLine(dotted|arrow);
drawLine(dotted); // defaults to arrow end style
drawLine(circle); // defaults to dashed line style
drawLine(); // defaults to dashed|arrow
```

In addition to ensuring that mode expressions contain at most one mode from
each mode category, StaticMode provides `is_mode_expr` and `has_no_other_modes`
templates that can be used to implement additional type checking on mode expressions.
The following implementation of `drawLine()` performs compile time checks
to ensure that the mode expression argument is valid and doesn't contain any
unsupported mode categories.

```c++
template<typename ModeExpr=staticmode::ModeSet<> > // default to empty ModeSet
void drawLine(ModeExpr /*modes*/={}) {
    static_assert(staticmode::is_mode_expr<ModeExpr>::value == true,
            "/modes/ argument has unexpected type. Expected a Mode or ModeSet.");

    using AcceptedModes = staticmode::type_pack<LineStyle, EndStyle>;
    static_assert(staticmode::has_no_other_modes<AcceptedModes, ModeExpr>::value,
            "/modes/ argument contains a mode from an unexpected category (enum class). "
            "Expected at most one line style and one end style.");

    using lineStyle_t = staticmode::get_mode_t<LineStyle, ModeExpr, /*default:*/decltype(dashed)>;
    using endStyle_t = staticmode::get_mode_t<EndStyle, ModeExpr, /*default:*/decltype(arrow)>;

    drawLine_(lineStyle_t{});
    drawRightEnd_(endStyle_t{});
}
```


## More Examples and Documentation

For more ideas about how StaticMode can be used, look at the examples
(each example has a descriptive comment at the top of the file).

- [`examples/two-modes-1.h`](examples/two-modes-1.h)
- [`examples/two-modes-2.h`](examples/two-modes-2.h)
- [`examples/required-mode.h`](examples/required-mode.h)

For background and a detailed walk-through of the implementation techniques,
the slides and code from my talk at Melbourne C++ Meetup are included:
[`docs/melb-cpp-talk-feb-2017/README.md`](docs/melb-cpp-talk-feb-2017/README.md).


## Additional Features

In addition to the features outlined above, StaticMode provides the following features:

### `ModeType<T>` base class for each mode category

Each `Mode<T,X>` derives from a `ModeType<T>` base class. Continuing the above example,
this means that you could write a `drawLine_()` method that matches any line style:

```c++
void drawLine_(staticmode::ModeType<LineStyle>) { std::cout << "/////"; }
```

This is useful when you want to implement a general case, and only write specific overloads for a few modes.

### `Mode<T,X>` implements the `std::integral_constant` interface

`Mode<T,X>` instances have a conversion operator that automatically
converts them to the underlying `enum class` value if needed. They also implement
`operator()` that returns the same value, so that they can be used as
function objects.


### `has_mode` template

In addition to the `get_mode_t`, `is_mode_expr` and `has_no_other_modes` templates
mentioned above, the `has_mode` template can be used to test whether a mode expression
contains a representative of a particular mode category. This can be used to implement
required categories (see `examples/required-mode.cpp`).


---
Everything in this repository is Copyright &copy; 2017 Ross Bencina,
except where otherwise indicated.
For the license, please see the LICENSE file in the repository root (MIT license).
