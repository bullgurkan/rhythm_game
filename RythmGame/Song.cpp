#include "Song.hpp"
#include <stdlib.h>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

const sf::Vector2f zeroVector(0, 0);
const int extraTimeAtSongEnd = 10;

Song::Song(std::vector<Note*> notesIn, int perfectTime, int hitTime, int missTime, Skin& skin) : notes{ notesIn }, notesDefault{ notesIn }, perfectTime{ perfectTime }, hitTime{ hitTime }, missTime{ missTime }, skin{ skin }, inputs(), totalMissTimeInMs{ 0 }, endTime{ (notes.size() > 0) ? (*(--notes.end()))->hitTime + extraTimeAtSongEnd + missTime : 100000 }
{
	int amountOfColors = 0;
	for (Note* note : notes)
	{
		if (note->color + 1 > amountOfColors)
			amountOfColors = note->color + 1;
	}



	skin.prepareMiddleForSong(amountOfColors);

	if (amountOfColors == 1)
	{
		std::srand(notesDefault[0]->hitTime);

		for (Note* note : notesDefault)
		{
			float angle = (float)((double)(std::rand() % 10000) * 2 * M_PI / 10000);
			note->setEndPos(sf::Vector2f(std::sin(angle), std::cos(angle)) * skin.getMiddleRadius());
		}
	}
	else
	{
		for (Note* note : notesDefault)
		{
			note->setEndPos(getMiddleColorPositionAtTime(note->hitTime, note->color));
		}
	}

	notes = notesDefault;
}

Song::~Song()
{

	for (Note* note : notesDefault)
		delete note;

}

void Song::render(int time, sf::RenderWindow& window)
{
	skin.renderMiddle(time, window, 0);
	std::vector<Note*>::iterator it = notes.begin();
	while (it != notes.end())
	{
		sf::Vector2f pos = (*it)->getPosition(time, skin);
		if (pos != zeroVector)
		{
			skin.renderNote(time, window, pos, (*it)->getPosition(time - 1, skin), (*it)->color);
			if ((*it)->noteType == Note::NoteType::HOLD_END)
				if (time > (*it)->holdStart->hitTime)
				{
					skin.renderHoldLine(time, window, skin.getNoteMiddle(pos, (*it)->getPosition(time - 1, skin)), getMiddleColorPositionAtTime(time, (*it)->color), (*it)->color);
				}
				else if (time > (*it)->holdStart->hitTime - 1000)
				{
					skin.renderHoldLine(time, window,
						skin.getNoteMiddle(pos, (*it)->getPosition(time - 1, skin)),
						skin.getNoteMiddle((*it)->holdStart->getPosition(time, skin), (*it)->holdStart->getPosition(time - 1, skin)),
						(*it)->color);
				}

					

			++it;
		}
		else
		{
			nonPoppedNoteBuffer.push_back(*it);
			it = notes.erase(it);
		}
	}

	//could be in popNoteWithColor for preformace
	clearOldNotesInPopBuffer(time);

}

/**
* pops the note with color color and returns how late the note was poped(this can be negative if it's too early)
*/
void Song::popNoteWithColor(InputData inputData)
{


	//could be in render for animations
	//clearOldNotesInPopBuffer(inputData.time);


	std::vector<Note*>::iterator it = nonPoppedNoteBuffer.begin();
	while (it != nonPoppedNoteBuffer.end())
	{
		if ((*it)->color == inputData.colorToPop)
		{
			int hitDelta = inputData.time - (*it)->hitTime;
			noteHitUpdate(hitDelta, inputData.colorToPop, (*it)->noteType);
			nonPoppedNoteBuffer.erase(it);
			return;
		}
		else
		{
			it++;
		}
	}



	for (it = notes.begin(); it != notes.end(); ++it)
	{
		if ((*it)->color == inputData.colorToPop)
		{
			int hitDelta = inputData.time - (*it)->hitTime;
			switch (inputData.inputType)
			{
			case InputManager::InputType::PRESSED:

				if (((*it)->noteType == Note::NoteType::HOLD_START || (*it)->noteType == Note::NoteType::PRESS) && hitDelta > -missTime)
				{
					notes.erase(it);
					noteHitUpdate(hitDelta, inputData.colorToPop, (*it)->noteType);
					return;
				}
			case InputManager::InputType::RELEASED:
				if ((*it)->noteType == Note::NoteType::HOLD_END && hitDelta > -(*it)->getHoldTime())
				{
					notes.erase(it);
					noteHitUpdate(hitDelta, inputData.colorToPop, (*it)->noteType);
					return;
				}
			default:
				break;
			}


		}


	}
}

bool Song::songHasEnded(int time)
{
	return time >= endTime;
}

float Song::getAccuracy()
{
	return 1 - (float)totalMissTimeInMs / (hitTime * notesDefault.size());
}

void Song::clearOldNotesInPopBuffer(int time)
{
	std::vector<Note*>::iterator it = nonPoppedNoteBuffer.begin();
	while (it != nonPoppedNoteBuffer.end() && time - (*it)->hitTime > missTime)
	{
		noteHitUpdate(missTime, (*it)->color, (*it)->noteType);
		it = nonPoppedNoteBuffer.erase(it);
	}
}

void Song::noteHitUpdate(int hitDelta, int color, Note::NoteType noteType)
{
	if (hitDelta < -hitTime)
	{
		if (hitDelta < -missTime)
			return;
		//skin.showHitMark(Skin::HitType::EARLY_MISS);
		totalMissTimeInMs += hitTime;
		if (noteType == Note::NoteType::HOLD_START)
			onHoldStartMiss(color);
	}
	else if (hitDelta < -perfectTime)
	{
		skin.showHitMark(Skin::HitType::EARLY_HIT);
		totalMissTimeInMs -= hitDelta;
	}
	else if (hitDelta < perfectTime)
	{
		skin.showHitMark(Skin::HitType::PERFECT);
	}
	else if (hitDelta < hitTime)
	{
		skin.showHitMark(Skin::HitType::LATE_HIT);
		totalMissTimeInMs += hitDelta;
	}
	else if (hitDelta <= missTime)
	{
		//skin.showHitMark(Skin::HitType::LATE_MISS);
		totalMissTimeInMs += hitTime;
		if (noteType == Note::NoteType::HOLD_START)
			onHoldStartMiss(color);
	}
}

void Song::onHoldStartMiss(int color)
{
	std::vector<Note*>::iterator it = notes.begin();
	while (it != notes.end())
	{
		std::cout << (*it)->color << std::endl;
		if ((*it)->color == color && (*it)->noteType == Note::NoteType::HOLD_END)
		{
			notes.erase(it);
			totalMissTimeInMs += hitTime;
			break;
		}
		it++;

	}
}

float Song::getMiddleRotation(int time)
{
	return 0.0f;
}

sf::Vector2f Song::getMiddleColorPositionAtTime(int time, int color)
{
	float rotation = getMiddleRotation(time);
	return sf::Vector2f(static_cast<float>(std::sin(rotation + skin.degreeDistance * (static_cast<double>(color) + 0.5f))), static_cast<float>(std::cos(rotation + skin.degreeDistance * (static_cast<double>(color) + 0.5f)))) * skin.getMiddleRadius();
}
