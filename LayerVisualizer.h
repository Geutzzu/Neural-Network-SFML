#pragma once

#include "Layer.h"

template <typename T>
class LayerVisualizer {
private:
    const Layer& layer;
    std::vector<T> neurons; // can switch between different types of neurons (Neuron, VisualNeuron, etc.)
    sf::Vector2f position;

public:
    LayerVisualizer(const Layer& layer, sf::Vector2f position);

    void drawNeurons(sf::RenderWindow& window);
    void drawConnections(sf::RenderWindow& window);
};

template <typename T>
LayerVisualizer<T>::LayerVisualizer(const Layer& layer, sf::Vector2f position) : layer(layer), position(position) {
    for (int i = 0; i < this->layer.GetNumberOutputs(); i++) {
        T neuron(position + sf::Vector2f(0, i * 100), this->layer.GetLayerData().GetActivation(i));
        neurons.push_back(neuron);
    }
}

template <typename T>
void LayerVisualizer<T>::drawNeurons(sf::RenderWindow& window) {

    /// this->drawConnections(window); - > we need to draw the connections first

    /// find the minimum activation and the maximum activation

    const std::vector<double>& activations = this->layer.GetLayerData().GetActivations();
    double minActivation = *std::min_element(activations.begin(), activations.end());
    double maxActivation = *std::max_element(activations.begin(), activations.end());

    for (int i = 0; i < this->neurons.size(); i++) {
        this->neurons[i].draw(window, minActivation, maxActivation);
    }
}

template <typename T>
void LayerVisualizer<T>::drawConnections(sf::RenderWindow& window) {
    int numberInputs = this->layer.GetNumberInputs();
    int numberOutputs = this->layer.GetNumberOutputs();
    const vector<double>& weights = this->layer.GetWeights();

    Vector2f offset = Vector2f(-280, -40); /// offset for the connections

    // Find the minimum and maximum weights
    double minWeight = *min_element(weights.begin(), weights.end()); /// is computationally expensive - can be made in neural net class
    double maxWeight = *max_element(weights.begin(), weights.end()); /// is computationally expensive - only for ilustration

    for (int i = 0; i < numberInputs; i++) {
        for (int j = 0; j < numberOutputs; j++) {
            double weight = weights[i * numberOutputs + j]; // Get the weight for the current connection

            // Normalize the weight to the range [0, 1] and scale it to the desired line width range
            float lineWidth = 10.0f * ((std::abs(weight) - minWeight) / (maxWeight - minWeight));

            sf::Color lineColor = weight >= 0 ? sf::Color::Blue : sf::Color::Red; // Set the line color based on the weight value

            sf::Vector2f start(this->position.x + offset.x, this->position.y + i * 100 - offset.y);
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
