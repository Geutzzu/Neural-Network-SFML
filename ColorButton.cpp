#include "ColorButton.h"




ColorButton::ColorButton(float x, float y, float width, float height, sf::Color color, sf::Color hoverColor, sf::Color displayColor, function<void()> onClickFunction, const sf::Font& font, const string& text)
	: Button(x, y, width, height, color, hoverColor, onClickFunction, font, text), displayColor(displayColor) {
	float padding = 10.0f;  // Adjust as needed
	colorDisplay.setSize(sf::Vector2f(width - 2 * padding, height - 2 * padding));
	colorDisplay.setPosition(x + padding, y + padding);
	colorDisplay.setFillColor(displayColor);
}

ColorButton::ColorButton(const ColorButton& button) : Button(button), displayColor(button.displayColor) {
	this->colorDisplay = button.colorDisplay;

}

ColorButton& ColorButton::operator=(const ColorButton& button) {
	if (this != &button) {
		Button::operator=(button);
		displayColor = button.displayColor;
		colorDisplay = button.colorDisplay;
	}
	return *this;
}

void ColorButton::draw(sf::RenderWindow& window) {
    Button::draw(window);
    window.draw(colorDisplay);
}


void ColorButton::setPosition(float x, float y) {
	Button::setPosition(x, y);
	colorDisplay.setPosition(x + 10.0f, y + 10.0f);
}