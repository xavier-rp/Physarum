#pragma once
#include <iostream>
#include <vector>
#include <random>

#include "Grid.hpp"
#include "Agent.hpp"


std::vector<Agent> build_list_of_agents_origin_radial(int nb_of_agents) {

	std::vector<Agent> list_of_agents;

	for (int i{ 0 }; i < nb_of_agents; i++) {
		float delta_theta = 360.0f / nb_of_agents;

		list_of_agents.push_back(Agent{ sf::Vector2f {0, 0}, i * delta_theta });
	}

	return list_of_agents;
}

std::vector<Agent> build_list_of_agents_random_in_grid(int nb_of_agents, Grid& grid) {

	std::vector<Agent> list_of_agents;

	std::random_device rd{};
	std::mt19937 gen{rd()};

	//std::normal_distribution<> d{0.0, 10.0};
	std::uniform_real_distribution<float> dis_x{grid.x_min, grid.x_max};
	std::uniform_real_distribution<float> dis_y{grid.y_min, grid.y_max};
	std::uniform_real_distribution<float> dis_theta{0.0, 360.0};

	for (int i{ 0 }; i < nb_of_agents; i++) {

		list_of_agents.push_back(Agent{ sf::Vector2f {dis_x(gen), dis_y(gen)}, dis_theta(gen) });
	}

	return list_of_agents;
}

std::vector<Agent> build_list_of_agents_random_on_circle(int nb_of_agents, float radius) {

	std::vector<Agent> list_of_agents;

	std::random_device rd{};
	std::mt19937 gen{rd()};

	//std::normal_distribution<> d{0.0, 10.0};
	std::uniform_real_distribution<float> dis_theta{0.0, 360.0};
	float theta{};

	for (int i{ 0 }; i < nb_of_agents; i++) {

		theta = dis_theta(gen);
		list_of_agents.push_back(Agent{ sf::Vector2f {cos(theta)* radius, sin(theta)* radius}, dis_theta(gen) });
	}

	return list_of_agents;
}

std::vector<Agent> build_list_of_agents_random_on_cat(int nb_of_agents, float x_span) {

	std::vector<Agent> list_of_agents;

	std::random_device rd{};
	std::mt19937 gen{rd()};

	//std::normal_distribution<> d{0.0, 10.0};
	std::uniform_real_distribution<float> dis_theta{0.0, 360.0};
	float theta{};
	float x_step{ x_span / static_cast<float>(nb_of_agents) };
	float current_x{};
	for (int i{ 0 }; i < nb_of_agents; i++) {
		current_x = -x_span / 2.0f + i * x_step;
		if (current_x < -x_span / 2.0f + x_span / 3.0f) {
			list_of_agents.push_back(Agent{ sf::Vector2f {current_x, -std::abs(current_x + (x_span / 2 - x_span / 6)) + x_span / 2.5f}, 0.0f, 0.0f });
		}
		else if (current_x > x_span / 6.0f) {
			list_of_agents.push_back(Agent{ sf::Vector2f {current_x, -std::abs(current_x - x_span / 3.0f) + x_span / 3.0f}, 0.0f, 0.0f });
		}
		else if (current_x > -x_span / 2.0f + x_span / 3.0f && current_x < 0.0f) {
			list_of_agents.push_back(Agent{ sf::Vector2f {current_x, 0.03f * (current_x - (-x_span / 2 + 5 * x_span / 12)) * (current_x - (-x_span / 2 + 5 * x_span / 12)) - x_span / 3}, 0.0f, 0.0f });
		}
		else if (current_x > 0.0f && current_x < x_span / 6.0f) {
			list_of_agents.push_back(Agent{ sf::Vector2f {current_x, 0.03f * (current_x - x_span / 12) * (current_x - x_span / 12) - x_span / 3}, 0.0f, 0.0f });
		}
	}

	x_step = x_span / 8 / 500;
	for (int i{ 0 }; i < 500; i++) {
		current_x = -x_span / 16 + i * x_step;
		list_of_agents.push_back(Agent{ sf::Vector2f {current_x, std::abs(current_x)}, dis_theta(gen), 0.0f });
	}

	return list_of_agents;
}

std::vector<Agent> build_list_of_agents_random_inside_circle(int nb_of_agents, float radius) {

	std::vector<Agent> list_of_agents;

	std::random_device rd{};
	std::mt19937 gen{rd()};

	//std::normal_distribution<> d{0.0, 10.0};
	std::uniform_real_distribution<float> dis_theta{0.0, 360.0};
	std::uniform_real_distribution<float> dis_zero_one{0.0f, 1.0f};
	float theta{};

	for (int i{ 0 }; i < nb_of_agents; i++) {
		float radius_multiplicator{ dis_zero_one(gen) };
		theta = dis_theta(gen);
		list_of_agents.push_back(Agent{ sf::Vector2f {cos(theta)* radius* radius_multiplicator, sin(theta)* radius* radius_multiplicator}, dis_theta(gen) });
	}

	return list_of_agents;
}