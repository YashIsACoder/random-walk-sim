#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "RandomWalk.hpp"

struct RenderConfig {
    float scale = 5.0f;
    float cameraDistance = 60.f;

    // Camera rotation (radians)
    float yaw   = 0.0f;   // Y-axis
    float pitch = 0.0f;   // X-axis

    bool showAxes = true;

    sf::Color trailColor = sf::Color::Green;
    sf::Color headColor  = sf::Color::Red;
    std::size_t maxTrail = 10'000;
};


class Renderer {
public:
    explicit Renderer(sf::RenderWindow& window);
    // Public method: project world coordinates to screen coordinates
    sf::Vector2f project(const Position& p) const;

    void setConfig(const RenderConfig& cfg);
    void render(const RandomWalk& sim);

private:
    sf::Vector2f projectToScreen(const Position& p) const;
    void updateVertexBuffer(const std::vector<Position>& path);
    void drawAxes();

    sf::RenderWindow& window_;
    RenderConfig cfg_;

    sf::VertexBuffer trailBuffer_;
    sf::CircleShape head_;
};
