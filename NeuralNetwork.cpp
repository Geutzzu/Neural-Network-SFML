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

void NeuralNetwork::InitializeWeightsAndBiases() { /// initialize the weights and biases of the neural network
	for (int i = 0; i < this->layers.size(); i++) { /// for each layer
		this->layers[i].InitializeWeightsAndBiases(); /// initialize the weights and biases of the layer
	}
}

vector<double> NeuralNetwork::CalculateOutputs(const vector<double>& inputs) { /// goes through each layer and calculates the outputs
	vector<double> outputs = inputs; /// initialize the outputs to the inputs
	for (int i = 0; i < this->layers.size(); i++) { /// for each layer
		outputs = this->layers[i].CalculateOutputs(outputs); /// calculate the outputs of the layer
	}

	return outputs;
}

vector<double> NeuralNetwork::CalculateNeuronOutputs(const vector<double>& inputs, int specificLayer, int activeNeuron) {
	vector<double> outputs = inputs;
	for (int i = 0; i < this->layers.size(); i++) {
		outputs = this->layers[i].CalculateOutputs(outputs);
		if (i == specificLayer) {
			for (int j = 0; j < outputs.size(); j++) {
				if (j != activeNeuron) {
					outputs[j] = 0;
				}
			}
		}
	}
	return outputs;
}

int NeuralNetwork::Classify(const vector<double>& inputs) { /// classify the inputs from 1 
	vector<double> outputs = CalculateOutputs(inputs); /// calculate the outputs of the neural network

	int classification = 0; /// initialize the classification to 0
	for (int i = 1; i < outputs.size(); i++) { /// for each output
		if (outputs[i] > outputs[classification]) { /// if the output is greater than the current classification
			classification = i; /// set the classification to the output
		}
	}
	return classification; /// return the classification
}

double NeuralNetwork::CostFunction(const DataPoint& dataPoint) { /// calculate the cost of the data point
	vector<double> outputs = CalculateOutputs(dataPoint.GetInputs()); /// calculate the outputs of the neural network
	double cost = 0; /// initialize the cost to 0
	Cost<CostType::MeanSquareError> costFunction; /// initialize the cost function
	for (int i = 0; i < outputs.size(); i++) { /// for each output
		cost += costFunction.Function({ outputs[i] }, { dataPoint.GetOutputs()[i] }); /// the cost between the output and the expected output
	}
	return cost; /// return the cost
}

double NeuralNetwork::CostFunction(const set<DataPoint>& dataPoints) { /// calculate the cost of the data points
	double cost = 0; /// initialize the cost to 0
	for (const auto& dataPoint : dataPoints) { /// for each data point
		cost += CostFunction(dataPoint); /// add the cost of the data point to the cost
	}
	return cost; /// return the cost
}

void NeuralNetwork::UpdateGradientsForDataPoint(const DataPoint& dataPoint) { /// update the gradients for the data point

	vector<double> outputs = CalculateOutputs(dataPoint.GetInputs()); /// calculate the outputs of the neural network

	vector<double> specificValues; /// initialize the specific values - specific for each terminal node

	specificValues = CalculateOutputSpecificValues(dataPoint.GetOutputs()); /// calculate the specific values for the terminal node

	this->layers[this->layers.size() - 1].UpdateGradients(specificValues); /// update the gradients for the terminal node

	for (int i = this->layers.size() - 2; i >= 0; i--) { /// for each hidden layer
		specificValues = CalculateHiddenLayerOutputSpecificValues(specificValues, i + 1); /// calculate the specific values for the hidden layer
		this->layers[i].UpdateGradients(specificValues); /// update the gradients for the hidden layer
	}

}


void NeuralNetwork::Learn(vector<DataPoint> trainingBatch, double learningRate, double momentum) {
	for (int i = 0; i < trainingBatch.size(); i++) {
		this->UpdateGradientsForDataPoint(trainingBatch[i]);
	}
	this->ApplyAllGradients(learningRate / trainingBatch.size(), momentum);
}

