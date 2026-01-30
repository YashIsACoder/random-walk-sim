#include "Renderer.hpp"
#include <algorithm>
#include <cstdint>
#include <cmath>

Renderer::Renderer(sf::RenderWindow& window)
    : window_(window),
      trailBuffer_(sf::PrimitiveType::LineStrip)
{
    trailBuffer_.setUsage(sf::VertexBuffer::Usage::Dynamic);

    head_.setRadius(4.f);
    head_.setOrigin({4.f, 4.f});
    head_.setFillColor(sf::Color::Red);
}

void Renderer::setConfig(const RenderConfig& cfg) {
    cfg_ = cfg;
    head_.setFillColor(cfg_.headColor);
}


static Position rotatePoint(const Position& p, float yaw, float pitch) {
    // Convert to float
    float x = static_cast<float>(p.x);
    float y = static_cast<float>(p.y);
    float z = static_cast<float>(p.z);

    // Rotate around Y (yaw)
    float cosY = std::cos(yaw);
    float sinY = std::sin(yaw);
    float x1 =  cosY * x + sinY * z;
    float z1 = -sinY * x + cosY * z;

    // Rotate around X (pitch)
    float cosP = std::cos(pitch);
    float sinP = std::sin(pitch);
    float y2 =  cosP * y - sinP * z1;
    float z2 =  sinP * y + cosP * z1;

    return {
        static_cast<int>(x1),
        static_cast<int>(y2),
        static_cast<int>(z2)
    };
}

sf::Vector2f Renderer::projectToScreen(const Position& p) const {
    const auto size = window_.getSize();

    Position r = rotatePoint(p, cfg_.yaw, cfg_.pitch);

    float x = static_cast<float>(r.x);
    float y = static_cast<float>(r.y);
    float z = static_cast<float>(r.z);

    float depth = cfg_.cameraDistance + z;
    depth = std::max(depth, 1.0f);

    float x_proj = x / depth;
    float y_proj = y / depth;

    return {
        size.x * 0.5f + x_proj * cfg_.scale * cfg_.cameraDistance,
        size.y * 0.5f - y_proj * cfg_.scale * cfg_.cameraDistance
    };
}


void Renderer::updateVertexBuffer(const std::vector<Position>& path) {
    if (path.empty()) return;

    std::size_t count = std::min(cfg_.maxTrail, path.size());
    std::size_t start = path.size() - count;

    std::vector<sf::Vertex> vertices(count);

    for (std::size_t i = 0; i < count; ++i) {
        const auto& p = path[start + i];
        vertices[i].position = projectToScreen(p);

        float alpha = 50.f + 205.f * (static_cast<float>(i) / count);
        vertices[i].color = sf::Color(
            cfg_.trailColor.r,
            cfg_.trailColor.g,
            cfg_.trailColor.b,
            static_cast<std::uint8_t>(alpha)
        );
    }

    if (!trailBuffer_.create(vertices.size()))
        return;

    trailBuffer_.update(vertices.data());
}


void Renderer::render(const RandomWalk& sim) {
    if (cfg_.showAxes) {
        drawAxes();
    }

    const auto& path = sim.trajectory();
    if (path.empty()) return;

    updateVertexBuffer(path);
    window_.draw(trailBuffer_);

    head_.setPosition(projectToScreen(path.back()));
    window_.draw(head_);
}

void Renderer::drawAxes() {
    constexpr int L = 20;

    struct Axis {
        Position start;
        Position end;
        sf::Color color;
    };

    std::array<Axis, 3> axes = {{
        {{0, 0, 0}, {L, 0, 0}, sf::Color::Red},   // X
        {{0, 0, 0}, {0, L, 0}, sf::Color::Green}, // Y
        {{0, 0, 0}, {0, 0, L}, sf::Color::Blue}   // Z
    }};

    for (const auto& a : axes) {
        sf::Vertex line[2];
        line[0].position = projectToScreen(a.start);
        line[1].position = projectToScreen(a.end);
        line[0].color = line[1].color = a.color;
        window_.draw(line, 2, sf::PrimitiveType::Lines);
    }
}

sf::Vector2f Renderer::project(const Position& p) const {
    return projectToScreen(p);  // calls existing private function
}
