

#include "MainLoop.h"
#include <thread>


using namespace std;
using namespace sf;

MainLoop* MainLoop::getInstance() {
	static MainLoop instance;
	return &instance;
}


void MainLoop::initializeClassDropdown() {
    this->classDropdown.addOption(new ColorButton(0, 0, 150, 50, Color(50, 50, 50), Color(105, 105, 105), Color::Blue, [this]() { this->dataColor = Color(0, 0, 255); }, this->textureManager->getFont("roboto"), ""));
    this->classDropdown.addOption(new ColorButton(0, 0, 150, 50, Color(50, 50, 50), Color(105, 105, 105), Color::Red, [this]() { this->dataColor = Color(255, 0, 0); }, this->textureManager->getFont("roboto"), ""));
    
}


MainLoop::MainLoop() : window(sf::VideoMode(1920, 1080), "Neural Network"), network({ 2, 5, 2 }), networkVisualizer(network, sf::Vector2f(450, 200)), textureManager(TextureManager::getInstance()), gameState(GameState::InputingData), learningRateSlider(720, 40, 200, 20, learningRate, "Learning Rate"),
classDropdown(10, 800, 150, 50, Color(50, 50, 50), Color(105, 105, 105), Color(25, 25, 25), {} , TextureManager::getInstance()->getFont("roboto"), ""), costGraph(200, 25, 200, 50) { 
	this->window.setFramerateLimit(500);
	this->circles = map<DataPoint, sf::CircleShape>();
	this->dataPoints = set<DataPoint>();
	this->epoch = 0;
	this->learningRate = 0.1;
	this->dataSetEmpty = true;
	this->numberClasses = 2;
	this->dataColor = Color(255, 0, 0);
	this->classColors = { Color(0, 0, 255),  // Red
        Color(255, 0, 0),       // Blue
        Color(0, 255, 0),       // Green
        Color(255, 255, 0),     // Yellow
        Color(128, 0, 128),     // Purple
        Color(255, 165, 0)      // Orange
    };
	this->maxCost = 0;
	this->graphDisplayed = true;
	this->statsX = StandardizationCalculator();
	this->statsY = StandardizationCalculator();

	/// class dropdown
	this->initializeClassDropdown();

	/// cost graph
	
    // Create the two views
    this->plotView = sf::View(sf::FloatRect(0, 0, window.getSize().x / 2, window.getSize().y));
    this->networkView = sf::View(sf::FloatRect(0, 0, window.getSize().x / 2, window.getSize().y));

    // Position the views side by side
    this->plotView.setViewport(sf::FloatRect(0.5f, 0, 0.5f, 1));
	this->plotView.move(window.getSize().x / 2, 0);
    this->networkView.setViewport(sf::FloatRect(0, 0, 0.5f, 1));
    
    /// highlight
    
    this->clock.restart();

    this->highlightMode = false;

    /// buttons and sliders
	this->resetInputsButton = Button(10, 900, 150, 50, Color(50, 50, 50), Color(105, 105, 105), [this]() { this->resetInputs(); }, TextureManager::getInstance()->getFont("roboto"), "Reset Inputs");
    this->resetTrainingButton = Button(200, 900, 180, 50, Color(50, 50, 50), Color(105, 105, 105), [this]() { this->resetTraining(); }, TextureManager::getInstance()->getFont("roboto"), "Reset Training");
    this->toggleHighlightMode = Button(420, 900, 200, 50, Color(50, 50, 50), Color(105, 105, 105), [this]() { this->toggleHighlight(); }, TextureManager::getInstance()->getFont("roboto"), "Toggle Highlight");
    this->nextPointButton = Button(100 + this->networkVisualizer.getTotalWidth() + 50,  400 , 25, 150, Color(50, 50, 50), Color(105, 105, 105), [this]() {
        /// sets current data point to the next one in the set if possible
        try {
            if (this->dataPoints.empty()) 
                throw out_of_range("No elements in the set");

            auto nextIt = next(this->currentDataPoint); /// get the next iterator
            if (nextIt != this->dataPoints.end()) { /// if the next iterator is not the end
                this->currentDataPoint = nextIt; /// set the current data point to the next one
            }
            else {
                throw out_of_range("No next element in the set");
            }
        }
        catch (const exception& e) {
            // Handle the exception
            cerr << "Exception: " << e.what() << std::endl;
        }
    }, this->textureManager->getFont("roboto"), ">");

    this->previousPointButton = Button(100, 400, 25, 150, Color(50, 50, 50), Color(105, 105, 105), [this]() {
        /// sets current data point to the previous one in the set if possible
        try {
            if (this->dataPoints.empty())
                throw out_of_range("No elements in the set");
            if (this->currentDataPoint != this->dataPoints.begin()) { /// if the current data point is not the first
                --this->currentDataPoint; /// move the current data point to the previous one
            }
            else {
                throw out_of_range("No previous element in the set");
            }
        }
        catch (const exception& e) {
            // Handle the exception
            cerr << "Exception: " << e.what() << std::endl;
        }
    }, this->textureManager->getFont("roboto"), "<");

	

}



