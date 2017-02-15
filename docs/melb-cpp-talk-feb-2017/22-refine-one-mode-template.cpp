//!clang++ -std=c++11 -Weverything -Wno-c++98-compat 22-refine-one-mode-template.cpp -o z22.out && ./z22.out

// First refinement:
//
// Change the representation of our mode selector types:
//   * use `enum class` to represent a category of modes
//     - provides a semantic hint about the contents of the category
//     - provides a type to represent the category (the enum class type)
//   * wrap enum members in a Mode template struct so that we have one type per value
//
// To save typing, use `decltype` in function definitions, to avoid having
// to declare both a type and a value (e.g. we have Mode<...> solid, but
// not solid_type.)
//
// Make sure that you understand the definition of the Mode template:
//
//   template<typename T, T X>
//   struct Mode;
//
// The first parameter will be an enum class *type*. The second parameter will
// be an enum *value* belonging to enum class type T.

#include <iostream> // cout

// ............................................................................
// Library code

template<typename T, T X> // <-- Important: T will be an enum class type, X is an instance of T
struct Mode {
    // default ctor allow default initialization of constexpr instances:
    constexpr Mode() {} // C++11: constexpr
};

// ............................................................................

enum class LineStyle { dotted, dashed, solid }; // C++11: scoped enumeration

// dotted, dashed, solid are constexpr instances of three distinct types:
constexpr Mode<LineStyle, LineStyle::dotted> dotted;
constexpr Mode<LineStyle, LineStyle::dashed> dashed;
constexpr Mode<LineStyle, LineStyle::solid> solid;

class AsciiPainter {
public:
    void drawLine(decltype(dotted)) { // C++11: decltype
        std::cout << "..........\n";  // decltype(dotted): Mode<LineStyle, LineStyle::dotted>
    }

    void drawLine(decltype(dashed)) {
        std::cout << "----------\n";
    }

    void drawLine(decltype(solid)) {
        std::cout << "__________\n";
    }
};

int main()
{
    AsciiPainter painter;
    painter.drawLine(dotted);
    painter.drawLine(dashed);
    painter.drawLine(solid);
}
