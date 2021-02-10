#pragma once
#include "NotePosFunc.hpp"

class Note 
{
public:
	enum class NoteType
	{
		PRESS,
		HOLD_START,
		HOLD_END
	};
	Note(int hitTime, NotePosFunc *func, int color, NoteType type);
	Note(int hitTime, NotePosFunc *func, int color, int holdNoteLength);
	~Note();
	sf::Vector2f getPosition(int time, Skin &skin);
	void setEndPos(sf::Vector2f endPosIn);
	int color;
	int hitTime;
	int holdNoteLength;
	NoteType noteType;
private:
	sf::Vector2f endPos;
	NotePosFunc *func;

};