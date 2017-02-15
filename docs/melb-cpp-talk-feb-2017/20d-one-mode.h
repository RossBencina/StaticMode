#ifndef INCLUDED_20D_ONE_MODE_H
#define INCLUDED_20D_ONE_MODE_H

// Mechanism: Select among multiple drawLine() implementations by overloading
// on separate types representing each mode. The mode flags are constexpr
// instances of each mode type.

// It's important to understand how this example works, because it's
// the basis of the technique that we'll be developing in this talk.

// You can study the full example in one file in: 21-refine-one-mode-consolidated.cpp

// [Aside: this is also the basis of a technique called "tag dispatch".
// I'm refraining from talking about tag dispatch here because our goal
// is different (we're specifying flag parameters for a public API) and tag
// dispatch is a big topic on its own!)]

struct dotted_type {};
struct dashed_type {};
struct solid_type {};

constexpr dotted_type dotted = {}; // C++11: constexpr
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

#endif /* INCLUDED_20D_ONE_MODE_H */
