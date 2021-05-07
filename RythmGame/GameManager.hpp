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
	void LoadSong(SongData* selectedSongData);
	void Start();

	std::vector<SongData> songs;
	SongLoader songLoader;
	float playbackSpeed;
private:
	sf::RenderWindow window;
	Skin skin;
	SongPlayer *songPlayer;
	SongData* selectedSongData;
	InputManager input;
	MenuManager menuManager;
	sf::Font font;
	long time;

};