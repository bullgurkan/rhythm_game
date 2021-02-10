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
		stream.open(songData.songDir + songData.songFileName, std::ifstream::in);
	}

}

void SongLoader::loadGeneral(SongData& songData)
{
	openFile(songData);

	songData.music = new sf::Music();
	songData.image = new sf::Texture();

	std::vector<std::pair<std::string, std::string>> data = loadTagData("General");
	for (auto& line : data)
	{
		if (line.first == "AudioFilename")
		{
			songData.music->openFromFile(songData.songDir + line.second);
		}
		else if (line.first == "ImageFilename")
		{
			songData.image->loadFromFile(songData.songDir + line.second);
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
	std::map<int, Note*> holdNoteStarts;
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
				int color = line[index++];
				Note* note;
				switch (noteType)
				{
				
					
				case Note::NoteType::PRESS:
					note = new Note(noteHitTime, func, color, noteType);
					break;
				case Note::NoteType::HOLD_START:
					note = new Note(noteHitTime, func, color, noteType);
					holdNoteStarts.insert_or_assign(color, note);
					break;

				case Note::NoteType::HOLD_END:
					note = new Note(noteHitTime, func, color, holdNoteStarts[color]);
					break;
				default:
					break;
				}

				songData.notes.push_back(note);
			}
			else
				break;
		}
	}
}



std::vector<std::pair<std::string, std::string>> SongLoader::loadTagData(std::string tagName)
{
	std::vector<std::pair<std::string, std::string>> tagData;
	if (stream.is_open())
	{
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
						tagData.push_back(std::pair<std::string, std::string>(line.substr(0, index), line.substr(static_cast<long>(index) + 1)));
					}

				}
				else
					return tagData;
			}
		}
	}
	return tagData;
}




