
#include "Neuron.h"
#include <sstream>
#include <iomanip>

void Neuron::initializeValues(Vector2f position) {
	this->textureManager = TextureManager::getInstance();
	this->position = position;

	this->circle.setRadius(40);
	this->circle.setOutlineThickness(2);
	this->circle.setOutlineColor(Color::White);
	this->circle.setPosition(position);

	this->font = textureManager->getFont("roboto");



	this->text.setFont(this->font);
	stringstream ss;
	ss << fixed << setprecision(2) << value;
	string valueStr = ss.str();

	this->text.setString(valueStr);
	FloatRect textRect = text.getLocalBounds();
	this->text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

	this->text.setPosition(this->position.x + circle.getRadius(), this->position.y + circle.getRadius());
}

Neuron::Neuron(Vector2f position, const double& value) : value(value) {

	this->initializeValues(position);
}

Neuron::Neuron(const Neuron& neuron) : value(neuron.value) {
	this->initializeValues(neuron.position);
}

Neuron& Neuron::operator=(Neuron& neuron) {

	if (this != &neuron) {
		this->initializeValues(neuron.position);
	}
	return *this;
}


void Neuron::draw(sf::RenderWindow& window, double minimum, double maximum) {

	// Normalize the value to the range [0, 1]
	double normalizedValue = (value - minimum) / (maximum - minimum);
	normalizedValue = std::max(0.0, std::min(1.0, normalizedValue)); // Clamp to [0, 1]

	// Interpolate the color based on the normalized value
	sf::Uint8 colorValue = static_cast<sf::Uint8>(255 * normalizedValue);
	circle.setFillColor(sf::Color(colorValue, colorValue, colorValue));

	if (normalizedValue > 0.5) {
		this->text.setFillColor(sf::Color::Black);
	}
	else {
		this->text.setFillColor(sf::Color::White);
	}

	stringstream ss;
	ss << fixed << setprecision(2) << value;
	string value = ss.str();
	this->text.setString(value);

	window.draw(this->circle);

	window.draw(this->text);
}