void NeuralNetwork::ApplyAllGradients(double learningRate, double momentum) {
	for (int i = 0; i < this->layers.size(); i++) {
		this->layers[i].ApplyGradients(learningRate, momentum);
	}
}



vector <double> NeuralNetwork::CalculateOutputSpecificValues(const vector<double>& expectedOutputs) {

	vector<double> specificValues(expectedOutputs.size()); /// initialize the specific values - specific for each terminal node

	Activation<ActivationType::Sigmoid> function; /// initialize the activation function
	Cost<CostType::MeanSquareError> costFunction; /// initialize the cost function

	for (int i = 0; i < specificValues.size(); i++) { /// for each specific value
		double costDerivative = costFunction.Derivative(this->GetLayerData(this->layers.size() - 1).GetActivation(i), expectedOutputs[i]); /// calculate the cost derivative
		double activationDerivative = function.Derivative(this->GetLayerData(this->layers.size() - 1).GetWeightedInput(i)); /// calculate the activation derivative
		specificValues[i] = costDerivative * activationDerivative; /// set the specific value to the cost derivative * activation derivative
	}
	return specificValues; /// return the specific values

}


vector <double> NeuralNetwork::CalculateHiddenLayerOutputSpecificValues(const vector<double>& oldSpecificValues, int oldIndex) { /// actually previous specific values

	vector<double> newSpecificValues(this->GetLayerData(oldIndex - 1).GetNumberOutputs()); /// initialize the specific values - specific for each terminal node

	for (int i = 0; i < newSpecificValues.size(); i++) { /// for each specific value
		double sum = 0; /// initialize the sum to 0
		for (int j = 0; j < oldSpecificValues.size(); j++) { /// for each old specific value
			sum += oldSpecificValues[j] * this->layers[oldIndex].GetWeight(i, j); /// add the old specific value * the weight to the sum
		}
		sum *= Activation<ActivationType::Sigmoid>().Derivative(this->GetLayerData(oldIndex - 1).GetWeightedInput(i)); /// multiply the sum by the derivative of the activation function
		newSpecificValues[i] = sum; /// set the specific value to the sum
	}

	return newSpecificValues; /// return the specific values

}



vector <DataPoint> NeuralNetwork::ClassifyAll(const vector<DataPoint>& dataPoints) { /// classify all the data points
	vector<DataPoint> predictedValues(dataPoints.size()); /// initialize the classifications
	for (int i = 0; i < dataPoints.size(); i++) { /// for each data point
		DataPoint result = dataPoints[i]; /// get the data point

		result.SetOutputs(CalculateOutputs(dataPoints[i].GetInputs())); /// set the classification

		/*for (int j = 0; j < result.GetOutputs().size(); j++)
			cout << result.GetOutputs()[j] << " ";*/

		predictedValues[i] = result; /// set the classification
	}
	return predictedValues; /// return the classifications
}

DataPoint NeuralNetwork::Classify(const DataPoint& dataPoint) { /// classify a single data point
	DataPoint result = dataPoint; /// get the data point
	result.SetOutputs(CalculateOutputs(dataPoint.GetInputs())); /// set the classification
	return result; /// return the classification
}



const LayerData& NeuralNetwork::GetLayerData(int index) const { return this->layers[index].GetLayerData(); }

const Layer& NeuralNetwork::GetLayer(int index) const { return this->layers[index]; }

const vector<Layer>& NeuralNetwork::GetLayers() const { return this->layers; }

int NeuralNetwork::GetNumberLayers() const { return this->layers.size(); }

int NeuralNetwork::GetLayerIndex(const Layer& layer) const {
	for (int i = 0; i < this->layers.size(); i++) {
		if (&this->layers[i] == &layer) {
			return i;
		}
	}
	return -1;
}
