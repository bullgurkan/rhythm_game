#include "Song.hpp"

const sf::Vector2f zeroVector(0,0);
const float tickSize = 0.01f;

Song::Song(std::vector<Note> notes, float timeToKeepPastNotes, Skin &skin) : notes{ notes }, noteTimeInBuffer{ timeToKeepPastNotes }, skin{ skin }
{
	int amountOfcolors = 0;
	for (Note& note : notes)
	{
		if (note.color > amountOfcolors)
			amountOfcolors = note.color;
	}

	skin.prepareMiddleForSong(amountOfcolors);

	for (Note& note : notes)
	{
	}
}

Song::~Song()
{
	
}

void Song::render(long time, sf::RenderWindow &window)
{
	skin.renderMiddle(time, window,  0);
	std::vector<Note>::iterator it = notes.begin();
	while(it != notes.end())
	{
		sf::Vector2f pos = (*it).getPosition(time);
		if (pos != zeroVector)
		{
			
			//std::cout << "render" << std::endl;
			skin.renderNote(time, window, pos, (*it).getPosition(time - tickSize), 0);
			++it;
		}
		else
		{
			nonPoppedNoteBuffer.push_back(*it);
			it = notes.erase(it);
			
		}
	}
}

/**
* pops the note with color color and returns how late the note was poped(this can be negative if it's too early)
*/
float Song::popNoteWithColor(long time, int colorToPop, Note::NoteType noteType)
{
	clearOldNotesInPopBuffer(time);


	for (std::vector<Note>::iterator it = nonPoppedNoteBuffer.begin(); it != nonPoppedNoteBuffer.end(); ++it)
	{
		if (it->color == colorToPop && it->noteType == noteType)
		{
			float hitdelta = it->hitTime - time;
			nonPoppedNoteBuffer.erase(it);
			return hitdelta;
		}
	}

	for (std::vector<Note>::iterator it = notes.begin(); it != notes.end(); ++it)
	{
		
		
			if (it->color == colorToPop && it->noteType == noteType)
			{
				float hitdelta = it->hitTime - time;
				switch (noteType)
				{
				case Note::NoteType::PRESS:
					if (hitdelta < -noteTimeInBuffer)
					{
						notes.erase(it);
						return hitdelta;
					}
				case Note::NoteType::HOLD:
					if (hitdelta < -it->holdNoteLength)
					{

					}
				default:
					break;
				}
				
				
			}
		
		
	}

	return 0;
}

void Song::clearOldNotesInPopBuffer(long time)
{
	std::vector<Note>::iterator removePos;
	for (std::vector<Note>::iterator it = nonPoppedNoteBuffer.begin(); it != nonPoppedNoteBuffer.end(); ++it)
	{
		if (time - it->hitTime > noteTimeInBuffer)
			removePos = it;
	}
	nonPoppedNoteBuffer.erase(nonPoppedNoteBuffer.begin(), removePos);
}
