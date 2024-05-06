#pragma once


#include <NeuralNetwork.h>
#include <LayerVisualizer.h>
#include "Button.h"
#include "TextureManager.h"

enum class GameState {
    InputingData,
    Training,
};



using namespace std;

template <typename T>
class NetworkVisualizer {
private:
    NeuralNetwork& network;
    std::vector<LayerVisualizer<T>> layers;
    std::vector<pair<Button, Button> > addOrRemoveNeuron;
	std::pair<Button, Button> addOrRemoveLayer;
    std::vector<T> inputNeurons;
    sf::Vector2f position;
	TextureManager* textureManager;
    

public:
	NetworkVisualizer(NeuralNetwork& network, sf::Vector2f position); /// constructor
	NetworkVisualizer(const NetworkVisualizer& networkVisualizer); /// copy constructor
	NetworkVisualizer& operator=(NetworkVisualizer& networkVisualizer); /// assignment operator

	/// add or remove neuron
    void addNeuron(int layerIndex);
	void removeNeuron(int layerIndex);
	void addLayer();
    void removeLayer();

    /// add or remove layer

    void initializeValues(sf::Vector2f position);
    void draw(sf::RenderWindow& window, GameState gameState, bool dataSetEmpty);
	void checkEvents(sf::RenderWindow& window, sf::Event event, sf::View& view);

	void setPosition(sf::Vector2f position) { this->position = position; }

	int getNumberLayers() const { return this->network.GetNumberLayers(); }
	int getNumberOutputs() const { return this->network.GetLayer(this->network.GetNumberLayers() - 1).GetNumberOutputs(); }

};

template <typename T>
void NetworkVisualizer<T>::initializeValues(sf::Vector2f position) {

	this->network.InitializeWeightsAndBiases();
    this->position = position;
	this->textureManager = TextureManager::getInstance();

    for (int i = 0; i < this->network.GetLayer(0).GetNumberInputs(); i++) {
        T neuron(this->position + sf::Vector2f(-320, i * 100), this->network.GetLayer(0).GetLayerData().GetInput(i));
        inputNeurons.push_back(neuron);
    }
    /// Button::Button(float x, float y, float width, float height, Color color, Color hoverColor, function<void()> onClickFunction, const Font& font, const string& text)
    for (int i = 0; i < this->network.GetNumberLayers(); i++) {
        LayerVisualizer<T> layerVisualizer(this->network.GetLayer(i), position + sf::Vector2f(i * 340, 0));
        layers.push_back(layerVisualizer);
        Button addNeuronButton(position.x + i * 340 - 23, position.y - 75, 50, 50, Color(128, 128, 128), Color(160, 160, 160), [i, this]() {
                this->addNeuron(i);
          }, textureManager->getFont("roboto"), "+");
        Button removeNeuronButton(position.x + i * 340 + 75 - 23, position.y - 75, 50, 50, Color(128, 128, 128), Color(160, 160, 160), [i, this]() {
                this->removeNeuron(i); 
          }, textureManager->getFont("roboto"), "-");


		this->addOrRemoveNeuron.push_back(make_pair(addNeuronButton, removeNeuronButton));
    }
	Button addLayerButton(position.x + (this->network.GetNumberLayers() - 1) * 340 - 23, position.y + 500, 50, 50, Color(128, 128, 128), Color(160, 160, 160), [this]() { this->addLayer(); }, textureManager->getFont("roboto"), "+");
	Button removeLayerButton(position.x + (this->network.GetNumberLayers() - 1) * 340 + 75 - 23, position.y + 500, 50, 50, Color(128, 128, 128), Color(160, 160, 160), [this]() { this->removeLayer(); }, textureManager->getFont("roboto"), "-");
	this->addOrRemoveLayer = make_pair(addLayerButton, removeLayerButton);
}

template <typename T>
NetworkVisualizer<T>::NetworkVisualizer(NeuralNetwork& network, sf::Vector2f position) : network(network) {
    this->initializeValues(position);
}

template <typename T>
NetworkVisualizer<T>::NetworkVisualizer(const NetworkVisualizer& networkVisualizer) : network(networkVisualizer.network) {
    this->initializeValues(networkVisualizer.position);
}

template <typename T>
NetworkVisualizer<T>& NetworkVisualizer<T>::operator=(NetworkVisualizer& networkVisualizer) {
    if (this != &networkVisualizer) {
        this->initializeValues(networkVisualizer.position);
    }
    return *this;
}



template <typename T>
void NetworkVisualizer<T>::draw(sf::RenderWindow& window, GameState gameState, bool dataSetEmpty) {
    const std::vector<double>& activations = this->network.GetLayer(0).GetLayerData().GetActivations();
    double minActivation = *std::min_element(activations.begin(), activations.end());
    double maxActivation = *std::max_element(activations.begin(), activations.end());


	if (gameState == GameState::InputingData) {
        for (int i = 0; i < this->layers.size(); i++) {

            if (i == this->layers.size() - 1 && !dataSetEmpty) {
                this->addOrRemoveNeuron[i].first.setVisible(false);
                this->addOrRemoveNeuron[i].second.setVisible(false);
            }
            else {
                this->addOrRemoveNeuron[i].first.draw(window);
                this->addOrRemoveNeuron[i].second.draw(window);
            }
           
        }

        this->addOrRemoveLayer.first.draw(window);
        this->addOrRemoveLayer.second.draw(window);
	}
    else {
		this->addOrRemoveLayer.first.setVisible(false);
		this->addOrRemoveLayer.second.setVisible(false);
		for (int i = 0; i < this->layers.size(); i++) {
            
			this->addOrRemoveNeuron[i].first.setVisible(false);
			this->addOrRemoveNeuron[i].second.setVisible(false);
		}

    }

    for (int i = 0; i < this->layers.size(); i++) {
        this->layers[i].drawConnections(window);
    }


    for (int i = 0; i < this->layers.size(); i++) {
        this->layers[i].drawNeurons(window);
    }

	

    for (int i = 0; i < this->inputNeurons.size(); i++) {
        this->inputNeurons[i].draw(window, minActivation, maxActivation);
    }

}

