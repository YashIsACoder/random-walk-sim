// src/main.cpp
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <vector>
#include <iostream>

#include "RandomWalk.hpp"
#include "Renderer.hpp"
#include "Statistics.hpp"

namespace fs = std::filesystem;

/* ---------- utilities ---------- */

static std::string timestamp_now() {
    using namespace std::chrono;
    auto t = system_clock::to_time_t(system_clock::now());
    std::tm tm{};
#if defined(_WIN32)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    std::ostringstream ss;
    ss << std::put_time(&tm, "%Y%m%d_%H%M%S");
    return ss.str();
}

static void save_csv(const std::vector<Position>& path) {
    fs::create_directories("data");
    std::ofstream out("data/walk_" + timestamp_now() + ".csv");
    out << "step,x,y,z\n";
    for (std::size_t i = 0; i < path.size(); ++i) {
        out << i << "," << path[i].x << "," << path[i].y << "," << path[i].z << "\n";
    }
    out.close();
}

// Try several common font paths (macOS, Linux, Windows). Returns true if font opened.
static bool try_load_font(sf::Font &font) {
    std::vector<fs::path> candidates = {
        // macOS (common)
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/System/Library/Fonts/Arial.ttf",
        // Linux (common)
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
        // Windows
        "C:\\Windows\\Fonts\\arial.ttf",
        // local repo fallback
        "fonts/DejaVuSans.ttf",
        "fonts/arial.ttf"
    };

    for (const auto &p : candidates) {
        std::error_code ec;
        if (p.empty()) continue;
        if (!fs::exists(p, ec)) continue;
        if (font.openFromFile(p)) {
            std::cout << "Loaded font from: " << p << "\n";
            return true;
        }
    }
    return false;
}

/* ---------- main ---------- */

