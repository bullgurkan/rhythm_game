#pragma once
#include <vector>
#include <SFML/Window/Keyboard.hpp>
class Song;

class InputManager
{
public:
	InputManager();
	void pollInputs(int time, Song &song);
	enum class InputType
	{
		PRESSED,
		RELEASED
	};

private:
	std::vector<sf::Keyboard::Key> keyBinds;
	std::vector<bool> keyPressed;
	
};