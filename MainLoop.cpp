

#include "MainLoop.h"
#include <thread>


using namespace std;
using namespace sf;

MainLoop* MainLoop::getInstance() {
    static MainLoop instance;
    return &instance;
}

void MainLoop::initializeClassDropdown() {
    this->classDropdown.addOption(new ColorButton(0, 0, 200, 50, Color(50, 50, 50), Color(105, 105, 105), Color::Blue, [this]() { this->dataColor = Color(0, 0, 255); }, TextureManager::getInstance()->getFont("roboto"), ""));
    this->classDropdown.addOption(new ColorButton(0, 0, 200, 50, Color(50, 50, 50), Color(105, 105, 105), Color::Red, [this]() { this->dataColor = Color(255, 0, 0); }, TextureManager::getInstance()->getFont("roboto"), ""));
}

void MainLoop::initializeActivationDropdown() {
    this->activationDropdown.addOption(new Button(0, 0, this->normalButtonWidth, this->normalButtonHeight, this->buttonColor, this->hoverColor, [this]() { this->activationType = ActivationType::ReLU; }, TextureManager::getInstance()->getFont("roboto"), "ReLU"));
    this->activationDropdown.addOption(new Button(0, 0, this->normalButtonWidth, this->normalButtonHeight, this->buttonColor, this->hoverColor, [this]() { this->activationType = ActivationType::Sigmoid; }, TextureManager::getInstance()->getFont("roboto"), "Sigmoid"));
    this->activationDropdown.addOption(new Button(0, 0, this->normalButtonWidth, this->normalButtonHeight, this->buttonColor, this->hoverColor, [this]() { this->activationType = ActivationType::TanH; }, TextureManager::getInstance()->getFont("roboto"), "TanH"));
    this->activationDropdown.addOption(new Button(0, 0, this->normalButtonWidth, this->normalButtonHeight, this->buttonColor, this->hoverColor, [this]() { this->activationType = ActivationType::SiLU; }, TextureManager::getInstance()->getFont("roboto"), "SiLU"));
    this->activationDropdown.addOption(new Button(0, 0, this->normalButtonWidth, this->normalButtonHeight, this->buttonColor, this->hoverColor, [this]() { this->activationType = ActivationType::Softmax; }, TextureManager::getInstance()->getFont("roboto"), "Softmax"));
}

void MainLoop::initializeCostDropdown() {
    this->costDropdown.addOption(new Button(0, 0, this->normalButtonWidth, this->normalButtonHeight, this->buttonColor, this->hoverColor, [this]() { this->costType = CostType::MeanSquareError; }, TextureManager::getInstance()->getFont("roboto"), "MSE"));
    this->costDropdown.addOption(new Button(0, 0, this->normalButtonWidth, this->normalButtonHeight, this->buttonColor, this->hoverColor, [this]() { this->costType = CostType::CrossEntropy; }, TextureManager::getInstance()->getFont("roboto"), "Cross Entropy"));
}


