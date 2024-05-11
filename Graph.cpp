#include "Graph.h"
#include <iostream>

Graph::Graph(float x, float y, float width, float height)
    : position(x, y), size(width, height), maxValue(1.0) {}

Graph::Graph(const Graph& other)
    : position(other.position), size(other.size), vertices(other.vertices), maxValue(other.maxValue) {}

Graph& Graph::operator=(const Graph& other) {
    if (this != &other) {
        position = other.position;
        size = other.size;
        vertices = other.vertices;
        maxValue = other.maxValue;
    }
    return *this;
}

void Graph::update(const double& value) {
    const size_t MAX_VERTICES = 1e6;

    if (vertices.size() >= MAX_VERTICES) {
        vertices.erase(vertices.begin());
    }

    float normalizedValue = size.y * (1.0f - (value / maxValue));
    try {
        vertices.push_back(sf::Vertex(sf::Vector2f(vertices.size(), position.y + normalizedValue), sf::Color::White));
    }
    catch (const std::bad_alloc& e) {
        std::cerr << "Memory allocation failed: " << e.what() << '\n';

    }
}

void Graph::draw(sf::RenderWindow& window) {
    sf::VertexArray va(sf::LinesStrip, vertices.size());
    float scaleX = size.x / static_cast<float>(vertices.size());
    int i = 0;
    for (const auto& vertex : vertices) {
        va[i] = vertex;
        va[i].position.x *= scaleX;
        va[i].position.x += position.x;
        ++i;
    }
    window.draw(va);
}

void Graph::reset() {
	vertices.clear();
}