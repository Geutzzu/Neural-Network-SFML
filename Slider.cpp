#include "Slider.h"
#include <iostream>
#include <algorithm>



Slider::Slider(float x, float y, float width, float height, double& value, std::string name) : value(value) // initialize the reference
{
    this->name = name;

    track.setPosition(x, y);
    track.setSize(sf::Vector2f(width, height));
    track.setFillColor(sf::Color::White);

    handle.setSize(sf::Vector2f(20, height));
    handle.setFillColor(sf::Color::Red);
    handle.setPosition(x, y);

    if (!font.loadFromFile("Resources/Roboto-Black.ttf")) { // replace with path to your font file
        std::cout << "Error loading font\n";
    }
    std::cout << "Font loaded\n";
    text.setFont(font);
    text.setCharacterSize(24); // in pixels
    text.setFillColor(sf::Color::White);
    text.setPosition(x, y - text.getCharacterSize() - 5);

    isDragging = false;
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
            text.setString(name + ":" + std::to_string(value));
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
