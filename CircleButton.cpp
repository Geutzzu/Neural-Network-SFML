
#include "CircleButton.h"



CircleButton::CircleButton(float x, float y, float radius, sf::Color color, sf::Color hoverColor, function<void()> onClickFunction, const sf::Font& font, const string& text)
    : Button(x, y, 2 * radius, 2 * radius, color, hoverColor, onClickFunction, font, text) {
    circleShape.setRadius(radius);
    circleShape.setPosition(x, y);
    circleShape.setFillColor(normalColor);
}

CircleButton::CircleButton(const CircleButton& button) : Button(button), circleShape(button.circleShape) {
}

CircleButton& CircleButton::operator=(const CircleButton& button) {
	if (this != &button) {
		Button::operator=(button);
		circleShape = button.circleShape;
	}
	return *this;
}

void CircleButton::draw(sf::RenderWindow& window) {
    if (visible) {
        window.draw(circleShape);
        window.draw(buttonText);
    }
}

void CircleButton::checkEvents(sf::RenderWindow& window, sf::Event& event) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    if (circleShape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        circleShape.setFillColor(hoverColor);
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            onClick();
        }
    }
    else {
        circleShape.setFillColor(normalColor);
    }
}