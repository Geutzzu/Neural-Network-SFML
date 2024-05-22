#include "Slider.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <sstream>


void Slider::initializeVariables(float x, float y, float width, float height, double& value, std::string name, double minValue, double maxValue) {
    this->value = value;
    this->name = name;
    this->minValue = minValue;
    this->maxValue = maxValue;
	this->previousValue = value;

    track.setPosition(x, y);
    track.setSize(sf::Vector2f(width, height));
    track.setFillColor(sf::Color::White);

    handle.setSize(sf::Vector2f(20, height + 10));
    sf::Color gray(128, 128, 128);
    handle.setFillColor(gray);

    // Calculate the initial position of the handle based on the value
    float handleX = x + ((value - minValue) / (maxValue - minValue)) * (track.getSize().x - handle.getSize().x);
    float handleY = y + (height - handle.getSize().y) / 2;
    handle.setPosition(handleX, handleY);

    this->font = TextureManager::getInstance()->getFont("roboto");
    text.setFont(font);
    text.setCharacterSize(22); // in pixels
    text.setFillColor(sf::Color::White);
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << value;
    text.setString(name + ": " + ss.str());

    // Calculate the center of the slider
    float sliderCenter = x + width / 2;

    // Calculate the position of the text
    float textX = sliderCenter - text.getLocalBounds().width / 2;
    float textY = y - text.getCharacterSize() - 10;
    text.setPosition(int(textX), int(textY));

    this->isDragging = false;
}


Slider::Slider(float x, float y, float width, float height, double& value, std::string name, double minValue, double maxValue) : value(value) {
    this->initializeVariables(x, y, width, height, value, name, minValue, maxValue);
}

Slider::Slider(const Slider& other) : value(value) {
    this->initializeVariables(other.track.getPosition().x, other.track.getPosition().y, other.track.getSize().x, other.track.getSize().y, other.value, other.name, other.minValue, other.maxValue);
}

/// does not attribute the reference - you cant do that with this operator
Slider& Slider::operator=(const Slider& other) {
    if (this != &other) {
        this->initializeVariables(other.track.getPosition().x, other.track.getPosition().y, other.track.getSize().x, other.track.getSize().y, other.value, other.name, other.minValue, other.maxValue);
    }
    return *this;
}




void Slider::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (handle.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
            isDragging = true;
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased) {
        isDragging = false;
    }
    else if (event.type == sf::Event::MouseMoved) {
        if (isDragging) {
            float newX = event.mouseMove.x - handle.getSize().x / 2;
            newX = std::max(newX, track.getPosition().x);
            newX = std::min(newX, track.getPosition().x + track.getSize().x - handle.getSize().x);
            handle.setPosition(newX, handle.getPosition().y);

			/// modify the value based on the position of the handle
            this->value = minValue + ((newX - track.getPosition().x) / (track.getSize().x - handle.getSize().x)) * (maxValue - minValue);

			if (this->previousValue != value) {
                
                std::stringstream ss;
                ss << std::fixed << std::setprecision(2) << value;
                text.setString(name + ": " + ss.str());

                // Update the position of the text
                float textX = track.getPosition().x + track.getSize().x / 2 - text.getLocalBounds().width / 2;
                text.setPosition(int(textX), int(text.getPosition().y));

				this->previousValue = value; /// so we update the text only when the value changes
            }

 
        }
    }
}

void Slider::setText(std::string str) {
    this->text.setString(str);
}

void Slider::draw(sf::RenderWindow& window) {
    window.draw(text);
    window.draw(track);
    window.draw(handle);
}
