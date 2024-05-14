#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "TextureManager.h"

class Slider {
private:
    sf::RectangleShape track;
    sf::RectangleShape handle;
    sf::Font font;
    sf::Text text;
    std::string name;
    double& value; // reference to a float
    bool isDragging;

public:

    Slider() = default;

    Slider(float x, float y, float width, float height, double& value, std::string name);
	Slider(const Slider& slider);
	Slider& operator=(const Slider& slider) = delete;


	void initializeVariables(float x, float y, float width, float height, double& value, std::string name);
    void handleEvent(const sf::Event& event);
    void setText(std::string str);
    void draw(sf::RenderWindow& window);
};