#pragma once

#include "Layer.h"
#include "NeuronPlot.h"
#include "NeuralNetwork.h"

template <typename T>
class LayerVisualizer {
private:
    const Layer& layer;
	NeuralNetwork& network; /// we need this for the plot neuron - we must have access to this and we must pass it as a parameter so we can calculate outputs for each neuron
    std::vector<T> neurons; // can switch between different types of neurons (Neuron, VisualNeuron, etc.)
    sf::Vector2f position;

public:
	LayerVisualizer() = delete; /// we don't want to have a default constructor due to the references
	LayerVisualizer(const Layer& layer, sf::Vector2f position, NeuralNetwork& network); /// we initialize the layer and the position

	void drawNeurons(sf::RenderWindow& window, const vector<Color>& classColors, const ActivationType& activation, bool discretized); /// we draw the neurons
	void drawConnections(sf::RenderWindow& window, double minWeight, double maxWeight); /// we draw the connections
};

template <typename T>
LayerVisualizer<T>::LayerVisualizer(const Layer& layer, sf::Vector2f position, NeuralNetwork& network) : layer(layer), position(position), network(network) {
    for (int i = 0; i < this->layer.GetNumberOutputs(); i++) {
        T neuron(position + sf::Vector2f(0, i * 100), this->layer.GetLayerData().GetActivation(i));
        this->neurons.push_back(neuron);
    }
}


// in the LayerVisualizer.cpp file
template <>
LayerVisualizer<NeuronPlot>::LayerVisualizer(const Layer& layer, sf::Vector2f position, NeuralNetwork& network);



template <typename T>
void LayerVisualizer<T>::drawNeurons(sf::RenderWindow& window, const vector<Color>& classColors, const ActivationType& activation, bool discretized) {

    /// this->drawConnections(window); - > we need to draw the connections first

    /// find the minimum activation and the maximum activation

    const std::vector<double>& activations = this->layer.GetLayerData().GetActivations();
    double minActivation = *std::min_element(activations.begin(), activations.end());
    double maxActivation = *std::max_element(activations.begin(), activations.end());

    for (int i = 0; i < this->neurons.size(); i++) {
        this->neurons[i].draw(window, minActivation, maxActivation);
    }
}

template <>
void LayerVisualizer<NeuronPlot>::drawNeurons(sf::RenderWindow& window, const vector<Color>& classColors, const ActivationType& activation, bool discretized);


template <typename T>
void LayerVisualizer<T>::drawConnections(sf::RenderWindow& window, double minWeight, double maxWeight) {
    int numberInputs = this->layer.GetNumberInputs();
    int numberOutputs = this->layer.GetNumberOutputs();
    const vector<double>& weights = this->layer.GetWeights();

    Vector2f offset = Vector2f(-280, -40); /// offset for the connections

    double weightRange = maxWeight - minWeight;

    for (int i = 0; i < numberInputs; i++) {
        sf::Vector2f start(this->position.x + offset.x, this->position.y + i * 100 - offset.y);
        for (int j = 0; j < numberOutputs; j++) {
            double weight = weights[i * numberOutputs + j]; // Get the weight for the current connection

            // Normalize the weight to the range [0, 1] and scale it to the desired line width range
            float lineWidth = 10.0f * ((abs(weight) - minWeight) / weightRange);

            // Calculate the alpha value based on the absolute value of the weight
            sf::Uint8 alpha = static_cast<Uint8>(255 * min(1.0, abs(weight)));

            sf::Color lineColor = weight >= 0 ? Color(0, 0, 255, alpha) : Color(255, 0, 0, alpha); // Set the line color based on the weight value

            sf::Vector2f end(this->position.x + 300 + offset.x, this->position.y + j * 100 - offset.y);
            sf::Vector2f direction = end - start;
            sf::Vector2f unitDirection = direction / sqrt(direction.x * direction.x + direction.y * direction.y);
            sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

            sf::Vector2f offset = (lineWidth / 2.f) * unitPerpendicular;

            sf::ConvexShape line;
            line.setPointCount(4);
            line.setPoint(0, start + offset);
            line.setPoint(1, start - offset);
            line.setPoint(2, end - offset);
            line.setPoint(3, end + offset);

            line.setFillColor(lineColor);

            window.draw(line);
        }
    }
}