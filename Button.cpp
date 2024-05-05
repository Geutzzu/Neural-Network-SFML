
#include <Button.h>
#include <iostream>



Button::Button(float x, float y, float width, float height, Color color, Color hoverColor, function<void()> onClickFunction, const Font& font, const string& text)
    : RectangleShape(Vector2f(width, height)), normalColor(color), hoverColor(hoverColor), onClick(onClickFunction) {
    this->setPosition(x, y);
    this->setFillColor(normalColor);

    buttonText.setFont(font);
    buttonText.setString(text);
    buttonText.setCharacterSize(24);
    buttonText.setFillColor(Color::Black);
    buttonText.setPosition(x, y);

    FloatRect textRect = buttonText.getLocalBounds();
    buttonText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    buttonText.setPosition(Vector2f(x + width / 2.0f, y + height / 2.0f));
}

Button::Button(const Button& button) : RectangleShape(button), normalColor(button.normalColor), hoverColor(button.hoverColor), onClick(button.onClick) {
	buttonText = button.buttonText;
}

Button& Button::operator=(const Button& button) {
	if (this != &button) {
		RectangleShape::operator=(button);
		normalColor = button.normalColor;
		hoverColor = button.hoverColor;
		onClick = button.onClick;
		buttonText = button.buttonText;
	}
	return *this;
}

void Button::checkEvents(RenderWindow& window, Event& event) {
    Vector2i mousePos = Mouse::getPosition(window);

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