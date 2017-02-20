//!clang++ -std=c++11 -Weverything -Wno-c++98-compat -Wno-unused-const-variable type-erasure.cpp -I../include -o type-erasure.out && ./type-erasure.out

// This example demonstrates using Modes to configure a class implementation.
// It uses type erasure to expose behavior through a base class interface.
//
// `Painter` is an abstract base class that exposes a `drawLine()` virtual method.
// `AsciiPainterT` is a template that derives from `Painter` and implements
// `drawLine`. `AsciiPainterT` implements a family of `drawLine` implementations.
// The desired implementation is configured at compile time using Modes.

#include <iostream> // cout
#include <memory> // unique_ptr

#include "StaticMode.h"

enum class LineStyle { dotted, dashed, solid };

constexpr staticmode::Mode<LineStyle, LineStyle::dotted> dotted;
constexpr staticmode::Mode<LineStyle, LineStyle::dashed> dashed;
constexpr staticmode::Mode<LineStyle, LineStyle::solid> solid;

enum class EndStyle { no_ends, arrows, circles };

constexpr staticmode::Mode<EndStyle, EndStyle::no_ends> no_ends;
constexpr staticmode::Mode<EndStyle, EndStyle::arrows> arrows;
constexpr staticmode::Mode<EndStyle, EndStyle::circles> circles;

enum class Fruit { apple, orange, banana }; // not an accepted mode

constexpr staticmode::Mode<Fruit, Fruit::apple> apple;
constexpr staticmode::Mode<Fruit, Fruit::orange> orange;
constexpr staticmode::Mode<Fruit, Fruit::banana> banana;


class Painter {
public:
    virtual ~Painter();

    virtual void drawLine() = 0;
};

Painter::~Painter() {}


template<typename ModeExpr=staticmode::ModeSet<> > // defaults to empty ModeSet
class AsciiPainterT : public Painter {
    // Notice that we put type checks at the class-level here:

    static_assert(staticmode::is_mode_expr<ModeExpr>::value == true,
            "/ModeExpr/ template argument has unexpected type. Expected a Mode or ModeSet.");

    using AcceptedModes = staticmode::type_pack<LineStyle, EndStyle>;
    static_assert(staticmode::has_no_other_modes<AcceptedModes, ModeExpr>::value,
            "/ModeExpr/ template argument contains a mode from an unexpected category (enum class). "
            "Expected at most one line style and one end style.");

    using lineStyle_t = staticmode::get_mode_t<LineStyle, ModeExpr, /*default:*/decltype(solid)>;
    using endStyle_t = staticmode::get_mode_t<EndStyle, ModeExpr, /*default:*/decltype(no_ends)>;

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

template<typename ModeExpr=staticmode::ModeSet<> >
std::unique_ptr<Painter> buildAsciiPainter(ModeExpr /*modes*/={})
{
    return std::unique_ptr<Painter>(new AsciiPainterT<ModeExpr>());
}

int main()
{
    // Example 1: direct instantiation of an AsciiPainterT
    Painter *p1 = new AsciiPainterT<decltype(dashed|arrows)>();
    p1->drawLine();
    delete p1;

    // Example 2: using a factory function, passing mode constants as a parameter
    std::unique_ptr<Painter> p2(buildAsciiPainter(dotted|circles));
    p2->drawLine();

    std::unique_ptr<Painter> p3(buildAsciiPainter(dashed));
    p3->drawLine();

    std::unique_ptr<Painter> p4(buildAsciiPainter());
    p4->drawLine();

    // The following are correctly caught as compile errors.
    // Uncomment any of the lines below and you'll get an informative
    // static_assert-based compiler error.

    //buildAsciiPainter(dotted | solid);               // mutually exclusive flags
    //buildAsciiPainter(dashed | apple);               // apple: invalid mode category
    //buildAsciiPainter(42);                           // invalid argument type
    //ModeSet<int, char, float> X{};
    //buildAsciiPainter(ModeSet<int, char, float>{});  // malformed ModeSet
}
