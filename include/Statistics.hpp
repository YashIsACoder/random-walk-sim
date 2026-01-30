#pragma once
#include "Walker.hpp"
#include <vector>

namespace Statistics {
    double meanSquaredDisplacement(const std::vector<Position>& path);
    double endToEndDistance(const std::vector<Position>& path);
}
