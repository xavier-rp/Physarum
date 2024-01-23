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
#include "AgentCreationUtils.hpp"


#include "ColorGradient.hpp"

int main()
{

	std::vector<rgb> stops;

	stops.push_back(rgb{ 255,211,25 }); // red
	stops.push_back(rgb{ 255,144,31 }); // red
	stops.push_back(rgb{ 255,41,117 }); // green
	stops.push_back(rgb{ 242,34,255 }); // blue
	stops.push_back(rgb{ 140,30,255 }); // blue

	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile("audio/Chupi.wav"))
	{
		std::cout << "Failed to load audio file!" << std::endl;
		return -1;
	}

	const sf::Int16* samples = buffer.getSamples();
	sf::Uint64 sampleCount = buffer.getSampleCount();
	unsigned int sampleRate = buffer.getSampleRate();


	int window_width{ 600 };
	int window_height{ 400 };

	ColorMap color_map{ 1000, stops, true, false, 1 };

	TrailMap trail_map(window_width, window_height);

	Grid grid(static_cast<float>(window_width), static_cast<float>(window_height));

	sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Physarum.exe");

	std::vector<Agent> list_of_agents;

	list_of_agents = build_list_of_agents_random_on_circle(30000, window_width / 5.0f);
	//list_of_agents = build_list_of_agents_random_on_cat(10000, window_width / 2.0f);
	//list_of_agents = build_list_of_agents_random_inside_circle(40000, window_width / 5.0f);
	//list_of_agents = build_list_of_agents_uniform_circle(10000);
	//list_of_agents = build_list_of_agents_random_in_grid(5000, grid);

	Simulation sim{ grid, list_of_agents, trail_map };

	Renderer renderer{ sim, grid, color_map };

	// initialize and play our custom stream
	MyStream playing_stream;
	playing_stream.load(buffer);

	// Start playing the sound
	playing_stream.play();

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