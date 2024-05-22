

#include "NetworkVisualizer.h"

/// NeuronPlot(Vector2f position, const double& value, NeuralNetwork& neuralNetwork, int index, int layerIndex);

template<>
void NetworkVisualizer<NeuronPlot>::initializeInputNeurons() {
    for (int i = 0; i < this->network.GetLayer(0).GetNumberInputs(); i++) {
        NeuronPlot neuron(this->position + sf::Vector2f(-320, i * 100), this->network.GetLayer(0).GetLayerData().GetInput(i), this->network, i, -1); /// !
        inputNeurons.push_back(neuron);
    }
}

template<>
void NetworkVisualizer<NeuronPlot>::drawInputNeurons(sf::RenderWindow& window, double minActivation, double maxActivation, const ActivationType& activation, bool discretize) {
    for (int i = 0; i < this->inputNeurons.size(); i++) {
		if (discretize) {
			this->inputNeurons[i].visualizePlotDiscretized(window, this->classColors, activation);
		}
        else {
			this->inputNeurons[i].visualizePlot(window, this->classColors, activation);
		}
    }
}