MainLoop::MainLoop() : window(sf::VideoMode(1920, 1080), "Neural Network", Style::Fullscreen), network({ 2, 3, 2 }), networkVisualizer(NetworkVisualizer<Neuron>(network, Vector2f(450, 350), this->classColors)), gameState(GameState::InputingData), learningRate(0.1), momentum(0.9), learningRateSlider(750, 40, 200, 15, this->learningRate, "Learning Rate", 0, 2),
normalButtonHeight(40), normalButtonWidth(135),
momentumSlider(750, 100, 200, 15, this->momentum, "Momentum", 0, 1),
classDropdown(20, 920, 200, 50, Color(50, 50, 50), Color(105, 105, 105), Color(25, 25, 25), TextureManager::getInstance()->getFont("roboto"), ""),
activationDropdown(590, 20, this->normalButtonWidth, this->normalButtonHeight, Color(128, 128, 128), Color(160, 160, 160), Color::Transparent, TextureManager::getInstance()->getFont("roboto"), "Sigmoid", true),
costDropdown(430, 20, this->normalButtonWidth, this->normalButtonHeight, Color(128, 128, 128), Color(160, 160, 160), Color::Transparent, TextureManager::getInstance()->getFont("roboto"), "Cross Entropy", true),
costGraph(200, 25, 200, 50), threadPool(8) {
    this->window.setFramerateLimit(2500);
    this->circles = map<DataPoint, CircleShape>();
    this->dataPoints = set<DataPoint>();
    this->epoch = 0;
    this->activationType = ActivationType::Sigmoid;
    this->costType = CostType::CrossEntropy;
    this->dataSetEmpty = true;
    this->numberClasses = 2;
    this->dataColor = Color(255, 0, 0);
    this->buttonColor = Color(128, 128, 128);
    this->hoverColor = Color(160, 160, 160);
    this->displayColor = Color(50, 50, 50);
    this->backgroundColor = Color(255, 255, 255);
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
    this->isVisualizing = false;

    ///clocks
    this->dataPointClock.restart();


    /// plot visualization
    this->pixelSize = 12;
    int width = this->window.getSize().x / (2 * this->pixelSize);
    int height = this->window.getSize().y / this->pixelSize;
    this->pixels = VertexArray(Triangles, width * height * 6); // 6 vertices for each pixel (2 triangles)
    this->computePlotPositions();
    this->isDiscretized = true;

    /// class dropdown
    this->initializeClassDropdown();
    this->initializeActivationDropdown();
    this->initializeCostDropdown();

    /// cost graph

    // we will have two views for each half of the screen
    this->plotView = View(FloatRect(0, 0, this->window.getSize().x / 2, this->window.getSize().y));
    this->networkView = View(FloatRect(0, 0, this->window.getSize().x / 2, this->window.getSize().y));

    // position the views side by side
    this->plotView.setViewport(sf::FloatRect(0.5f, 0, 0.5f, 1));
    this->plotView.move(this->window.getSize().x / 2, 0);
    this->networkView.setViewport(sf::FloatRect(0, 0, 0.5f, 1));

    /// highlight

    this->clock.restart();

    this->highlightMode = false;


    /// buttons and sliders
    this->resetInputsButton = Button(20, 1000, 200, 50, this->buttonColor, this->hoverColor, [this]() { this->resetInputs(); }, TextureManager::getInstance()->getFont("roboto"), "Reset Inputs");
    this->resetOrStartTrainingButton = Button(20 + 238, 1000, 200, 50, this->buttonColor, this->hoverColor, [this]() { this->resetOrStartTraining(); }, TextureManager::getInstance()->getFont("roboto"), "Start Training");
    this->toggleHighlightMode = Button(20 + 2 * 238, 1000, 200, 50, this->buttonColor, this->hoverColor, [this]() { this->toggleHighlight(); }, TextureManager::getInstance()->getFont("roboto"), "Toggle Highlight");
    this->toggleVisualizeMode = Button(20 + 3 * 238, 1000, 200, 50, this->buttonColor, this->hoverColor, [this]() { this->toggleVisualize(); }, TextureManager::getInstance()->getFont("roboto"), "Toggle Visualize");

    this->nextPointButton = Button(100 + visit([](auto&& arg) { return arg.getNumberOutputs(); }, networkVisualizer) + 50, 400, 25, 150, this->buttonColor, this->hoverColor, [this]() {
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
        }, TextureManager::getInstance()->getFont("roboto"), ">");
    this->previousPointButton = Button(100, 400, 25, 150, this->buttonColor, this->hoverColor, [this]() {
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
        }, TextureManager::getInstance()->getFont("roboto"), "<");

    /// undo redo
    this->undoStack = stack<DataPoint>();
    this->redoStack = stack<DataPoint>();

}


void MainLoop::toggleVisualize() {
    this->visualizeMode = !this->visualizeMode;
    if (!this->visualizeMode)
        this->networkVisualizer = NetworkVisualizer<Neuron>(this->network, Vector2f(450, 350), this->classColors);
    else
        this->networkVisualizer = NetworkVisualizer<NeuronPlot>(this->network, Vector2f(450, 350), this->classColors);
}

