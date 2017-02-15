//!clang++ -std=c++11 -Weverything -Wno-c++98-compat 23-refine-one-mode-defaults.cpp -o z23.out && ./z23.out

// Second refinement:
//   * add a base class to Mode: ModeType<T> to support a general-case overload
//   * default parameter to drawLine(solid) to allow zero-argument invocation

// (The general-case overload is not so useful for the line drawing example.
// But sometimes an implementation will decompose in a way that allows the
// general case to be used most of the time, with only a few modes needing
// mode-specific overloaded functions.)

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

class AsciiPainter {
public:
    void drawLine(decltype(dotted)) {
        std::cout << "..........\n";
    }

    // Try: comment out this function and observe that the default-case overload is invoked
    void drawLine(decltype(dashed)) {
        std::cout << "----------\n";
    }

    void drawLine(decltype(solid)={}) { // add ={} to make a zero-argument default
        std::cout << "__________\n";
    }

#if 1 // default case:
    void drawLine(ModeType<LineStyle>) {
        std::cout << "##########\n";
    }
#endif
};

int main()
{
    AsciiPainter painter;
    painter.drawLine(dotted);
    painter.drawLine(dashed);
    painter.drawLine(solid);

    painter.drawLine(); // invoke zero-argument default
}
