#pragma once

#include "Button.h"

class CircleButton : public Button {
protected:
    sf::CircleShape circleShape;

public:
	CircleButton() = default;


    CircleButton(float x, float y, float radius, sf::Color color, sf::Color hoverColor, function<void()> onClickFunction, const sf::Font& font, const string& text);

	CircleButton(const CircleButton& button);
	CircleButton& operator=(const CircleButton& button);


    void draw(sf::RenderWindow& window) override;
	void checkEvents(sf::RenderWindow& window, sf::Event& event) override;
};