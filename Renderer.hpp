#pragma once
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>

#include "SFML/Graphics.hpp"

#include "Simulation.hpp"
#include "ColorMap.hpp"


class Renderer {

public:
	Grid& grid;
	Simulation& simulation;
	ColorMap& color_map;
	sf::VertexArray agent_vertices;
	sf::VertexArray trail_map_vertices;

	Renderer(Simulation& simulation, Grid& grid, ColorMap& color_map) :
		simulation{ simulation },
		grid{ grid },
		color_map{ color_map } {
		agent_vertices = sf::VertexArray(sf::Points, simulation.get_list_of_agents().size());
		trail_map_vertices = sf::VertexArray(sf::Points, static_cast<int>(grid.width) * static_cast<int>(grid.height));
	}

	sf::Vector2f cartesian_to_grid(sf::Vector2f cartesian_position) {
		sf::Vector2f grid_position;
		grid_position.x = static_cast<float>(static_cast<int>(cartesian_position.x + grid.x_max));
		grid_position.y = static_cast<float>(static_cast<int>(-1.0f*cartesian_position.y + grid.y_max));

		return grid_position;
	}

	void render_agents() {

		int i{ 0 };
		for (Agent& agent : simulation.get_list_of_agents()) {

			agent_vertices[i].position = cartesian_to_grid( agent.get_position() );
			agent_vertices[i].color = sf::Color::Green;
			i += 1;
		}
	}

	void render_trail_map() {

		int k{ 0 };
		for (int i{ 0 }; i < static_cast<int>(grid.height); i++) {
			for (int j{ 0 }; j < static_cast<int>(grid.width); j++) {
				sf::Vector2f pos;
				pos.x = static_cast<float>(j);
				pos.y = static_cast<float>(i);
				trail_map_vertices[k].position = pos;

				float scent_value{ simulation.trail_map.matrix[i][j] };
				if (scent_value > 0.0) {
					uint8_t color_value{};
					color_value = static_cast<uint8_t>(255 * scent_value);
					//trail_map_vertices[k].color = sf::Color(color_value, color_value, color_value);
					trail_map_vertices[k].color = color_map.color_vec[static_cast<int>(scent_value * 100)];
					simulation.trail_map.matrix[i][j] = std::max(0.0f, simulation.trail_map.matrix[i][j] - 0.05f);
				}
				else {
					trail_map_vertices[k].color = sf::Color::Black;
				}
				k += 1;
				
			}

		}

	}

};