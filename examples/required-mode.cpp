//!clang++ -std=c++11 -Weverything -Wno-c++98-compat -Wno-unused-const-variable required-mode.cpp -I../include -o required-mode.out && ./required-mode.out

// This example uses `has_mode` to require that a line style be supplied to `drawLine()`.
// In other respects it is the same as two-modes-1.cpp.
// Note that since there must be a line style, we don't support a default
// argument to draw line.
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

    template<typename ModeExpr>
    void drawLine(ModeExpr /*modes*/) { // no default parameter
        static_assert(staticmode::is_mode_expr<ModeExpr>::value == true,
                "/modes/ argument has unexpected type. Expected a Mode or ModeSet.");

        static_assert(staticmode::has_mode<LineStyle, ModeExpr>::value == true,
                "/modes/ is missing required mode: line style.");

        using AcceptedModes = staticmode::type_pack<LineStyle, EndStyle>;
        static_assert(staticmode::has_no_other_modes<AcceptedModes, ModeExpr>::value,
                "/modes/ argument contains a mode from an unexpected category (enum class). "
                "Expected at most one line style and one end style.");

        using lineStyle_t = staticmode::get_mode_t<LineStyle, ModeExpr, /*default:*/decltype(solid)>;
        using endStyle_t = staticmode::get_mode_t<EndStyle, ModeExpr, /*default:*/decltype(no_ends)>;

        drawLeftEnd_(endStyle_t{});
        drawLine_(lineStyle_t{});
        drawRightEnd_(endStyle_t{});
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

    // The following are correctly caught as compile errors.
    // Uncomment any of the lines below and you'll get an informative
    // static_assert-based compiler error.

    //painter.drawLine();                   // missing parameter
    //painter.drawLine(arrows);             // missing required mode: line style
}
