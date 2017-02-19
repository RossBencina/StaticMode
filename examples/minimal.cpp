//!clang++ -std=c++11 -Weverything -Wno-c++98-compat -Wno-unused-const-variable minimal.cpp -I../include -o minimal.out && ./minimal.out

// This is the minimal example given in README.md
// It is similar to two-modes-1.cpp except that it defines a smaller set
// of modes, and does not perform any additional type checking in `drawLine()`.

#include <iostream> // cout

#include "StaticMode.h"

enum class LineStyle { dotted, dashed };

constexpr staticmode::Mode<LineStyle, LineStyle::dotted> dotted;
constexpr staticmode::Mode<LineStyle, LineStyle::dashed> dashed;

enum class EndStyle { arrow, circle };

constexpr staticmode::Mode<EndStyle, EndStyle::arrow> arrow;
constexpr staticmode::Mode<EndStyle, EndStyle::circle> circle;

inline void drawLine_(decltype(dotted)) { std::cout << "....."; }
inline void drawLine_(decltype(dashed)) { std::cout << "-----"; }
inline void drawRightEnd_(decltype(arrow)) { std::cout << ">"; }
inline void drawRightEnd_(decltype(circle)) { std::cout << "o"; }

template<typename ModeExpr=staticmode::ModeSet<> > // default to empty ModeSet
void drawLine(ModeExpr /*modes*/={}) {
    using lineStyle_t = staticmode::get_mode_t<LineStyle, ModeExpr, /*default:*/decltype(dashed)>;
    using endStyle_t = staticmode::get_mode_t<EndStyle, ModeExpr, /*default:*/decltype(arrow)>;

    drawLine_(lineStyle_t{});
    drawRightEnd_(endStyle_t{});
    std::cout << "\n";
}

int main()
{
#if 0
    // test drawLine_ and drawRightEnd_
    drawLine_(dotted);
    drawLine_(dashed);
    drawRightEnd_(arrow);
    drawRightEnd_(circle);
    std::cout << "\n";
#endif

    // test mode expressions
    dotted|arrow;
    dashed|circle;
    //dashed|dotted; // correctly fails to compile. can't be dashed *and* dotted

    // example usage:
    drawLine(dotted|circle);
    drawLine(dotted); // defaults to arrow end style
    drawLine(circle); // defaults to dashed line style
    drawLine(); // defaults to dashed|arrow
}