void MainLoop::updateHighlight() {
    if (!this->dataSetEmpty && this->currentDataPoint != this->dataPoints.end()) {
        // Calculating a sinusoidal value based on the elapsed time
        float time = this->clock.getElapsedTime().asSeconds();
        float alpha = 127.5f * sin(2.0f * 3.14159f * time) + 127.5f;

        // Getting the circle corresponding to the current data point

        CircleShape& circle = this->circles[*this->currentDataPoint];

        // Setting the position and radius of the highlight
        this->highlight.setRadius(circle.getRadius() + 2);
        this->highlight.setPosition(circle.getPosition().x + circle.getRadius() - this->highlight.getRadius(),
            circle.getPosition().y + circle.getRadius() - this->highlight.getRadius());
        this->highlight.setFillColor(Color::Transparent);
        this->highlight.setOutlineThickness(6);
        this->highlight.setOutlineColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(alpha)));
    }
}


void MainLoop::toggleHighlight() {
    this->highlightMode = !this->highlightMode;
    if (!this->highlightMode)
        this->highlight.setOutlineColor(Color::Transparent);
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
    this->dataSetEmpty = true;
	this->stopTraining();
    this->undoStack = stack<DataPoint>();
    this->redoStack = stack<DataPoint>();
    this->currentDataPoint = this->dataPoints.end();
    this->network.calculateOutputs({ 0, 0 }, this->activationType); /// we do this for the visualization
}


void MainLoop::startTraining() {
    this->gameState = GameState::Training;
    this->maxCost = this->network.costFunction(this->dataPoints, this->activationType, this->costType);
    this->costGraph.reset();
    this->resetOrStartTrainingButton.setString("Reset Training");
}


void MainLoop::stopTraining() {
    this->network.initializeWeightsAndBiases();
    this->epoch = 0;
    this->maxCost = 0;
    this->gameState = GameState::InputingData;
    this->costGraph.reset();
    this->resetOrStartTrainingButton.setString("Start Training");
}

void MainLoop::resetOrStartTraining() {

    if (this->gameState == GameState::Training) {
		this->stopTraining();
    }
    else if (this->gameState == GameState::InputingData && !this->dataPoints.empty()) {
		this->startTraining();
        
    }
    
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
        return this->network.costFunction(this->dataPoints, this->activationType, this->costType) / this->maxCost;
    else
        return 0;
}

void MainLoop::computePlotPositions() {
    int width = this->window.getSize().x / (2 * pixelSize);
    int height = this->window.getSize().y / pixelSize;
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            Vector2f pos(x * this->pixelSize, y * this->pixelSize);

            // Set the position and color of each vertex
            int i = (x + y * width) * 6; // index for this pixel
            // First triangle
            this->pixels[i + 0].position = pos + Vector2f(this->window.getSize().x / 2, 0);
            this->pixels[i + 1].position = pos + Vector2f(this->pixelSize, 0) + Vector2f(this->window.getSize().x / 2, 0);
            this->pixels[i + 2].position = pos + Vector2f(0, this->pixelSize) + Vector2f(this->window.getSize().x / 2, 0);
            // Second triangle
            this->pixels[i + 3].position = pos + Vector2f(0, this->pixelSize) + Vector2f(this->window.getSize().x / 2, 0);
            this->pixels[i + 4].position = pos + Vector2f(this->pixelSize, 0) + Vector2f(this->window.getSize().x / 2, 0);
            this->pixels[i + 5].position = pos + Vector2f(this->pixelSize, this->pixelSize) + Vector2f(this->window.getSize().x / 2, 0);
        }
    }

}

void MainLoop::rgbToHsv(int r, int g, int b, double* h, double* s, double* v) {
    double red = r / 255.0;
    double green = g / 255.0;
    double blue = b / 255.0;

    double max = std::max(red, std::max(green, blue));
    double min = std::min(red, std::min(green, blue));

    *h = 0;
    if (max != min) {
        if (max == red) {
            *h = 60 * (0 + (green - blue) / (max - min));
        }
        else if (max == green) {
            *h = 60 * (2 + (blue - red) / (max - min));
        }
        else if (max == blue) {
            *h = 60 * (4 + (red - green) / (max - min));
        }
    }

    *h = std::fmod(*h, 360);
    if (*h < 0) {
        *h += 360;
    }

    *s = (max == 0) ? 0 : (max - min) / max;
    *v = max;
}

