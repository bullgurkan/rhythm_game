#pragma once
#include <vector>
#include <SFML/Window/Keyboard.hpp>
#include "Song.hpp"

class InputManager
{
public:
	InputManager();
	void pollInputs(float time, Song &song);

private:
	std::vector<sf::Keyboard::Key> keyBinds;
	std::vector<bool> keyPressed;
};