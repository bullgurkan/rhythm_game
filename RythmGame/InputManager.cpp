#include "InputManager.hpp"
#include "SongPlayer.hpp"
#include <iostream>

InputManager::InputManager(std::unordered_map<sf::Keyboard::Key, KeyBind> keyBinds, std::unordered_map<sf::Mouse::Button, KeyBind> mouseButtonBinds) : keyBinds{ keyBinds }, mouseButtonBinds{ mouseButtonBinds }
{}

void InputManager::executeInputEvent(int time, SongPlayer* song, MenuManager& menuManager, sf::Event e)
{

	KeyBind::KeyState keyState = KeyBind::KeyState::Released;

	switch (e.type)
	{
	case sf::Event::EventType::KeyPressed:
		keyState = KeyBind::KeyState::Pressed;
		[[fallthrough]];
	case sf::Event::EventType::KeyReleased:
	{
		auto keyBindSearch = keyBinds.find(e.key.code);
		if (keyBindSearch != keyBinds.end())
			for (KeyBind::KeyAction action : keyBindSearch->second.actions)
				executeAction(time, song, menuManager, action, keyBindSearch->second, keyState);
	}
	break;

	case sf::Event::EventType::MouseButtonPressed:
		keyState = KeyBind::KeyState::Pressed;
		[[fallthrough]];
	case sf::Event::EventType::MouseButtonReleased:
	{
		auto keyBindSearch = mouseButtonBinds.find(e.mouseButton.button);
		if (keyBindSearch != mouseButtonBinds.end())
			for (KeyBind::KeyAction action : keyBindSearch->second.actions)
				executeAction(time, song, menuManager, action, keyBindSearch->second, keyState);
	}
	break;
	case sf::Event::EventType::TextEntered:
		menuManager.textEntered(e.text.unicode);
		break;
	case sf::Event::EventType::MouseMoved:
		menuManager.mouseMoved(e.mouseMove.x, e.mouseMove.y);
		break;
	default:
		break;
	}


}


void InputManager::executeAction(int time, SongPlayer* song, MenuManager& menuManager, KeyBind::KeyAction action, KeyBind& keyBind, KeyBind::KeyState keyState)
{

	if (action == KeyBind::KeyAction::InGameColor)
	{
		if (song != nullptr && keyBind.keyAmountToColorId != nullptr)
		{
			auto keyColorSearch = (*keyBind.keyAmountToColorId).find(song->songData->amountOfKeys);
			if(keyColorSearch != (*keyBind.keyAmountToColorId).end())
				song->popNoteWithColor(InputData(time, keyColorSearch->second, keyState));
		}
			
	}
	else
	{
		menuManager.keyChanged(action, keyState);
	}

}
