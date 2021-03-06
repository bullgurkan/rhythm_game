#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Note.hpp"

struct SongData
{
public:
	sf::Texture *image;
	sf::Sound *music;
	sf::SoundBuffer *musicData;
	std::map<std::wstring, std::wstring> metadata;
	std::wstring songDir;
	std::wstring songFileName;
	std::vector<Note*> notes;
	float difficulty;
	int amountOfKeys;
	int previewTime;
	~SongData();
};