#ifndef INCLUDED_20A_ONE_MODE_H
#define INCLUDED_20A_ONE_MODE_H

// Mechanism: Flags that select a drawing method

// This is perhaps the most common way to implement a selector flag
// parameter. In C++11 you might want to consider using
// `enum class` instead of `enum`.

// This is just an example of one way to implement drawLine()
// with a mode select parameter. We won't explore this
// implementation method further.

enum LineStyle { dotted, dashed, solid };

class AsciiPainter {
public:
    void drawLine(LineStyle lineStyle) {
        switch (lineStyle) {
        case dotted:
            std::cout << "..........\n";
            break;
        case dashed:
            std::cout << "----------\n";
            break;
        case solid:
            std::cout << "__________\n";
            break;
        }
    }
};

#endif /* INCLUDED_20A_ONE_MODE_H */
