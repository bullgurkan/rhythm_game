#include "SongData.hpp"

SongData::~SongData()
{
	if (image != nullptr)
		delete image;
	if (music != nullptr)
	{
		delete music;
		delete musicData;
	}
		
	for (auto note : notes)
		delete note;
}
