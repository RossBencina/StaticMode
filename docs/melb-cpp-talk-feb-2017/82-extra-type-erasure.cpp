//!clang++ -std=c++11 -Weverything -Wno-c++98-compat -Wno-unused-const-variable 82-extra-type-erasure.cpp -o z82.out && ./z82.out

// Define an abstract Painter base class (interface)
// Provide an implementation of Painter: AsciiPainterT<> that is a template,
// parameterised by a mode expression.  Instantiate template instances
// using mode expressions -- either directly, or using a factory function
// buildAsciiPainter.

#include <iostream> // cout
#include <type_traits> // is_same
#include <memory> // unique_ptr

#include "35-typechecking-tools.h"

// ............................................................................
// Library code

template<typename T>
struct ModeType {}; // aka mode category

template<typename T, T X>
struct Mode : ModeType<T> {
    constexpr Mode() {}

    // typechecking support:

    using value_type = T; // value_type is the enum class type
    using value_types = type_pack_<T>; // a list of one type
};

// Type checking predicates:

template<typename T>
struct is_mode_ : std::false_type {}; // C++11: false_type, true_type. e.g. false_type = integral_constant<bool, false>

template<typename T, T X>
struct is_mode_<Mode<T, X> > : std::true_type {};

template<typename T>
using is_mode = is_mode_<remove_cv_t_<T> >;


template<typename... Ts>
struct ModeSet {

    // typechecking support:

    using value_types = type_pack_<typename Ts::value_type...>; // value_types is the list of enum class types in this set

    // Invariants:

    // A ModeSet is a set of zero or more Modes
    static_assert(and_<is_mode_<Ts>...>::value,
        "A ModeSet must contain only Modes");

    // A ModeSet_ contains at most one Mode_ from each mode category (enum class).
    static_assert(contains_duplicate_no_cv<value_types>::value == false,
        "Duplicate mode category detected (two or more values from the same enum class). "
        "A ModeSet may contain at most one Mode from each mode category (enum class).");
};

// Type checking predicates:

template<typename T>
struct is_mode_set_ : std::false_type {}; // C++11: false_type, true_type

template<typename... Ts>
struct is_mode_set_<ModeSet<Ts...> > : std::true_type {};

template<typename T>
using is_mode_set = is_mode_set_<remove_cv_t_<T> >;

template<typename T>
struct is_mode_expr : or_<is_mode<T>, is_mode_set<T> > {};

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

class Painter {
public:
    virtual ~Painter();

    virtual void drawLine() = 0;
};

Painter::~Painter() {}

template<typename ModesExpr=ModeSet<> > // default to empty ModeSet
class AsciiPainterT : public Painter {
    // Notice that we put type checks at the class-level here:

    static_assert(is_mode_expr<ModesExpr>::value == true, "expected a Mode or ModeSet");
    using AcceptedModes = type_pack_<LineStyle, EndStyle>;
    static_assert(is_subset_no_cv<typename ModesExpr::value_types, AcceptedModes>::value,
            "drawLine() ModesExpr argument contains a mode from an invalid category (enum class). "
            "expected at most one line style and one end style.");

    using lineStyle_t = get_mode_t<LineStyle, ModesExpr, /*default:*/decltype(solid)>;
    using endStyle_t = get_mode_t<EndStyle, ModesExpr, /*default:*/decltype(no_ends)>;

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

    void drawLine() override {
        drawLeftEnd_(endStyle_t{});
        drawLine_(lineStyle_t{});
        drawRightEnd_(endStyle_t{});
        std::cout << "\n";
    }
};

template<typename ModesExpr=ModeSet<> >
std::unique_ptr<Painter> buildAsciiPainter(ModesExpr={})
{
    return std::unique_ptr<Painter>(new AsciiPainterT<ModesExpr>());
}

int main()
{
    Painter *p1 = new AsciiPainterT<decltype(dashed|arrows)>();
    p1->drawLine();
    delete p1;

    std::unique_ptr<Painter> p2(buildAsciiPainter(dotted|circles));
    p2->drawLine();

    std::unique_ptr<Painter> p3(buildAsciiPainter(dashed));
    p3->drawLine();

    std::unique_ptr<Painter> p4(buildAsciiPainter());
    p4->drawLine();

    // correctly caught errors:
    //buildAsciiPainter(dotted | solid);               // mutually exclusive flags
    //buildAsciiPainter(dashed | red);                 // red: invalid mode category
    //buildAsciiPainter(42);                           // invalid argument type
    //ModeSet<int, char, float> X{};
    //buildAsciiPainter(ModeSet<int, char, float>{});  // malformed ModeSet
}
