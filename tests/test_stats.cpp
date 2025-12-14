#include "Statistics.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

constexpr double EPS = 1e-9;

void test_case(const std::string& name,
               const std::vector<Position>& path,
               double expected_msd,
               double expected_eed) {
    double msd = Statistics::MSD(path);
    double eed = Statistics::EED(path);

    std::cout << "Running " << name << "...\n";
    std::cout << "  MSD = " << msd << " (expected " << expected_msd << ")\n";
    std::cout << "  EED = " << eed << " (expected " << expected_eed << ")\n";

    assert(std::fabs(msd - expected_msd) < EPS && "MSD mismatch");
    assert(std::fabs(eed - expected_eed) < EPS && "EED mismatch");

    std::cout << "  ✅ Passed\n\n";
}

int main() {
    // 1D walk along x-axis: (0,0,0) → (1,0,0) → (2,0,0)
    {
        std::vector<Position> path = {{0,0,0}, {1,0,0}, {2,0,0}};
        // MSD = average of squared distances from origin:
        // (0^2 + 1^2 + 2^2)/3 = (0 + 1 + 4)/3 = 5/3
        double expected_msd = 5.0/3.0;
        // EED = distance from start (0,0,0) to end (2,0,0) = 2
        double expected_eed = 2.0;
        test_case("1D x-axis walk", path, expected_msd, expected_eed);
    }

    // 2D walk: (0,0,0) → (0,1,0) → (0,2,0)
    {
        std::vector<Position> path = {{0,0,0}, {0,1,0}, {0,2,0}};
        // MSD = (0^2 + 1^2 + 2^2)/3 = 5/3
        double expected_msd = 5.0/3.0;
        // EED = distance from (0,0,0) to (0,2,0) = 2
        double expected_eed = 2.0;
        test_case("2D y-axis walk", path, expected_msd, expected_eed);
    }

    // 3D walk: (0,0,0) → (1,1,1)
    {
        std::vector<Position> path = {{0,0,0}, {1,1,1}};
        // MSD = (0^2 + (1^2+1^2+1^2))/2 = (0 + 3)/2 = 1.5
        double expected_msd = 1.5;
        // EED = distance from (0,0,0) to (1,1,1) = sqrt(3)
        double expected_eed = std::sqrt(3.0);
        test_case("3D diagonal walk", path, expected_msd, expected_eed);
    }

    std::cout << "All tests passed!\n";
    return 0;
}
