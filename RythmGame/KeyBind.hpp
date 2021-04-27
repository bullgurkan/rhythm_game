#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <vector>
#include <unordered_map>
#include <SFML/Window/Event.hpp>



struct KeyBind
{
	enum class KeyAction
	{
		Up,
		Left,
		Down,
		Right,
		Click,
		Back,
		InGameColor
	};

	enum class KeyState
	{
		Pressed,
		Released
	};

public:
	KeyBind(std::vector<KeyAction> actions, std::unordered_map<int, int> *keyAmountToColorId);
	std::vector<KeyAction> actions;
	std::unordered_map<int, int>* keyAmountToColorId;
};