int main() {
    // Window
    sf::RenderWindow window(
        sf::VideoMode(sf::Vector2u{1100, 800}),
        "Random Walk Visualizer (SFML 3)"
    );
    window.setFramerateLimit(60);

    // Font
    sf::Font font;
    if (!try_load_font(font)) {
        std::cerr << "Could not find a system font. Place a TTF in ./fonts/ (e.g. DejaVuSans.ttf)\n";
        return 1;
    }

    // Simulation defaults
    Dimension dim = Dimension::TwoD;
    StepType stepType = StepType::Lattice;
    unsigned int seed = 42;

    RandomWalk sim(dim, stepType, seed);

    // Renderer (vertex-buffer based)
    Renderer renderer(window);
    RenderConfig cfg;
    cfg.scale = 5.0f;
    cfg.trailColor = sf::Color::Green;
    cfg.headColor = sf::Color::Red;
    cfg.maxTrail = 10'000;
    renderer.setConfig(cfg);

    // State
    bool running = false;
    double speed = 600.0; // steps per second
    double accumulator = 0.0;

    sf::Clock frameClock;

    // Main loop
    while (window.isOpen()) {
        // SFML 3: pollEvent returns std::optional<sf::Event>
        while (const auto eventOpt = window.pollEvent()) {
            const auto &ev = *eventOpt;

            // Window closed
            if (ev.is<sf::Event::Closed>()) {
                window.close();
                break;
            }

            // Key pressed
            if (ev.is<sf::Event::KeyPressed>()) {
                const auto *kp = ev.getIf<sf::Event::KeyPressed>();
                if (!kp) continue;
                using Key = sf::Keyboard::Key;

                switch (kp->code) {
                    case Key::Escape:
                        window.close();
                        break;

                    case Key::Space:
                        running = !running;
                        break;

                    case Key::R:
                        sim.reset();
                        break;

                    case Key::K:
                        save_csv(sim.trajectory());
                        break;

                    case Key::Equal: // '+' key (often Shift+'=')
                        speed *= 1.2;
                        break;

                    case Key::Hyphen: // '-' key
                        speed *= 0.8;
                        break;

                    case Key::Num1:
                        dim = Dimension::OneD;
                        sim.walker().setDimension(dim);
                        sim.reset();
                        break;

                    case Key::Num2:
                        dim = Dimension::TwoD;
                        sim.walker().setDimension(dim);
                        sim.reset();
                        break;

                    case Key::Num3:
                        dim = Dimension::ThreeD;
                        sim.walker().setDimension(dim);
                        sim.reset();
                        break;

                    case Key::L:
                        stepType = StepType::Lattice;
                        sim.walker().setStepType(stepType);
                        sim.reset();
                        break;

                    case Key::G:
                        stepType = StepType::Gaussian;
                        sim.walker().setStepType(stepType);
                        sim.reset();
                        break;

                    case Key::Q:
                        cfg.cameraDistance *= 1.1f;
                        renderer.setConfig(cfg);
                        break;

                    case Key::E:
                        cfg.cameraDistance *= 0.9f;
                        renderer.setConfig(cfg);
                        break;

                    case Key::A:
                        cfg.yaw -= 0.05f;
                        renderer.setConfig(cfg);
                        break;

                    case Key::D:
                        cfg.yaw += 0.05f;
                        renderer.setConfig(cfg);
                        break;

                    case Key::W:
                        cfg.pitch += 0.05f;
                        renderer.setConfig(cfg);
                        break;

                    case Key::S:
                        cfg.pitch -= 0.05f;
                        renderer.setConfig(cfg);
                        break;

                    case Key::M:
                        cfg.showAxes = !cfg.showAxes;
                        renderer.setConfig(cfg);
                        break;

                    default:
                        break;
                }
            }

            // Mouse wheel scroll (zoom)
            if (ev.is<sf::Event::MouseWheelScrolled>()) {
                const auto *mw = ev.getIf<sf::Event::MouseWheelScrolled>();
                if (mw) {
                    if (mw->wheel == sf::Mouse::Wheel::Vertical) {
                        cfg.scale *= (mw->delta > 0.f ? 1.08f : 0.92f);
                        cfg.scale = std::clamp(cfg.scale, 0.5f, 200.0f);
                        renderer.setConfig(cfg);
                    } else if (mw->wheel == sf::Mouse::Wheel::Horizontal) {
                        // could horizontally pan if needed
                        cfg.scale *= (mw->delta > 0.f ? 1.02f : 0.98f);
                        cfg.scale = std::clamp(cfg.scale, 0.5f, 200.0f);
                        renderer.setConfig(cfg);
                    }
                }
            }

            // Resize event — keep the window view centered when resized
            if (ev.is<sf::Event::Resized>()) {
                const auto* r = ev.getIf<sf::Event::Resized>();
                if (r) {
                    sf::View view = window.getDefaultView();
                    view.setSize(sf::Vector2f(static_cast<float>(r->size.x),
                                              static_cast<float>(r->size.y)));
                    window.setView(view);
                }
            }
        } // end event loop

        // Simulation stepping (accumulator pattern)
        double dt = frameClock.restart().asSeconds();
        if (running) {
            accumulator += dt;
            double stepTime = 1.0 / speed;
            int maxSteps = 100000;
            while (accumulator >= stepTime && maxSteps-- > 0) {
                sim.stepOnce();
                accumulator -= stepTime;
            }
        }

        // Trim for performance
        sim.trimTrail(cfg.maxTrail);

        // Draw
        window.clear(sf::Color(25, 25, 25));
        renderer.render(sim);

        if (cfg.showAxes && dim == Dimension::ThreeD) {
            sf::Vector2f projectedXEnd = renderer.project({20, 0, 0});
            sf::Vector2f projectedYEnd = renderer.project({0, 20, 0});
            sf::Vector2f projectedZEnd = renderer.project({0, 0, 20});

            // X label
            sf::Text xLabel(font);
            xLabel.setString("X");
            xLabel.setCharacterSize(14);
            xLabel.setFillColor(sf::Color::Red);
            xLabel.setPosition(projectedXEnd);

            // Y label
            sf::Text yLabel(font);
            yLabel.setString("Y");
            yLabel.setCharacterSize(14);
            yLabel.setFillColor(sf::Color::Green);
            yLabel.setPosition(projectedYEnd);

            // Z label
            sf::Text zLabel(font);
            zLabel.setString("Z");
            zLabel.setCharacterSize(14);
            zLabel.setFillColor(sf::Color::Blue);
            zLabel.setPosition(projectedZEnd);

            window.draw(xLabel);
            window.draw(yLabel);
            window.draw(zLabel);
        }


        // Draw HUD in screen coordinates
        std::ostringstream hudss;
        hudss << "Status: " << (running ? "Running" : "Paused") << "\n"
              << "Steps: " << sim.stepIndex() << "\n"
              << "Speed: " << static_cast<int>(speed) << " steps/s\n"
              << "MSD: " << std::fixed << std::setprecision(3)
              << Statistics::meanSquaredDisplacement(sim.trajectory()) << "\n"
              << "End dist: " << Statistics::endToEndDistance(sim.trajectory()) << "\n"
              << "Dim: " << static_cast<int>(dim)
              << " | Step: " << (stepType == StepType::Lattice ? "Lattice" : "Gaussian") << "\n\n"
              << "Keys: Space play/pause | R reset | K save CSV | +/- speed | 1/2/3 dim | L/G step | Q / E : camera zoom (3D)";

            sf::Text hud(font);
            hud.setString(hudss.str());
            hud.setCharacterSize(14);
            hud.setFillColor(sf::Color::White);
            hud.setPosition(sf::Vector2f{10.f, 10.f});
            window.draw(hud);

        window.display();
    }

    return 0;
}
