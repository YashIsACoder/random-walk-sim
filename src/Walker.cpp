#include "Walker.hpp"

Walker::Walker(Dimension dim, unsigned int seed) : dim_(dim), rng_(seed) {}

void Walker::step() {
    switch (dim_) {
        case Dimension::OneD:
            pos_.x += direction_(rng_);
            break;
        
        case Dimension::TwoD:
            if (direction_(rng_) > 0) {
                pos_.x += direction_(rng_);
            } else {
                pos_.y += direction_(rng_);
            }
            break;
        
        case Dimension::ThreeD:
            int axis = std::uniform_int_distribution<int>(0,2)(rng_);
            if (axis == 0) pos_.x += direction_(rng_);
            if (axis == 1) pos_.y += direction_(rng_);
            if (axis == 2) pos_.z += direction_(rng_);
            break;
    }
}

void Walker::reset() {
    pos_ = {};
}

const Position& Walker::position() const noexcept {
    return pos_;
}