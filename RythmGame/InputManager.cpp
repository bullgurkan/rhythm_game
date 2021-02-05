#include "InputManager.hpp"
#include "Song.hpp"

InputManager::InputManager() : keyBinds{ { sf::Keyboard::Key::D, sf::Keyboard::Key::F, sf::Keyboard::Key::J, sf::Keyboard::Key::K } }, keyPressed(keyBinds.size())
{}

void InputManager::pollInputs(int time, Song &song)
{

    for (int i = 0; i < keyBinds.size(); i++)
    {
        
        if (sf::Keyboard::isKeyPressed(keyBinds[i]))
        {
            if (!keyPressed[i])
            {
                keyPressed[i] = true;
                song.popNoteWithColor(InputData(time, i, InputType::PRESSED));
            }
                
        }
        else
        {
            if (keyPressed[i])
            {
                keyPressed[i] = false;
                song.popNoteWithColor(InputData(time, i, InputType::RELEASED));
            }
                
        }
    }

}
