//!clang++ -std=c++11 -Weverything -Wno-c++98-compat -Wno-unused-const-variable 34-demo-1.cpp -o z34.out && ./z34.out

// Combined demo of mode expressions, get_mode, and the two-parameter
// getLine() function that we developed earlier.

// At the end of this file we highlight that there is no protection
// against a number of illegal/undesirable cases. In the next file we
// will add type checking support to address these issues.

#include <iostream> // cout
#include <type_traits> // is_same

// ............................................................................
// Library code

template<typename T>
struct ModeType {}; // aka mode category

template<typename T, T X>
struct Mode : ModeType<T> {
    constexpr Mode() {}
};

template<typename... Ts>
struct ModeSet {};

// ModeSets can be constructed by combining Mode values using operator|:

// Mode<T,X> | Mode<U,Y> -> ModeSet< Mode<T,X>, Mode<U,Y> >
template<typename T, T X, typename U, U Y>
constexpr ModeSet<Mode<T,X>, Mode<U,Y> > operator|(Mode<T,X>, Mode<U,Y>) { return {}; }

// ModeSet<Ts...> | Mode<U,Y> -> ModeSet<Ts..., Mode<U,Y> >
template<typename... Ts, typename U, U Y>
constexpr ModeSet<Ts..., Mode<U,Y> > operator|(ModeSet<Ts...>, Mode<U,Y>) { return {}; }

// Mode<T,X> | ModeSet<Us...> -> ModeSet< Mode<T,X>, Us...>
template<typename T, T X, typename... Us>
constexpr ModeSet<Mode<T,X>, Us...> operator|(Mode<T,X>, ModeSet<Us...>) { return {}; }

// ModeSet<Ts...> | ModeSet<Us...> -> ModeSet<Ts..., Us...>
template<typename... Ts, typename... Us>
constexpr ModeSet<Ts..., Us...> operator|(ModeSet<Ts...>, ModeSet<Us...>) { return {}; }

// ............................................................................

// get_mode<KeyEnumClass, ModeExpr, Default>
//
// type parameters:
//   KeyEnumClass -- the type of the enum class used in Mode and ModeType
//   ModeExpr -- a ModeSet or a single mode
//   Default -- the type returned if KeyEnumClass does not match any mode in ModeExpr
//
// output:
//   get_mode<K, M, D>::type will either be a Mode<KeyEnumClass,X> matching
//   KeyEnumClass or it will be of type Default.
//
// example usage:
// using lineStyle_t = get_mode_t<LineStyle, ModeExpr, /*default:*/decltype(solid)>;
//   or
// auto lineStyle = get_mode_t<LineStyle, ModeExpr, /*default:*/decltype(solid)> {};

/// General case, returns Default
/// This will match when none of the specializations below match.

template <typename KeyEnumClass, typename ModeExpr, typename Default>
struct get_mode {
    using type = typename std::remove_cv<Default>::type; // C++11: remove_cv
};

/// Specialization when ModeExpr is a single ModeSet with matching KeyEnumClass

template <typename KeyEnumClass, KeyEnumClass X, typename Default>
struct get_mode<KeyEnumClass, Mode<KeyEnumClass, X>, Default> {
    using type = Mode<KeyEnumClass, X>;
};

/// Specialization when ModeExpr is a ModeSet

// Recursing case: front element does not have KeyEnumClass
template <typename KeyEnumClass, typename EnumClass, EnumClass X, typename... Ts, typename Default>
struct get_mode<KeyEnumClass, ModeSet<Mode<EnumClass, X>, Ts...>, Default>
{
    using type = typename get_mode<KeyEnumClass, ModeSet<Ts...>, Default>::type;
};

// Terminating matching case: front element has KeyEnumClass
template <typename KeyEnumClass, KeyEnumClass X, typename... Ts, typename Default>
struct get_mode<KeyEnumClass, ModeSet<Mode<KeyEnumClass, X>, Ts...>, Default> {
    using type = Mode<KeyEnumClass, X>;
};

