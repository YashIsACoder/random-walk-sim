#pragma once
#include "Walker.hpp"
#include <vector>

class RandomWalk {
public:
    RandomWalk(Dimension dim = Dimension::TwoD, StepType type = StepType::Lattice, unsigned int seed = std::random_device{}());

    // run N steps at once (fast-forward)
    void run(std::size_t steps);

    // step exactly once (useful for animation)
    void stepOnce();

    // reset
    void reset();

    const std::vector<Position>& trajectory() const noexcept { return path_; }

   // get current step index
    std::size_t stepIndex() const noexcept { return path_.size() ? path_.size()-1 : 0; } 

    Walker& walker() noexcept { return walker_; }

    // trim trail to keep only last N points (for performance)
    void trimTrail(std::size_t keep);

private:
    Walker walker_;
    std::vector<Position> path_;
};
