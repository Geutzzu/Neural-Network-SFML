#pragma once


#include "NeuralNetwork.h"
#include "LayerVisualizer.h"
#include "Button.h"
#include "TextureManager.h"
#include "NeuronPlot.h"
#include <stack>

enum class GameState {
    InputingData,
    Training,
};


template <typename T>
class NetworkVisualizer {
private:
	NeuralNetwork& network; /// we use this reference to change the network according to the inputs from the visualizer
    std::vector<LayerVisualizer<T>> layers; /// !
    std::vector<pair<Button, Button> > addOrRemoveNeuron;
	std::pair<Button, Button> addOrRemoveLayer;
    sf::Text layersText;
    std::vector<T> inputNeurons;
    sf::Vector2f position;
	TextureManager* textureManager;
	std::vector<sf::Color>& classColors;
    

public:
	NetworkVisualizer(NeuralNetwork& network, sf::Vector2f position, std::vector<sf::Color>& classColors); /// constructor
	NetworkVisualizer(const NetworkVisualizer& networkVisualizer); /// copy constructor
	NetworkVisualizer& operator=(const NetworkVisualizer& networkVisualizer); /// assignment operator

	/// add or remove neuron
    void addNeuron(int layerIndex);
	void removeNeuron(int layerIndex);
	void addLayer();
    void removeLayer();

    /// add or remove layer
    void initializeInputNeurons();
	void drawInputNeurons(sf::RenderWindow& window, double minActivation, double maxActivation, const ActivationType& activation, bool discretized);
    void initializeValues(sf::Vector2f position);
    void draw(sf::RenderWindow& window, GameState gameState, bool dataSetEmpty, const ActivationType& activation, bool discretized);
	void checkEvents(sf::RenderWindow& window, sf::Event event, sf::View& view, stack<DataPoint>& undoStack, stack<DataPoint>& redoStack);
	

	void setPosition(sf::Vector2f position) { this->position = position; }

	int getNumberLayers() const { return this->network.getNumberLayers(); }
	int getNumberOutputs() const { return this->network.getLayer(this->network.getNumberLayers() - 1).getNumberOutputs(); }
    int getTotalWidth() const { return this->network.getNumberLayers() * 340; }

};


/// specialization for initializeValues for NeuronPlot
template<typename T>
void NetworkVisualizer<T>::initializeInputNeurons() {
    for (int i = 0; i < this->network.getLayer(0).getNumberInputs(); i++) {
        T neuron(this->position + sf::Vector2f(-320, i * 100), this->network.getLayer(0).getLayerData().getInput(i)); /// !
        inputNeurons.push_back(neuron);
    }
}

template<>
void NetworkVisualizer<NeuronPlot>::initializeInputNeurons();

template <typename T>
void NetworkVisualizer<T>::initializeValues(sf::Vector2f position) {

	/// this->network.InitializeWeightsAndBiases(); - it worked fine until I added toggle between views
    this->position = position;
	this->textureManager = TextureManager::getInstance();

	this->initializeInputNeurons();

    /// Button::Button(float x, float y, float width, float height, Color color, Color hoverColor, function<void()> onClickFunction, const Font& font, const string& text)
    for (int i = 0; i < this->network.getNumberLayers(); i++) {
        LayerVisualizer<T> layerVisualizer(this->network.getLayer(i), position + sf::Vector2f(i * 340, 0), this->network);
        layers.push_back(layerVisualizer);
        Button addNeuronButton(position.x + i * 340 - 23, position.y - 75, 50, 50, Color(128, 128, 128), Color(160, 160, 160), [i, this]() {
                this->addNeuron(i);
          }, textureManager->getFont("roboto"), "+");
        Button removeNeuronButton(position.x + i * 340 + 75 - 23, position.y - 75, 50, 50, Color(128, 128, 128), Color(160, 160, 160), [i, this]() {
                this->removeNeuron(i); 
          }, textureManager->getFont("roboto"), "-");


		this->addOrRemoveNeuron.push_back(make_pair(addNeuronButton, removeNeuronButton));
    }

	///Button addLayerButton(position.x + (this->network.GetNumberLayers() - 1) * 340 - 23, position.y + 500, 50, 50, Color(128, 128, 128), Color(160, 160, 160), [this]() { this->addLayer(); }, textureManager->getFont("roboto"), "+");
	///Button removeLayerButton(position.x + (this->network.GetNumberLayers() - 1) * 340 + 75 - 23, position.y + 500, 50, 50, Color(128, 128, 128), Color(160, 160, 160), [this]() { this->removeLayer(); }, textureManager->getFont("roboto"), "-");
    /// this will NOT be placed in the network view it will be independent
    Button addLayerButton(220, 105, 50, 50, Color(128, 128, 128), Color(160, 160, 160), [this]() { this->addLayer(); }, textureManager->getFont("roboto"), "+");
    Button removeLayerButton(220 + 75, 105, 50, 50, Color(128, 128, 128), Color(160, 160, 160), [this]() { this->removeLayer(); }, textureManager->getFont("roboto"), "-");

	this->layersText.setCharacterSize(22); /// text size in pixels
	this->layersText.setFont(textureManager->getFont("roboto"));
	this->layersText.setFillColor(Color::White);
	this->layersText.setString("Add or Remove\nHidden Layers");
	this->layersText.setPosition(10, 100);

	this->addOrRemoveLayer = make_pair(addLayerButton, removeLayerButton);
}