template <typename T>
void NetworkVisualizer<T>::addNeuron(int layerIndex) {
    // Create a list of layer sizes for the new NeuralNetwork

    std::vector<int> layerSizes;

    layerSizes.push_back(2); // Input layer

    for (int i = 0; i < this->network.GetNumberLayers(); ++i) {
        int numNeurons = this->network.GetLayer(i).GetNumberOutputs();
        if (i == layerIndex) {
            ++numNeurons;
        }
        layerSizes.push_back(numNeurons);
    }

    // Create a new NeuralNetwork with an additional neuron in the specified layer
    NeuralNetwork newNetwork(layerSizes);

    // Replace the old network with the new one
    this->network = newNetwork;

    // Reinitialize the NetworkVisualizer
    this->layers.clear();
    this->addOrRemoveNeuron.clear();
    this->inputNeurons.clear();
    this->initializeValues(this->position);
}

template <typename T>
void NetworkVisualizer<T>::removeNeuron(int layerIndex) {
	// Create a list of layer sizes for the new NeuralNetwork
	std::vector<int> layerSizes;

	layerSizes.push_back(2); // Input layer

	for (int i = 0; i < this->network.GetNumberLayers(); ++i) {
		int numNeurons = this->network.GetLayer(i).GetNumberOutputs();
		if (i == layerIndex) {
			--numNeurons;
		}
		layerSizes.push_back(numNeurons);
	}

	// Create a new NeuralNetwork with an additional neuron in the specified layer
	NeuralNetwork newNetwork(layerSizes);

	// Replace the old network with the new one
	this->network = newNetwork;

	// Reinitialize the NetworkVisualizer
	this->layers.clear();
	this->addOrRemoveNeuron.clear();
	this->inputNeurons.clear();
	this->initializeValues(this->position);
}


template<typename T>
void NetworkVisualizer<T>::addLayer() {
	// Create a list of layer sizes for the new NeuralNetwork
	std::vector<int> layerSizes;

	layerSizes.push_back(2); // Input layer

	layerSizes.push_back(1); // New layer

	for (int i = 0; i < this->network.GetNumberLayers(); ++i) {
		int numNeurons = this->network.GetLayer(i).GetNumberOutputs();
		layerSizes.push_back(numNeurons);
	}


	// Create a new NeuralNetwork with an additional neuron in the specified layer
	NeuralNetwork newNetwork(layerSizes);

	// Replace the old network with the new one
	this->network = newNetwork;

	// Reinitialize the NetworkVisualizer
	this->layers.clear();
	this->addOrRemoveNeuron.clear();
	this->inputNeurons.clear();
	this->initializeValues(this->position);
}

template<typename T>
void NetworkVisualizer<T>::removeLayer() {
    // Create a list of layer sizes for the new NeuralNetwork
    std::vector<int> layerSizes;

    try {
        if (this->network.GetNumberLayers() <= 1) {
            throw std::runtime_error("Cannot remove layer: Network has no layers to remove");
        }

        layerSizes.push_back(2); // Input layer

        for (int i = 1; i < this->network.GetNumberLayers(); ++i) {
            int numNeurons = this->network.GetLayer(i).GetNumberOutputs();
            layerSizes.push_back(numNeurons);
        }

        // Create a new NeuralNetwork with an additional neuron in the specified layer
        NeuralNetwork newNetwork(layerSizes);

        // Replace the old network with the new one
        this->network = newNetwork;

        // Reinitialize the NetworkVisualizer
        this->layers.clear();
        this->addOrRemoveNeuron.clear();
        this->inputNeurons.clear();
        this->initializeValues(this->position);
    }
    catch (const std::runtime_error& e) {
        cout << e.what() << std::endl;
    }
}



template <typename T>
void NetworkVisualizer<T>::checkEvents(sf::RenderWindow& window, sf::Event event, sf::View& view) {
    // Convert the mouse position from pixel to world coordinates
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos, view);



    sf::Event worldEvent = event;
    worldEvent.mouseButton.x = worldPos.x;
    worldEvent.mouseButton.y = worldPos.y;

        for (int i = 0; i < this->addOrRemoveNeuron.size(); i++) {
			if (this->addOrRemoveNeuron[i].first.getVisible()) {
				this->addOrRemoveNeuron[i].first.checkEvents(window, worldEvent);
				this->addOrRemoveNeuron[i].second.checkEvents(window, worldEvent);
			}
        }
        if (this->addOrRemoveLayer.first.getVisible()) {
            this->addOrRemoveLayer.first.checkEvents(window, worldEvent);
            this->addOrRemoveLayer.second.checkEvents(window, worldEvent); // Update the second button's event as well
        }

   
}