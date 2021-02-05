#pragma once
#include <SFML/System/Vector2.hpp>

class Note 
{
public:
	enum class NoteType
	{
		PRESS,
		HOLD_START,
		HOLD_END
	};
	Note(int hitTime, sf::Vector2f direction, int color, NoteType type);
	Note(int hitTime, sf::Vector2f direction, int color, int holdNoteLength);
	sf::Vector2f getPosition(int time);
	void setEndPos(sf::Vector2f endPosIn);
	int color;
	int hitTime;
	int holdNoteLength;
	NoteType noteType;
private:
	
	sf::Vector2f direction;
	sf::Vector2f endPos;

};