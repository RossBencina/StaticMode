//!clang++ -std=c++11 -Weverything -Wno-c++98-compat 20-one-mode.cpp -o z20.out && ./z20.out

// For starters, we'll only support specifying one mode: a line style.

// The code below shows how a client would draw some lines. There are
// many ways to implement this interface. Try to think about how you
// would implement this interface before looking at the implementations.

// It's important to study the `20d-one-mode.h` version -- it's the
// mechanism that we'll be using in this talk.

// You can study the full example in one file in: 21-refine-one-mode-consolidated.cpp

#include <iostream> // cout

// four ways to implement AsciiPainter
// (uncomment one at a time to build and run this file)
#include "20a-one-mode.h"
//#include "20b-one-mode.h"
//#include "20c-one-mode.h"
//#include "20d-one-mode.h"

int main()
{
    AsciiPainter painter;
    painter.drawLine(dotted);
    painter.drawLine(dashed);
    painter.drawLine(solid);
}

// Q: What does the implementation of AsciiPainter::drawLine(...) look like?
