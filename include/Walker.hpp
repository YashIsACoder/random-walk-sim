#pragma once

#include <random>

struct Position {
    int x{0};
    int y{0};
    int z{0};
};

enum class Dimension {
    OneD = 1,
    TwoD = 2,
    ThreeD = 3
};

class Walker {
public:
    Walker(Dimension dim, unsigned int seed = std::random_device{}());

    void step();
    void reset();

    const Position& position() const noexcept;
private:
    Dimension dim_;
    Position pos_;

    std::mt19937 rng_;
    std::uniform_int_distribution<int> direction_{-1,1};
};