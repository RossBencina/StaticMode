# Flag Syntax

Now we're going to implement or-able flag syntax.

## Multiple Parameter Syntax vs. Flag Syntax

As an *interface* for selecting modes, there are two options:
(1) multiple, separate parameters, one for each mode type,
(2) or-able flags passed as a single argument. We've already looked
at defining functions that take each mode as a separate
parameter. We'll continue to use that approach for dispatch in our
implementation. But for a client API it's worth considering
or-able flags. I've listed some pros and cons of each approach below.

#### Multiple Parameter Syntax

```c++
painter.drawLine(dotted, arrows);
```

Pros:
* Explicit -- good when all parameters are required (no defaults)
* Relatively type-safe

Cons:
* Clients have to remember argument order
* Tedious to implement defaults if you want to default the first but not the second parameter
(as demonstrated earlier). This also violates argument
ordering expectations (e.g. consider that a one-argument call that
passes an end-style such as `drawLine(arrows)` violates the convention
that the line style comes first.)

#### Flag Syntax

```c++
painter.drawLine(dotted|arrows);
```

Pros:
* Client doesn't have to remember the correct argument order.
* Any mode type can be omitted/defaulted without breaking ordering expectations.
* ~Implementing a default for an omitted flag is easy if the underlying
  representation makes it easy. (e.g. if we're using bitfields for each mode,
  and the zero value represents the default.)

Cons:
* ~May require runtime checks for incompatible options
e.g. `drawLine(dotted|dashed|solid)`. (But we'll see a compile-time solution.)


> Rich Hickey has an entertaining talk about making code simpler,
> that argues for avoiding ordered syntax when order doesn't matter:
> * [Rails Conf 2012 Keynote: Simplicity Matters](https://www.youtube.com/watch?v=rI8tNMsozo0)

## Where We're Heading

Want to support building up expressions out of mode value-types
that we can pass as a single argument:

```c++
painter.drawLine(dotted|arrows);
```
(The mode expression is `dotted|arrows`, `drawLine(expr)` takes a single "mode expression" parameter.)

Also, we want to support combining more than two modes:

```c++
painter.drawLine(dotted|arrows|thick);
```

We need some kind of *expression* type that can contain any number of modes.

The implementation of `drawLine` will take one of these mode expressions,
extract the desired modes, and delegate to the underlying implementation details.

In the end, `drawLine` will look like this.

```c++
template<typename ModeExpr=ModeSet<> > // default to empty ModeSet
void drawLine(ModeExpr={}) {
    auto lineStyle = get_mode_t<LineStyle, ModeExpr, /*default:*/decltype(solid)> {};
    auto endStyle = get_mode_t<EndStyle, ModeExpr, /*default:*/decltype(no_ends)> {};

    drawLeftEnd_(endStyle);
    drawLine_(lineStyle);
    drawRightEnd_(endStyle);
    std::cout << "\n";
}
```

In the next three files we'll:

1. sketch out a form for the operators and expression types
2. implement `operator|` to support constructing mode expressions using the "`dotted|arrows`" syntax
3. implement the `get_mode_t` template, which extracts a specific mode from a mode expression
