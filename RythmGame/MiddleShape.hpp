#pragma once
#include <SFML/Graphics/Shape.hpp>

struct MiddleShape
{
	MiddleShape(sf::Shape* shape) : shape{ shape }, lastHitTime { 0 } {}
	int lastHitTime;
	sf::Shape* shape;
};