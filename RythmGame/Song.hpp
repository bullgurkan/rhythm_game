#pragma once
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Audio.hpp>
#include "Note.hpp"
#include "Skin.hpp"
#include "InputData.hpp"

class Song
{
public:
	Song(std::vector<Note*> notes, int perfectTime, int hitTime, int missTime, Skin &skin);
	~Song();
	void render(int time, sf::RenderWindow &window);
	void popNoteWithColor(InputData inputData);
	bool songHasEnded(int time);
	float getAccuracy();

private:
	void clearOldNotesInPopBuffer(int time);
	std::vector<Note*> notesDefault;
	std::vector<Note*> notes;
	std::vector<Note*> nonPoppedNoteBuffer;
	std::vector<InputData> inputs;

	int perfectTime;
	int hitTime;
	int missTime;
	int totalMissTimeInMs;
	int endTime;
	Skin &skin;
	sf::SoundBuffer songBuffer;
	void noteHitUpdate(int hitTime, int color, Note::NoteType noteType);
	void onHoldStartMiss(int color);
};