void MainLoop::hsvToRgb(double h, double s, double v, int* r, int* g, int* b) {
    double c = v * s;
    double x = c * (1 - std::abs(std::fmod(h / 60.0, 2) - 1));
    double m = v - c;

    double rPrime, gPrime, bPrime;
    if (h >= 0 && h < 60) {
        rPrime = c, gPrime = x, bPrime = 0;
    }
    else if (h >= 60 && h < 120) {
        rPrime = x, gPrime = c, bPrime = 0;
    }
    else if (h >= 120 && h < 180) {
        rPrime = 0, gPrime = c, bPrime = x;
    }
    else if (h >= 180 && h < 240) {
        rPrime = 0, gPrime = x, bPrime = c;
    }
    else if (h >= 240 && h < 300) {
        rPrime = x, gPrime = 0, bPrime = c;
    }
    else {
        rPrime = c, gPrime = 0, bPrime = x;
    }

    *r = (rPrime + m) * 255;
    *g = (gPrime + m) * 255;
    *b = (bPrime + m) * 255;
}




void MainLoop::visualizePlotDiscretized()
{
    int width = this->window.getSize().x / (2 * pixelSize);
    int height = this->window.getSize().y / pixelSize;

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            double input_1 = x / static_cast<double>(width);
            double input_2 = y / static_cast<double>(height);
            vector<double> outputs = this->network.calculateOutputs({ input_1, input_2 }, this->activationType);

            // Find the index of the maximum output
            int maxIndex = max_element(outputs.begin(), outputs.end()) - outputs.begin();

            // Set all outputs to 0, except the maximum one
            for (int i = 0; i < outputs.size(); i++) {
                outputs[i] = (i == maxIndex);
            }

            // Get the color for this pixel
            Color color = this->classColors[maxIndex];

            int i = (x + y * width) * 6; // index for this pixel

            for (int j = 0; j < 6; j++) {
                this->pixels[i + j].color = color;
            }
        }
    }

    // Draw the pixels
    this->window.draw(this->pixels);

    if (this->currentDataPoint != this->dataPoints.end())
        this->network.calculateOutputs((*this->currentDataPoint).getInputs(), this->activationType); /// we do this for the visualization
}

