#pragma once

#include <iostream>
#include <vector>
#include <cmath>

#include "SFML/Graphics.hpp"

#include "Grid.hpp"
#include "Agent.hpp"
#include "TrailMap.hpp"


class Simulation {


public:
	Grid grid;
	std::vector<Agent> list_of_agents{};
	TrailMap trail_map;
	std::random_device rd{};
	std::mt19937 gen{rd()};
	std::uniform_real_distribution<float> dis{0.0, 1.0};

	Simulation(Grid grid, std::vector<Agent> list_of_agents, TrailMap trail_map) :
		grid{ grid },
		list_of_agents{ list_of_agents },
		trail_map{ trail_map } {
	}

	float sense(Agent& agent, float sensor_angle_offset) {
		float sensor_angle = agent.get_orientation() + sensor_angle_offset;

		sf::Vector2i sensor_matrix_position;
		sensor_matrix_position.x = static_cast<int>(agent.get_position().x + agent.sensor_offset_distance * cos(sensor_angle) + grid.x_max);
		sensor_matrix_position.y = static_cast<int>(-1.0f * (agent.get_position().y + agent.sensor_offset_distance * sin(sensor_angle) ) + grid.y_max);
		float sum{ 0 };

		for (int offset_x = -agent.sensor_width; offset_x <= agent.sensor_width; offset_x++) {
			for (int offset_y = -agent.sensor_width; offset_y <= agent.sensor_width; offset_y++) {

				sf::Vector2i pixel_pos = sensor_matrix_position + sf::Vector2i(offset_x, offset_y);

				if (pixel_pos.x < static_cast<int>(grid.width) && pixel_pos.x >= 0 && pixel_pos.y < static_cast<int>(grid.height) && pixel_pos.y >= 0) {
					//std::cout << trail_map.matrix[pixel_pos.y][pixel_pos.x] << std::endl;
					sum += trail_map.matrix[pixel_pos.y][pixel_pos.x];
				}
			}
		}
		//std::cout << std::endl;
		return sum;
	}

	void update_agent_position(Agent& agent, bool perturb) {

		sf::Vector2f current_position{agent.get_position()};

		if (perturb) {
			current_position += sf::Vector2f(2.0f * (dis(gen) - 0.5f), 2.0f * (dis(gen) - 0.5f));
			agent.orientation = 360.0f * dis(gen);
		}



		sf::Vector2i matrix_position;
		matrix_position.x = static_cast<int>(agent.get_position().x + grid.x_max);
		matrix_position.y = static_cast<int>(-1.0f * agent.get_position().y + grid.y_max);

		std::vector<sf::Vector2i> pixels_in_vicinity;

		float weight_forward = sense(agent, 0);	
		float weight_left = sense(agent, agent.sensor_angle_offset);
		float weight_right = sense(agent, -agent.sensor_angle_offset);

		if (weight_forward >= weight_left && weight_forward >= weight_right) {
			agent.update_orientation(agent.get_orientation() + (dis(gen) - 0.5f) * agent.sensor_angle_offset);;
		}
		else if (weight_left > weight_right) {
			agent.update_orientation(agent.get_orientation() + agent.sensor_angle_offset * dis(gen)); // TODO 0.2f is a turning speed
		}
		else if (weight_right > weight_left) {
			agent.update_orientation(agent.get_orientation() - agent.sensor_angle_offset * dis(gen)); // TODO 0.2f is a turning speed
		}
		else {
			if (dis(gen) < 0.5) {
				agent.update_orientation(agent.get_orientation() + agent.sensor_angle_offset * dis(gen));// TODO 0.2f is a turning speed
			}
			else {
				agent.update_orientation(agent.get_orientation() - agent.sensor_angle_offset * dis(gen));// TODO 0.2f is a turning speed
			}


		}

		if (current_position.x < grid.x_min || current_position.x > grid.x_max) {

			current_position.x =  std::max(std::min(current_position.x, grid.x_max), grid.x_min);
			//agent.update_x_velocity(agent.get_velocity_vector().x * -1.0f);
			agent.set_position(current_position);
			agent.update_orientation(dis(gen) * 360.0f);
		}
		if (current_position.y < grid.y_min || current_position.y > grid.y_max) {

			current_position.y = std::max(std::min(current_position.y, grid.y_max), grid.y_min);
			//agent.update_y_velocity(agent.get_velocity_vector().y * -1.0f);
			agent.set_position(current_position);
			agent.update_orientation(dis(gen) * 360.0f);
		}
		else {
			agent.set_position(agent.get_velocity_vector() + current_position);
		}

		

	}

	void update_trail_map_with_agents(Agent& agent) {

		sf::Vector2i matrix_position;
		matrix_position.x = static_cast<int>(agent.get_position().x + grid.x_max);
		matrix_position.y = static_cast<int>(- 1.0f * agent.get_position().y + grid.y_max);
		if (matrix_position.x < static_cast<int>(grid.width) && matrix_position.x >= 0 && matrix_position.y < static_cast<int>(grid.height) && matrix_position.y >= 0) {
			trail_map.matrix[matrix_position.y][matrix_position.x] = 1.0f;
		}

	}

	void update_trail_map() {

		TrailMap empty_trail_map(trail_map.width, trail_map.height);

		for (int i{ 0 }; i < static_cast<int>(grid.height); i++) {
			for (int j{ 0 }; j < static_cast<int>(grid.width); j++) {
				float sum{};

				for (int offset_x = -1; offset_x <= 1; offset_x++) {
					for (int offset_y = -1; offset_y <= 1; offset_y++) {

						sf::Vector2i pixel_pos = sf::Vector2i(j, i) + sf::Vector2i(offset_x, offset_y);

						if (pixel_pos.x < static_cast<int>(grid.width) && pixel_pos.x >= 0 && pixel_pos.y < static_cast<int>(grid.height) && pixel_pos.y >= 0) {
							//std::cout << trail_map.matrix[pixel_pos.y][pixel_pos.x] << std::endl;
							sum += trail_map.matrix[pixel_pos.y][pixel_pos.x];
						}
					}
				}

				empty_trail_map.matrix[i][j] = std::max(0.0f, sum / 9);
			}
		}
		trail_map = empty_trail_map;
	}

	void add_list_of_agents(std::vector<Agent> new_list_of_agents) {
		list_of_agents = new_list_of_agents;
	}

	std::vector<Agent> get_list_of_agents() {
		return list_of_agents;
	}

	void step() {
		float nudge{ dis(gen) };
		bool perturb{ false };

		if (nudge < 0.01) {
			perturb = true;
			std::cout << "Perturb!\n";
		}
		for (Agent& agent : list_of_agents) {
				
			update_agent_position(agent, perturb);
			//update_trail_map();
			update_trail_map_with_agents(agent);
			

		}

	}
};