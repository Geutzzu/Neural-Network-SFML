#include "Dropdown.h"


Dropdown::Dropdown() : ColorButton() {
	this->isOpen = false;
	this->selectedButton = nullptr;
}


void Dropdown::initializeVariables(float x, float y, float width, float height, sf::Color color, sf::Color hoverColor, sf::Color displayColor, const vector<Button*>& buttons) {
    this->buttons = buttons;
    this->isOpen = false;

    float buttonY = y + height;
    for (Button* button : buttons) {
        button->setPosition(x, buttonY);
        buttonY += button->getSize().y;
    }
}

Dropdown::Dropdown(float x, float y, float width, float height, sf::Color color, sf::Color hoverColor, sf::Color displayColor, const vector<Button*>& buttons, const sf::Font& font, const string& text)
    : ColorButton(x, y, width, height, color, hoverColor, displayColor, [this, buttons]() { 
    this->isOpen = !this->isOpen; 
    }, font, text) {
    initializeVariables(x, y, width, height, color, hoverColor, displayColor, buttons);
    this->selectedButton = nullptr;
}

Dropdown::Dropdown(const Dropdown& dropdown)
    : ColorButton(dropdown) {
    initializeVariables(dropdown.getPosition().x, dropdown.getPosition().y, dropdown.getSize().x, dropdown.getSize().y, dropdown.normalColor, dropdown.hoverColor, dropdown.displayColor, dropdown.buttons);
	if (dropdown.selectedButton != nullptr) {
		this->selectedButton = new Button(*dropdown.selectedButton);
	}
    else {
		this->selectedButton = nullptr;
	}
}

Dropdown& Dropdown::operator=(const Dropdown& dropdown) {
    if (this != &dropdown) {
        ColorButton::operator=(dropdown);
        initializeVariables(dropdown.getPosition().x, dropdown.getPosition().y, dropdown.getSize().x, dropdown.getSize().y, dropdown.normalColor, dropdown.hoverColor, dropdown.displayColor, dropdown.buttons);
		if (dropdown.selectedButton != nullptr) {
			this->selectedButton = new Button(*dropdown.selectedButton);
		}
		else {
			this->selectedButton = nullptr;
		}
    }
    return *this;
}

void Dropdown::addOption(Button* button) {
    this->buttons.push_back(button);
}

void Dropdown::checkEvents(sf::RenderWindow& window, sf::Event& event) {
    Button::checkEvents(window, event);
    if (isOpen) {
        for (Button* button : buttons) {
            button->checkEvents(window, event);
            /// if the button was clicked we set the selected button to the clicked button
            if (button->getIsClicked(window, event)) {

                /// check the type of the button
                /// if it is a color button we create a new color button
                /// if it is a circle button we create a new circle button
                ColorButton* colorButton = dynamic_cast<ColorButton*>(button);
                CircleButton* circleButton = dynamic_cast<CircleButton*>(button);
				Button* auxButton = dynamic_cast<Button*>(button);
                Button* aux = nullptr;

                if (colorButton != nullptr) { /// you can do a dropdown with any button descendant
                    aux = new ColorButton(*colorButton);
                }
                else if (circleButton != nullptr) {
                    aux = new CircleButton(*circleButton);
                }
                else if (auxButton != nullptr) {
					aux = new Button(*auxButton);
                }

                /// if there is a selected button we delete it
                if (selectedButton != nullptr) {
                    delete selectedButton;
                }
                this->selectedButton = aux;

                isOpen = false;
            }
        }
    }
    if (this->selectedButton != nullptr) {
        this->selectedButton->checkHover(window);
    }
}

void Dropdown::draw(sf::RenderWindow& window) {
	if (selectedButton == nullptr) {
        Button::draw(window);
	}
    if (isOpen) {
        for (Button* button : buttons) {
            button->draw(window);
        }
    }
    if (selectedButton != nullptr) {
        this->selectedButton->setPosition(this->getPosition().x, this->getPosition().y);
        this->selectedButton->draw(window);
    }
	    
}

bool Dropdown::getIsOpen() const {
    return isOpen;
}

void Dropdown::setIsOpen(bool isOpen) {
    this->isOpen = isOpen;
}

Dropdown::~Dropdown() {
    for (Button* button : buttons) {
        delete button;
    }
	if (selectedButton != nullptr) {
		delete selectedButton;
	}
}