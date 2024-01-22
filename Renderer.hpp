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
	int pulseSequence{ 0 };
	int beginPulseStepIndex{ 0 };

	Renderer(Simulation& simulation, Grid& grid, ColorMap& color_map) :
		simulation{ simulation },
		grid{ grid },
		color_map{ color_map } {
		agent_vertices = sf::VertexArray(sf::Points, simulation.get_list_of_agents().size());
		trail_map_vertices = sf::VertexArray(sf::Points, 0);  //sf::VertexArray(sf::Points, static_cast<int>(grid.width) * static_cast<int>(grid.height));
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

	void render_trail_map1() {

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
					if (simulation.pulse_flag) {
						//trail_map_vertices[k].color = color_map.color_vec[static_cast<int>(scent_value * color_map.max_iter)];
						//trail_map_vertices[k].color.a = color_value;

						//trail_map_vertices[k].color = pulse_color_map.color_vec[static_cast<int>(scent_value * pulse_color_map.max_iter)];

						trail_map_vertices[k].color = color_map.color_vec[static_cast<int>(std::min(scent_value + 0.1f, 1.0f) * color_map.max_iter)];
						/*
						if (scent_value <= 0.5) {
							trail_map_vertices[k].color = pulse_color_map.color_vec[static_cast<int>(scent_value * pulse_color_map.max_iter)];
						}
						else {
							trail_map_vertices[k].color = color_map.color_vec[static_cast<int>(scent_value * color_map.max_iter)];
						}
						*/

						//trail_map_vertices[k].color.a = static_cast<uint8_t>(207);
					}
					else {
						trail_map_vertices[k].color = color_map.color_vec[static_cast<int>(scent_value * color_map.max_iter)];
					}

					simulation.trail_map.matrix[i][j] = std::max(0.0f, simulation.trail_map.matrix[i][j] - 0.05f);
				}
				else {
					trail_map_vertices[k].color = sf::Color::Black;
				}
				k += 1;
				
			}

		}

	}

	void render_trail_map() {
		trail_map_vertices.clear();

		//int k{ 0 };
		
		if (simulation.pulse_flag) {
			

			if (simulation.stepCount - pulseSequence == beginPulseStepIndex) {
				pulseSequence += 1;
			}
			else {
				beginPulseStepIndex = simulation.stepCount;
				pulseSequence = 1;
			}

		}

		for (int i{ 0 }; i < static_cast<int>(grid.height); i++) {
			for (int j{ 0 }; j < static_cast<int>(grid.width); j++) {
				/*
				Needed to write the vertex position. It does not fetch the position of the pixel, it sets it.
				*/

				float scent_value{ simulation.trail_map.matrix[i][j] };
				if (scent_value > 0.0) {
					sf::Vector2f pos;
					pos.x = static_cast<float>(j);
					pos.y = static_cast<float>(i);
					sf::Vertex pixel;
					pixel.position = pos;
					//trail_map_vertices[k].position = pos;

					if (simulation.pulse_flag) {
						float colorModifier{ std::max(- std::abs(static_cast<float>(pulseSequence) * 0.01f - 0.2f) + 0.2f, 0.0f)};

						//trail_map_vertices[k].color = color_map.color_vec[static_cast<int>(std::min(scent_value + colorModifier, 1.0f) * color_map.max_iter)];
						
						pixel.color = color_map.color_vec[static_cast<int>(std::min(scent_value + colorModifier, 1.0f) * color_map.max_iter)];
						//scent_value = std::min(scent_value + static_cast<float>(pulseSequence) * 0.01f, 1.0f);
						//simulation.trail_map.matrix[i][j] = scent_value + 0.03;

						//trail_map_vertices[k].color = color_map.color_vec[static_cast<int>(scent_value * color_map.max_iter)];

						/*
						if (scent_value <= 0.5) {
							trail_map_vertices[k].color = pulse_color_map.color_vec[static_cast<int>(scent_value * pulse_color_map.max_iter)];
						}
						else {
							trail_map_vertices[k].color = color_map.color_vec[static_cast<int>(scent_value * color_map.max_iter)];
						}
						*/

						//trail_map_vertices[k].color.a = static_cast<uint8_t>(207);
					}
					else {
						pixel.color = color_map.color_vec[static_cast<int>(scent_value * color_map.max_iter)];
						//trail_map_vertices[k].color = color_map.color_vec[static_cast<int>(scent_value * color_map.max_iter)];

					}
					trail_map_vertices.append(pixel);
					simulation.trail_map.matrix[i][j] = std::max(0.0f, scent_value - 0.025f);
				}
				//else {
				//	trail_map_vertices[k].color = sf::Color::Black;
				//}
				//k += 1;

			}

		}

	}


};