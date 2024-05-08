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
	bool visible = false;
	TextureManager* textureManager;


public:
    Button() = default;

	void init(float x, float y, float width, float height, sf::Color color, sf::Color hoverColor, function<void()> onClickFunction, const sf::Font& font, const string& text);
	void initText();

    Button(float x, float y, float width, float height, sf::Color color, sf::Color hoverColor, function<void()> onClickFunction, const sf::Font& font, const string& text);

	Button(const Button& button);
	Button& operator=(const Button& button);

    virtual void checkEvents(sf::RenderWindow& window, Event& event);
	virtual void checkEvents(sf::RenderWindow& window);
    virtual void draw(RenderWindow& window);
    virtual void checkHover(RenderWindow& window);

    virtual void setOnClick(function<void()> onClickFunction);
	virtual void setPosition(float x, float y) { RectangleShape::setPosition(x, y); this->initText(); }
	
    

    void setVisible(bool visible) { this->visible = visible;  }
	bool getVisible() const { return visible; }
    bool getIsHovered(RenderWindow& window) const;
	void setFillColorDropdown(sf::Color color) { normalColor = color; RectangleShape::setFillColor(color); }
	bool getIsClicked(RenderWindow& window, Event& event) const;

    function<void()> getOnClickFunction() const { return onClick; }

};

