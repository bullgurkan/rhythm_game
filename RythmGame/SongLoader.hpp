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
	void loadNotesFromOsuFile(SongData& songData, int speed);
	void loadNotesFromSongFile(SongData& songData);

	std::wifstream stream;
	SongData* openSongData;
	std::vector<std::pair<std::wstring, std::wstring>> loadTagData(std::wstring tagName);
};
