#pragma once
#include <fstream>
#include <string>
#include "SongData.hpp"
class SongLoader
{
public:
	~SongLoader();
	void loadGeneral(SongData &songData);
	void loadMetadata(SongData& songData);
	void loadDifficulty(SongData& songData);
	void loadNotes(SongData& songData);
private:
	void openFile(SongData& songData);
private:
	std::ifstream stream;
	SongData* openSongData;
	std::vector<std::pair<std::string, std::string>> loadTagData(std::string tagName);
};
