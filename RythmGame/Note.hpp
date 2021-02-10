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
	Note(int hitTime, NotePosFunc *func, int color, Note* holdStart);
	~Note();
	sf::Vector2f getPosition(int time, Skin &skin);
	void setEndPos(sf::Vector2f endPosIn);
	int getHoldTime();
	int color;
	int hitTime;
	NoteType noteType;
	Note* holdStart;
private:
	sf::Vector2f endPos;
	NotePosFunc *func;


};