void MainLoop::updateHighlight() {
    if (!this->dataSetEmpty) {
        // Calculate a sinusoidal value based on the elapsed time
        float time = this->clock.getElapsedTime().asSeconds();
        float alpha = 127.5f * sin(2.0f * 3.14159f * time) + 127.5f;

        // Get the circle corresponding to the current data point
        sf::CircleShape& circle = this->circles[*this->currentDataPoint];

        // Set the position and radius of the highlight
        this->highlight.setRadius(circle.getRadius());
        this->highlight.setPosition(circle.getPosition().x + circle.getRadius() - this->highlight.getRadius(),
        circle.getPosition().y + circle.getRadius() - this->highlight.getRadius());
        this->highlight.setFillColor(Color::Transparent);
        this->highlight.setOutlineThickness(3);
        this->highlight.setOutlineColor(sf::Color(255, 255, 0, static_cast<sf::Uint8>(alpha)));
    }
}


void MainLoop::toggleHighlight() {
	this->highlightMode = !this->highlightMode;
}

/// for standardization
double MainLoop::calculateMeanX() {
    return this->statsX.getMean();
}

double MainLoop::calculateStdDevX() {
    return this->statsX.getStandardDeviation();
}

double MainLoop::calculateMeanY() {
    return this->statsY.getMean();
}

double MainLoop::calculateStdDevY() {
    return this->statsY.getStandardDeviation();
}

void MainLoop::resetInputs() {
	this->dataPoints.clear();
	this->circles.clear();
	this->gameState = GameState::InputingData;
	this->dataSetEmpty = true;
    this->epoch = 0;
    this->maxCost = 0;
	this->costGraph.reset();
}

void MainLoop::resetTraining() {
	this->network.InitializeWeightsAndBiases();
	this->epoch = 0;
	this->maxCost = 0;
	this->gameState = GameState::InputingData;
    this->costGraph.reset();
   
}

void MainLoop::drawRightHalfScreen() {
    this->window.setView(this->plotView);
	sf::RectangleShape rectangle;
	rectangle.setSize(sf::Vector2f(this->window.getSize().x / 2, this->window.getSize().y));
	rectangle.setPosition(this->window.getSize().x / 2, 0);
	rectangle.setFillColor(Color::White);

	this->window.draw(rectangle);
}

const double& MainLoop::getCostPercentage() {
	if (this->maxCost != 0)
	    return this->network.CostFunction(this->dataPoints) / this->maxCost;
	else
		return 0;
}

