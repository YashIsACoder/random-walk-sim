#pragma once
#include <random>

struct Position {
    int x{0};
    int y{0};
    int z{0};
};

enum class Dimension : int {
    OneD = 1,
    TwoD = 2,
    ThreeD = 3
};

enum class StepType {
    Lattice,    // +/-1 in axis
    Gaussian    // gaussian step (approximates Brownian)
};

class Walker {
public:
    Walker(Dimension dim = Dimension::TwoD, StepType type = StepType::Lattice, unsigned int seed = std::random_device{}());

    // take one step
    void step();

    // get current position
    const Position& pos() const noexcept { return pos_; }

    void reset() noexcept { pos_ = {}; }

    // setters
    void setSeed(unsigned int s);
    void setDimension(Dimension d) noexcept { dim_ = d; }
    void setStepType(StepType t) noexcept { type_ = t; }

private:
    Dimension dim_;
    StepType type_;
    Position pos_;

    std::mt19937 rng_;
    std::uniform_int_distribution<int> coin_{0, 1}; // for coin flips
    std::normal_distribution<double> gauss_{0.0, 1.0};

    int latticeStep() {
        return coin_(rng_) ? 1 : -1;
    }
};
