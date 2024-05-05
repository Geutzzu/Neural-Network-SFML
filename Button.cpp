
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

void Button::setOnClick(function<void()> onClickFunction) {
    onClick = onClickFunction;
}

void Button::draw(RenderWindow& window) {
    window.draw(static_cast<RectangleShape&>(*this));
    window.draw(buttonText);
	this->visible = true;
}
