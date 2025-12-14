#include "RandomWalk.hpp"
#include "Statistics.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>

int main(int argc, char* argv[]) {
    // Default values
    std::size_t steps = 10'000;
    Dimension dim = Dimension::TwoD;
    std::string output = "../data/";

    // Parse arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--steps" && i + 1 < argc) {
            steps = static_cast<std::size_t>(std::stoul(argv[++i]));
        } else if (arg == "--dim" && i + 1 < argc) {
            int d = std::stoi(argv[++i]);
            if (d == 1) dim = Dimension::OneD;
            else if (d == 2) dim = Dimension::TwoD;
            else if (d == 3) dim = Dimension::ThreeD;
            else {
                std::cerr << "Invalid dimension: " << d << "\n";
                return 1;
            }
        } else if (arg == "--output" && i + 1 < argc) {
            output += argv[++i];
        } else {
            std::cerr << "Unknown option: " << arg << "\n";
            std::cerr << "Usage: ./random_walk --steps N --dim D --output file.csv\n";
            return 1;
        }
    }

    // Run simulation
    RandomWalk walk(dim, 42);
    walk.run(steps);

    const auto& path = walk.trajectory();
    std::cout << "Steps: " << steps << "\n";
    std::cout << "MSD: " << Statistics::MSD(path) << "\n";
    std::cout << "End-to-end distance: " << Statistics::EED(path) << "\n";

    // Save trajectory
    std::ofstream out(output);
    out << "step,x,y,z\n";
    for (std::size_t i = 0; i < path.size(); ++i) {
        out << i << "," << path[i].x << "," << path[i].y << "," << path[i].z << "\n";
    }
    std::cout << "Trajectory saved to " << output << "\n";

    return 0;
}