void MainLoop::visualizePlot()
{
    int width = this->window.getSize().x / (2 * pixelSize);
    int height = this->window.getSize().y / pixelSize;

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            double input_1 = x / static_cast<double>(width);
            double input_2 = y / static_cast<double>(height);
            vector<double> outputs = this->network.calculateOutputs({ input_1, input_2 }, this->activationType);

            // we initialize the color in linear RGB
            double r = 0, g = 0, b = 0;
            double totalWeight = 0;

            
			for (int i = 0; i < outputs.size(); i++) { /// calculating the weighted sum of the colors in linear RGB
				double r_i = this->classColors[i].r / 255.0; /// to convert into liniar rgb we divide by 255
                double g_i = this->classColors[i].g / 255.0;
                double b_i = this->classColors[i].b / 255.0;

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
    this->window.draw(this->pixels);

    if (this->currentDataPoint != this->dataPoints.end())
        this->network.calculateOutputs((*this->currentDataPoint).getInputs(), this->activationType); /// we do this for the visualization
}




void MainLoop::undo() {
    if (!this->undoStack.empty()) {
        // Remove the last data point
        DataPoint point = undoStack.top();
        this->undoStack.pop();

        // Check if the currentDataPoint is the one being removed

        if (this->currentDataPoint != this->dataPoints.end()) { /// if the current data point is the end iterator
            if (*this->currentDataPoint == point) {
                if (this->currentDataPoint != this->dataPoints.begin()) {
                    --this->currentDataPoint; // Move to the previous data point
                }
                else if (this->currentDataPoint != this->dataPoints.end()) {
                    this->currentDataPoint = this->dataPoints.end(); // No previous data point, set to end

                }
            }
        }


        // we remove from dataPoints and circles
        this->dataPoints.erase(point);
        this->circles.erase(point);

		if (this->dataPoints.empty()) this->dataSetEmpty = true; /// if the data points are empty the data set is empty

        // push onto redo stack
        this->redoStack.push(point);
    }
}

void MainLoop::redo() {
    if (!this->redoStack.empty()) {
        // Add the data point back
        DataPoint point = redoStack.top();
        this->redoStack.pop();

        // Add to dataPoints and circles
        this->dataPoints.insert(point);

        /// calculating the data color from the point
        Color pointColor;
        for (int i = 0; i < this->numberClasses; i++) {
            if (point.getOutputs()[i] == 1) {
                pointColor = this->classColors[i];
            }
        }


        CircleShape circle(10);
        circle.setFillColor(pointColor);
        circle.setPosition(point.getInputs()[0] * this->window.getSize().x / 2 + this->window.getSize().x / 2, point.getInputs()[1] * this->window.getSize().y);
        circle.setOutlineThickness(2); // Set the thickness of the outline
        circle.setOutlineColor(sf::Color::White); // Set the color of the outline

        this->circles[point] = circle; // Assuming 'circle' is the corresponding CircleShape

        if (this->dataSetEmpty)
            this->currentDataPoint = this->dataPoints.begin(); /// we must initialize this

        if (this->undoStack.empty()) this->dataSetEmpty = false; /// if the undo stack is empty the data set is not empty

        // Push onto undo stack
        this->undoStack.push(point);
    }
}


void MainLoop::addDataPoint(Event& event) {

    if (this->dataColor != Color(0, 0, 0)) /// we do not want to add black data points
    {
        CircleShape circle(10);
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

        DataPoint point;
        for (int i = 0; i < this->numberClasses; i++) {
            if (this->dataColor == this->classColors[i]) {
				vector<double> outputs(this->numberClasses, 0); /// we initialize the outputs to 0
                outputs[i] = 1;
				point = DataPoint({ normalizedX, normalizedY }, outputs); /// we add the data point
				this->dataPoints.insert(point); /// we insert the data point
                this->circles[point] = circle;
                if (this->dataSetEmpty)
                    this->currentDataPoint = this->dataPoints.begin(); /// we must initialize this
            }
        }
        this->dataSetEmpty = false;

        undoStack.push(point); /// push the data point to the undo stack

        /// we reset redo stack
        this->redoStack = stack<DataPoint>();

    }

}


void MainLoop::holdForDataPoint() {
    if (this->isAddingDataPoints && Mouse::isButtonPressed(Mouse::Left) && dataPointClock.getElapsedTime().asMilliseconds() > 50) {
        // Create a new event with the current mouse position
        Event newEvent;
        newEvent.type = Event::MouseButtonPressed;
        newEvent.mouseButton.button = Mouse::Left;
        newEvent.mouseButton.x = Mouse::getPosition(window).x;
        newEvent.mouseButton.y = Mouse::getPosition(window).y;

        // Add the data point
        this->addDataPoint(newEvent);

        // Restart the clock
        dataPointClock.restart();
    }
}

void MainLoop::eventHandler(sf::Event& event, bool& space) {
    if (event.type == Event::Closed) {
        this->window.close();
    }

    /// undo redo

    if (event.type == Event::KeyPressed && this->gameState == GameState::InputingData) {
        if (event.key.control) {
            if (event.key.code == Keyboard::Z) {
                this->undo();
            }
            else if (event.key.code == Keyboard::Y) {
                this->redo();
            }
        }

    }


    /// buttons and sliders
    sf::Vector2i pixelPos = Mouse::getPosition(this->window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos, this->networkView);
    sf::Vector2f worldPosDefault = window.mapPixelToCoords(pixelPos, this->window.getDefaultView());

    sf::Event worldEvent = event;
    Event defaultWorldEvent = event;

    worldEvent.mouseButton.x = worldPos.x;
    worldEvent.mouseButton.y = worldPos.y;
    defaultWorldEvent.mouseButton.x = worldPosDefault.x;
    defaultWorldEvent.mouseButton.y = worldPosDefault.y;

    this->resetInputsButton.checkEvents(this->window, defaultWorldEvent);
    this->resetOrStartTrainingButton.checkEvents(this->window, defaultWorldEvent);
    this->toggleHighlightMode.checkEvents(this->window, defaultWorldEvent);
    this->toggleVisualizeMode.checkEvents(this->window, defaultWorldEvent);

    if (this->highlightMode) {
        this->previousPointButton.checkEvents(this->window, worldEvent);
        this->nextPointButton.checkEvents(this->window, worldEvent);
    }


    /// this->networkVisualizer.checkEvents(this->window, event, this->networkView); - before varaint
    visit([&](auto&& arg) { arg.checkEvents(this->window, event, this->networkView, this->undoStack, this->redoStack); }, networkVisualizer); /// after variant


    this->learningRateSlider.handleEvent(event);
    this->momentumSlider.handleEvent(event);

    /// dropdowns

    if (this->gameState == GameState::InputingData)
    {
        this->classDropdown.checkEvents(this->window, event);
        this->activationDropdown.checkEvents(this->window, event);
        this->costDropdown.checkEvents(this->window, event);

        /// we need to update the this->classColor for inputing data and avoiding exceptions
        // this->updateNumberOfClasses();

    }

    if (event.type == Event::MouseButtonPressed && event.mouseButton.x > this->window.getSize().x / 2 && this->gameState == GameState::InputingData) { /// for adding data points
        if (event.mouseButton.button == Mouse::Left) {

            this->addDataPoint(event);
            isAddingDataPoints = true;
            dataPointClock.restart();
        }
    }
    else if (event.type == Event::MouseButtonReleased) {
        if (event.mouseButton.button == Mouse::Left) {
            isAddingDataPoints = false;
        }
    }

    /// visualization
    if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
        this->isVisualizing = !this->isVisualizing;
    }

    /// switch between discretized and non discretized with d
    if (event.type == Event::KeyPressed && event.key.code == Keyboard::D) {
        this->isDiscretized = !this->isDiscretized;
    }

    if (event.type == Event::MouseWheelScrolled)
    {
        if (event.mouseWheelScroll.delta > 0)
            this->networkView.zoom(0.9f); // Zoom in
        else if (event.mouseWheelScroll.delta < 0)
            this->networkView.zoom(1.1f); // Zoom out
    }

    if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter && this->gameState == GameState::InputingData && !this->dataPoints.empty()) {
		this->startTraining();
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::G) /// for testing purposes
    {
        this->costGraph.reset();
    }
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::C) /// for testing purposes
    {
        this->graphDisplayed = !this->graphDisplayed;
    }

    /// reading and writing
    if (event.type == Event::KeyPressed && this->gameState == GameState::InputingData) {
        if (event.key.control) {
			if (event.key.code == Keyboard::S) {
				this->writeToFile("dataSet.txt");
			}
            else if (event.key.code == Keyboard::O) {
				this->readFromFile("dataSet.txt");
			}

        }
    }

}


