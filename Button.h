#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>
#include <string>
#include <functional>
#include "TextureManager.h"


using namespace std;
using namespace sf;

class Button : public sf::RectangleShape { 
protected:
    sf::Color normalColor;
    sf::Color hoverColor;
    function<void()> onClick;
	sf::Font font;
    sf::Text buttonText;
	bool visible;
	TextureManager* textureManager;
	bool wasPressed;


public:
    Button() = default;

	void init(int x, int y, float width, float height, sf::Color color, sf::Color hoverColor, function<void()> onClickFunction, const sf::Font& font, const string& text);
	void initText();

    Button(float x, float y, float width, float height, sf::Color color, sf::Color hoverColor, function<void()> onClickFunction, const sf::Font& font, const string& text);

	Button(const Button& button);
	Button& operator=(const Button& button);

	virtual Button* clone() const { return new Button(*this); }

    virtual void checkEvents(sf::RenderWindow& window, Event& event);
	virtual void checkEvents(sf::RenderWindow& window);
    virtual void draw(RenderWindow& window);
    virtual void checkHover(RenderWindow& window);

    virtual void setOnClick(function<void()> onClickFunction);
	virtual void setPosition(float x, float y) { RectangleShape::setPosition(x, y); this->initText(); }
	
    

    void setVisible(bool visible) { this->visible = visible;  }
	bool getVisible() const { return this->visible; }
    bool getIsHovered(RenderWindow& window) const;
	void setFillColorDropdown(sf::Color color) { this->normalColor = color; RectangleShape::setFillColor(color); }
	bool getIsClicked(RenderWindow& window, Event& event) const;
	bool getWasPressed() const { return this->wasPressed; }

    function<void()> getOnClickFunction() const { return onClick; }

	virtual ~Button() {};

};

