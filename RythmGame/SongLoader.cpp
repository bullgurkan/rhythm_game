#include "SongLoader.hpp"
#include "Note.hpp"
#include "LineNotePosFunc.hpp"
#include <iostream>

int osu_default_note_speed = 1000;

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
		stream.open(songData.songDir + songData.songFileName, std::wifstream::in);
	}

}

void SongLoader::loadGeneral(SongData& songData)
{
	openFile(songData);

	songData.music = new sf::Sound();
	songData.musicData = new sf::SoundBuffer();
	songData.image = new sf::Texture();

	std::vector<std::pair<std::wstring, std::wstring>> data = loadTagData(L"General");
	for (auto& line : data)
	{
		if (line.first == L"AudioFilename")
		{
			std::wstring fileName = line.second;

			if (line.second.find_first_not_of(' ') != -1)
			{
				std::vector<char> buffer;
				std::ifstream tempStream = std::ifstream(songData.songDir + line.second.substr(line.second.find_first_not_of(' ')), std::ifstream::binary);
				if (tempStream.is_open())
				{

					tempStream.seekg(0, tempStream.end);
					const auto length = tempStream.tellg();
					if (!length)
						return;
					buffer.resize(length);
					tempStream.seekg(0, tempStream.beg);
					

					auto start = &*buffer.begin();
					tempStream.read(start, length);
					tempStream.close();
					
					
					
					songData.musicData->loadFromMemory(&buffer[0], buffer.size() * sizeof(char));
					//songData.musicData->loadFromFile("C/Users/spp104/Documents/rhythm_game/x64/Release/Songs/(aperson)Tranzport/15._(Arkitech)_Tranzport.mp3");
					songData.music->setBuffer(*songData.musicData);
					std::cout << songData.music->getBuffer()->getDuration().asSeconds() << std::endl;
				}
			}


		}
		else if (line.first == L"ImageFilename")
		{
			if (line.second.find_first_not_of(' ') != -1)
			{
				std::vector<char> buffer;
				std::ifstream tempStream = std::ifstream(songData.songDir + line.second.substr(line.second.find_first_not_of(' ')), std::ifstream::binary);
				if (tempStream.is_open())
				{
					tempStream.seekg(0, tempStream.end);
					const auto length = tempStream.tellg();
					if (!length)
						return;
					buffer.resize(length);
					tempStream.seekg(0, tempStream.beg);
					auto start = &*buffer.begin();
					tempStream.read(start, length);
					tempStream.close();
					songData.image->loadFromMemory(&buffer[0], buffer.size() * sizeof(char));
				}
			}
		}
	}

}
void SongLoader::loadMetadata(SongData& songData)
{
	if (songData.metadata.size() > 0)
		return;
	openFile(songData);

	std::vector<std::pair<std::wstring, std::wstring>> data = loadTagData(L"Metadata");
	for (auto& line : data)
	{
		songData.metadata.insert(line);
	}



	stream.seekg(0, stream.beg);

	//This should be REPLACED with a check or osu key count
	std::wstring line;
	std::getline(stream, line);
	if (line.find(L"osu") != std::string::npos)
		songData.metadata.insert(std::pair<std::wstring, std::wstring>(L"Keys", L"4"));

	auto keyTagSearch = songData.metadata.find(L"Keys");
	if (keyTagSearch != songData.metadata.end())
		songData.amountOfKeys = std::stoi(keyTagSearch->second);
}
void SongLoader::loadDifficulty(SongData& songData)
{
	openFile(songData);
	std::vector<std::pair<std::wstring, std::wstring>> data = loadTagData(L"Difficulty");
	for (auto& line : data)
		if (line.first == L"Difficulty")
			songData.difficulty = std::stoi(line.second);
}

bool lineHasTag(std::wstring& line, std::wstring tagName)
{
	if (line[0] == '[')
	{

		size_t index = line.find_first_of(']');

		if (index != -1)
		{

			if (tagName == L"")
				return true;
			else
				return line.substr(1, index - 1) == tagName;


		}
	}
	return false;

}

int readInt(std::wstring& line, int& index)
{
	return static_cast<unsigned int>(line[index++]) << 8 | static_cast<unsigned int>(line[index++]);
}

