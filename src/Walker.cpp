#include "Walker.hpp"
#include <random>

Walker::Walker(Dimension dim, StepType type, unsigned int seed)
    : dim_(dim), type_(type), pos_{}, rng_(seed) {}

void Walker::setSeed(unsigned int s) {
    rng_.seed(s);
}

void Walker::step() {
    if (type_ == StepType::Lattice) {
        switch (dim_) {
            case Dimension::OneD:
                pos_.x += latticeStep();
                break;
            case Dimension::TwoD: {
                // choose axis uniformly
                if (coin_(rng_)) pos_.x += latticeStep();
                else pos_.y += latticeStep();
                break;
            }
            case Dimension::ThreeD: {
                std::uniform_int_distribution<int> axis(0,2);
                int a = axis(rng_);
                if (a==0) pos_.x += latticeStep();
                else if (a==1) pos_.y += latticeStep();
                else pos_.z += latticeStep();
                break;
            }
        }
    } else { // Gaussian step: add rounded gaussian value to each coord depending on dim
        if (dim_ == Dimension::OneD) {
            pos_.x += static_cast<int>(std::round(gauss_(rng_)));
        } else if (dim_ == Dimension::TwoD) {
            pos_.x += static_cast<int>(std::round(gauss_(rng_)));
            pos_.y += static_cast<int>(std::round(gauss_(rng_)));
        } else {
            pos_.x += static_cast<int>(std::round(gauss_(rng_)));
            pos_.y += static_cast<int>(std::round(gauss_(rng_)));
            pos_.z += static_cast<int>(std::round(gauss_(rng_)));
        }
    }
}
