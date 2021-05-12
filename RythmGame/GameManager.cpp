#include "GameManager.hpp"
#include "LineNotePosFunc.hpp"
#include "AutoPanel.hpp"
#include "Button.hpp"
#include <iostream>
#include <filesystem>

#include "KeyBindDefault.hpp"
#include "SceneDefaults.hpp"


const sf::Vector2f screenSize(1920, 1080);
const std::vector<sf::Color> defaultNoteColors = { sf::Color(255, 0, 102), sf::Color(0, 255, 0), sf::Color(148, 0, 211), sf::Color::Cyan };
const std::vector<sf::Color> defaultHitColors = { sf::Color::Red, sf::Color(255,140,0), sf::Color::White, sf::Color(0, 191, 255), sf::Color::Blue };


GameManager::GameManager() : skin(30, defaultNoteColors, defaultHitColors, screenSize, sf::Color(255, 255, 255, 130)), window(sf::VideoMode((unsigned int)screenSize.x, (unsigned int)screenSize.y), "Rhythm Game"), songPlayer{ nullptr }, input(keyBinds, mouseButtonBinds), selectedSongData{ nullptr }, playbackSpeed{ 1 }, menuManager(*this)
{

	playbackSpeed = 1.0f;
	std::string basepath = std::filesystem::current_path().string();
	window.setKeyRepeatEnabled(false);
	font.loadFromFile(basepath + "/Fonts/good_times_rg.ttf");

	for (const auto& entry : std::filesystem::directory_iterator(basepath + "/Songs"))
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
							songLoader.loadDifficulty(songs[songs.size() - 1]);
						}
					}
				}

			}
		}

	}


	GenerateScenes(menuManager, font, *this);
	menuManager.setActiveScene(MenuManager::Scene::SONG_SELECT);


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

	if (selectedSongData == nullptr)
		return;
	songLoader.loadNotes(*selectedSongData);

	songPlayer = new SongPlayer(selectedSongData, 30, 100, 300, skin);
	this->selectedSongData = selectedSongData;

	//move to option file and or modifier
	//playbackSpeed = std::stof(loadedSongData->metadata["Speed"]);

	this->selectedSongData->music->stop();
	this->selectedSongData->music->setPitch(playbackSpeed);
	time = -3000;
	menuManager.setActiveScene(MenuManager::Scene::IN_GAME);
}

void GameManager::Start()
{

	time = -3000;
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
			input.executeInputEvent(static_cast<int>(time * playbackSpeed), (songPlayer != nullptr && !songPlayer->songHasEnded(static_cast<int>(time * playbackSpeed))) ? songPlayer : nullptr, menuManager, event);

		}
		currentTime = (long)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		if (currentTime - lastTime != 0)
		{
			time += (currentTime - lastTime);
			lastTime = currentTime;
			if (songPlayer != nullptr && !songPlayer->songHasEnded(static_cast<int>(time * playbackSpeed)))
			{
				
				if (time > 0 && selectedSongData->music->getStatus() != sf::Music::Playing)
				{
					selectedSongData->music->play();
					selectedSongData->music->setPlayingOffset(sf::milliseconds(static_cast<int>(time * playbackSpeed)));
				}

				songPlayer->render(static_cast<int>(time * playbackSpeed), window);
			}
			else
			{
				if (songPlayer != nullptr)
				{
					//TODO save replay
					menuManager.setActiveScene(MenuManager::Scene::POST_GAME);
					songPlayer = nullptr;
				}

			}

			menuManager.render(time, window, skin);
			window.display();
			window.clear();
		}
	}
}

int GameManager::getSongAccuracy()
{
	return static_cast<int>(songPlayer->getAccuracy() * 100);
}