void MainLoop::visualizePlot()
{
    int pixelSize = 16; // Adjust this to make the pixels larger or smaller
    int width = this->window.getSize().x / (2 * pixelSize);
    int height = this->window.getSize().y / pixelSize;

    // Define a vector of colors for each output
    vector<Color> colors = { Color::Blue, Color::Red, Color::Green, Color::Yellow, Color::Magenta, Color::Cyan };

    VertexArray container(Points, width * height);
    for (int i = 0; i < width * height; i++) {
        container[i].position = Vector2f((i % width) * pixelSize, (i / width) * pixelSize) + Vector2f(this->window.getSize().x / 2, 0);
        container[i].color = Color::White;
    }
    double input_1, input_2;
    DataPoint output;

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            input_1 = x / static_cast<double>(width);
            input_2 = y / static_cast<double>(height);
            vector<double> outputs = this->network.CalculateOutputs({ input_1, input_2 });

            // Find the index of the maximum output
            int maxIndex = max_element(outputs.begin(), outputs.end()) - outputs.begin();

            // Set all outputs to 0, except the maximum one
            for (int i = 0; i < outputs.size(); i++) {
                outputs[i] = (i == maxIndex) ? 1 : 0;
            }

            output = DataPoint({ input_1, input_2 }, outputs);

            Vertex& pixel = container[x + y * width];
            pixel.color = colors[maxIndex];
        }
    }

    // Draw the container
    for (int i = 0; i < container.getVertexCount(); i++) {
        sf::RectangleShape pixel(sf::Vector2f(pixelSize, pixelSize));
        pixel.setPosition(container[i].position);
        pixel.setFillColor(container[i].color);
        this->window.draw(pixel);
    }

    this->network.CalculateOutputs((*this->currentDataPoint).getInputs());
}