template <typename T>
NetworkVisualizer<T>::NetworkVisualizer(NeuralNetwork& network, sf::Vector2f position, std::vector<sf::Color>& classColors) : network(network), classColors(classColors) {
    this->initializeValues(position);
}

template <typename T>
NetworkVisualizer<T>::NetworkVisualizer(const NetworkVisualizer& networkVisualizer) : network(networkVisualizer.network), classColors(networkVisualizer.classColors) {
    this->initializeValues(networkVisualizer.position);
}

template <typename T>
NetworkVisualizer<T>& NetworkVisualizer<T>::operator=(const NetworkVisualizer& networkVisualizer) {
    if (this != &networkVisualizer) {
        this->initializeValues(networkVisualizer.position);
		this->classColors = networkVisualizer.classColors;
		this->network = networkVisualizer.network;
    }
    return *this;
}

template <typename T>
void NetworkVisualizer<T>::drawInputNeurons(sf::RenderWindow& window, double minActivation, double maxActivation, const ActivationType& activation, bool discretized) {
    for (int i = 0; i < this->inputNeurons.size(); i++) {
        this->inputNeurons[i].draw(window, minActivation, maxActivation);
    }
}

template <>
void NetworkVisualizer<NeuronPlot>::drawInputNeurons(sf::RenderWindow& window, double minActivation, double maxActivation, const ActivationType& activation, bool discretized);


template <typename T>
void NetworkVisualizer<T>::draw(sf::RenderWindow& window, GameState gameState, bool dataSetEmpty, const ActivationType& activation, bool discretized) {
    const std::vector<double>& activations = this->network.getLayer(0).getLayerData().getActivations();
    double minActivation = *std::min_element(activations.begin(), activations.end());
    double maxActivation = *std::max_element(activations.begin(), activations.end());

    double minWeight = 0;
	double maxWeight = 0;



    for (int i = 0; i < this->network.getNumberLayers(); i++) {
		minWeight = std::min(minWeight, *std::min_element(this->network.getLayer(i).getWeights().begin(), this->network.getLayer(i).getWeights().end()));
		maxWeight = std::max(maxWeight, *std::max_element(this->network.getLayer(i).getWeights().begin(), this->network.getLayer(i).getWeights().end()));
	}   


    for (int i = 0; i < this->layers.size(); i++) {
        this->layers[i].drawConnections(window, minWeight, maxWeight);
    }


    for (int i = 0; i < this->layers.size(); i++) {
        this->layers[i].drawNeurons(window, this->classColors, activation, discretized);
    }
	
    this->drawInputNeurons(window, minActivation, maxActivation, activation, discretized);
    
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

        /// show on the default view
        View view = window.getView();
        window.setView(window.getDefaultView());

        this->addOrRemoveLayer.first.draw(window);
        this->addOrRemoveLayer.second.draw(window);


        window.draw(this->layersText);
        window.setView(view);
    }
    else {
        this->addOrRemoveLayer.first.setVisible(false);
        this->addOrRemoveLayer.second.setVisible(false);
        for (int i = 0; i < this->layers.size(); i++) {

            this->addOrRemoveNeuron[i].first.setVisible(false);
            this->addOrRemoveNeuron[i].second.setVisible(false);
        }

    }

}


