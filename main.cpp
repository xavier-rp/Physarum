#include <iostream>
#include <cmath>
#include <random>
#include <fstream>
#include <vector>
#include <complex>

#include <fftw3.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Grid.hpp"
#include "Agent.hpp"
#include "Simulation.hpp"
#include "Renderer.hpp"
#include "TrailMap.hpp"
#include "ColorMap.hpp"
#include "CustomAudioStream.hpp"
#include "SamplesRenderer.hpp"

std::vector<Agent> build_list_of_agents_uniform_circle(int nb_of_agents) {
	
	std::vector<Agent> list_of_agents;

	for (int i{ 0 }; i < nb_of_agents; i++) {
		float delta_theta = 360.0f / nb_of_agents;

		list_of_agents.push_back(Agent{ sf::Vector2f {0, 0}, i*delta_theta});
	}

	return list_of_agents;
}

std::vector<Agent> build_list_of_agents_random(int nb_of_agents, Grid& grid) {

	std::vector<Agent> list_of_agents;

	std::random_device rd{};
	std::mt19937 gen{rd()};

	//std::normal_distribution<> d{0.0, 10.0};
	std::uniform_real_distribution<float> dis_x{grid.x_min, grid.x_max};
	std::uniform_real_distribution<float> dis_y{grid.y_min, grid.y_max};
	std::uniform_real_distribution<float> dis_theta{0.0, 360.0};

	for (int i{ 0 }; i < nb_of_agents; i++) {

		list_of_agents.push_back(Agent{ sf::Vector2f {dis_x(gen), dis_y(gen)}, dis_theta(gen)});
	}

	return list_of_agents;
}

std::vector<Agent> build_list_of_agents_random_inside_circle(int nb_of_agents, float radius) {

	std::vector<Agent> list_of_agents;

	std::random_device rd{};
	std::mt19937 gen{rd()};

	//std::normal_distribution<> d{0.0, 10.0};
	std::uniform_real_distribution<float> dis_theta{0.0, 360.0};
	float theta{};

	for (int i{ 0 }; i < nb_of_agents; i++) {

		theta = dis_theta(gen);
		list_of_agents.push_back(Agent{ sf::Vector2f {cos(theta)*radius, sin(theta)* radius}, dis_theta(gen)});
	}

	return list_of_agents;
}

int main()
{
	// Load audio file
	// load an audio buffer from a sound file
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile("audio/lm.wav"))
	{
		std::cout << "Failed to load audio file!" << std::endl;
		return -1;
	}

	const sf::Int16* samples = buffer.getSamples();
	sf::Uint64 sampleCount = buffer.getSampleCount();
	unsigned int sampleRate = buffer.getSampleRate();

	int window_width{ 22050 };
	int window_height{ 600 };

	Grid grid(static_cast<float>(window_width), static_cast<float>(window_height));
	int nbSamplesToDraw = 88200;
	SamplesRenderer samplesRenderer{ buffer, grid, nbSamplesToDraw};

	// initialize and play our custom stream
	MyStream playing_stream;
	playing_stream.load(buffer);

	// Create window for visualization
	sf::RenderWindow window(sf::VideoMode(1600, 600), "Samples");

	// Start playing the sound
	playing_stream.play();
	sf::Clock globalClock;
	sf::Clock clock;
	sf::Time elapsed1 = clock.getElapsedTime();
	int currentSampleIndex{};

	// Main loop
	while (window.isOpen())
	{
		// Handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Clear the window
		
		window.clear();

		currentSampleIndex = playing_stream.getCurrentSampleIndex(playing_stream.getPlayingOffset());

		if (elapsed1.asSeconds() > 0.0166f) {
			samplesRenderer.renderSamples(globalClock.getElapsedTime().asSeconds() * sampleRate);
			clock.restart();
		}


		//std::cout << currentSampleIndex << " ; " << elapsed1.asSeconds() * sampleRate << std::endl;
		
		//renderer.render_agents();
		window.clear(sf::Color::Black);
		window.draw(samplesRenderer.chunkToDraw1);
		window.draw(samplesRenderer.chunkToDraw2);
		//window.draw(samplesRenderer.verticesChannel1[currentSampleIndex: currentSampleIndex + nbSamplesToDraw];
		//window.draw(samplesRenderer.verticesChannel2);

		// Calculate the frequency spectrum
		/*
		std::vector<float> spectrum(88200 / 2);
		for (std::size_t i = currentSampleIndex; i < currentSampleIndex + 88200 / 2; ++i)
		{
			float sum = 0;
			for (unsigned int j = 0; j < 2; ++j)
			{
				sum += samples[i * 2 + j];
			}
			spectrum[i - currentSampleIndex] = sum / 2.0;
		}
		*/

		// Update the size of the frequency bars based on the spectrum data
		/*
		for (std::size_t i = 0; i < 44100; ++i)
		{
			bar.setSize(sf::Vector2f(bar.getSize().x, samples[441000 + 2*i]));
			bar.setPosition(i * bar.getSize().x, window.getSize().y);
			window.draw(bar);
		}
		*/
		//std::cout << "Fok" << std::endl;
		// Display the window
		window.display();
		elapsed1 = clock.getElapsedTime();
	}

	return 0;
}

