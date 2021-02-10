#pragma once
#include <SFML/System/Vector2.hpp>
#include "Skin.hpp"

class NotePosFunc
{
public:
	NotePosFunc(int speed) : speed{ speed } {};
	virtual sf::Vector2f GetPosition(sf::Vector2f endPos, int timeLeftUntilHit, Skin &skin) = 0;
protected:
	int speed;
};