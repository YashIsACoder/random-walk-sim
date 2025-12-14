#pragma once

#include "Walker.hpp"
#include <vector>

namespace Statistics {
    // mean squared displacement
    double MSD(const std::vector<Position>& path);
    // end to end distance
    double EED(const std::vector<Position>& path);
}