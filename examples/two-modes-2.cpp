//!clang++ -std=c++11 -Weverything -Wno-c++98-compat -Wno-unused-const-variable two-modes-2.cpp -I../include -o two-modes-2.out && ./two-modes-2.out

// This example demonstrates:
//   - using `operator|` with constexpr Modes to create mode expressions (see `main()`)
//   - using `is_mode_expr` and `has_no_other_modes` to type check mode expressions
//   - using `get_mode_t` to extract modes (with default) from a mode expressions
//
// This version uses a combination of member function overloading and template
// member functions to implement the line drawing. See `two-modes-1.cpp` for
// an alternative approach.
//
// Uncomment the lines at the end of `main()` to test the compile-time error checking.

#include <iostream> // cout

#include "StaticMode.h"

enum class LineStyle { dotted, dashed, solid };

constexpr staticmode::Mode<LineStyle, LineStyle::dotted> dotted;
constexpr staticmode::Mode<LineStyle, LineStyle::dashed> dashed;
constexpr staticmode::Mode<LineStyle, LineStyle::solid> solid;

enum class EndStyle { no_ends, arrows, circles };

constexpr staticmode::Mode<EndStyle, EndStyle::no_ends> no_ends;
constexpr staticmode::Mode<EndStyle, EndStyle::arrows> arrows;
constexpr staticmode::Mode<EndStyle, EndStyle::circles> circles;

enum class Fruit { apple, orange, banana }; // not an accepted mode for drawLine()

constexpr staticmode::Mode<Fruit, Fruit::apple> apple;
constexpr staticmode::Mode<Fruit, Fruit::orange> orange;
constexpr staticmode::Mode<Fruit, Fruit::banana> banana;


class AsciiPainter {
private:
    void drawLineBody_(decltype(dotted)) { std::cout << ".........."; }
    void drawLineBody_(decltype(dashed)) { std::cout << "----------"; }
    void drawLineBody_(decltype(solid)) { std::cout << "__________"; }

    // Workaround for MSVC 2015 error C2062 "type 'unknown-type' unexpected"
    // when using decltype() in a template parameter argument list.
    // See: http://stackoverflow.com/questions/41001482
    using no_ends_type = decltype(no_ends);
    using arrows_type = decltype(arrows);
    using circles_type = decltype(circles);

    template<LineStyle X>
    void drawLine_(staticmode::Mode<LineStyle,X> lineStyle, no_ends_type) {
        std::cout << " ";
        drawLineBody_(lineStyle);
        std::cout << "\n";
    }

    template<LineStyle X>
    void drawLine_(staticmode::Mode<LineStyle,X> lineStyle, arrows_type) {
        std::cout << "<";
        drawLineBody_(lineStyle);
        std::cout << ">\n";
    }

    template<LineStyle X>
    void drawLine_(staticmode::Mode<LineStyle,X> lineStyle, circles_type) {
        std::cout << "o";
        drawLineBody_(lineStyle);
        std::cout << "o\n";
    }

public:
    template<typename ModeExpr=staticmode::ModeSet<> > // default to empty ModeSet
    void drawLine(ModeExpr /*modes*/={}) {
        static_assert(staticmode::is_mode_expr<ModeExpr>::value == true,
                "/modes/ argument has unexpected type. Expected a Mode or ModeSet.");

        using AcceptedModes = staticmode::type_pack<LineStyle, EndStyle>;
        static_assert(staticmode::has_no_other_modes<AcceptedModes, ModeExpr>::value,
                "/modes/ argument contains a mode from an unexpected category (enum class). "
                "Expected at most one line style and one end style.");

        using lineStyle_t = staticmode::get_mode_t<LineStyle, ModeExpr, /*default:*/decltype(solid)>;
        using endStyle_t = staticmode::get_mode_t<EndStyle, ModeExpr, /*default:*/decltype(no_ends)>;

        drawLine_(lineStyle_t{}, endStyle_t{});
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

    // The following are correctly caught as compile errors.
    // Uncomment any of the lines below and you'll get an informative
    // static_assert-based compiler error.

    //painter.drawLine(dotted | solid);     // mutually exclusive flags
    //painter.drawLine(dashed | banana);    // banana: invalid mode category for drawLine()
    //painter.drawLine(42);                 // invalid argument type
}
