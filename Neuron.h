#pragma once

#include "Layer.h"
#include "TextureManager.h"

class Neuron {

private:
	sf::CircleShape circle;
	sf::Font font;
	sf::Text text;
	const double& value; // reference to a float
	TextureManager* textureManager;
	Vector2f position;
public:

	Neuron() = delete;
	Neuron(Vector2f, const double& value);
	Neuron(const Neuron& neuron);
	Neuron& operator=(Neuron& neuron);

	void initializeValues(Vector2f);

	void draw(sf::RenderWindow& window, double minimum, double maximum);

};


