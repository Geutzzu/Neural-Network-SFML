
#include "LayerVisualizer.h"




template <>
LayerVisualizer<NeuronPlot>::LayerVisualizer(const Layer& layer, sf::Vector2f position, NeuralNetwork& network) : layer(layer), position(position), network(network) {
	int layerIndex = this->network.getLayerIndex(this->layer);
	for (int i = 0; i < this->layer.getNumberOutputs(); i++) {
		NeuronPlot neuron(position + sf::Vector2f(0, i * 100), this->layer.getLayerData().getActivation(i), this->network, i, layerIndex);
		neurons.push_back(neuron);
	}
}

template <>
void LayerVisualizer<NeuronPlot>::drawNeurons(sf::RenderWindow& window, const vector<Color>& classColors, const ActivationType& activation, bool discretized) {
	for (int i = 0; i < this->neurons.size(); i++) {
		if (discretized) {
			this->neurons[i].visualizePlotDiscretized(window, classColors, activation);
		}
		else {
			this->neurons[i].visualizePlot(window, classColors, activation);
		}
	}
}
