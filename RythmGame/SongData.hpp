#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Note.hpp"

struct SongData
{
public:
	sf::Texture image;
	sf::Music music;
	std::map<std::string, std::string> metadata;
	std::string dataPath;
	std::vector<Note*> notes;
	int difficulty;


};