void SongLoader::loadNotes(SongData& songData)
{
	//a note is written as follows: funcType, funcData(length varies by func), noteType, hitTime, color, holdNoteLength(if holdNote)
	if (songData.notes.size() > 0)
		return;
	openFile(songData);
	std::wstring line;

	std::getline(stream, line);
	if (line.find(L"osu") != std::string::npos)
	{
		std::getline(stream, line);
		loadNotesFromOsuFile(songData, osu_default_note_speed);
	}
	else
		loadNotesFromSongFile(songData);
}



std::vector<std::pair<std::wstring, std::wstring>> SongLoader::loadTagData(std::wstring tagName)
{

	std::vector<std::pair<std::wstring, std::wstring>> tagData;
	if (stream.is_open())
	{
		std::wstring line;
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
				if (!lineHasTag(line, L""))
				{
					if (line.find_first_of(L"//") == 0)
						continue;
					int index = (int)line.find_first_of(':');
					if (index != -1)
					{
						tagData.push_back(std::pair<std::wstring, std::wstring>(line.substr(0, index), line.substr(1 + static_cast<long>(index))));
					}

				}
				else
					return tagData;
			}
		}
	}
	return tagData;
}

void SongLoader::loadNotesFromOsuFile(SongData& songData, int speed)
{
	std::wstring line;
	bool foundTag = false;
	std::map<int, Note*> holdNoteStarts;
	while (std::getline(stream, line))
	{
		if (!foundTag)
		{
			if (lineHasTag(line, L"HitObjects"))
				foundTag = true;
		}
		else
		{
			if (!lineHasTag(line, L""))
			{
				if (line.find_first_of(L"//") == 0)
					continue;

				int index = 0;

				int amountOfColors = 4;
				int color = (std::stoi(line.substr(index, line.find_first_of(','))) / 64 - 1) / 2;
				index = static_cast<int>(line.find_first_of(',', index)) + 1;
				index = static_cast<int>(line.find_first_of(',', index)) + 1;

				//std::cout << color << std::endl;

				int noteHitTime = std::stoi(line.substr(index, line.find_first_of(',', index)));
				index = static_cast<int>(line.find_first_of(',', index)) + 1;

				int noteTypeNum = std::stoi(line.substr(index, line.find_first_of(',', index)));
				index = static_cast<int>(line.find_first_of(',', index)) + 1;
				index = static_cast<int>(line.find_first_of(',', index)) + 1;
				int endTime = std::stoi(line.substr(index, line.find_first_of(':', index)));


				NotePosFunc* func = new LineNotePosFunc(speed);

				std::map<int, Note*>::iterator it = holdNoteStarts.begin();

				while (it != holdNoteStarts.end())
				{
					if (it->second->hitTime < noteHitTime)
					{
						songData.notes.push_back(it->second);
						it = holdNoteStarts.erase(it);
					}
					else
					{
						it++;
					}
				}


				if (noteTypeNum != 128)
				{
					songData.notes.push_back(new Note(noteHitTime, func, color, Note::NoteType::PRESS));
				}
				else
				{
					Note* note = new Note(noteHitTime, func, color, Note::NoteType::HOLD_START);
					songData.notes.push_back(note);
					Note* note2 = new Note(endTime, new LineNotePosFunc(speed), color, note);
					holdNoteStarts.insert_or_assign(color, note2);

				}


			}
			else
				break;
		}
	}

	while (holdNoteStarts.size() > 0)
	{
		std::map<int, Note*>::iterator it = holdNoteStarts.begin();
		Note* noteToAdd = nullptr;
		while (it != holdNoteStarts.end())
		{
			if (noteToAdd == nullptr || it->second->hitTime < noteToAdd->hitTime)
			{
				noteToAdd = it->second;
			}
			it++;
		}

		songData.notes.push_back(noteToAdd);
		it = holdNoteStarts.begin();
		while (it != holdNoteStarts.end())
		{
			if (it->second == noteToAdd)
			{
				holdNoteStarts.erase(it);
				break;
			}
			it++;

		}

	}
}

void SongLoader::loadNotesFromSongFile(SongData& songData)
{
	std::wstring line;
	bool foundTag = false;
	std::map<int, Note*> holdNoteStarts;
	while (std::getline(stream, line))
	{
		if (!foundTag)
		{
			if (lineHasTag(line, L"Notes"))
				foundTag = true;
		}
		else
		{
			if (!lineHasTag(line, L""))
			{
				if (line.find_first_of(L"//") == 0)
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