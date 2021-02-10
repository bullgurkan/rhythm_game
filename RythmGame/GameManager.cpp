#include "GameManager.hpp"
#include "LineNotePosFunc.hpp"
#include <iostream>

const sf::Vector2f screenSize(1920, 1080);
const std::vector<sf::Color> defaultNoteColors = { sf::Color(255, 0, 102), sf::Color::Green, sf::Color(128, 0, 255), sf::Color::Cyan };
const std::vector<sf::Color> defaultHitColors = { sf::Color::Red, sf::Color(255,140,0), sf::Color::White, sf::Color(0, 191, 255), sf::Color::Blue };

GameManager::GameManager() : skin(20, defaultNoteColors, defaultHitColors, screenSize), window(sf::VideoMode((unsigned int)screenSize.x, (unsigned int)screenSize.y), "Rhythm Game"), loadedSong{ nullptr }, input()
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
        
    SongData sd;
    sd.dataPath = "C:/Users/Gustav/source/repos/RythmGame/Songs/TestSong/TestSong.txt";
    songLoader.loadNotes(sd);

  
    loadedSong = new Song(sd.notes, 30, 100, 1000, skin);
    
}

void GameManager::Start()
{

    long time = 0;
    long lastTime = (long)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    long currentTime = 0;
    bool hasPrintedAcc = false;

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

            if (loadedSong != nullptr && !loadedSong->songHasEnded(time))
            {
                input.pollInputs(time, *loadedSong);
                window.clear();
                loadedSong->render(time, window);
                window.display();
            }
            else
            {
                if (!hasPrintedAcc)
                {
                    std::cout << loadedSong->getAccuracy() * 100 << "%" << std::endl;
                    hasPrintedAcc = true;
                }
               
            }

           
        }
    }
}

