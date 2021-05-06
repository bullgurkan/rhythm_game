#include "GameManager.hpp"
#include "LineNotePosFunc.hpp"
#include "AutoPanel.hpp"
#include "Button.hpp"
#include <iostream>
#include <filesystem>

#include "KeyBindDefault.hpp"
#include "SceneDefaults.hpp"


const sf::Vector2f screenSize(1920, 1080);
const std::vector<sf::Color> defaultNoteColors = { sf::Color(255, 0, 102), sf::Color::Green, sf::Color(128, 0, 255), sf::Color::Cyan };
const std::vector<sf::Color> defaultHitColors = { sf::Color::Red, sf::Color(255,140,0), sf::Color::White, sf::Color(0, 191, 255), sf::Color::Blue };

const std::string pathBase = "C:/Users/spp104/Documents/rhythm_game/x64/Release/";

GameManager::GameManager() : skin(30, defaultNoteColors, defaultHitColors, screenSize, sf::Color(255, 255, 255, 150)), window(sf::VideoMode((unsigned int)screenSize.x, (unsigned int)screenSize.y), "Rhythm Game"), songPlayer{ nullptr }, input(keyBinds, mouseButtonBinds), selectedSongData{ nullptr }, playbackSpeed{ 1 }, menuManager()
{
	window.setKeyRepeatEnabled(false);
	font.loadFromFile(pathBase + "Fonts/good_times_rg.ttf");

	for (const auto& entry : std::filesystem::directory_iterator(pathBase + "Songs"))
	{

		if (entry.is_directory())
		{
			for (const auto& songEntry : std::filesystem::directory_iterator(entry.path()))
			{
				if (songEntry.is_regular_file())
				{
					int index = static_cast<int>(songEntry.path().filename().wstring().find('.'));
					if (index != -1)
					{

						std::wstring songFileType = songEntry.path().filename().wstring().substr(index);
						if (songFileType.find(L"song") != -1 || songFileType.find(L"osu") != -1)
						{
							songs.push_back(SongData());
							songs[songs.size() - 1].songDir = entry.path().wstring() + L"/";
							songs[songs.size() - 1].songFileName = songEntry.path().filename().wstring();
							songLoader.loadMetadata(songs[songs.size() - 1]);
						}
					}
				}

			}
		}

	}


	GenerateScenes(menuManager, font, *this);
	menuManager.setActiveScene(MenuManager::Scene::SONG_SELECT);
	playbackSpeed = 1;

}

GameManager::~GameManager()
{
	if (songPlayer != nullptr)
		delete songPlayer;
}

void GameManager::LoadSong(SongData* selectedSongData)
{



	if (songPlayer != nullptr)
	{
		delete songPlayer;
		songPlayer = nullptr;
	}


	songLoader.loadNotes(*selectedSongData);

	songPlayer = new SongPlayer(selectedSongData, 30, 100, 300, skin);
	this->selectedSongData = selectedSongData;

	//move to option file and or modifier
	//playbackSpeed = std::stof(loadedSongData->metadata["Speed"]);


	this->selectedSongData->music->setVolume(25);
	this->selectedSongData->music->setPitch(playbackSpeed);
	menuManager.setActiveScene(MenuManager::Scene::IN_GAME);
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
			input.executeInputEvent(static_cast<int>(time * playbackSpeed), (songPlayer != nullptr && !songPlayer->songHasEnded(static_cast<int>(time * playbackSpeed))) ? songPlayer : nullptr, menuManager, event);

		}
		currentTime = (long)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		if (currentTime - lastTime != 0)
		{
			time += (currentTime - lastTime);
			lastTime = currentTime;
			if (!hasPrintedAcc && songPlayer != nullptr && !songPlayer->songHasEnded(static_cast<int>(time * playbackSpeed)))
			{
				
				if (time > 0 && selectedSongData->music->getStatus() != sf::Music::Playing)
				{
					selectedSongData->music->play();
					selectedSongData->music->setPlayingOffset(sf::milliseconds(static_cast<int>(time * playbackSpeed)));
				}


				window.clear();
				songPlayer->render(static_cast<int>(time * playbackSpeed), window);
			}
			else
			{
				if (!hasPrintedAcc && songPlayer != nullptr)
				{
					std::cout << songPlayer->getAccuracy() * 100 << "%" << std::endl;
					hasPrintedAcc = true;
				}

			}

			menuManager.render(time, window, skin);
			window.display();
			window.clear();
		}
	}
}

