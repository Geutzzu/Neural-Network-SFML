#pragma once

#include "CircleButton.h"
#include "ColorButton.h"
#include <vector>
#include "TextureManager.h"

class Dropdown : public ColorButton {
private:
    Button* selectedButton;
    std::vector<Button*> buttons;
    bool isOpen;

public:

	Dropdown();
    Dropdown(float x, float y, float width, float height, sf::Color color, sf::Color hoverColor, sf::Color displayColor, const vector<Button*>& buttons, const sf::Font& font, const string& text);
    Dropdown(const Dropdown& dropdown);
    Dropdown& operator=(const Dropdown& dropdown);

    void initializeVariables(float x, float y, float width, float height, sf::Color color, sf::Color hoverColor, sf::Color displayColor, const vector<Button*>& buttons);

    void addOption(Button* button);
    void checkEvents(sf::RenderWindow& window, sf::Event& event) override;
    void draw(sf::RenderWindow& window) override;
    bool getIsOpen() const;
    void setIsOpen(bool isOpen);

    ~Dropdown();
};