template <typename T>
void NetworkVisualizer<T>::addNeuron(int layerIndex) {
    try {

        // Create a list of layer sizes for the new NeuralNetwork
        std::vector<int> layerSizes;

        layerSizes.push_back(2); // Input layer

        for (int i = 0; i < this->network.getNumberLayers(); ++i) {
            int numNeurons = this->network.getLayer(i).getNumberOutputs();
            if (i == layerIndex) {
                if (i == this->network.getNumberLayers() - 1 && numNeurons >= 6) {
                    throw std::runtime_error("Cannot add neuron: Maximum of 6 neurons for the last layer reached");
                }
                else if (numNeurons >= 8) {
                    throw std::runtime_error("Cannot add neuron: Maximum of 8 neurons per layer reached");
                }
                ++numNeurons;
            }
            layerSizes.push_back(numNeurons);
        }

        // Replace the old network with the new one
        this->network = NeuralNetwork(layerSizes);

        // Reinitialize the NetworkVisualizer
        this->layers.clear();
        this->addOrRemoveNeuron.clear();
        this->inputNeurons.clear();
        this->initializeValues(this->position);
        this->network.initializeWeightsAndBiases();

    }
    catch (const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
}

template <typename T>
void NetworkVisualizer<T>::removeNeuron(int layerIndex) {
    try {
        // Create a list of layer sizes for the new NeuralNetwork
        std::vector<int> layerSizes;

        layerSizes.push_back(2); // Input layer

        for (int i = 0; i < this->network.getNumberLayers(); ++i) {
            int numNeurons = this->network.getLayer(i).getNumberOutputs();
            if (i == layerIndex) {
                if (i == this->network.getNumberLayers() - 1 && numNeurons <= 2) {
                    throw std::runtime_error("Cannot remove neuron: Minimum of 2 neurons for the last layer reached");
                }
                else if (numNeurons <= 1) {
                    throw std::runtime_error("Cannot remove neuron: Layer must have at least one neuron");
                }
                --numNeurons;
            }
            layerSizes.push_back(numNeurons);
        }

        // Replace the old network with the new one
        this->network = NeuralNetwork(layerSizes);

        // Reinitialize the NetworkVisualizer
        this->layers.clear();
        this->addOrRemoveNeuron.clear();
        this->inputNeurons.clear();
        this->initializeValues(this->position);
        this->network.initializeWeightsAndBiases();
    }
    catch (const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
}



template<typename T>
void NetworkVisualizer<T>::addLayer() {


	// Create a list of layer sizes for the new NeuralNetwork
	std::vector<int> layerSizes;

	layerSizes.push_back(2); // Input layer

	layerSizes.push_back(1); // New layer

	for (int i = 0; i < this->network.getNumberLayers(); ++i) {
		int numNeurons = this->network.getLayer(i).getNumberOutputs();
		layerSizes.push_back(numNeurons);
	}


    // Replace the old network with the new one
    this->network = NeuralNetwork(layerSizes);

	// Reinitialize the NetworkVisualizer
	this->layers.clear();
	this->addOrRemoveNeuron.clear();
	this->inputNeurons.clear();
	this->initializeValues(this->position);
    this->network.initializeWeightsAndBiases();
}

template<typename T>
void NetworkVisualizer<T>::removeLayer() {
    // Create a list of layer sizes for the new NeuralNetwork
    std::vector<int> layerSizes;

    try {
        if (this->network.getNumberLayers() <= 1) {
            throw std::runtime_error("Cannot remove layer: Network has no layers to remove");
        }

        layerSizes.push_back(2); // Input layer

        for (int i = 1; i < this->network.getNumberLayers(); ++i) {
            int numNeurons = this->network.getLayer(i).getNumberOutputs();
            layerSizes.push_back(numNeurons);
        }

        // Replace the old network with the new one
        this->network = NeuralNetwork(layerSizes);

        // Reinitialize the NetworkVisualizer
        this->layers.clear();
        this->addOrRemoveNeuron.clear();
        this->inputNeurons.clear();
        this->initializeValues(this->position);
        this->network.initializeWeightsAndBiases();
    }
    catch (const std::runtime_error& e) {
        cout << e.what() << std::endl;
    }
}



template <typename T>
void NetworkVisualizer<T>::checkEvents(sf::RenderWindow& window, sf::Event event, sf::View& view, stack<DataPoint>& undoStack, stack<DataPoint>& redoStack) {
    // Convert the mouse position from pixel to world coordinates
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos, view);

	sf::Event defaultEvent = event;
	defaultEvent.mouseButton.x = pixelPos.x;
	defaultEvent.mouseButton.y = pixelPos.y;

    sf::Event worldEvent = event;
    worldEvent.mouseButton.x = worldPos.x;
    worldEvent.mouseButton.y = worldPos.y;

        for (int i = 0; i < this->addOrRemoveNeuron.size(); i++) {
			if (this->addOrRemoveNeuron[i].first.getVisible()) {
				this->addOrRemoveNeuron[i].first.checkEvents(window, worldEvent);
				this->addOrRemoveNeuron[i].second.checkEvents(window, worldEvent);
				/// we check if we have to reset the undo redo stack
				if ((this->addOrRemoveNeuron[i].first.getWasPressed() || this->addOrRemoveNeuron[i].second.getWasPressed()) && i == this->addOrRemoveNeuron.size() - 1) {
					undoStack = stack<DataPoint>();
					redoStack = stack<DataPoint>();
				}
			}
        }
        if (this->addOrRemoveLayer.first.getVisible()) {
            this->addOrRemoveLayer.first.checkEvents(window, defaultEvent);
            this->addOrRemoveLayer.second.checkEvents(window, defaultEvent); // Update the second button's event as well
        }

   
}