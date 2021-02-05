#pragma once
#include "Note.hpp"
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics.hpp>
#include "Skin.hpp"
#include "InputManager.hpp"
#include "InputData.hpp"

class Song
{
public:
	Song(std::vector<Note> notes, float perfectTime, float hitTime, float missTime, Skin &skin);
	~Song();
	void render(long time, sf::RenderWindow &window);
	void popNoteWithColor(InputData inputData);
	

private:
	void clearOldNotesInPopBuffer(long time);
	std::vector<Note> notesDefault;
	std::vector<Note> notes;
	std::vector<Note> nonPoppedNoteBuffer;
	std::vector<InputData> inputs;

	float perfectTime;
	float hitTime;
	float missTime;
	int totalMissTimeInMs;
	Skin &skin;
	void noteHitUpdate(int hitTime, int color, Note::NoteType noteType);
	void onHoldStartMiss(int color);
};
