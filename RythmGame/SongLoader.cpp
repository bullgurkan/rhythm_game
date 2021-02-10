#include "SongLoader.hpp"
#include "Note.hpp"
#include "LineNotePosFunc.hpp"
#include <iostream>

SongLoader::~SongLoader()
{
	stream.close();
}

void SongLoader::openFile(SongData& songData)
{
	if (openSongData != &songData)
	{
		if (stream.is_open())
			stream.close();
		stream.open(songData.dataPath, std::ifstream::in);
	}

}

void SongLoader::loadGeneral(SongData& songData)
{
	openFile(songData);
	std::vector<std::pair<std::string, std::string>> data = loadTagData("General");
	for (auto& line : data)
	{
		if (line.first == "AudioFilename")
		{
			songData.music.openFromFile(line.second);
		}
		else if (line.first == "ImageFilename")
		{
			songData.image.loadFromFile(line.second);
		}
	}
}
void SongLoader::loadMetadata(SongData& songData)
{
	if (songData.metadata.size() > 0)
		return;
	openFile(songData);
	std::vector<std::pair<std::string, std::string>> data = loadTagData("Metadata");
	for (auto& line : data)
		songData.metadata.insert(line);

}
void SongLoader::loadDifficulty(SongData& songData)
{
	openFile(songData);
	std::vector<std::pair<std::string, std::string>> data = loadTagData("Difficulty");
	for (auto& line : data)
		if (line.first == "Difficulty")
			songData.difficulty = std::stoi(line.second);
}

bool lineHasTag(std::string& line, std::string tagName)
{
	if (line[0] == '[')
	{

		size_t index = line.find_first_of(']');

		if (index != -1)
		{

			if (tagName == "")
				return true;
			else
				return line.substr(1, index - 1) == tagName;


		}
	}
	return false;

}

int readInt(std::string& line, int& index)
{
	return static_cast<unsigned int>(line[index++]) << 8 | static_cast<unsigned int>(line[index++]);
}

void SongLoader::loadNotes(SongData& songData)
{
	//a note is written as follows: funcType, funcData(length varies by func), noteType, hitTime, color, holdNoteLength(if holdNote)
	if (songData.notes.size() > 0)
		return;
	openFile(songData);
	std::string line;
	bool foundTag = false;
	while (std::getline(stream, line))
	{
		if (!foundTag)
		{
			if (lineHasTag(line, "Notes"))
				foundTag = true;
		}
		else
		{
			if (!lineHasTag(line, ""))
			{
				if (line.find_first_of("//") == 0)
					continue;

				int index = 0;
				NotePosFunc* func = nullptr;
				switch (line[index++])
				{
				case 0:

					func = new LineNotePosFunc(readInt(line, index));
					break;
				case 1:

					break;
				}

				Note::NoteType noteType = (Note::NoteType)(line[index++]);
				int noteHitTime = readInt(line, index);
				switch (noteType)
				{
				case Note::NoteType::PRESS:
				case Note::NoteType::HOLD_START:
					songData.notes.push_back(new Note(noteHitTime, func, line[index++], noteType));
					break;
				case Note::NoteType::HOLD_END:
					//songData.notes.push_back(new Note(line[index++] - '0', func, line[index++] - '0', line[index++] - '0'));
					break;
				default:
					break;
				}

			}
			else
				break;
		}
	}
}



std::vector<std::pair<std::string, std::string>> SongLoader::loadTagData(std::string tagName)
{
	if (stream.is_open())
	{
		std::vector<std::pair<std::string, std::string>> tagData;
		std::string line;
		bool foundTag = false;
		while (std::getline(stream, line))
		{
			if (!foundTag)
			{
				if (lineHasTag(line, tagName))
					foundTag = true;
			}
			else
			{
				if (!lineHasTag(line, ""))
				{
					if (line.find_first_of("//") == 0)
						continue;
					int index = (int)line.find_first_of(':');
					if (index != -1)
					{
						tagData.push_back(std::pair<std::string, std::string>(line.substr(0, index), line.substr((long)index + 1)));
					}

				}
				else
					return tagData;
			}
		}
	}
}




