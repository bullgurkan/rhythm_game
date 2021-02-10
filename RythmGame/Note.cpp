#include "Note.hpp"
#include <iostream>


Note::Note(int hitTime, NotePosFunc *func, int color, NoteType noteType) : hitTime{ hitTime }, endPos{ sf::Vector2f(0,0) }, func{ func }, color{ color }, noteType{ noteType }, holdNoteLength{0}
{
	if (noteType == NoteType::HOLD_END)
	{
		std::cout << "NoteType was HOLD_END, but the PRESS/HOLD_START constructor was used, note will be changed to press to prevent crashes" << std::endl;
		noteType = NoteType::PRESS;
	}
		
}
Note::Note(int hitTime, NotePosFunc* func, int color, int holdNoteLength) : hitTime{ hitTime }, endPos{ sf::Vector2f(0,0) }, func{ func }, color{ color }, noteType{ NoteType::HOLD_END }, holdNoteLength{ holdNoteLength }
{}

Note::~Note()
{
	delete func;
}

sf::Vector2f Note::getPosition(int time, Skin &skin)
{
	if (time <= hitTime)
	{
		return endPos + func->GetPosition(endPos, hitTime - time, skin);
	}
		
	else
		return sf::Vector2f(0, 0);
}

void Note::setEndPos(sf::Vector2f endPosIn)
{
	
	endPos = endPosIn;
}



