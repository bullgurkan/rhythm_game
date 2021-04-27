#pragma once
#include "InputManager.hpp"

struct InputData
{
public:
	int time;
	int colorToPop;
	KeyBind::KeyState keyState;

	InputData(int time, int colorToPop, KeyBind::KeyState keyState) :time{ time }, colorToPop{ colorToPop }, keyState{ keyState } {};

};