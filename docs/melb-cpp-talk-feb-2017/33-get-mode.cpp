//!clang++ -std=c++11 -Weverything -Wno-c++98-compat -Wno-unused-const-variable 33-get-mode.cpp -o z33.out && ./z33.out

// Implement get_mode over mode expressions
//
// A mode expression can be:
//   - a single Mode<T,X> instance
//   - a ModeSet<...> containing zero or more modes
//
// Return a default if no matching mode is found.

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

enum class Colour { red, green, blue }; // you wouldn't use modes for color, just for testing

constexpr Mode<Colour, Colour::red> red;
constexpr Mode<Colour, Colour::green> green;
constexpr Mode<Colour, Colour::blue> blue;

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

// Test case. Extracts one mode:
// printLineStyle() extracts the line style from the provided mode expression
// and prints the line style name.

inline void printLineStyle_(decltype(dotted)) { std::cout << "dotted\n";}
inline void printLineStyle_(decltype(dashed)) { std::cout << "dashed\n";}
inline void printLineStyle_(decltype(solid)) { std::cout << "solid\n";}

template<typename ModeExpr>
void printLineStyle(ModeExpr)
{
    printLineStyle_(get_mode_t<LineStyle, ModeExpr, decltype(solid)>{});
}

int main()
{
    printLineStyle(arrows|painterly);                 // default: solid
    printLineStyle(sketchy|dotted|green);             // dotted
    printLineStyle(plain|arrows|red|dashed);          // dashed

    printLineStyle((sketchy|arrows)|solid);           // solid
    printLineStyle(arrows|(dotted|sketchy));          // dotted
    printLineStyle((sketchy|arrows)|(dotted|green));  // dotted
}
