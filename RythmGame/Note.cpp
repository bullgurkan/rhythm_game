#include "Note.hpp"
#include <iostream>


Note::Note(float hitTime, sf::Vector2f direction, int color, NoteType noteType) : hitTime{ hitTime }, endPos{ sf::Vector2f(0,0) }, direction{ direction }, color{ color }, noteType{ noteType }, holdNoteLength{0}
{
	if (noteType == NoteType::HOLD_END)
	{
		std::cout << "NoteType was HOLD_END, but the PRESS/HOLD_START constructor was used, note will be changed to press to prevent crashes" << std::endl;
		noteType = NoteType::PRESS;
	}
		
}
Note::Note(float hitTime, sf::Vector2f direction, int color, float holdNoteLength) : hitTime{ hitTime }, endPos{ sf::Vector2f(0,0) }, direction{ direction }, color{ color }, noteType{ NoteType::HOLD_END }, holdNoteLength{ holdNoteLength }
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



