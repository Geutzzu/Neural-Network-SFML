
#include "Button.h"
#include <iostream>


void Button::initText() {

	FloatRect textRect = this->buttonText.getLocalBounds();
	this->buttonText.setOrigin(int(textRect.left + textRect.width / 2.0f), int(textRect.top + textRect.height / 2.0f));
	this->buttonText.setPosition(int(this->getPosition().x + this->getSize().x / 2.0f), int(this->getPosition().y + this->getSize().y / 2.0f));

	/// fixing text pixelation


}


void Button::init(int x, int y, float width, float height, Color color, Color hoverColor, function<void()> onClickFunction, const Font& font, const string& text) {
	this->setPosition(x, y);
	this->setSize(Vector2f(width, height));
	this->setFillColor(color);
	this->normalColor = color;
	this->hoverColor = hoverColor;
	this->onClick = onClickFunction;
	this->textureManager = TextureManager::getInstance();

	this->buttonText.setPosition(int(x), int(y));
	this->font = font;
	this->buttonText.setFont(font);
	this->buttonText.setString(text);
	this->buttonText.setCharacterSize(20);
	this->buttonText.setFillColor(Color::Black);
	

	this->initText();
}


Button::Button(float x, float y, float width, float height, Color color, Color hoverColor, function<void()> onClickFunction, const Font& font, const string& text)
    : RectangleShape(Vector2f(width, height)), normalColor(color), hoverColor(hoverColor), onClick(onClickFunction) {
	this->init(x, y, width, height, color, hoverColor, onClickFunction, font, text);
}

Button::Button(const Button& button) : RectangleShape(button), normalColor(button.normalColor), hoverColor(button.hoverColor), onClick(button.onClick) {
	this->buttonText = button.buttonText;
	this->textureManager = TextureManager::getInstance();
	this->font = textureManager->getFont("roboto");
	this->buttonText.setFont(this->font);
	this->buttonText.setString(button.buttonText.getString());
	this->buttonText.setCharacterSize(20);
	this->buttonText.setFillColor(Color::Black);
	this->initText();

}

Button& Button::operator=(const Button& button) {
	if (this != &button) {
		RectangleShape::operator=(button);
		this->normalColor = button.normalColor;
		this->hoverColor = button.hoverColor;
		this->onClick = button.onClick;
		this->textureManager = TextureManager::getInstance();
		this->font = textureManager->getFont("roboto");
		this->buttonText.setFont(this->font);
		this->buttonText.setString(button.buttonText.getString());
		this->buttonText.setCharacterSize(20);
		this->buttonText.setFillColor(Color::Black);

		this->initText();
	}
	return *this;
}

void Button::checkEvents(RenderWindow& window, Event& event) {
    Vector2i mousePos = Vector2i(event.mouseButton.x, event.mouseButton.y);


    if (this->getGlobalBounds().contains(mousePos.x, mousePos.y)) {
        this->setFillColor(hoverColor);
        if (event.type == Event::MouseButtonReleased) {
            onClick();
        }
    }
    else {
        this->setFillColor(normalColor);
    }
	
}

void Button::checkEvents(RenderWindow& window) {
	Vector2i mousePos = Mouse::getPosition(window);

	if (this->getGlobalBounds().contains(mousePos.x, mousePos.y)) {
		this->setFillColor(hoverColor);
		if (Mouse::isButtonPressed(Mouse::Left)) {
			onClick();
		}
	}
	else {
		this->setFillColor(normalColor);
	}
}



void Button::checkHover(RenderWindow& window) {
    Vector2i mousePos = Mouse::getPosition(window);

    if (this->getGlobalBounds().contains(mousePos.x, mousePos.y)) {
        this->setFillColor(hoverColor);
	}
	else {
		this->setFillColor(normalColor);

	}

}

void Button::setOnClick(function<void()> onClickFunction) {
    onClick = onClickFunction;
}

void Button::draw(RenderWindow& window) {
    window.draw(static_cast<RectangleShape&>(*this));
    window.draw(buttonText);
	this->visible = true;
}


bool Button::getIsHovered(RenderWindow& window) const {
	Vector2i mousePos = Mouse::getPosition(window);
	return this->getGlobalBounds().contains(mousePos.x, mousePos.y);
}

bool Button::getIsClicked(RenderWindow& window, Event& event) const {
	Vector2i mousePos = Mouse::getPosition(window);
	return this->getGlobalBounds().contains(mousePos.x, mousePos.y) && event.type == Event::MouseButtonReleased;
}