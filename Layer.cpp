
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>

#include "NeuralNetwork.h"
#include "DataPoint.h"
#include "Layer.h"
#include "LayerData.h"
#include "Cost.h"
#include "Activation.h"


using namespace std;


Layer::Layer(int numberInputs, int numberOutputs) {
	this->numberInputs = numberInputs;
	this->numberOutputs = numberOutputs;
	this->weights.resize(numberInputs * numberOutputs);
	this->biases.resize(numberOutputs);
	this->costGradientBiases.resize(numberOutputs);
	this->costGradientWeights.resize(numberInputs * numberOutputs);
	this->layerData = LayerData(numberInputs, numberOutputs);

}

void Layer::InitializeWeightsAndBiases() {
	/// randomly initialize the weights and biases
	for (int i = 0; i < this->weights.size(); i++) {
		this->weights[i] = (rand() % 1000) / 1000.0 - 0.5; /// random number between -0.5 and 0.5
	}
	for (int i = 0; i < this->biases.size(); i++) {
		this->biases[i] = (rand() % 1000) / 1000.0 - 0.5; /// random number between -0.5 and 0.5
	}
}


/*vector<double> Layer::CalculateOutputs(const vector<double>& inputs) const {
	vector<double> weightedInputs(this->numberOutputs); /// initialize the outputs to the number of outputs
	for (int i = 0; i < this->numberOutputs; i++) { /// for each output
		double weightedInput = this->biases[i]; /// initialize the output to the bias
		for (int j = 0; j < this->numberInputs; j++) { /// for each input
			weightedInput += inputs[j] * GetWeight(j, i); /// add the input * weight to the output
		}
		weightedInputs[i] = weightedInput; /// set the output to the output
	}
	Activation<ActivationType::Sigmoid> function; /// initialize the activation function
	vector<double> activations(this->numberOutputs); /// initialize the outputs to the number of outputs
	for (int i = 0; i < weightedInputs.size(); i++) { /// for each output
		weightedInputs[i] = function.Activate(activations, i); /// activate the output
	}

	return weightedInputs; /// return the outputs
}*/

vector<double> Layer::CalculateOutputs(const vector<double>& inputs) {
	this->layerData.SetInputs(inputs); /// set the inputs of the layer data
	for (int i = 0; i < this->numberOutputs; i++) { /// for each output
		double weightedInput = this->biases[i]; /// initialize the output to the bias
		for (int j = 0; j < this->numberInputs; j++) { /// for each input
			weightedInput += inputs[j] * GetWeight(j, i); /// add the input * weight to the output
		}
		this->layerData.SetWeightedInput(i, weightedInput); /// set the output to the output
	}
	Activation<ActivationType::ReLU> function; /// initialize the activation function
	for (int i = 0; i < this->numberOutputs; i++) { /// for each output
		this->layerData.SetActivation(i, function.Activate(this->layerData.GetWeightedInputs(), i)); /// activate the output
	}

	return this->layerData.GetActivations(); /// return the outputs
}


void Layer::UpdateGradients(const vector<double>& specificValues) {
	
	double partialDerivative = 0;

	for (int i = 0; i < this->numberOutputs; i++) {
		for (int j = 0; j < this->numberInputs; j++) {
			///partial derivative of the cost function with respect to the weight
			partialDerivative = specificValues[i] * this->layerData.GetInput(j); /// the specific value * the input
			this->costGradientWeights[j + i * this->numberInputs] += partialDerivative; /// add the partial derivative to the gradient
		}
		partialDerivative = specificValues[i] * 1.0; /// the specific value
		this->costGradientBiases[i] += partialDerivative; /// add the partial derivative to the gradient
	}
}


void Layer::ApplyGradients(double learningRate) {
	for (int i = 0; i < this->numberOutputs; i++) {
		for (int j = 0; j < this->numberInputs; j++) {
			this->weights[j + i * this->numberInputs] -= learningRate * this->costGradientWeights[j + i * this->numberInputs]; /// subtract the gradient from the weight
			this->costGradientWeights[j + i * this->numberInputs] = 0; /// reset the gradient
		}
		this->biases[i] -= learningRate * this->costGradientBiases[i]; /// subtract the gradient from the bias
		this->costGradientBiases[i] = 0; /// reset the gradient
	}
}