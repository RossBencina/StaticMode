#ifndef INCLUDED_20B_ONE_MODE_H
#define INCLUDED_20B_ONE_MODE_H

// Mechanism: (data-driven) Constant values that parameterise the drawing algorithm

// A data-driven approach makes a lot of sense for the
// dotted/dashed line drawing example. In a real system,
// the parameter might be a type that represents the mark-space
// ratio of drawn line to gaps.

// This is just an example of one way to implement drawLine()
// with a mode select parameter. We won't explore this
// implementation method further.

enum LineStyle : char { // C++11: specify underlying type
    dotted = '.',
    dashed = '-',
    solid = '_'
};

class AsciiPainter {
public:
    void drawLine(LineStyle lineStyle) {
        for (size_t i=0; i < 10; ++i)
            std::cout << static_cast<char>(lineStyle);
        std::cout << '\n';
    }
};

#endif /* INCLUDED_20B_ONE_MODE_H */
