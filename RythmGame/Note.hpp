#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

class Note 
{
public:
	Note(float hitTime, sf::Vector2f direction, int color);
	sf::Vector2f getPosition(long time);
	void setEndPos(sf::Vector2f endPosIn);
	int color;
	float hitTime;
private:
	
	sf::Vector2f direction;
	sf::Vector2f endPos;

};