// (Terminating empty set case is matched by the general template.)

/// get_mode_t alias type so we don't have to write "typename get_mode<x,y,z>::type"
template<typename KeyEnumClass, typename ModeExpr, typename Default> // C++11: alias template
using get_mode_t = typename get_mode<KeyEnumClass, ModeExpr, Default>::type;

// ............................................................................

enum class LineStyle { dotted, dashed, solid };

constexpr Mode<LineStyle, LineStyle::dotted> dotted;
constexpr Mode<LineStyle, LineStyle::dashed> dashed;
constexpr Mode<LineStyle, LineStyle::solid> solid;

enum class EndStyle { no_ends, arrows, circles };

constexpr Mode<EndStyle, EndStyle::no_ends> no_ends;
constexpr Mode<EndStyle, EndStyle::arrows> arrows;
constexpr Mode<EndStyle, EndStyle::circles> circles;

enum class Colour { red, green, blue }; // you wouldn't use modes for color, just for testing

constexpr Mode<Colour, Colour::red> red;
constexpr Mode<Colour, Colour::green> green;
constexpr Mode<Colour, Colour::blue> blue;


class AsciiPainter {
private:
    void drawLine_(decltype(dotted)) { std::cout << ".........."; }
    void drawLine_(decltype(dashed)) { std::cout << "----------"; }
    void drawLine_(decltype(solid)) { std::cout << "__________"; }

    void drawLeftEnd_(decltype(no_ends)) { std::cout << " "; }
    void drawLeftEnd_(decltype(arrows)) { std::cout << "<"; }
    void drawLeftEnd_(decltype(circles)) { std::cout << "o"; }

    void drawRightEnd_(decltype(no_ends)) {}
    void drawRightEnd_(decltype(arrows)) { std::cout << ">"; }
    void drawRightEnd_(decltype(circles)) { std::cout << "o"; }
public:

    template<typename ModesExpr=ModeSet<> > // default to empty ModeSet
    void drawLine(ModesExpr={}) {
        auto lineStyle = get_mode_t<LineStyle, ModesExpr, /*default:*/decltype(solid)> {};
        auto endStyle = get_mode_t<EndStyle, ModesExpr, /*default:*/decltype(no_ends)> {};

        drawLeftEnd_(endStyle);
        drawLine_(lineStyle);
        drawRightEnd_(endStyle);
        std::cout << "\n";
    }
};

int main()
{
    AsciiPainter painter;
    painter.drawLine(dotted | no_ends);
    painter.drawLine(dashed | arrows);
    painter.drawLine(solid | circles);

    painter.drawLine(dashed); // default EndStyle: no_ends

    painter.drawLine(arrows); // default LineStyle: solid

    painter.drawLine(); // default LineStyle and EndStyle: solid|no_ends

    /*
        Set the `#if 0` below to `#if 1` and you'll find that the file still compiles and runs,
        but that all of these problem cases are handled in different ways. Ideally we'd like
        them all to fail to compile.
    */

#if 0
    // problems:
    painter.drawLine(dotted | solid);               // mutually exclusive flags (takes first valid)
    painter.drawLine(dashed | red);                 // red: invalid mode category (ignores it)
    painter.drawLine(42);                           // invalid parameter type (ignores it, uses defaults)
    painter.drawLine(ModeSet<int, char, float>{});  // malformed ModeSet (ignores it, uses defaults)

    /*
        You'd need to check most of these problems at runtime if you were
        using a bitmask enum.

        Gameplan for type checking:
            * make ModeSets fail to compile unless they
                (1) only contain Modes
                (2) contain no duplicate Modes
            * check that ModeExpr argument is a Mode or ModeSet

            * check for "invalid mode category", by checking that
              the mode expression contains only a subset of
              supported modes.
    */
#endif
}
