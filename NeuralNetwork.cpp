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
#include "NetworkData.h"


using namespace std;

NeuralNetwork::NeuralNetwork(const vector<int>& layerSizes) {
	for (int i = 0; i < layerSizes.size() - 1; i++) {
		this->layers.push_back(Layer(layerSizes[i], layerSizes[i + 1])); /// add a layer to the neural network
	}
}

void NeuralNetwork::initializeWeightsAndBiases() { /// initialize the weights and biases of the neural network
	for (int i = 0; i < this->layers.size(); i++) { /// for each layer
		this->layers[i].initializeWeightsAndBiases(); /// initialize the weights and biases of the layer
	}
}

vector<double> NeuralNetwork::calculateOutputs(const vector<double>& inputs, const ActivationType& activation) { /// goes through each layer and calculates the outputs
	vector<double> outputs = inputs; /// initialize the outputs to the inputs
	for (int i = 0; i < this->layers.size(); i++) { /// for each layer
		outputs = this->layers[i].calculateOutputs(outputs, activation); /// calculate the outputs of the layer
	}

	return outputs;
}



vector<double> NeuralNetwork::calculateNeuronOutputsDiscretized(const vector<double>& inputs, int specificLayer, int activeNeuron, const ActivationType& activation) {
	/// we set all values to 0 except for the active neuron from a layer and continue the forward pass
	vector<double> outputs = inputs;

	if (specificLayer == -1) { /// if we want to plot the neuron from the input layer
		if (activeNeuron == 0) { /// if we want to plot the first neuron
			outputs[0] = 0;
		}
		else { /// if we want to plot the second neuron
			outputs[1] = 0;
		}
		outputs = this->calculateOutputs(outputs, activation);
		return outputs;
	}


	for (int i = 0; i < this->layers.size(); i++) {
		outputs = this->layers[i].calculateOutputs(outputs, activation); /// calculate the outputs of the layer

		if (i == specificLayer) {
			if (i != this->layers.size() - 1) {
				for (int j = 0; j < outputs.size(); j++) {
					if (j != activeNeuron) { /// if the neuron is not the active neuron
						outputs[j] = 0; /// set the output to 0
					}
				}
			}
			else {
				for (int j = 0; j < outputs.size(); j++) {
					if (j != activeNeuron) { /// if the neuron is not the active neuron
						outputs[j] = 0.8; /// set the output to a value close to 1 but not one to see the difference - you could call this a confidence bound
					}
				}
			}

		}
	}
	return outputs;
}



vector<double> NeuralNetwork::calculateNeuronOutputs(const vector<double>& inputs, int specificLayer, int activeNeuron, const ActivationType& activation) {
	/// we set all values to 0 except for the active neuron from a layer and continue the forward pass
	vector<double> outputs = inputs;

	if (specificLayer == -1) { /// if we want to plot the neuron from the input layer
		if (activeNeuron == 0) { /// if we want to plot the first neuron
			outputs[0] = 0;
		}
		else { /// if we want to plot the second neuron
			outputs[1] = 0;
		}
		outputs = this->calculateOutputs(outputs, activation);
		return outputs;
	}


	for (int i = 0; i < this->layers.size(); i++) {
		outputs = this->layers[i].calculateOutputs(outputs, activation); /// calculate the outputs of the layer

		if (i == specificLayer) {
			if (i != this->layers.size() - 1) {
				for (int j = 0; j < outputs.size(); j++) {
					if (j != activeNeuron) { /// if the neuron is not the active neuron
						outputs[j] = 0; /// set the output to 0
					}
				}
			}
			else {
				for (int j = 0; j < outputs.size(); j++) {
					if (j != activeNeuron) { /// if the neuron is not the active neuron
						outputs[j] = 0.01; /// here we place it low so we can see the color changes when interpolating
					}
				}
			}

		}
	}
	return outputs;
}

int NeuralNetwork::classify(const vector<double>& inputs, const ActivationType& activation) { /// classify the inputs from 1 
	vector<double> outputs = calculateOutputs(inputs, activation); /// calculate the outputs of the neural network

	int classification = 0; /// initialize the classification to 0
	for (int i = 1; i < outputs.size(); i++) { /// for each output
		if (outputs[i] > outputs[classification]) { /// if the output is greater than the current classification
			classification = i; /// set the classification to the output
		}
	}
	return classification; /// return the classification
}

double NeuralNetwork::costFunction(const DataPoint& dataPoint, const ActivationType& activation, const CostType& costType) { /// calculate the cost of the data point
	vector<double> outputs = calculateOutputs(dataPoint.GetInputs(), activation); /// calculate the outputs of the neural network
	double cost = 0; /// initialize the cost to 0
	Cost costFunction(costType); /// initialize the cost function
	for (int i = 0; i < outputs.size(); i++) { /// for each output
		cost += costFunction.Function({ outputs[i] }, { dataPoint.GetOutputs()[i] }); /// the cost between the output and the expected output
	}
	return cost; /// return the cost
}

double NeuralNetwork::costFunction(const set<DataPoint>& dataPoints, const ActivationType& activation, const CostType& costType) { /// calculate the cost of the data points
	double cost = 0; /// initialize the cost to 0
	for (const auto& dataPoint : dataPoints) { /// for each data point
		cost += costFunction(dataPoint, activation, costType); /// add the cost of the data point to the cost
	}
	return cost; /// return the cost
}

