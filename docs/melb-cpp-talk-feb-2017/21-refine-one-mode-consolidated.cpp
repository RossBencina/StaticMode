//!clang++ -std=c++11 -Weverything -Wno-c++98-compat 21-refine-one-mode-consolidated.cpp -o z21.out && ./z21.out

// This file is exactly what we had in 20-one-mode.cpp plus 20d-one-mode.h

// In the following files we'll make some refinements to provide
// a Mode abstraction, which will use for the rest of the talk.

#include <iostream> // cout

struct dotted_type {};
struct dashed_type {};
struct solid_type {};

constexpr dotted_type dotted = {};
constexpr dashed_type dashed = {};
constexpr solid_type solid = {};

class AsciiPainter {
public:
    void drawLine(dotted_type) {
        std::cout << "..........\n";
    }

    void drawLine(dashed_type) {
        std::cout << "----------\n";
    }

    void drawLine(solid_type) {
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
