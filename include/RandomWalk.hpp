#pragma once

#include "Walker.hpp"
#include <vector>

class RandomWalk {
public:
    RandomWalk(Dimension dim, unsigned int seed = std::random_device{}());

    void run(std::size_t steps);
    const std::vector<Position>& trajectory() const noexcept;
    
private:
    Walker walker_;
    std::vector<Position> path_;
};