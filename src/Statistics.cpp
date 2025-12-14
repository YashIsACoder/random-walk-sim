#include "Statistics.hpp"
#include <cmath>

// mean squared displacement
namespace Statistics {
    double MSD(const std::vector<Position>& path) {
        double sum = 0;
        double count = static_cast<double>(path.size());

        for (const auto &p: path) {
            sum += p.x*p.x + p.y*p.y+ p.z*p.z;
        }

        return sum / count;
    }
    // end to end distance
    double EED(const std::vector<Position>& path) {
        const auto &p = path.back();
        double norm = std::sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
        return norm;
    }
}