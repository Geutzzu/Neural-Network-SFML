#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "TextureManager.h"

class Slider {
private:
    sf::RectangleShape track;
    sf::RectangleShape handle;
    sf::Text text;
    std::string name;
    double& value; // reference to the value of the slider
	double previousValue; /// so we dont update the text every frame
    double minValue; // minimum value of the slider
    double maxValue; // maximum value of the slider
    bool isDragging;

public:

    Slider() = default;

    Slider(float x, float y, float width, float height, double& value, std::string name, double minValue, double maxValue);
    Slider(const Slider& slider);
    Slider& operator=(const Slider& slider);

    void initializeVariables(float x, float y, float width, float height, double& value, std::string name, double minValue, double maxValue);
    void handleEvent(const sf::Event& event);
    void setText(std::string str);
    void draw(sf::RenderWindow& window);
};