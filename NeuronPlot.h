#pragma once

#include "Layer.h"
#include "NeuralNetwork.h"


class NeuronPlot {
private:
	NeuralNetwork& network; /// reference to the neural network
	const double& value; /// reference to the value of the neuron
	int index; /// the index of the neuron
	int layerIndex; /// the index of the layer
	sf::Vector2f position;  /// position of the neuron
	sf::RectangleShape plotContainer; /// the container of the neuron plot
	sf::VertexArray pixels; /// the pixels of the plot
	int pixelSize; /// the size of the pixel

	/// precompute the positions of the pixels
	static std::vector<sf::Vector2f> pixelPositions; // static vector for positions
	static bool positionsComputed; /// if the positions are computed
	static void computePixelPositions();


public:

	NeuronPlot() = delete; /// we don't want to have a default constructor due to the references
	NeuronPlot(Vector2f position, const double& value, NeuralNetwork& neuralNetwork, int index, int layerIndex);
	NeuronPlot(const NeuronPlot& neuronPlot);
	NeuronPlot& operator=(NeuronPlot& neuronPlot) = delete; /// we don't want to have the assignment operator becouse we have references



	void visualizePlot(sf::RenderWindow& window, const vector<Color>& colors, const ActivationType& activation); /// we visualize the plot
	void draw(sf::RenderWindow& window, const vector<Color>& colors); /// to remember that the draw function for this class has a different signature



};