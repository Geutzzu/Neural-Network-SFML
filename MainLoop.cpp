

#include "MainLoop.h"
#include <thread>


using namespace std;
using namespace sf;

MainLoop* MainLoop::getInstance() {
	static MainLoop instance;
	return &instance;
}


MainLoop::MainLoop() : window(sf::VideoMode(1920, 1080), "Neural Network"), network({ 2, 5, 2 }), networkVisualizer(network, sf::Vector2f(450, 200)), textureManager(TextureManager::getInstance()), gameState(GameState::InputingData) {
	this->window.setFramerateLimit(500);
	this->circles = vector<CircleShape>();
	this->dataPoints = set<DataPoint>();
	this->epoch = 0;

    // Create the two views
    this->plotView = sf::View(sf::FloatRect(0, 0, window.getSize().x / 2, window.getSize().y));
    this->networkView = sf::View(sf::FloatRect(0, 0, window.getSize().x / 2, window.getSize().y));

    // Position the views side by side
    this->plotView.setViewport(sf::FloatRect(0.5f, 0, 0.5f, 1));
	this->plotView.move(window.getSize().x / 2, 0);
    this->networkView.setViewport(sf::FloatRect(0, 0, 0.5f, 1));
    
    /// buttons
	this->resetInputsButton = Button(10, 900, 150, 50, Color::Green, Color::Red, [this]() { this->resetInputs(); }, this->textureManager->getFont("roboto"), "Reset Inputs");

}

void MainLoop::resetInputs() {
	this->dataPoints.clear();
	this->circles.clear();
	this->gameState = GameState::InputingData;
    this->epoch = 0;
}

void MainLoop::drawRightHalfScreen() {
    this->window.setView(this->plotView);
	sf::RectangleShape rectangle;
	rectangle.setSize(sf::Vector2f(this->window.getSize().x / 2, this->window.getSize().y));
	rectangle.setPosition(this->window.getSize().x / 2, 0);
	rectangle.setFillColor(Color::White);

	this->window.draw(rectangle);
}

void MainLoop::visualizePlot()
{
    int pixelSize = 16; // Adjust this to make the pixels larger or smaller
    int width = this->window.getSize().x / (2 * pixelSize);
    int height = this->window.getSize().y / pixelSize;

    // Define a vector of colors for each output
    vector<Color> colors = { Color::Blue, Color::Red, Color::Green, Color::Yellow, Color::Magenta, Color::Cyan, Color::White, Color::Black };

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
}


void MainLoop::eventHandler(sf::Event& event, bool& space) {
    if (event.type == sf::Event::Closed) {
        this->window.close();
    }

    /// buttons
	this->resetInputsButton.checkEvents(this->window, event);

	this->networkVisualizer.checkEvents(this->window, event, this->networkView);

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
            this->circles.push_back(sf::CircleShape(10));
            this->circles.back().setFillColor(sf::Color::Blue);
            this->circles.back().setPosition(event.mouseButton.x, event.mouseButton.y);
            this->circles.back().setOutlineThickness(2); // Set the thickness of the outline
            this->circles.back().setOutlineColor(sf::Color::White); // Set the color of the outline
            double normalizedX = event.mouseButton.x / static_cast<double>(this->window.getSize().x / 2) - 1;
            double normalizedY = event.mouseButton.y / static_cast<double>(this->window.getSize().y);
            this->dataPoints.insert(DataPoint({ normalizedX, normalizedY }, { 1, 0 }));
        }
        else if (event.mouseButton.button == sf::Mouse::Right) {
            this->circles.push_back(sf::CircleShape(10));
            this->circles.back().setFillColor(sf::Color::Red);
            this->circles.back().setPosition(event.mouseButton.x, event.mouseButton.y);
            this->circles.back().setOutlineThickness(2); // Set the thickness of the outline
            this->circles.back().setOutlineColor(sf::Color::White); // Set the color of the outline
            double normalizedX = event.mouseButton.x / static_cast<double>(this->window.getSize().x / 2) - 1;
            double normalizedY = event.mouseButton.y / static_cast<double>(this->window.getSize().y);
            this->dataPoints.insert(DataPoint({ normalizedX, normalizedY }, { 0, 1 }));
        }
    }

    if (event.type == sf::Event::MouseWheelScrolled)
    {
        if (event.mouseWheelScroll.delta > 0)
            this->networkView.zoom(0.9f); // Zoom in
        else if (event.mouseWheelScroll.delta < 0)
            this->networkView.zoom(1.1f); // Zoom out
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
        this->gameState = GameState::Training;
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

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        this->plotView.move(-10, 0); // Move left
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        this->plotView.move(10, 0); // Move right
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        this->plotView.move(0, -10); // Move up
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        this->plotView.move(0, 10); // Move down
    }


}


void MainLoop::trainingState() {

    this->epoch++;
    


    for (const auto& dataPoint : this->dataPoints) {
        this->network.UpdateGradientsForDataPoint(dataPoint); /// update the gradients for the data point
    }
    this->network.ApplyAllGradients(0.1 / this->dataPoints.size()); /// apply the gradients to the weights and biases

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        this->visualizePlot();
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




    while (this->window.isOpen()) {

        sf::Event event;
        while (this->window.pollEvent(event))
        {
            this->eventHandler(event, space);
        }
        
        /// other inputs
		this->zoomHandler();



        this->window.clear();
        this->drawRightHalfScreen();
        this->window.setView(this->plotView); /// for the plot view

        if (this->gameState == GameState::Training) {
			this->trainingState(); /// this handles all the training logic and drawing
        }

         for (int i = 0; i < this->circles.size(); i++) {
             this->window.draw(this->circles[i]); /// data points 
         }

        /// cost text
        

        /// neural net visualization and neural net view
        this->window.setView(this->networkView);
        this->networkVisualizer.draw(this->window, this->gameState); /// this visualizes the neural network


        /// text and buttons
        this->window.setView(this->window.getDefaultView());
        costFunctionText.setString("Cost: " + std::to_string(this->network.CostFunction(this->dataPoints)));
        this->window.draw(costFunctionText);
		epochText.setString("Epoch: " + std::to_string(this->epoch));
		this->window.draw(epochText);

		this->resetInputsButton.draw(this->window);

        this->window.display();

    }
}



