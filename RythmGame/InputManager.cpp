#include "InputManager.hpp"

InputManager::InputManager() : keyBinds{ { sf::Keyboard::Key::D, sf::Keyboard::Key::F, sf::Keyboard::Key::J, sf::Keyboard::Key::K } }, keyPressed(keyBinds.size())
{}

void InputManager::pollInputs(float time, Song &song)
{

    for (size_t i = 0; i < keyBinds.size(); i++)
    {
        
        if (sf::Keyboard::isKeyPressed(keyBinds[i]))
        {
            if (!keyPressed[i])
                song.popNoteWithColor(time, i, KeyEvents::PRESSED);
        }
        else
        {
            if (keyPressed[i])
                song.popNoteWithColor(time, i, KeyEvents::RELEASED);
        }
    }

}
