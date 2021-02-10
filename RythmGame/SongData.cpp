#include "SongData.hpp"

SongData::~SongData()
{
	if (image != nullptr)
		delete image;
	if (music != nullptr)
		delete music;

}
