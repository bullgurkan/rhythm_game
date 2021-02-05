#pragma once
#include "InputManager.hpp"

struct InputData
{
public:
	int time;
	int colorToPop;
	InputManager::InputType inputType;

	InputData(int time, int colorToPop, InputManager::InputType inputType) :time{ time }, colorToPop{ colorToPop }, inputType{ inputType } {};

};