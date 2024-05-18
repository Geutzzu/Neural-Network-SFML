#include "Dropdown.h"
#include <iostream>

Dropdown::Dropdown() : ColorButton() {
	this->isOpen = false;
	this->selectedButton = nullptr;
	this->selectedIndex = -1;
	this->reverse = false;
	this->position = Vector2f(0, 0);
	this->width = 0;
	this->height = 0;
	this->displayColor = Color::Transparent;
	this->hoverColor = Color::Transparent;
	this->color = Color::Transparent;
}


Dropdown::Dropdown(float x, float y, float width, float height, sf::Color color, sf::Color hoverColor, sf::Color displayColor, const sf::Font& font, const string& text, bool reverse)
    : ColorButton(x, y, width, height, color, hoverColor, displayColor, [this]() {
    this->isOpen = !this->isOpen;
        }, font, text), reverse(reverse) {
    initializeVariables(x, y, width, height, color, hoverColor, displayColor);
    this->selectedButton = nullptr;
    this->selectedIndex = -1;
}

void Dropdown::initializeVariables(float x, float y, float width, float height, sf::Color color, sf::Color hoverColor, sf::Color displayColor) {

    this->isOpen = false;
	this->position = Vector2f(x, y);
	this->width = width;
	this->height = height;
	this->displayColor = displayColor;
	this->hoverColor = hoverColor;
	this->color = color;


    float buttonY;
    if (reverse) {
        buttonY = y + height;
    } else {
        buttonY = y - height;
    }

    for (Button* button : buttons) {
        button->setPosition(x, buttonY);
        if (reverse) {
            buttonY = buttonY + button->getSize().y;
        } else {
            buttonY = buttonY - button->getSize().y;
        }
    }
}



Dropdown::Dropdown(float x, float y, float width, float height, sf::Color color, sf::Color hoverColor, sf::Color displayColor, const sf::Font& font, const string& text)
    : ColorButton(x, y, width, height, color, hoverColor, displayColor, [this]() { 
    this->isOpen = !this->isOpen; 
    }, font, text) {
    initializeVariables(x, y, width, height, color, hoverColor, displayColor);
    this->selectedButton = nullptr;
	this->selectedIndex = -1;

}

Dropdown::Dropdown(const Dropdown& dropdown) : ColorButton(dropdown) { /// it does not work, I dont know why, I dont need it in my project anyways
	this->initializeVariables(dropdown.position.x, dropdown.position.y, dropdown.width, dropdown.height, dropdown.color, dropdown.hoverColor, dropdown.displayColor);

    /// delete the memory currently allocated

	if (this->selectedButton != nullptr) {
		delete this->selectedButton;
		this->selectedButton = nullptr; // Set the pointer to null after deletion
	}

    this->selectedIndex = -1;

	for (Button* button : buttons) {
		if (button != nullptr) {
			delete button;
			button = nullptr; // Set the pointer to null after deletion
		}
	}
    /// add new buttons via function
	for (Button* button : dropdown.buttons) {
		this->addOption(button->clone());
	}

    /*
    /// delete the memory currently allocated

    for (int i = 0; i < buttons.size(); i++) {
		this->removeOption(i);
    }

	if (this->selectedButton != nullptr) {
		delete this->selectedButton;
		this->selectedButton = nullptr; // Set the pointer to null after deletion
	}
	this->selectedIndex = -1;
    buttons.clear();
    /// add new buttons via function
	for (Button* button : dropdown.buttons) {
		this->addOption(button->clone());
	}*/
}

