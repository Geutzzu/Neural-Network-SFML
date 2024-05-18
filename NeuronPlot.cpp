#include "NeuronPlot.h"

using namespace std;
using namespace sf;

NeuronPlot::NeuronPlot(Vector2f position, const double& value, NeuralNetwork& neuralNetwork, int index, int layerIndex) : position(position), value(value), network(neuralNetwork), index(index), layerIndex(layerIndex)  {
	
	this->plotContainer.setSize(Vector2f(50, 50));
	this->plotContainer.setPosition(position);
	///this->plotContainer.setFillColor(Color::White);
	this->plotContainer.setOutlineColor(Color::Black);
	this->plotContainer.setOutlineThickness(1);
	this->pixelSize = 8;
	int width = 80 / this->pixelSize;
	int height = 80 / this->pixelSize;
	this->pixels = VertexArray(Triangles, width * height * 6);
}


NeuronPlot::NeuronPlot(const NeuronPlot& neuronPlot) : position(neuronPlot.position), value(neuronPlot.value), network(neuronPlot.network), plotContainer(neuronPlot.plotContainer), index(neuronPlot.index), layerIndex(neuronPlot.layerIndex), pixelSize(neuronPlot.pixelSize), pixels(neuronPlot.pixels) {
}

/// vector<double> NeuralNetwork::CalculateNeuronOutputs(const vector<double>& inputs, int specificLayer, int activeNeuron) 
void NeuronPlot::visualizePlot(sf::RenderWindow& window, const vector<Color>& classColors, const ActivationType& activation) {
    int width = 80 / this->pixelSize;
    int height = 80 / this->pixelSize;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            double input_1 = x / static_cast<double>(width);
            double input_2 = y / static_cast<double>(height);
            vector<double> outputs = this->network.CalculateNeuronOutputs({ input_1, input_2 }, this->layerIndex, this->index, activation);


            // Find the index of the maximum output
            int maxIndex = max_element(outputs.begin(), outputs.end()) - outputs.begin();

            // Set all outputs to 0, except the maximum one
            for (int i = 0; i < outputs.size(); i++) {
                outputs[i] = (i == maxIndex) ? 1 : 0;
            }

            // Get the color for this pixel
            Color color = classColors[maxIndex];

            // Calculate the position of this pixel
            Vector2f pos(x * this->pixelSize, y * this->pixelSize);

            // Set the position and color of each vertex
            int i = (x + y * width) * 6; // index for this pixel
            // First triangle
            this->pixels[i + 0].position = pos + this->position;
            this->pixels[i + 1].position = pos + Vector2f(this->pixelSize, 0) + this->position;
            this->pixels[i + 2].position = pos + Vector2f(0, this->pixelSize) + this->position;
            // Second triangle
            this->pixels[i + 3].position = pos + Vector2f(0, this->pixelSize) + this->position;
            this->pixels[i + 4].position = pos + Vector2f(this->pixelSize, 0) + this->position;
            this->pixels[i + 5].position = pos + Vector2f(this->pixelSize, this->pixelSize) + this->position;
            for (int j = 0; j < 6; j++) {
                this->pixels[i + j].color = color;
            }
        }
    }

    // Draw the pixels
    window.draw(this->pixels);
}


void NeuronPlot::draw(sf::RenderWindow& window, const vector<Color>& colors) {
	/// this->visualizePlot(window, this->network);
	window.draw(this->plotContainer);
}

