#include "GameManager.hpp"

const sf::Vector2f screenSize(1920, 1080);
const std::vector<sf::Color> defaultColors = { sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Cyan };

GameManager::GameManager() : skin(1, defaultColors, screenSize), window(sf::VideoMode((unsigned int)screenSize.x, (unsigned int)screenSize.y), "Rhythm Game"), loadedSong{ nullptr }, input()
{}

GameManager::~GameManager()
{
    if (loadedSong != nullptr)
        delete loadedSong;
}

void GameManager::LoadSong()
{
    if (loadedSong != nullptr)
    {
        delete loadedSong;
        loadedSong = nullptr;
    }
        
    std::vector<Note> notes;
    notes.push_back(Note(10000, sf::Vector2f(0.01f, 0.01f), 0));
    loadedSong = new Song(notes, 1.0f, skin);
    
}

void GameManager::Start()
{

    long time = 0;
    long lastTime = (long)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    long currentTime = 0;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return;
            }
                
        }
        currentTime = (long)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        if (currentTime - lastTime != 0)
        {
            time += currentTime - lastTime;
            lastTime = currentTime;


            if (loadedSong != nullptr)
            {
                input.pollInputs(time, *loadedSong);

                window.clear();
                loadedSong->render(time, window);
                window.display();
            }
            else
                return;

           
        }


    }
}