Dropdown& Dropdown::operator=(const Dropdown& dropdown) { /// nu merge bine si nu stiu sigur de ce

    if (this != &dropdown) {
		ColorButton::operator=(dropdown);
        this->initializeVariables(dropdown.position.x, dropdown.position.y, dropdown.width, dropdown.height, dropdown.color, dropdown.hoverColor, dropdown.displayColor);


        /// delete the memory currently allocated

        if (this->selectedButton != nullptr) {
            delete this->selectedButton;
            this->selectedButton = nullptr; // Set the pointer to null after deletion

        }
        this->selectedIndex = -1;

        for (Button* button : buttons) {
            if (button != nullptr) {
                delete button;
                button = nullptr; // Set the pointer to null after deletion
            }
        }
        /// add new buttons via function
        for (Button* button : dropdown.buttons) {
            this->addOption(button->clone());
        }
    }
	
    return *this;
}


void Dropdown::setPositions() {
    float x = this->getPosition().x;
    float y;

    if (reverse) {
        y = this->getPosition().y + this->getSize().y;
    }
    else {
        y = this->getPosition().y - this->getSize().y;
    }

    for (Button* button : buttons) {
        button->setPosition(x, y);

        if (reverse) {
            y += button->getSize().y;
        }
        else {
            y -= button->getSize().y;
        }
    }
}

void Dropdown::addOption(Button* button) {
    this->buttons.push_back(button);
	this->setPositions();
	/// this->selectedIndex += 1 ;
}

void Dropdown::removeOption(int index) {
    try {
		if (index < 0 || index >= buttons.size()) {
			throw out_of_range("Index out of range");
		}
		if (buttons.size() == 0) {
			throw out_of_range("No buttons to remove");
		}
        
		if (this->selectedButton != nullptr && this->selectedIndex == index) {
			delete this->selectedButton; /// !
			this->selectedButton = nullptr;
		}
        delete buttons[index]; /// !
        
        this->buttons.erase(buttons.begin() + index);
        this->setPositions();
        /// this->selectedIndex -= 1; /// becouse we remove the button at index

	}
    catch (const out_of_range& e) {
		cout << e.what() << endl;
    }
}

void Dropdown::checkEvents(sf::RenderWindow& window, sf::Event& event) {
    ColorButton::checkEventsDropdown(window, event);
    if (isOpen) {
		int cnt = 0;
        for (Button* button : buttons) {
            button->checkEvents(window);
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
				this->selectedIndex = cnt;
				

                isOpen = false;

            }
            cnt++;
        }
    }

    if (this->selectedButton != nullptr) {
        this->selectedButton->checkHover(window);
    }
}


void Dropdown::drawOutline(sf::RenderWindow& window) {
    sf::RectangleShape outlineRect;
    outlineRect.setPosition(ColorButton::getPosition());
    outlineRect.setSize(ColorButton::getSize());
    outlineRect.setOutlineColor(sf::Color::White);
    outlineRect.setOutlineThickness(3);
    outlineRect.setFillColor(sf::Color::Transparent);
    window.draw(outlineRect);
}

void Dropdown::draw(sf::RenderWindow& window) {
	if (selectedButton == nullptr) {
        ColorButton::draw(window);
	}
    
    if (selectedButton != nullptr) {
        this->selectedButton->setPosition(this->getPosition().x, this->getPosition().y);
        this->selectedButton->draw(window);
    }
    if (isOpen) {
        for (Button* button : buttons) {
            button->draw(window);
        }
        this->drawOutline(window);
    }
	
}

bool Dropdown::getIsOpen() const {
    return isOpen;
}

void Dropdown::setIsOpen(bool isOpen) {
    this->isOpen = isOpen;
}

void Dropdown::setButtons(const vector<Button*>& buttons) {
	this->buttons = buttons;
	this->setPositions();
}

int Dropdown::getNumButtons() const {
	return buttons.size();
}

Dropdown::~Dropdown() {
    for (Button* button : buttons) {
        if (button != nullptr) {
            delete button;
            button = nullptr; // Set the pointer to null after deletion
        }
    }
    if (selectedButton != nullptr) {
        delete selectedButton;
        selectedButton = nullptr; // Set the pointer to null after deletion
    }
}