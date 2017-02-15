//!clang++ -std=c++03 -Weverything 03-running-example.cpp -o z03.out && ./z03.out

#include <iostream>

int main()
{
    /*
        Our running example will be "drawing" horizontal lines
        using ASCII art. The lines can be drawn in three styles:
    */
    std::cout << "drawLine(dotted):\n";
    std::cout << "..........\n";
    std::cout << "drawLine(dashed):\n";
    std::cout << "----------\n";
    std::cout << "drawLine(solid):\n";
    std::cout << "__________\n";
    /*
        Lines are always the same length. (Of course in a )

        Later we'll also add the option of having different
        end styles. They can be combined with line styles
        in any combination.
    */
    std::cout << "drawLine(solid|no_ends):\n";
    std::cout << " __________\n";
    std::cout << "drawLine(dashed|arrows):\n";
    std::cout << "<---------->\n";
    std::cout << "drawLine(solid|circles):\n";
    std::cout << "o__________o\n";
    /*
        In summary, we have two categories of "modes":

          (1) LineStyle is one of { dotted, dashed, solid }

          (2) EndStyle is one of { no_ends, arrows, circles }

        It's critical to understand that in each of these "modes"
        the options are mutually exclusive. For example, it's
        impossible to have a LineStyle that is both solid and dashed,
        or an EndStyle that has no_ends and circles.
    */
}
