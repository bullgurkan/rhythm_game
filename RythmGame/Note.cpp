#include "Note.hpp"


Note::Note(float hitTime, sf::Vector2f direction, int color) : hitTime{ hitTime }, endPos{ sf::Vector2f(0,0) }, direction{ direction }, color{ color }
{}

sf::Vector2f Note::getPosition(long time)
{
	if (time <= hitTime)
		return endPos + direction * (hitTime - time);
	else
		return sf::Vector2f(0, 0);
}

void Note::setEndPos(sf::Vector2f endPosIn)
{
	endPos = endPosIn;
}



