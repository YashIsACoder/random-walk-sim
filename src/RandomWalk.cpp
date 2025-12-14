#include "RandomWalk.hpp"

RandomWalk::RandomWalk(Dimension dim, unsigned int seed)
    : walker_(dim, seed) {
    path_.push_back(walker_.position());
}

void RandomWalk::run(std::size_t steps) {
    for (std::size_t i = 0; i < steps; ++i) {
        walker_.step();
        path_.push_back(walker_.position());
    }
}

const std::vector<Position>& RandomWalk::trajectory() const noexcept {
    return path_;
}