void MainLoop::zoomHandler() {
    if (Keyboard::isKeyPressed(Keyboard::Left))
    {
        this->networkView.move(-10, 0); // Move left
    }
    else if (Keyboard::isKeyPressed(Keyboard::Right))
    {
        this->networkView.move(10, 0); // Move right
    }
    else if (Keyboard::isKeyPressed(Keyboard::Up))
    {
        this->networkView.move(0, -10); // Move up
    }
    else if (Keyboard::isKeyPressed(Keyboard::Down))
    {
        this->networkView.move(0, 10); // Move down
    }

    static sf::Vector2i lastMousePosition = Mouse::getPosition(this->window);

    if (sf::Mouse::isButtonPressed(Mouse::Right)) {
        sf::Vector2i currentMousePosition = Mouse::getPosition(this->window);

        // Check if the cursor is on the left half of the window
        if (currentMousePosition.x < this->window.getSize().x / 2) {
            sf::Vector2i delta = lastMousePosition - currentMousePosition;

            this->networkView.move(static_cast<float>(delta.x), static_cast<float>(delta.y));
        }
    }

    lastMousePosition = sf::Mouse::getPosition(this->window);

}


void MainLoop::trainingState() {
    this->epoch++;

    // Update the gradient for each data point
    for (auto it = this->dataPoints.begin(); it != this->dataPoints.end(); ++it) {
        const auto& dataPoint = *it;

        if (it == this->currentDataPoint) /// we do this so we can visualize the point in the network (its outputs)
            continue;

        this->network.updateGradientsForDataPoint(dataPoint, this->activationType, this->costType);
    }
    this->network.updateGradientsForDataPoint(*this->currentDataPoint, this->activationType, this->costType);
    this->network.applyAllGradients(this->learningRate / this->dataPoints.size(), this->momentum);


	/// visualization
    if (this->isVisualizing) {
        if (this->isDiscretized)
            this->visualizePlotDiscretized();
        else
            this->visualizePlot();
    }
}


