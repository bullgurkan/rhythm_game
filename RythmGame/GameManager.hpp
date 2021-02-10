#pragma once
#include <SFML/Graphics.hpp>
#include "Song.hpp"
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
	Song *loadedSong;
	InputManager input;
	SongLoader songLoader;
	std::vector<SongData> songsInMenu;
};