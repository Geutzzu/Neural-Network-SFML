#pragma once



#include "CircleButton.h"
#include "ColorButton.h"
#include <vector>
#include "TextureManager.h"

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif


class Dropdown : public ColorButton {
private:
    Button* selectedButton;
	int selectedIndex;
    vector<Button*> buttons;
    bool reverse;
    bool isOpen;

public:

	Dropdown();
    Dropdown(float x, float y, float width, float height, sf::Color color, sf::Color hoverColor, sf::Color displayColor, const vector<Button*>& buttons, const sf::Font& font, const string& text, bool reverse);
    Dropdown(float x, float y, float width, float height, sf::Color color, sf::Color hoverColor, sf::Color displayColor, const vector<Button*>& buttons, const sf::Font& font, const string& text);
    Dropdown(const Dropdown& dropdown);
    Dropdown& operator=(const Dropdown& dropdown);

    void initializeVariables(float x, float y, float width, float height, sf::Color color, sf::Color hoverColor, sf::Color displayColor, const vector<Button*>& buttons);


    void setPositions();
    void addOption(Button* button);
	void removeOption(int index);



    void checkEvents(sf::RenderWindow& window, sf::Event& event) override;
    void draw(sf::RenderWindow& window) override;
    bool getIsOpen() const;
    void setIsOpen(bool isOpen);

	void setButtons(const vector<Button*>& buttons);

	int getNumButtons() const;

    ~Dropdown();
};