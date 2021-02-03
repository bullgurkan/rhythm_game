#pragma once
#include "Note.hpp"
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics.hpp>
#include "Skin.hpp"
#include "InputManager.hpp"

class Song
{
public:
	Song(std::vector<Note> notes, float timeToKeepPastNotes, Skin &skin);
	~Song();
	void render(long time, sf::RenderWindow &window);
	float popNoteWithColor(long time, int colorToPop, Note::NoteType noteType);
	

private:
	void clearOldNotesInPopBuffer(long time);
	std::vector<Note> notes;
	std::vector<Note> nonPoppedNoteBuffer;
	float noteTimeInBuffer;
	Skin &skin;

};
