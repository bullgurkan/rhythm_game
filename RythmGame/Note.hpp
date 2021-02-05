#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

class Note 
{
public:
	enum class NoteType
	{
		PRESS,
		HOLD_START,
		HOLD_END
	};
	Note(float hitTime, sf::Vector2f direction, int color, NoteType type);
	Note(float hitTime, sf::Vector2f direction, int color, float holdNoteLength);
	sf::Vector2f getPosition(long time);
	void setEndPos(sf::Vector2f endPosIn);
	int color;
	float hitTime;
	float holdNoteLength;
	NoteType noteType;
private:
	
	sf::Vector2f direction;
	sf::Vector2f endPos;

};