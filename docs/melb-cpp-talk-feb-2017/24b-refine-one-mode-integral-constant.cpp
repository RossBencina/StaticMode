//!clang++ -std=c++11 -Weverything -Wno-c++98-compat 24b-refine-one-mode-integral-constant.cpp -o z24b.out && ./z24b.out

// This file is optional. It discusses the pros and cons of using
// std::integral_constant instead of our Mode template.
// We won't refer to std::integral_constant again.

// An alternative to our own Mode template is std::integral_constant
// There's an example definition of it in 99-ref-integral_constant.md.
// It has a similar definition to our Mode template.
//
// Pros:
//   * it's standard, so we don't need to define any support code
//   * it defines some useful member functionality which we might
//     want to use (or reproduce.)
//
// Cons:
//   * doesn't have a base class, so general-case default requires a template (see example below)
//   * later, we'll be defining operators over our Mode template, and
//     we want our operators to only work with our Modes. We wouldn't
//     want to write operators/expression-templates that match all
//     integral_constants.

// Bottom line: If you only need to do tag dispatch, integral_constant
// is a good option to consider. But I don't think it's sufficient to
// implement the type-safe or-able flags technique I present in this talk.

#include <iostream> // cout
#include <type_traits> // integral_constant

enum class LineStyle { dotted, dashed, solid };

constexpr std::integral_constant<LineStyle, LineStyle::dotted> dotted = {}; // C++11: std::integral_constant
constexpr std::integral_constant<LineStyle, LineStyle::dashed> dashed = {};
constexpr std::integral_constant<LineStyle, LineStyle::solid> solid = {};

class AsciiPainter {
public:

    void drawLine(decltype(dotted)) {
        std::cout << "..........\n";
    }

    void drawLine(decltype(dashed)) {
        std::cout << "----------\n";
    }

    void drawLine(decltype(solid)) {
        std::cout << "__________\n";
    }
#if 1 // default case:
    template<LineStyle X>
    void drawLine(std::integral_constant<LineStyle, X>) {
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
}
