//!clang++ -std=c++11 -Weverything -Wno-c++98-compat 27-two-modes-alt-impls.cpp -o z27.out && ./z27.out

// (continued from previous file)
// Two additional ways of decomposing the implementation of drawLine()
// for two modes.

// Each factoring can be enabled by setting the `#if 0` to `#if 1`
// at the top of its block. Make sure only one block is enabled at a time.

// The main point of this file is to show that there is likely
// more than one way to implement functions that have multiple
// mode categories.

// Notice again that we have to define some extra one-parameter functions
// to support defaulting the first (but not the second) parameter; and
// to support the zero-argument case.

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
// ............................................................................
#if 1
    // brute force (same as previous file) notice: no templates for dispatch
    void drawLine(decltype(dotted), decltype(no_ends)={}) { std::cout << " ..........\n"; } // NB: default LineEnd
    void drawLine(decltype(dashed), decltype(no_ends)={}) { std::cout << " ----------\n"; }
    void drawLine(decltype(solid), decltype(no_ends)={}) { std::cout << " __________\n"; }
    void drawLine(decltype(dotted), decltype(arrows)) { std::cout << "<..........>\n"; }
    void drawLine(decltype(dashed), decltype(arrows)) { std::cout << "<---------->\n"; }
    void drawLine(decltype(solid), decltype(arrows)) { std::cout << "<__________>\n"; }
    void drawLine(decltype(dotted), decltype(circles)) { std::cout << "o..........o\n"; }
    void drawLine(decltype(dashed), decltype(circles)) { std::cout << "o----------o\n"; }
    void drawLine(decltype(solid), decltype(circles)) { std::cout << "o__________o\n"; }

    void drawLine(decltype(no_ends)={}) { drawLine(solid, no_ends); } // NB: one parameter versions with default LineStyle
    void drawLine(decltype(arrows)) { drawLine(solid, arrows); }
    void drawLine(decltype(circles)) { drawLine(solid, circles); }
#endif
// ............................................................................
#if 0
// Orthogonal components (flat template dispatch)
// This implementation separates drawing the line, the left, and the right
// line end. Each function is selected using overloading.
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
    template<LineStyle X, EndStyle Y=EndStyle::no_ends>
    void drawLine(Mode<LineStyle,X> lineStyle, Mode<EndStyle,Y> endStyle={}) { // both params or LineStyle
        drawLeftEnd_(endStyle);
        drawLine_(lineStyle);
        drawRightEnd_(endStyle);
        std::cout << "\n";
    }

    template<EndStyle X=EndStyle::no_ends>
    void drawLine(Mode<EndStyle,X> endStyle={}) { // EndStyle or no params
        drawLine(solid, endStyle);
    }
#endif
// ............................................................................
#if 0
// Another way to decompose the problem (passthrough template dispatch)
// This implementation defines an outer function to draw the line ends
// for each EndStyle, which delegates to a function to draw the line.
// Notice how the LineStyle Mode type is captured as a template parameter in the
// outer function, so that the correct Mode type can be passed to the inner function.
private:
    void drawLine_(decltype(dotted)) { std::cout << ".........."; }
    void drawLine_(decltype(dashed)) { std::cout << "----------"; }
    void drawLine_(decltype(solid)) { std::cout << "__________"; }
public:
    template<LineStyle X>
    void drawLine(Mode<LineStyle,X> lineStyle, decltype(no_ends)={}) {
        std::cout << " ";
        drawLine_(lineStyle);
        std::cout << "\n";
    }

    template<LineStyle X>
    void drawLine(Mode<LineStyle,X> lineStyle, decltype(arrows)) {
        std::cout << "<";
        drawLine_(lineStyle);
        std::cout << ">\n";
    }

    template<LineStyle X>
    void drawLine(Mode<LineStyle,X> lineStyle, decltype(circles)) {
        std::cout << "o";
        drawLine_(lineStyle);
        std::cout << "o\n";
    }

    template<EndStyle X=EndStyle::no_ends>
    void drawLine(Mode<EndStyle,X> endStyle={}) {
        drawLine(solid, endStyle);
    }
#endif
// ............................................................................
};

int main()
{
    AsciiPainter painter;
    painter.drawLine(dotted, no_ends);
    painter.drawLine(dashed, arrows);
    painter.drawLine(solid, circles);
    painter.drawLine(solid, arrows);

    // If you try to compile the following, you'll get an error.
    // There is no implementation of drawLine that accepts line
    // styles for both parameters.
    //painter.drawLine(solid, dotted);

    painter.drawLine(dashed); // EndStyle is set via default parameter

    painter.drawLine(arrows); // LineStyle is defaulted

    painter.drawLine(); // both LineStyle and EndStyle are defaulted
}