void MainLoop::eventHandler(sf::Event& event, bool& space) {
    if (event.type == sf::Event::Closed) {
        this->window.close();
    }

    /// buttons and sliders
    sf::Vector2i pixelPos = sf::Mouse::getPosition(this->window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos, this->networkView);
	sf::Vector2f worldPosDefault = window.mapPixelToCoords(pixelPos, this->window.getDefaultView());

    sf::Event worldEvent = event;
	Event defaultWorldEvent = event;

    worldEvent.mouseButton.x = worldPos.x;
    worldEvent.mouseButton.y = worldPos.y;
	defaultWorldEvent.mouseButton.x = worldPosDefault.x;
	defaultWorldEvent.mouseButton.y = worldPosDefault.y;

	this->resetInputsButton.checkEvents(this->window, defaultWorldEvent);
	this->resetTrainingButton.checkEvents(this->window, defaultWorldEvent);
    this->toggleHighlightMode.checkEvents(this->window, defaultWorldEvent);

    if (this->highlightMode) {
        this->previousPointButton.checkEvents(this->window, worldEvent);
        this->nextPointButton.checkEvents(this->window, worldEvent);
    }

	


	this->networkVisualizer.checkEvents(this->window, event, this->networkView);

	this->learningRateSlider.handleEvent(event);

	this->classDropdown.checkEvents(this->window, event);


    /*if (event.KeyPressed && event.key.code == sf::Keyboard::L && space == false)
    {
        DataPoint point;
        this->dataPoints = point.GenerateDataPointsPixel(500);
        for (auto& dataPoint : this->dataPoints) 
        {
            if (dataPoint.GetOutputs()[0] == 1)
            {
                double normalizedX = dataPoint.GetInputs()[0] / static_cast<double>(this->window.getSize().x / 2) - 1;
                double normalizedY = dataPoint.GetInputs()[1] / static_cast<double>(this->window.getSize().y);
                this->circles.push_back(sf::CircleShape(10));
                this->circles.back().setFillColor(sf::Color::Blue);
                this->circles.back().setPosition(dataPoint.GetInputs()[0], dataPoint.GetInputs()[1]);
                this->circles.back().setOutlineThickness(2); // Set the thickness of the outline
                this->circles.back().setOutlineColor(sf::Color::White); // Set the color of the outline
                dataPoint.SetInputs({ normalizedX, normalizedY });
            }
            else if (dataPoint.GetOutputs()[1] == 1)
            {
                this->circles.push_back(sf::CircleShape(10));
                this->circles.back().setFillColor(sf::Color::Red);
                this->circles.back().setPosition(dataPoint.GetInputs()[0], dataPoint.GetInputs()[1]);
                this->circles.back().setOutlineThickness(2); // Set the thickness of the outline
                this->circles.back().setOutlineColor(sf::Color::White); // Set the color of the outline
                double normalizedX = dataPoint.GetInputs()[0] / static_cast<double>(this->window.getSize().x / 2) - 1;
                double normalizedY = dataPoint.GetInputs()[1] / static_cast<double>(this->window.getSize().y);
                dataPoint.SetInputs({ normalizedX, normalizedY });
            }
        }
        this->gameState = GameState::Training;
        space = true;
    }
    */
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.x > this->window.getSize().x / 2 && this->gameState == GameState::InputingData) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::CircleShape circle(10);
            circle.setFillColor(this->dataColor);
            circle.setPosition(event.mouseButton.x, event.mouseButton.y);
            circle.setOutlineThickness(2); // Set the thickness of the outline
            circle.setOutlineColor(sf::Color::White); // Set the color of the outline

            double normalizedX = event.mouseButton.x / static_cast<double>(this->window.getSize().x / 2) - 1;
            double normalizedY = event.mouseButton.y / static_cast<double>(this->window.getSize().y);

            double x = event.mouseButton.x;
            double y = event.mouseButton.y;

            this->statsX.add(x);
            this->statsY.add(y);

			/// double meanX = this->calculateMeanX(); /// for standardization
			/// double stdDevX = this->calculateStdDevX(); /// for standardization
			/// double meanY = this->calculateMeanY(); /// for standardization
			/// double stdDevY = this->calculateStdDevY(); /// for standardization

            /// double standardizedX = (event.mouseButton.x - meanX) / stdDevX;
            /// double standardizedY = (event.mouseButton.y - meanY) / stdDevY;


            for (int i = 0; i < this->numberClasses; i++) {
                if (this->dataColor == this->classColors[i]) {
                    vector<double> outputs(this->numberClasses, 0);
                    outputs[i] = 1;
                    DataPoint point({ normalizedX, normalizedY }, outputs); /// to change
                    this->dataPoints.insert(point);
                    this->circles[point] = circle;
                    if (this->dataSetEmpty)
                        this->currentDataPoint = this->dataPoints.begin(); /// we must initialize this
                }
            }
            this->dataSetEmpty = false;
        }
    }

    if (event.type == sf::Event::MouseWheelScrolled)
    {
        if (event.mouseWheelScroll.delta > 0)
            this->networkView.zoom(0.9f); // Zoom in
        else if (event.mouseWheelScroll.delta < 0)
            this->networkView.zoom(1.1f); // Zoom out
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter && this->gameState == GameState::InputingData) {
        this->gameState = GameState::Training;
		this->maxCost = this->network.CostFunction(this->dataPoints);
        this->costGraph.reset();
    }

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::G) /// for testing purposes
    {
        this->costGraph.reset();
	}
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C) /// for testing purposes
    {
		this->graphDisplayed = !this->graphDisplayed;
    }


}
void MainLoop::zoomHandler() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        this->networkView.move(-10, 0); // Move left
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        this->networkView.move(10, 0); // Move right
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        this->networkView.move(0, -10); // Move up
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        this->networkView.move(0, 10); // Move down
    }


}


void MainLoop::trainingState() {

    this->epoch++;
    
    /// we will update the gradient for a specific dta point last for visualization purposes - should not affect performance by much
    for (auto it = this->dataPoints.begin(); it != this->dataPoints.end(); ++it) {
        const auto& dataPoint = *it;

        if (it == this->currentDataPoint)
            continue;

        this->network.UpdateGradientsForDataPoint(dataPoint); /// update the gradients for the data point
    }
    this->network.UpdateGradientsForDataPoint(*this->currentDataPoint); /// update the gradients for the current data point

    this->network.ApplyAllGradients(this->learningRate / this->dataPoints.size()); /// apply the gradients to the weights and biases

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        this->visualizePlot();
    }

}


