#pragma once

#include "NeuralNetwork.h"
#include "LayerVisualizer.h"
#include "NetworkVisualizer.h"
#include "Neuron.h"
#include <set>
#include "Slider.h"
#include "Dropdown.h"
#include <map>
#include "Graph.h"
#include "StandardizationCalculator.h"

class MainLoop {
private:

	RenderWindow window;
	NeuralNetwork network;
	NetworkVisualizer<Neuron> networkVisualizer;
	TextureManager* textureManager;
	GameState gameState;
	set<DataPoint> dataPoints;
	map<DataPoint, CircleShape> circles;
	Graph costGraph;
	StandardizationCalculator statsX;
	StandardizationCalculator statsY;

	/// buttons and sliders
	Button resetInputsButton;
	Button resetTrainingButton;
	Slider learningRateSlider;
	Dropdown classDropdown;
	Button previousPointButton;
	Button nextPointButton;
	Button toggleHighlightMode;

	/// parameters
	int epoch;
	double learningRate;

	/// misc
	bool dataSetEmpty;
	bool highlightMode;
	bool graphDisplayed;
	Color dataColor;
	vector<Color> classColors;
	int numberClasses;
	set<DataPoint>::iterator currentDataPoint;
	double maxCost;

	/// plot visualization
	VertexArray pixels;
	int pixelSize;

	/// highlighting
	CircleShape highlight;
	Clock clock;

	/// views
	View plotView;
	View networkView;


	MainLoop();
	~MainLoop() = default;

public:

	MainLoop(const MainLoop& m) = delete;
	MainLoop& operator=(const MainLoop& m) = delete;

	static MainLoop* getInstance();

	/// initializing functions
	void initializeClassDropdown();

	/// functions for functionality
	void updateHighlight();
	void updateNumberOfClasses();
	void drawRightHalfScreen();
	void visualizePlot();
	void eventHandler(sf::Event& event, bool& space); /// to change the bool space
	void zoomHandler();
	void trainingState();
	void resetInputs();
	void resetTraining();
	void toggleHighlight();
	const double& getCostPercentage();

	/// standardization
	double calculateMeanX();
	double calculateMeanY();
	double calculateStdDevX();
	double calculateStdDevY();


	/// main loop
	void run();


};
