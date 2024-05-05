#include "Slider.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <sstream>


void Slider::initializeVariables(float x, float y, float width, float height, double& value, std::string name) {
    this->value = value;
    this->textureManager = TextureManager::getInstance();
    this->name = name;

    track.setPosition(x, y);
    track.setSize(sf::Vector2f(width, height));
    track.setFillColor(sf::Color::White);

    handle.setSize(sf::Vector2f(20, height + 10));
    sf::Color gray(128, 128, 128);
    handle.setFillColor(gray);
    // Calculate the center of the track and adjust for the size of the handle
    float handleX = x + (width - handle.getSize().x) / 2;
    float handleY = y + (height - handle.getSize().y) / 2;
    handle.setPosition(handleX, handleY);

    this->font = textureManager->getFont("roboto");
    text.setFont(font);
    text.setCharacterSize(24); // in pixels
    text.setFillColor(sf::Color::White);
    text.setPosition(x, y - text.getCharacterSize() - 10);

    isDragging = false;
}

Slider::Slider(float x, float y, float width, float height, double& value, std::string name) : value(value) {
    this->initializeVariables(x, y, width, height, value, name);
}

Slider::Slider(const Slider& other) : value(value) {
    this->initializeVariables(other.track.getPosition().x, other.track.getPosition().y, other.track.getSize().x, other.track.getSize().y, other.value, other.name);
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

            value = ((newX - track.getPosition().x) / (track.getSize().x - handle.getSize().x)) * 6 - 3;
            std::stringstream ss;
            ss << std::fixed << std::setprecision(2) << value;
            text.setString(name + ":" + ss.str());
        }
    }
}

void Slider::setText(std::string str) {
    this->text.setString(str);
}

void Slider::draw(sf::RenderWindow& window) {
    window.draw(track);
    window.draw(handle);
    window.draw(text);
}