void MainLoop::updateNumberOfClasses() {
    this->numberClasses = visit([](auto&& arg) { return arg.getNumberOutputs(); }, networkVisualizer);
    if (this->numberClasses < this->classDropdown.getNumButtons())
    {
        if (this->dataColor == this->classColors[this->numberClasses]) {
            this->dataColor = Color(0, 0, 0); /// if the color is black we want to not be able to input
        }

        for (int i = this->numberClasses; i < this->classDropdown.getNumButtons(); i++)
        {
            this->classDropdown.removeOption(i);
        }

    }
    else if (this->numberClasses > this->classDropdown.getNumButtons())
    {
        for (int i = this->classDropdown.getNumButtons(); i < this->numberClasses; i++)
        {
            this->classDropdown.addOption(new ColorButton(0, 0, 200, 50, Color(50, 50, 50), Color(105, 105, 105), this->classColors[i], [this, i]() { this->dataColor = this->classColors[i]; }, TextureManager::getInstance()->getFont("roboto"), ""));
        }
    }
}

/// reading and writing to file 
void MainLoop::readFromFile(const string& filename) {
    ifstream file;
    file.open(filename);
    if (!file) {
        cerr << "Unable to open file data.txt";
        return;
    }

	
    if (file.peek() == ifstream::traits_type::eof()) {
        cerr << "File " << filename << " is empty";
        return;
    }


    this->resetInputs();

    try {

        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            vector<double> inputs;
            vector<double> outputs;
            double input;
            double output;
            string arrow;
            while (iss >> input) {
                inputs.push_back(input);
                if (iss.peek() == ',') {
                    iss.ignore();
                }
                if (iss.peek() == ' ') {
                    iss >> arrow;
                    break;
                }
            }


            while (iss >> output) {
                outputs.push_back(output);
                if (iss.peek() == ',') {
                    iss.ignore();
                }
            }

            if (this->numberClasses != outputs.size())
                throw invalid_argument("The number of classes in the file does not match the number of classes in the program");


            DataPoint point(inputs, outputs);
            this->dataPoints.insert(point);
            Color pointColor;

            for (int i = 0; i < this->numberClasses; i++) {
                if (outputs[i] == 1) {
                    pointColor = this->classColors[i];
                }
            }
            CircleShape circle(10);
            circle.setFillColor(pointColor);
            circle.setPosition(inputs[0] * this->window.getSize().x / 2 + this->window.getSize().x / 2, inputs[1] * this->window.getSize().y);
            circle.setOutlineThickness(2);
            circle.setOutlineColor(Color::White);
            this->circles[point] = circle;


        }
        file.close();
        if (!this->dataPoints.empty())
            this->currentDataPoint = this->dataPoints.begin();
        this->dataSetEmpty = false;
        
    }
	catch (const exception& e) {
		cerr << "Exception: " << e.what() << std::endl;
	}
}

