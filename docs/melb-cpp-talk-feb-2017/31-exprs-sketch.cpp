//!clang++ -std=c++11 -Weverything -Wno-c++98-compat 31-exprs-sketch.cpp -o z31.out && ./z31.out

// As a way to ease in to things, this file develops an skeleton of the type
// signatures for mode expression operator|

// The next file develops the full implementation of each operator. You might
// find it easier to look at the full implementations in the next file.

// We use placeholder_type wherever we need a type that we haven't specified yet.

#include <iostream> // cout

// ............................................................................
// placeholder_type code

struct placeholder_type {}; // placeholder_type for stuff we haven't worked out yet

// ............................................................................
// Library code

template<typename T>
struct ModeType {}; // aka mode category

template<typename T, T X>
struct Mode : ModeType<T> {
    constexpr Mode() {}
};

// Dummy get_mode implementation, developed later. For now we're focusing on mode expressions.
template<typename KeyEnumClass, typename ModeExpr, typename Default>
struct get_mode {
    using type = placeholder_type;
};

template<typename KeyEnumClass, typename ModeExpr, typename Default> // C++11: alias template
using get_mode_t = typename get_mode<KeyEnumClass, ModeExpr, Default>::type;

// ............................................................................

/*
  For the sake of explanation lets introduce a third mode: RenderStyle:
*/
enum class LineStyle { dotted, dashed, solid };

constexpr Mode<LineStyle, LineStyle::dotted> dotted;
constexpr Mode<LineStyle, LineStyle::dashed> dashed;
constexpr Mode<LineStyle, LineStyle::solid> solid;

enum class EndStyle { no_ends, arrows, circles };

constexpr Mode<EndStyle, EndStyle::no_ends> no_ends;
constexpr Mode<EndStyle, EndStyle::arrows> arrows;
constexpr Mode<EndStyle, EndStyle::circles> circles;

enum class RenderStyle { plain, sketchy, painterly };

constexpr Mode<RenderStyle, RenderStyle::plain> plain;
constexpr Mode<RenderStyle, RenderStyle::sketchy> sketchy;
constexpr Mode<RenderStyle, RenderStyle::painterly> painterly;


template<typename T, typename U, typename V>
void drawLine_(T, U, V) {} // dummy implementation. see earlier code for multi-parameter dispatch implementations


/*
  We want to be able to write things like:

    drawLine(dotted|circles|sketchy);

  drawLine() could take various combinations of modes. We could
  declare it as follows:
*/

template<typename ModeExpression>
void drawLine(ModeExpression)
{
  /*
    We're going to use the same multi-parameter dispatch
    that we described earlier. We want something like:
  */

  using lineStyle_t = get_mode_t<LineStyle, ModeExpression, /*default:*/decltype(solid)>;
  using endStyle_t = get_mode_t<EndStyle, ModeExpression, /*default:*/decltype(no_ends)>;
  using renderStyle_t = get_mode_t<RenderStyle, ModeExpression, /*default:*/decltype(plain)>;
  drawLine_(lineStyle_t{}, endStyle_t{}, renderStyle_t{});
}

/*
    Now, how do we combine mode values?

    To combine two mode values we could do something like:
*/

// e.g.: a|b
// Mode<T,X> | Mode<U,Y> -> ?mode-expr

template<typename T, T X, typename U, U Y>
constexpr placeholder_type operator|(Mode<T,X>, Mode<U,Y>)
{
    return {};
}

/*
    For larger expressions we'd also need to combine
    a mode value with a compound expression:
*/

// e.g.: (...)|b
// ?mode-expr | Mode<U,Y> -> ?mode-expr
template<typename U, U Y>
constexpr placeholder_type operator|(placeholder_type, Mode<U,Y>)
{
    return {};
}

// e.g.: a|(...)
// Mode<T,X> | ?mode-expr -> ?mode-expr
template<typename T, T X>
constexpr placeholder_type operator|(Mode<T,X>, placeholder_type)
{
    return {};
}

/*
    And combine two compound expressions:
*/

// e.g.: (...)|(...)
// ?mode-expr | ?mode-expr -> ?mode-expr
constexpr placeholder_type operator|(placeholder_type, placeholder_type)
{
    return {};
}

/*
    It all type-checks correctly... (hit build)
*/

int main()
{
    drawLine(dotted|painterly);
    drawLine(sketchy|dotted);
    drawLine(circles|sketchy);

    drawLine(plain|dashed|arrows);

    std::cout << "done.";
}

/*
    But be aware that our use of placeholder_type above will refine to
    different types in different places.

    So, what is placeholder_type?

    Requirements:
      * it needs to store all Mode types combined in a mode expression,
        hence it's a collection of Mode<T,X> types
      * don't care what order the values are stored (although left-to-right makes sense)
      * don't need a tree structure, for our purposes: (a|b)|c ~ a|(b|c) ~ a|b|c,
        a flat vector/list/tuple would suffice
      * need to be able to iterate over it to implement get_mode

    C++11 variadic templates fit the bill and are relatively simple to work with:
*/

template<typename... Ts> // C++11: variadic template
struct ModeSet {}; // Ts... can be a list of 0 or more Mode<T,X> types (as defined here it could be any types)

/*
    For example, we expect (plain|dashed|arrows) to have the following type:
*/

using T = ModeSet<decltype(plain), decltype(dashed), decltype(arrows)>;

/*
    (We could also use std::tuple but that also stores runtime values,
    and some things will be easier if we have our own type.)

    Now we'll go ahead and implement `operator|`, then `get_mode`,
    finally we look at tightening up the type system.
*/
