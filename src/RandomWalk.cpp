#include "RandomWalk.hpp"

RandomWalk::RandomWalk(Dimension dim, StepType type, unsigned int seed)
    : walker_(dim, type, seed)
{
    path_.clear();
    path_.push_back(walker_.pos());
}

void RandomWalk::run(std::size_t steps) {
    for (std::size_t i = 0; i < steps; ++i) {
        walker_.step();
        path_.push_back(walker_.pos());
    }
}

void RandomWalk::stepOnce() {
    walker_.step();
    path_.push_back(walker_.pos());
}

void RandomWalk::reset() {
    walker_.reset();
    path_.clear();
    path_.push_back(walker_.pos());
}

void RandomWalk::trimTrail(std::size_t keep) {
    if (keep == 0) return;
    if (path_.size() > keep) {
        auto start = path_.end() - static_cast<std::ptrdiff_t>(keep);
        std::vector<Position> newp(start, path_.end());
        path_.swap(newp);
    }
}
