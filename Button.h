#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>
#include <string>
#include <functional>


using namespace std;
using namespace sf;

class Button : public sf::RectangleShape { /// este responsabil si pentru chipsuri si pentru butoane
protected:
    sf::Color normalColor;
    sf::Color hoverColor;
    function<void()> onClick;
    sf::Text buttonText;
	bool visible = false;

public:
    Button() = default;

    Button(float x, float y, float width, float height, sf::Color color, sf::Color hoverColor, function<void()> onClickFunction, const sf::Font& font, const string& text);

    void checkEvents(sf::RenderWindow& window, Event& event);

    void setOnClick(function<void()> onClickFunction);


    void draw(RenderWindow& window);

    void setVisible(bool visible) { this->visible = visible;  }
	bool getVisible() const { return visible; }


    function<void()> getOnClickFunction() const { return onClick; }

};

