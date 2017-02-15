#ifndef INCLUDED_20C_ONE_MODE_H
#define INCLUDED_20C_ONE_MODE_H

// Mechanism: Strategy design pattern using global constant
// strategies as our mode selection flags.

// This is just an example of one way to implement drawLine()
// with a mode select parameter. We won't explore this
// implementation method further.

class DrawLineStrategy {
public:
    virtual ~DrawLineStrategy();
    virtual void drawLine() const = 0;
};

class Dotted : public DrawLineStrategy {
public:
    virtual ~Dotted();
    virtual void drawLine() const override { std::cout << "..........\n"; } // C++11: override
};

class Dashed : public DrawLineStrategy {
public:
    virtual ~Dashed();
    virtual void drawLine() const override { std::cout << "----------\n"; }
};

class Solid : public DrawLineStrategy {
public:
    virtual ~Solid();
    virtual void drawLine() const override { std::cout << "__________\n"; }
};

extern const Dotted dotted;
extern const Dashed dashed;
extern const Solid solid;

class AsciiPainter {
public:
    void drawLine(const DrawLineStrategy& lineStrategy) {
        lineStrategy.drawLine();
    }
};





// NB: these should be in a .cpp file.
// Look away! I'm defining the strategy instances and dtor implementations
// here for expedience since they're only included once.
// Never define instances in a header file.

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wexit-time-destructors"

DrawLineStrategy::~DrawLineStrategy() {}
Dotted::~Dotted() {}
Dashed::~Dashed() {}
Solid::~Solid() {}

const Dotted dotted = {};
const Dashed dashed = {};
const Solid solid = {};

#pragma clang diagnostic pop

#endif /* INCLUDED_20CONE_MODE_H */
