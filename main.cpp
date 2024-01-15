#include <iostream>
#include <cmath>
#include <random>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Grid.hpp"
#include "Agent.hpp"
#include "Simulation.hpp"
#include "Renderer.hpp"
#include "TrailMap.hpp"
#include "ColorMap.hpp"
#include "CustomAudioStream.hpp"

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

int main() {
	/*
	// load an audio buffer from a sound file
	sf::SoundBuffer buffer;
	buffer.loadFromFile("audio/lm.wav");

	const sf::Int16* Samples = buffer.getSamples();
	sf::Uint64 sample_count = buffer.getSampleCount();


	std::cout << sample_count << std::endl;

	/*
	for (int i = 0; i < sample_count; i++) {
		std::cout << *(Samples + i) << std::endl;
	}
	

	sf::Music music;

	if (!music.openFromFile("audio/lm.wav"))
		return -1; // error
	music.play();
	*/

	
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