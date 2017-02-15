//!clang++ -std=c++1z -Weverything -Wno-c++98-compat 24a-cpp17-auto-template-parameter.cpp -o z24a.out && ./z24a.out
// WARNING: bleeding edge. tested in gcc 7, possibly clang 4

// This file is optional. It's a side note about C++17 auto template parameters.
// It requires clang 4 or GCC 7 to compile.

// When C++17 arrives, "auto template parameter" syntax will allow us to
// simplify the declaration of Mode instances as follows:

// C++11:
//  template<typename T, T X>
//  struct Mode : ModeType<T> { ... };
//
//  constexpr Mode<LineStyle, LineStyle::dotted> dotted; // <-- notice that LineStyle appears twice

// C++17: (this file)
//   template<auto X> // C++17: auto template parameter
//   struct Mode { ... };
//
//   constexpr Mode<LineStyle::dotted> dotted; // <-- simplified
//
// http://en.cppreference.com/w/cpp/language/auto

#include <iostream> // cout

// ............................................................................
// Library code

template<typename T>
struct ModeType {}; // aka mode category

template<auto X> // C++17: auto template parameter
struct Mode : ModeType<decltype(X)> {
    constexpr Mode() {}
};

// ............................................................................

enum class LineStyle { dotted, dashed, solid };

constexpr Mode<LineStyle::dotted> dotted; // note simplified syntax
constexpr Mode<LineStyle::dashed> dashed;
constexpr Mode<LineStyle::solid> solid;

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
};

int main()
{
    AsciiPainter painter;
    painter.drawLine(dotted);
    painter.drawLine(dashed);
    painter.drawLine(solid);
}
