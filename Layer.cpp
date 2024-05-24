
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

	this->prevDeltaBiases.resize(numberOutputs);
	this->prevDeltaWeights.resize(numberInputs * numberOutputs);

	this->layerData = LayerData(numberInputs, numberOutputs);

}

void Layer::initializeWeightsAndBiases() {
	/// Xavier initialization of the weights and biases
	double variance = 2.0 / (double)this->numberInputs;
	double standardDeviation = sqrt(variance);
	std::default_random_engine generator;
	std::normal_distribution<double> distribution(0.0, standardDeviation);

	for (int i = 0; i < this->weights.size(); i++) {
		this->weights[i] = distribution(generator);
	}
	for (int i = 0; i < this->biases.size(); i++) {
		this->biases[i] = distribution(generator);
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

vector<double> Layer::calculateOutputs(const vector<double>& inputs, const ActivationType& activationType) {
	this->layerData.setInputs(inputs); /// set the inputs of the layer data
	for (int i = 0; i < this->numberOutputs; i++) { /// for each output
		double weightedInput = this->biases[i]; /// initialize the output to the bias
		for (int j = 0; j < this->numberInputs; j++) { /// for each input
			weightedInput += inputs[j] * getWeight(j, i); /// add the input * weight to the output
		}
		this->layerData.setWeightedInput(i, weightedInput); /// set the output to the output
	}
	Activation function(activationType); /// initialize the activation function
	for (int i = 0; i < this->numberOutputs; i++) { /// for each output
		this->layerData.setActivation(i, function.Activate(this->layerData.getWeightedInputs(), i)); /// activate the output
	}

	return this->layerData.getActivations(); /// return the outputs
}


void Layer::updateGradients(const vector<double>& specificValues) {

	double partialDerivative = 0;

	for (int i = 0; i < this->numberOutputs; i++) {
		for (int j = 0; j < this->numberInputs; j++) {
			///partial derivative of the cost function with respect to the weight
			partialDerivative = specificValues[i] * this->layerData.getInput(j); /// the specific value * the input
			this->costGradientWeights[j + i * this->numberInputs] += partialDerivative; /// add the partial derivative to the gradient
		}
		partialDerivative = specificValues[i] * 1.0; /// the specific value
		this->costGradientBiases[i] += partialDerivative; /// add the partial derivative to the gradient
	}
}


void Layer::applyGradients(double learningRate, double momentum) {
	for (int i = 0; i < this->numberOutputs; i++) {
		for (int j = 0; j < this->numberInputs; j++) {
			double deltaWeight = learningRate * this->costGradientWeights[j + i * this->numberInputs] + momentum * this->prevDeltaWeights[j + i * this->numberInputs];
			this->weights[j + i * this->numberInputs] -= deltaWeight;
			this->prevDeltaWeights[j + i * this->numberInputs] = deltaWeight;
			this->costGradientWeights[j + i * this->numberInputs] = 0;
		}
		double deltaBias = learningRate * this->costGradientBiases[i] + momentum * this->prevDeltaBiases[i];
		this->biases[i] -= deltaBias;
		this->prevDeltaBiases[i] = deltaBias;
		this->costGradientBiases[i] = 0;
	}
}