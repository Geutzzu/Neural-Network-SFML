#include "NeuronPlot.h"

using namespace std;
using namespace sf;


/// static variables
vector<Vector2f> NeuronPlot::pixelPositions;
bool NeuronPlot::positionsComputed = false;

void NeuronPlot::computePixelPositions() { /// have to pay attention at pixel size over here
    int width = 80 / 8;
    int height = 80 / 8;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            Vector2f pos(x * 8, y * 8);
            pixelPositions.push_back(pos);
        }
    }
    positionsComputed = true;
}



NeuronPlot::NeuronPlot(Vector2f position, const double& value, NeuralNetwork& neuralNetwork, int index, int layerIndex) : position(position), value(value), network(neuralNetwork), index(index), layerIndex(layerIndex)  {
	
    if (!positionsComputed) {
        computePixelPositions();
    }
	this->plotContainer.setSize(Vector2f(80, 80));
	this->plotContainer.setPosition(position);
	///this->plotContainer.setFillColor(Color::White);
	this->plotContainer.setOutlineColor(Color::Black);
	this->plotContainer.setOutlineThickness(1);
	this->pixelSize = 8; /// has to be the same as in the static method
	int width = 80 / this->pixelSize;
	int height = 80 / this->pixelSize;
	this->pixels = VertexArray(Triangles, width * height * 6);

    /// border for the container
	this->plotContainer.setOutlineColor(Color::White);
	this->plotContainer.setOutlineThickness(2);

	/// computing the positions of the pixels
    for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
            int i = (x + y * width) * 6; // index for this pixel

			/// first triangle
			this->pixels[i + 0].position = pixelPositions[y + x * width] + this->position;
			this->pixels[i + 1].position = pixelPositions[y + x * width] + Vector2f(this->pixelSize, 0) + this->position;
			this->pixels[i + 2].position = pixelPositions[y + x * width] + Vector2f(0, this->pixelSize) + this->position;
			/// second triangle
			this->pixels[i + 3].position = pixelPositions[y + x * width] + Vector2f(0, this->pixelSize) + this->position;
			this->pixels[i + 4].position = pixelPositions[y + x * width] + Vector2f(this->pixelSize, 0) + this->position;
			this->pixels[i + 5].position = pixelPositions[y + x * width] + Vector2f(this->pixelSize, this->pixelSize) + this->position;

		}
	}

}


NeuronPlot::NeuronPlot(const NeuronPlot& neuronPlot) : position(neuronPlot.position), value(neuronPlot.value), network(neuronPlot.network), plotContainer(neuronPlot.plotContainer), index(neuronPlot.index), layerIndex(neuronPlot.layerIndex), pixelSize(neuronPlot.pixelSize), pixels(neuronPlot.pixels) {
}

/// vector<double> NeuralNetwork::CalculateNeuronOutputs(const vector<double>& inputs, int specificLayer, int activeNeuron) 
void NeuronPlot::visualizePlotDiscretized(sf::RenderWindow& window, const vector<Color>& classColors, const ActivationType& activation) {
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

            int i = (x + y * width) * 6; // index for this pixel

            for (int j = 0; j < 6; j++) {
                this->pixels[i + j].color = color;
            }
        }
    }

    // Draw the pixels
    window.draw(this->plotContainer);
    window.draw(this->pixels);
	
}

void NeuronPlot::visualizePlot(sf::RenderWindow& window, const vector<Color>& classColors, const ActivationType& activation) {
    int width = 80 / this->pixelSize;
    int height = 80 / this->pixelSize;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            double input_1 = x / static_cast<double>(width);
            double input_2 = y / static_cast<double>(height);
            vector<double> outputs = this->network.CalculateNeuronOutputs({ input_1, input_2 }, this->layerIndex, this->index, activation);

            // Initialize the color in linear RGB
            double r = 0, g = 0, b = 0;
            double totalWeight = 0;

            // Calculate the weighted sum of the colors in linear RGB
            for (int i = 0; i < outputs.size(); i++) {
                double r_i = classColors[i].r / 255.0;
                double g_i = classColors[i].g / 255.0;
                double b_i = classColors[i].b / 255.0;

                r += outputs[i] * r_i;
                g += outputs[i] * g_i;
                b += outputs[i] * b_i;
                totalWeight += outputs[i];
            }

            // Normalize the color in linear RGB
            r = min(1.0, max(0.0, r / totalWeight)) * 255;
            g = min(1.0, max(0.0, g / totalWeight)) * 255;
            b = min(1.0, max(0.0, b / totalWeight)) * 255;

            int i = (x + y * width) * 6; // index for this pixel

            for (int j = 0; j < 6; j++) {
                this->pixels[i + j].color = Color(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
            }
        }
    }

    // Draw the pixels
    window.draw(this->plotContainer);
    window.draw(this->pixels);
}


void NeuronPlot::draw(sf::RenderWindow& window, const vector<Color>& colors) {
	/// this->visualizePlot(window, this->network);
	window.draw(this->plotContainer);
}

