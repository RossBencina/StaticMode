//!clang++ -std=c++11 -Weverything -Wno-c++98-compat 26-two-modes.cpp -o z26.out && ./z26.out

// Goal: Demonstrate a few options for supporting two independent mode parameters.
// Observe how defaulting is handled.
//
// We will use the function overloading technique to implement two different modes:
//   LineStyle and EndStyle
//
// We want to accept four function call signatures:
//   1. drawLine(lineStyle, endStyle)
//   2. drawLine(lineStyle) // defaults to no EndStyle
//   3. drawLine(endStyle) // defaults to sold LineStyle
//   4. drawLine() // no  LineStyle nor EndStyle
//
// The specific implementations of drawLine() is not so important here.
// Ultimately when multiple modes are involved the decomposition
// will depend on what behavior the modes control.
//
// Notice also that we have to define some extra one-parameter functions
// to support defaulting the first (but not the second) parameter; and
// to support the zero-argument case.
//
// (continued next file)

#include <iostream> // cout

// ............................................................................
// Library code

template<typename T>
struct ModeType {}; // aka mode category

template<typename T, T X>
struct Mode : ModeType<T> {
    constexpr Mode() {}
};

// ............................................................................

enum class LineStyle { dotted, dashed, solid };

constexpr Mode<LineStyle, LineStyle::dotted> dotted;
constexpr Mode<LineStyle, LineStyle::dashed> dashed;
constexpr Mode<LineStyle, LineStyle::solid> solid;

enum class EndStyle { no_ends, arrows, circles };

constexpr Mode<EndStyle, EndStyle::no_ends> no_ends;
constexpr Mode<EndStyle, EndStyle::arrows> arrows;
constexpr Mode<EndStyle, EndStyle::circles> circles;

class AsciiPainter {
public:
    // brute force decomposition. one function for each mode combination (3 x 3 = 9)
    void drawLine(decltype(dotted), decltype(no_ends)={}) { std::cout << " ..........\n"; } // NB: default LineEnd
    void drawLine(decltype(dashed), decltype(no_ends)={}) { std::cout << " ----------\n"; }
    void drawLine(decltype(solid), decltype(no_ends)={}) { std::cout << " __________\n"; }
    void drawLine(decltype(dotted), decltype(arrows)) { std::cout << "<..........>\n"; }
    void drawLine(decltype(dashed), decltype(arrows)) { std::cout << "<---------->\n"; }
    void drawLine(decltype(solid), decltype(arrows)) { std::cout << "<__________>\n"; }
    void drawLine(decltype(dotted), decltype(circles)) { std::cout << "o..........o\n"; }
    void drawLine(decltype(dashed), decltype(circles)) { std::cout << "o----------o\n"; }
    void drawLine(decltype(solid), decltype(circles)) { std::cout << "o__________o\n"; }

    // addition overloads for default LineStyle (and zero-parameter case)
    void drawLine(decltype(no_ends)={}) { drawLine(solid, no_ends); } // NB: one parameter versions with default LineStyle
    void drawLine(decltype(arrows)) { drawLine(solid, arrows); }
    void drawLine(decltype(circles)) { drawLine(solid, circles); }
};

int main()
{
    AsciiPainter painter;
    painter.drawLine(dotted, no_ends);
    painter.drawLine(dashed, arrows);
    painter.drawLine(solid, circles);

    painter.drawLine(dashed); // EndStyle is set via default parameter

    painter.drawLine(arrows); // LineStyle is defaulted

    painter.drawLine(); // both LineStyle and EndStyle are defaulted
}
