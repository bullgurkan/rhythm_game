#pragma once
#include "NotePosFunc.hpp"

class LineNotePosFunc : public NotePosFunc
{
public:
	LineNotePosFunc(int speed);
	virtual sf::Vector2f GetPosition(sf::Vector2f endPos, int timeLeftUntilHit, Skin& skin);

};