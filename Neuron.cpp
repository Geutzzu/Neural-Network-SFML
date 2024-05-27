
#include "Neuron.h"
#include <sstream>
#include <iomanip>

void Neuron::initializeValues(Vector2f position) {
	this->position = position;
	this->previousValue = value;
	this->previousNormalizedValue = -1.0;

	this->circle.setRadius(40);
	this->circle.setOutlineThickness(2);
	this->circle.setOutlineColor(Color::White);
	this->circle.setPosition(position);
	
	this->font = TextureManager::getInstance()->getFont("roboto");

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
	this->text = neuron.text;
	this->text.setFont(TextureManager::getInstance()->getFont("roboto"));
	this->circle = neuron.circle;
	this->position = neuron.position;
	this->previousValue = neuron.previousValue;
	this->previousNormalizedValue = neuron.previousNormalizedValue;
		
}

Neuron& Neuron::operator=(Neuron& neuron) {

	if (this != &neuron) {
		this->text = neuron.text;
		this->text.setFont(TextureManager::getInstance()->getFont("roboto"));
		this->circle = neuron.circle;
		this->position = neuron.position;
		this->previousValue = neuron.previousValue;
		this->previousNormalizedValue = neuron.previousNormalizedValue;
	}
	return *this;
}


void Neuron::draw(sf::RenderWindow& window, double minimum, double maximum) {


	/// Normalize the value to [0, 1]
	double normalizedValue = (value - minimum) / (maximum - minimum);
	normalizedValue = max(0.0, min(1.0, normalizedValue)); // Clamp to [0, 1]

	// Only update the color if the normalized value has changed
	if (normalizedValue != previousNormalizedValue) {
		// Interpolate the color based on the normalized value
		sf::Uint8 colorValue = static_cast<Uint8>(255 * normalizedValue);
		circle.setFillColor(Color(colorValue, colorValue, colorValue));

		if (normalizedValue > 0.5) {
			this->text.setFillColor(Color::Black);
		}
		else {
			this->text.setFillColor(Color::White);
		}

		previousNormalizedValue = normalizedValue; // Update the previous normalized value
	}
	
	if (value != previousValue) { /// so we dont update each frame
		stringstream ss;
		ss << fixed << setprecision(2) << value;
		string valueStr = ss.str();
		this->text.setString(valueStr);
		FloatRect textRect = text.getLocalBounds(); /// so we recenter the text
		this->text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f); /// so we recenter the text
		this->text.setPosition(this->position.x + circle.getRadius(), this->position.y + circle.getRadius()); /// so we recenter the text

		previousValue = value; 
	}
	
	window.draw(this->circle);

	window.draw(this->text);
}





