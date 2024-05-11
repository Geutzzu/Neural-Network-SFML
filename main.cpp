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
#include <iomanip>
#include <unordered_map>
#include <sstream>

#include "Layer.h"
#include "NeuralNetwork.h"
#include "DataPoint.h"
#include "LayerData.h"
#include "Cost.h"
#include "Activation.h"
#include "NetworkData.h"
#include "NetworkVisualizer.h"
#include "TextureManager.h"
#include "Neuron.h"
#include "MainLoop.h"
#include "ColorButton.h"
#include "Dropdown.h"
#include "Graph.h"



using namespace std;
using namespace sf;



int main() {


	TextureManager* textureManager = TextureManager::getInstance();
	textureManager->loadFonts();

	MainLoop* mainLoop = MainLoop::getInstance();
	mainLoop->run();



	/*

	RenderWindow window(VideoMode(1920, 1080), "Neural Network Visualizer");
	/// ColorButton(float x, float y, float width, float height, sf::Color color, sf::Color hoverColor, sf::Color displayColor, function<void()> onClickFunction, const sf::Font & font, const string & text);

	vector <Button*> buttons;

	buttons.push_back(new ColorButton(100, 100, 100, 50, Color::Red, Color::Green, Color::Blue, []() { cout << "Button 1" << endl; }, textureManager->getFont("roboto"), "Button 1"));
	buttons.push_back(new ColorButton(100, 200, 100, 50, Color::Red, Color::Green, Color::Blue, []() { cout << "Button 2" << endl; }, textureManager->getFont("roboto"), "Button 2"));
	buttons.push_back(new ColorButton(100, 300, 100, 50, Color::Red, Color::Green, Color::Blue, []() { cout << "Button 3" << endl; }, textureManager->getFont("roboto"), "Button 3"));
	buttons.push_back(new ColorButton(100, 400, 100, 50, Color::Red, Color::Green, Color::Blue, []() { cout << "Button 4" << endl; }, textureManager->getFont("roboto"), "Button 4"));


	
	/// Dropdown(float x, float y, float width, float height, sf::Color color, sf::Color hoverColor, sf::Color displayColor, const sf::Font& font, const string& text, const vector<Button*>& buttons);

	Dropdown dropdown1(100, 500, 100, 50, Color::Red, Color::Green, Color::Blue, buttons, textureManager->getFont("roboto"), "Dropdown");


	Button* testButton = new ColorButton(100, 20, 100, 50, Color::Red, Color::Green, Color::Blue, []() { cout << "Test Button" << endl; }, textureManager->getFont("roboto"), "Test Button");
	Button* testButton2 = new ColorButton(100, 900, 100, 50, Color::Red, Color::Green, Color::Blue, []() { cout << "Test Button 2" << endl; }, textureManager->getFont("roboto"), "Test Button 2");
	dropdown1.addOption(testButton);
	dropdown1.addOption(testButton2);

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}

			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Key::Space) {
					dropdown1.removeOption(0);
				}
			}

			dropdown1.checkEvents(window, event);
		}
		window.clear();

		dropdown1.draw(window);
		window.display();
	}

	
    /*

	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Slider Example");
	/// using sliders to modify the weights and biases

	TextureManager* textureManager = TextureManager::getInstance();
	textureManager->loadFonts();


	/// Seting the data points
    DataPoint point;
    NeuralNetwork network({ 2, 3, 2 });
	vector<CircleShape> circles;
    GameState gameState = GameState::InputingData;

 
    vector<DataPoint> dataPoints;
    network.InitializeWeightsAndBiases(); /// initialize the weights and biases of the neural network

    NetworkVisualizer<Neuron> visualizer(network, Vector2f(500, 100));


	InitializeTrainingData(network, dataPoints, window, circles, gameState, visualizer);
    */





    /*
    /// normalize data points
	for (int i = 0; i < dataPoints.size(); i++) {
		double normalizedX = dataPoints[i].GetInputs()[0] / static_cast<double>(window.getSize().x / 2) - 1;
		double normalizedY = dataPoints[i].GetInputs()[1] / static_cast<double>(window.getSize().y);
		dataPoints[i].SetInputs({ normalizedX, normalizedY });
	}

	/// data sample 
	

	cout << "Data points:" << endl;
	for (int i = 0; i < dataPoints.size(); i++) {
		cout << "Point: " << i << " Inputs: " << dataPoints[i].GetInputs()[0] << " " << dataPoints[i].GetInputs()[1] << " Outputs: " << dataPoints[i].GetOutputs()[0] << " " << dataPoints[i].GetOutputs()[1] << endl;
	}

	cout << endl;


	
    
	

    cout << "Weights and biases:" << endl;
    for (int i = 0; i < network.GetLayers().size(); i++) {
        cout << "Layer: " << i << endl;
        for (int j = 0; j < network.GetLayers()[i].GetWeights().size(); j++) {
            cout << "Weight: " << j << " " << network.GetLayers()[i].GetWeights()[j] << endl;
        }
        for (int j = 0; j < network.GetLayers()[i].GetBiases().size(); j++) {
            cout << "Bias: " << j << " " << network.GetLayers()[i].GetBiases()[j] << endl;
        }
    }

    cout << endl;
    cout << endl;

    for (int epoch = 0; epoch < 1000; epoch++)
    {
        for (int i = 0; i < dataPoints.size(); i++) { /// for each data point
            network.UpdateGradientsForDataPoint(dataPoints[i]); /// update the gradients for the data point
        }
        network.ApplyAllGradients(0.1); /// apply the gradients to the weights and biases


    }
  
    vector <DataPoint> results;


	vector <DataPoint> dataPointsToClassify = point.GenerateDataPointsPixel(500);

	for (int i = 0; i < dataPointsToClassify.size(); i++) {
		double normalizedX = dataPointsToClassify[i].GetInputs()[0] / static_cast<double>(window.getSize().x / 2) - 1;
		double normalizedY = dataPointsToClassify[i].GetInputs()[1] / static_cast<double>(window.getSize().y);
		dataPointsToClassify[i].SetInputs({ normalizedX, normalizedY });
	}
    
	results = network.ClassifyAll(dataPointsToClassify);


	cout << "Weights and biases:" << endl;  
	for (int i = 0; i < network.GetLayers().size(); i++) {
		cout << "Layer: " << i << endl;
		for (int j = 0; j < network.GetLayers()[i].GetWeights().size(); j++) {
			cout << "Weight: " << j << " " << network.GetLayers()[i].GetWeights()[j] << endl;
		}
		for (int j = 0; j < network.GetLayers()[i].GetBiases().size(); j++) {
			cout << "Bias: " << j << " " << network.GetLayers()[i].GetBiases()[j] << endl;
		}
	}


	cout << "Data points:" << endl;



	for (int i = 0; i < dataPointsToClassify.size(); i++) {

		double first = results[i].GetOutputs()[0];
		double second = results[i].GetOutputs()[1];

		double firstClass = first > second ? 1 : 0;
		double secondClass = first > second ? 0 : 1;

		results[i].SetOutputs({ first, second });

		cout << "Point: " << i << " Expected: " << dataPointsToClassify[i].GetOutputs()[0] << " " << dataPointsToClassify[i].GetOutputs()[1] << " Outputs: " << results[i].GetOutputs()[0] << " " << results[i].GetOutputs()[1] << "Values: " << firstClass << ' ' << secondClass <<   endl;
	}

	point = DataPoint({ 0.5, 0.5 }, { 1, 0 });

 

	/// for each circle that we place on the screen we create a data point
    
	*/

	return 0;
}