void NeuralNetwork::updateGradientsForDataPoint(const DataPoint& dataPoint, const ActivationType& activationType, const CostType& costType) { /// update the gradients for the data point

	vector<double> outputs = calculateOutputs(dataPoint.GetInputs(), activationType); /// calculate the outputs of the neural network

	vector<double> specificValues; /// initialize the specific values - specific for each terminal node

	specificValues = calculateOutputSpecificValues(dataPoint.GetOutputs(), activationType, costType); /// calculate the specific values for the terminal node

	this->layers[this->layers.size() - 1].updateGradients(specificValues); /// update the gradients for the terminal node

	for (int i = this->layers.size() - 2; i >= 0; i--) { /// for each hidden layer
		specificValues = calculateHiddenLayerOutputSpecificValues(specificValues, i + 1, activationType, costType); /// calculate the specific values for the hidden layer
		this->layers[i].updateGradients(specificValues); /// update the gradients for the hidden layer
	}

}


void NeuralNetwork::learn(vector<DataPoint> trainingBatch, double learningRate, double momentum) {
	for (int i = 0; i < trainingBatch.size(); i++) {
		this->updateGradientsForDataPoint(trainingBatch[i], ActivationType::Sigmoid, CostType::MeanSquareError);
	}
	this->applyAllGradients(learningRate / trainingBatch.size(), momentum);
}

void NeuralNetwork::applyAllGradients(double learningRate, double momentum) {
	for (int i = 0; i < this->layers.size(); i++) {
		this->layers[i].applyGradients(learningRate, momentum); /// apply the gradients for each layer
	}
}



vector<double> NeuralNetwork::calculateOutputSpecificValues(const vector<double>& expectedOutputs, const ActivationType& activationType, const CostType& costType) {

	vector<double> specificValues(expectedOutputs.size()); /// initialize the specific values - specific for each terminal node

	Activation function(ActivationType::Sigmoid); /// initialize the activation function - this will be left on sigmoid since we have values in 0 - 1 interval
	Cost costFunction(costType); /// initialize the cost function

	for (int i = 0; i < specificValues.size(); i++) { /// for each specific value
		double costDerivative = costFunction.Derivative(this->getLayerData(this->layers.size() - 1).getActivation(i), expectedOutputs[i]); /// calculate the cost derivative
		double activationDerivative = function.Derivative(this->getLayerData(this->layers.size() - 1).getWeightedInput(i)); /// calculate the activation derivative
		specificValues[i] = costDerivative * activationDerivative; /// set the specific value to the cost derivative * activation derivative
	}
	return specificValues; /// return the specific values

}


vector<double> NeuralNetwork::calculateHiddenLayerOutputSpecificValues(const vector<double>& oldSpecificValues, int oldIndex, const ActivationType& activationType, const CostType& costType) { /// actually previous specific values

	vector<double> newSpecificValues(this->getLayerData(oldIndex - 1).getNumberOutputs()); /// initialize the specific values - specific for each terminal node
	Activation function(activationType); /// initialize the activation function

	for (int i = 0; i < newSpecificValues.size(); i++) { /// for each specific value
		double sum = 0; /// initialize the sum to 0
		for (int j = 0; j < oldSpecificValues.size(); j++) { /// for each old specific value
			sum += oldSpecificValues[j] * this->layers[oldIndex].getWeight(i, j); /// add the old specific value * the weight to the sum
		}
		sum *= function.Derivative(this->getLayerData(oldIndex - 1).getWeightedInputs(), i); /// multiply the sum by the derivative of the activation function
		newSpecificValues[i] = sum; /// set the specific value to the sum
	} /// this->layerData.GetWeightedInputs(), i

	return newSpecificValues; /// return the specific values

}



vector <DataPoint> NeuralNetwork::classifyAll(const vector<DataPoint>& dataPoints, const ActivationType& activation) { /// classify all the data points
	vector<DataPoint> predictedValues(dataPoints.size()); /// initialize the classifications
	for (int i = 0; i < dataPoints.size(); i++) { /// for each data point
		DataPoint result = dataPoints[i]; /// get the data point

		result.SetOutputs(calculateOutputs(dataPoints[i].GetInputs(), activation)); /// set the classification

		/*for (int j = 0; j < result.GetOutputs().size(); j++)
			cout << result.GetOutputs()[j] << " ";*/

		predictedValues[i] = result; /// set the classification
	}
	return predictedValues; /// return the classifications
}

DataPoint NeuralNetwork::classify(const DataPoint& dataPoint, const ActivationType& activation) { /// classify a single data point
	DataPoint result = dataPoint; /// get the data point
	result.SetOutputs(calculateOutputs(dataPoint.GetInputs(), activation)); /// set the classification
	return result; /// return the classification
}



const LayerData& NeuralNetwork::getLayerData(int index) const { return this->layers[index].getLayerData(); }

const Layer& NeuralNetwork::getLayer(int index) const { return this->layers[index]; }

const vector<Layer>& NeuralNetwork::getLayers() const { return this->layers; }

int NeuralNetwork::getNumberLayers() const { return this->layers.size(); }

int NeuralNetwork::getLayerIndex(const Layer& layer) const {
	for (int i = 0; i < this->layers.size(); i++) {
		if (&this->layers[i] == &layer) {
			return i;
		}
	}
	return -1;
}
