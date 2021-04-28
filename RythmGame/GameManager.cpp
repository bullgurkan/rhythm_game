#include "GameManager.hpp"
#include "LineNotePosFunc.hpp"
#include "AutoPanel.hpp"
#include "Button.hpp"
#include <iostream>

#include "KeyBindDefault.hpp"
#include "SceneDefaults.hpp"


const sf::Vector2f screenSize(1920, 1080);
const std::vector<sf::Color> defaultNoteColors = { sf::Color(255, 0, 102), sf::Color::Green, sf::Color(128, 0, 255), sf::Color::Cyan };
const std::vector<sf::Color> defaultHitColors = { sf::Color::Red, sf::Color(255,140,0), sf::Color::White, sf::Color(0, 191, 255), sf::Color::Blue };

const std::string pathBase = "C:/Users/spp104/source/repos/RythmGame/x64/Release/";

GameManager::GameManager() : skin(30, defaultNoteColors, defaultHitColors, screenSize, sf::Color(255, 255, 255, 150)), window(sf::VideoMode((unsigned int)screenSize.x, (unsigned int)screenSize.y), "Rhythm Game"), loadedSong{ nullptr }, input(keyBinds, mouseButtonBinds), loadedSongData{ nullptr }, playbackSpeed{ 1 }, menuManager()
{
	window.setKeyRepeatEnabled(false);
	font.loadFromFile("Fonts/good_times_rg.ttf");
	GenerateScenes(menuManager, font);
	menuManager.setActiveScene(MenuManager::Scene::SONG_SELECT);

}

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

	songsInMenu.push_back(SongData());
	songsInMenu[0].songDir = pathBase+"Songs/TestSong/";
	songsInMenu[0].songFileName = "TestSong.txt";
	songLoader.loadMetadata(songsInMenu[0]);



	songLoader.loadGeneral(songsInMenu[0]);
	songLoader.loadNotes(songsInMenu[0]);

	loadedSong = new SongPlayer(&songsInMenu[0], 30, 100, 300, skin);
	loadedSongData = &songsInMenu[0];

	//move to option file and or modifier
	//playbackSpeed = std::stof(loadedSongData->metadata["Speed"]);
	playbackSpeed = 5;

	loadedSongData->music->setVolume(25);
	loadedSongData->music->setPitch(playbackSpeed);

}

void GameManager::Start()
{

	long time = -3000;
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
			input.executeInputEvent(static_cast<int>(time * playbackSpeed), (loadedSong != nullptr && !loadedSong->songHasEnded(static_cast<int>(time * playbackSpeed))) ? loadedSong : nullptr, menuManager, event);

		}
		currentTime = (long)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		if (currentTime - lastTime != 0)
		{
			time += (currentTime - lastTime);
			lastTime = currentTime;

			if (!hasPrintedAcc && loadedSong != nullptr && !loadedSong->songHasEnded(static_cast<int>(time * playbackSpeed)))
			{
				if (time > 0 && loadedSongData->music->getStatus() != sf::Music::Playing)
				{
					loadedSongData->music->play();
					loadedSongData->music->setPlayingOffset(sf::milliseconds(static_cast<int>(time * playbackSpeed)));
				}


				window.clear();
				loadedSong->render(static_cast<int>(time * playbackSpeed), window);
			}
			else
			{
				if (!hasPrintedAcc)
				{
					if (loadedSong != nullptr)
						std::cout << loadedSong->getAccuracy() * 100 << "%" << std::endl;
					hasPrintedAcc = true;
				}

			}

			menuManager.render(time, window, skin);
			window.display();
			window.clear();
		}
	}
}

