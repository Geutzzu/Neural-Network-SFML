#pragma once


#include "Button.h"

class ColorButton : public Button {
protected:
    sf::RectangleShape colorDisplay;
    sf::Color displayColor;

public:

	ColorButton() = default;
    ColorButton(float x, float y, float width, float height, sf::Color color, sf::Color hoverColor, sf::Color displayColor, function<void()> onClickFunction, const sf::Font& font, const string& text);

	ColorButton(const ColorButton& button);
	ColorButton& operator=(const ColorButton& button);

	void setPosition(float x, float y) override;

    void draw(sf::RenderWindow& window) override;
};