void MainLoop::updateNumberOfClasses() {
	this->numberClasses = this->networkVisualizer.getNumberOutputs();
    if (this->numberClasses < this->classDropdown.getNumButtons())
    {
		for (int i = this->numberClasses; i < this->classDropdown.getNumButtons(); i++)
		{
			this->classDropdown.removeOption(i);
		}
	}
    else if (this->numberClasses > this->classDropdown.getNumButtons())
    {
        for (int i = this->classDropdown.getNumButtons(); i < this->numberClasses; i++)
        {
			this->classDropdown.addOption(new ColorButton(0, 0, 150, 50, Color(50, 50, 50), Color(105, 105, 105), this->classColors[i], [this, i]() { this->dataColor = this->classColors[i]; }, this->textureManager->getFont("roboto"), ""));
        }
    }
}

void MainLoop::run()
{

    this->network.InitializeWeightsAndBiases();

    this->gameState = GameState::InputingData;

    bool space = false;

    sf::Text costFunctionText;
	sf::Text epochText;
    sf::Font font;
    font = this->textureManager->getFont("roboto");
    costFunctionText.setFont(font);
    costFunctionText.setPosition(10, 10);
    costFunctionText.setCharacterSize(24);
    costFunctionText.setFillColor(sf::Color::White);
	epochText.setFont(font);
	epochText.setPosition(10, 40);
	epochText.setCharacterSize(24);
	epochText.setFillColor(sf::Color::White);
    this->previousPointButton.setPosition(50, 300);



    while (this->window.isOpen()) {

        sf::Event event;
        while (this->window.pollEvent(event))
        {
            this->eventHandler(event, space);
        }

        ////updates
        this->updateNumberOfClasses();
        if(this->highlightMode) this->updateHighlight();
		if(this->graphDisplayed) this->costGraph.update(this->getCostPercentage()); /// cost graph update

        this->nextPointButton.setPosition(100 + this->networkVisualizer.getTotalWidth() + 150, 300);

        /// other inputs
        this->zoomHandler();

        this->window.clear();
        this->drawRightHalfScreen();
        this->window.setView(this->plotView); /// for the plot view

        /// for the next and prev buttons to work
        if (!this->dataSetEmpty)
            network.CalculateOutputs((*this->currentDataPoint).getInputs());

        if (this->gameState == GameState::Training) {
            this->trainingState(); /// this handles all the training logic and drawing
            this->window.draw(this->highlight);
        }
        else if(!this->dataSetEmpty){
            network.CalculateOutputs((*this->currentDataPoint).getInputs());
			if (this->highlightMode) this->window.draw(this->highlight);
                
        }
        for (auto& pair : this->circles) {
            this->window.draw(pair.second);
        }
        

        /// cost text
        

        /// neural net visualization and neural net view
        this->window.setView(this->networkView);
        this->networkVisualizer.draw(this->window, this->gameState, this->dataSetEmpty); /// this visualizes the neural network
        
        if (this->highlightMode) {
			this->previousPointButton.draw(this->window);
            this->nextPointButton.draw(this->window);
        }
			
        /// text and buttons
        this->window.setView(this->window.getDefaultView());
        costFunctionText.setString("Loss: " + std::to_string(this->getCostPercentage()));
        this->window.draw(costFunctionText);
		epochText.setString("Epoch: " + std::to_string(this->epoch));
		this->window.draw(epochText);

		this->resetInputsButton.draw(this->window);
		this->resetTrainingButton.draw(this->window);
		this->learningRateSlider.draw(this->window);
		this->classDropdown.draw(this->window);
		this->toggleHighlightMode.draw(this->window);

        /// graph
		if(this->graphDisplayed) this->costGraph.draw(this->window);

        this->window.display();

    }
}