int main72()
{
	// load an audio buffer from a sound file
	sf::SoundBuffer buffer;
	buffer.loadFromFile("audio/lm.wav");
	const sf::Int16* Samples = buffer.getSamples();
	sf::Uint64 sample_count = buffer.getSampleCount();
	unsigned int SampleRate = buffer.getSampleRate();

	// initialize and play our custom stream
	MyStream playing_stream;
	playing_stream.load(buffer);

	playing_stream.play();

	

	// let it play until it is finished
	sf::Clock clock;
	sf::Time elapsed1 = clock.getElapsedTime();
	int currentSampleIndex{};

	while (elapsed1.asSeconds() < 5.0f) {
		elapsed1 = clock.getElapsedTime();
		currentSampleIndex = playing_stream.getCurrentSampleIndex(playing_stream.getPlayingOffset());
		std::cout << elapsed1.asSeconds() << " ; " << currentSampleIndex << std::endl;
	}

	playing_stream.stop();		

	return 0;
}

int main3() {
	/*
	// load an audio buffer from a sound file
	sf::SoundBuffer buffer;
	buffer.loadFromFile("audio/lm.wav");

	const sf::Int16* Samples = buffer.getSamples();
	sf::Uint64 sample_count = buffer.getSampleCount();


	std::cout << sample_count << std::endl;

	
	for (int i = 0; i < sample_count; i++) {
		std::cout << *(Samples + i) << std::endl;
	}
	*/
	

	sf::Music music;

	if (!music.openFromFile("audio/lm.wav"))
		return -1; // error
	music.play();
	

	
	std::cout << "Running main \n";

	int window_width{ 500 };
	int window_height{ 200 };

	ColorMap color_map{ 100, "purple_white", false, false, 1 };

	TrailMap trail_map(window_width, window_height);

	Grid grid(static_cast<float>(window_width), static_cast<float>(window_height));

	sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Physarum.exe");

	std::vector<Agent> list_of_agents;

	//list_of_agents = build_list_of_agents_random(10000, grid);
	//list_of_agents = build_list_of_agents_uniform_circle(100);
	
	/*
	int window_width{ 500 };
	int window_height{ 200 };
	list_of_agents = build_list_of_agents_random_inside_circle(30000, window_width / 5.0f);
	*/

	list_of_agents = build_list_of_agents_random_inside_circle(30000, window_width / 5.0f);


	Simulation sim{grid, list_of_agents, trail_map};

	Renderer renderer{ sim, grid, color_map }; 

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		renderer.render_trail_map();
		//renderer.render_agents();
		window.clear(sf::Color::Black);
		window.draw(renderer.trail_map_vertices);
		//window.draw(renderer.agent_vertices);
		window.display();
		sim.step();
	}

	return 0;
}