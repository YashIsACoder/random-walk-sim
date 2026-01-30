#include "Statistics.hpp"
#include <cmath>

namespace Statistics {

double meanSquaredDisplacement(const std::vector<Position>& path) {
    if (path.empty()) return 0.0;
    double sum = 0.0;
    for (const auto& p : path) {
        sum += static_cast<double>(p.x*p.x + p.y*p.y + p.z*p.z);
    }
    return sum / static_cast<double>(path.size());
}

double endToEndDistance(const std::vector<Position>& path) {
    if (path.empty()) return 0.0;
    const auto& p = path.back();
    return std::sqrt(static_cast<double>(p.x*p.x + p.y*p.y + p.z*p.z));
}

}
