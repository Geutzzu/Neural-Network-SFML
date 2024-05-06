#pragma once

#include <NeuralNetwork.h>
#include <LayerVisualizer.h>
#include <NetworkVisualizer.h>
#include <Neuron.h>
#include <set>
#include "Slider.h"
#include "Dropdown.h"

class MainLoop {
private:

	sf::RenderWindow window;
	NeuralNetwork network;
	NetworkVisualizer<Neuron> networkVisualizer;
	TextureManager* textureManager;
	GameState gameState;
	set<DataPoint> dataPoints;
	vector<CircleShape> circles;

	/// buttons and sliders
	Button resetInputsButton;
	Button resetTrainingButton;
	Slider learningRateSlider;
	Dropdown classDropdown;

	/// parameters
	int epoch;
	double learningRate;

	/// misc
	bool dataSetEmpty;
	Color dataColor;
	vector<Color> classColors;
	int numberClasses;


	/// views
	sf::View plotView;
	sf::View networkView;


	MainLoop();

public:

	MainLoop(const MainLoop& m) = delete;
	MainLoop& operator=(const MainLoop& m) = delete;

	static MainLoop* getInstance();

	/// initializing functions
	void initializeClassDropdown();

	/// functions for functionality
	void updateNumberOfClasses();
	void drawRightHalfScreen();
	void visualizePlot();
	void eventHandler(sf::Event& event, bool& space); /// to change the bool space
	void zoomHandler();
	void trainingState();
	void resetInputs();
	void resetTraining();


	/// main loop
	void run();


};
