#pragma once
#include <SFML/Graphics.hpp>
#include "SongPlayer.hpp"
#include <chrono>
#include "InputManager.hpp"
#include "SongLoader.hpp"

class GameManager
{
public:
	GameManager();
	~GameManager();
	void LoadSong();
	void Start();

private:
	sf::RenderWindow window;
	Skin skin;
	SongPlayer *loadedSong;
	SongData *loadedSongData;
	InputManager input;
	SongLoader songLoader;
	std::vector<SongData> songsInMenu;
	float playbackSpeed;
	MenuManager menuManager;
	sf::Font font;
};