void MainLoop::writeToFile(const string& filename) {
    ofstream file;
    file.open(filename);
    if (!file) {
        cerr << "Unable to open file " << filename;
        return;
    }

    if(this->dataSetEmpty) 
        return;

    for (const DataPoint& point : this->dataPoints) {
        const vector<double>& inputs = point.getInputs();
        const vector<double>& outputs = point.getOutputs();

        // Write inputs to file
        for (size_t i = 0; i < inputs.size(); ++i) {
            file << inputs[i];
            if (i != inputs.size() - 1) {
                file << ',';
            }
        }

        file << " -> "; // Separate inputs and outputs with an arrow

        // Write outputs to file
        for (size_t i = 0; i < outputs.size(); ++i) {
            file << outputs[i];
            if (i != outputs.size() - 1) {
                file << ',';
            }
        }

        file << '\n'; // Start a new line for each data point
    }

    file.close();
}


void MainLoop::run()
{

    this->network.initializeWeightsAndBiases();

    this->gameState = GameState::InputingData;

    bool space = false;

    Text costFunctionText;
    Text epochText;

    /// additional declarations (might move them to the class)
    costFunctionText.setFont(TextureManager::getInstance()->getFont("roboto"));
    costFunctionText.setPosition(10, 15);
    costFunctionText.setCharacterSize(22);
    costFunctionText.setFillColor(Color::White);
    epochText.setFont(TextureManager::getInstance()->getFont("roboto"));
    epochText.setPosition(10, 45);
    epochText.setCharacterSize(22);
    epochText.setFillColor(Color::White);
    this->previousPointButton.setPosition(50, 360); /// position for the highlight prev button (should refactor this its a pain to change it when needed)



    while (this->window.isOpen()) {

        sf::Event event;
        while (this->window.pollEvent(event))
        {
            this->eventHandler(event, space);
        }

        /// for adding data points
        this->holdForDataPoint();


        ////updates
        this->updateNumberOfClasses();
        if (this->highlightMode) this->updateHighlight();
        if (this->graphDisplayed) this->costGraph.update(this->getCostPercentage()); /// cost graph update

        /// highlight buttons position
        /// prev point 360
        this->nextPointButton.setPosition(100 + visit([](auto&& arg) { return arg.getTotalWidth(); }, networkVisualizer) + 150, 360);

        /// other inputs
        this->zoomHandler();

        this->window.clear(Color(14, 26, 37));

        this->drawRightHalfScreen();
        this->window.setView(this->plotView); /// for the plot view

        /// for the next and prev buttons to work
        if (!this->dataSetEmpty)
            network.calculateOutputs((*this->currentDataPoint).getInputs(), this->activationType);
        else {
			network.calculateOutputs({ 0, 0 }, this->activationType); /// we do this for the visualization
        }

        /// highlight and circles on the right
        if (this->gameState == GameState::Training) {
            this->trainingState(); /// this handles all the training logic and drawing
        }
        else if (!this->dataSetEmpty) {
            /// visualization
            if (this->isVisualizing) {
                if (this->isDiscretized)
                    this->visualizePlotDiscretized();
                else
                    this->visualizePlot();
            }

			network.calculateOutputs((*this->currentDataPoint).getInputs(), this->activationType); /// for the network visualization

        }
        for (auto& pair : this->circles) {
            this->window.draw(pair.second);
        }

        /// drawing the currentDataPoint for highlight
        if (!this->dataSetEmpty) {
            this->window.draw(this->circles[*this->currentDataPoint]); /// we draw it last so we can see it with the higlight also
            if (this->highlightMode) this->window.draw(this->highlight);
        }


        /// neural net visualization and neural net view
        this->window.setView(this->networkView);
        visit([&](auto&& arg) { arg.draw(this->window, this->gameState, this->dataSetEmpty, this->activationType, this->isDiscretized); }, this->networkVisualizer);/// this visualizes the neural network

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
        this->resetOrStartTrainingButton.draw(this->window);
        this->learningRateSlider.draw(this->window);
        this->momentumSlider.draw(this->window);

        ///dropdowns

        if (this->gameState == GameState::InputingData) {
            this->classDropdown.draw(this->window);
            this->activationDropdown.draw(this->window);
            this->costDropdown.draw(this->window);
        }

        /// printing to console activation type


        this->toggleHighlightMode.draw(this->window);
        this->toggleVisualizeMode.draw(this->window);

        /// graph
        if (this->graphDisplayed) this->costGraph.draw(this->window);

        this->window.display();

    }
}
