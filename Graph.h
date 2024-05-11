#pragma once


#include <SFML/Graphics.hpp>
#include <list>
#include <vector>

class Graph {

private:
    sf::Vector2f position;
    sf::Vector2f size;
    ///std::list<sf::Vertex> vertices; /// works withoud lod and it is ineficient
	std::vector<sf::Vertex> vertices;
    double maxValue;

public:
	Graph() = default;
    Graph(float x, float y, float width, float height);
    Graph(const Graph& other);
    Graph& operator=(const Graph& other);
    void update(const double& value);
    void draw(sf::RenderWindow& window);
	void reset();

};