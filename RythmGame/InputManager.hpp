#pragma once
#include <vector>
#include <unordered_map>
#include "KeyBind.hpp"
#include "MenuManager.hpp"
class SongPlayer;

class InputManager
{
public:
	InputManager(std::unordered_map<sf::Keyboard::Key, KeyBind> keyBinds, std::unordered_map<sf::Mouse::Button, KeyBind> mouseButtonBinds);
	void executeInputEvent(int time, SongPlayer* song, MenuManager& menuManager, sf::Event e);


private:
	void executeAction(int time, SongPlayer* song, MenuManager& menuManager, KeyBind::KeyAction action, KeyBind& keyBind, KeyBind::KeyState keyState);

	//std::vector<std::vector<KeyBind>> inGameKeyBinds;
	std::unordered_map<sf::Keyboard::Key, KeyBind> keyBinds;
	std::unordered_map<sf::Mouse::Button, KeyBind> mouseButtonBinds;
	//std::vector<bool> inGameKeysPressed;
	
};