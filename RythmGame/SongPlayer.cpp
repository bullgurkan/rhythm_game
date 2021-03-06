#include "SongPlayer.hpp"
#include <stdlib.h>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

const sf::Vector2f zeroVector(0, 0);
const int extraTimeAtSongEnd = 10;

SongPlayer::SongPlayer(SongData* songData, int perfectTime, int hitTime, int missTime, Skin& skin) : songData{ songData }, notesDefault{ songData->notes }, perfectTime{ perfectTime }, hitTime{ hitTime }, missTime{ missTime }, skin{ skin }, inputs(), totalMissTimeInMs{ 0 }, endTime{ (notesDefault.size() > 0) ? (*(--notesDefault.end()))->hitTime + extraTimeAtSongEnd + missTime : 100000 }
{
	/*
	int amountOfColors = 0;
	for (Note* note : notes)
	{
		//std::cout << (int)note->noteType << std::endl;
		if (note->color + 1 > amountOfColors)
			amountOfColors = note->color + 1;
	}
	*/


	skin.prepareMiddleForSong(songData->amountOfKeys);

	if (songData->amountOfKeys == 1)
	{
		std::srand(songData->notes[0]->hitTime);

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

SongPlayer::~SongPlayer()
{
	//for (Note* note : notesDefault)
		//delete note;

}

bool inBounds(sf::Vector2f pos, sf::Vector2f box)
{
	return pos.x > -box.x && pos.x < box.x&& pos.y > -box.y && pos.y < box.y;
}

void SongPlayer::render(int time, sf::RenderWindow& window)
{

	std::vector<Note*>::iterator it = notes.begin();
	sf::Vector2f windowSize = static_cast<sf::Vector2f>(window.getSize())/2.0f;

	int smallestHitTime = notes[0]->hitTime;
	while (it != notes.end())
	{
		sf::Vector2f pos = (*it)->getPosition(time, skin);
		if (pos != zeroVector)
		{
			
			sf::Vector2f prevPos = (*it)->getPosition(time - 1, skin);
			if (inBounds(pos, windowSize) || ((*it)->noteType == Note::NoteType::HOLD_END && (inBounds((*it)->holdStart->getPosition(time, skin), windowSize) || time > (*it)->holdStart->hitTime)))
			{
				skin.renderNote(time, window, pos, prevPos, (*it)->color, (*it)->hitTime == smallestHitTime && nonPoppedNoteBuffer.size() == 0);
				if ((*it)->noteType == Note::NoteType::HOLD_END)
					if (time > (*it)->holdStart->hitTime)
					{
						skin.renderHoldLine(time, window, skin.getNoteMiddle(pos, prevPos), getMiddleColorPositionAtTime(time, (*it)->color), (*it)->color, true);
					}
					else
					{
						skin.renderHoldLine(time, window, skin.getNoteMiddle(pos, prevPos), skin.getNoteMiddle((*it)->holdStart->getPosition(time, skin), (*it)->holdStart->getPosition(time - 1, skin)), (*it)->color, true);
					}
			}
				
			



			++it;
		}
		else
		{
			nonPoppedNoteBuffer.push_back(*it);
			it = notes.erase(it);
		}
	}



	skin.renderMiddle(time, window, getMiddleRotation(time) == 0 ? 0 : static_cast<float>(360 - 180 / M_PI * getMiddleRotation(time)));

	//could be in popNoteWithColor for preformace
	clearOldNotesInPopBuffer(time);

}

/**
* pops the note with color color and returns how late the note was poped(this can be negative if it's too early)
*/
void SongPlayer::popNoteWithColor(InputData inputData)
{

	//could be in render for animations
	//clearOldNotesInPopBuffer(inputData.time);

	skin.setColorPressed(inputData.keyState == KeyBind::KeyState::Pressed, inputData.colorToPop);

	std::vector<Note*>::iterator it = nonPoppedNoteBuffer.begin();
	while (it != nonPoppedNoteBuffer.end())
	{
		if ((*it)->color == inputData.colorToPop)
		{
			int hitDelta = inputData.time - (*it)->hitTime;
			noteHitUpdate(hitDelta, inputData.colorToPop, (*it)->noteType, inputData.time);
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
			switch (inputData.keyState)
			{
			case KeyBind::KeyState::Pressed:

				if (((*it)->noteType == Note::NoteType::HOLD_START || (*it)->noteType == Note::NoteType::PRESS) && hitDelta > -missTime)
				{
					notes.erase(it);
					noteHitUpdate(hitDelta, inputData.colorToPop, (*it)->noteType, inputData.time);
					return;
				}
				break;
			case KeyBind::KeyState::Released:
				if ((*it)->noteType == Note::NoteType::HOLD_END && hitDelta > -(*it)->getHoldTime())
				{
					notes.erase(it);
					noteHitUpdate(hitDelta, inputData.colorToPop, (*it)->noteType, inputData.time);
					return;
				}
				break;
			default:
				break;
			}


		}


	}
}

bool SongPlayer::songHasEnded(int time)
{
	//std::cout << time << " "<< endTime << std::endl;
	return time >= endTime;
}

float SongPlayer::getAccuracy()
{
	return 1 - (float)totalMissTimeInMs / (hitTime * notesDefault.size());
}

void SongPlayer::ReloadSong()
{
	notes = std::vector<Note*>(notesDefault);
	nonPoppedNoteBuffer.clear();
	inputs.clear();
}

void SongPlayer::clearOldNotesInPopBuffer(int time)
{
	std::vector<Note*>::iterator it = nonPoppedNoteBuffer.begin();
	while (it != nonPoppedNoteBuffer.end() && time - (*it)->hitTime > missTime)
	{
		noteHitUpdate(missTime, (*it)->color, (*it)->noteType, time);
		it = nonPoppedNoteBuffer.erase(it);
	}
}

void SongPlayer::noteHitUpdate(int hitDelta, int color, Note::NoteType noteType, int time)
{
	if (hitDelta < -hitTime)
	{
		if (hitDelta < -missTime)
			return;
		skin.showHitMark(Skin::HitType::EARLY_MISS, color, time);
		totalMissTimeInMs += hitTime;
		if (noteType == Note::NoteType::HOLD_START)
			onHoldStartMiss(color);
	}
	else if (hitDelta < -perfectTime)
	{
		skin.showHitMark(Skin::HitType::EARLY_HIT, color, time);
		totalMissTimeInMs -= hitDelta;
	}
	else if (hitDelta < perfectTime)
	{
		skin.showHitMark(Skin::HitType::PERFECT, color, time);
	}
	else if (hitDelta < hitTime)
	{
		skin.showHitMark(Skin::HitType::LATE_HIT, color, time);
		totalMissTimeInMs += hitDelta;
	}
	else if (hitDelta <= missTime)
	{
		skin.showHitMark(Skin::HitType::LATE_MISS, color, time);
		totalMissTimeInMs += hitTime;
		if (noteType == Note::NoteType::HOLD_START)
			onHoldStartMiss(color);
	}
}

void SongPlayer::onHoldStartMiss(int color)
{
	
	std::vector<Note*>::iterator it = nonPoppedNoteBuffer.begin();

	while (it != nonPoppedNoteBuffer.end())
	{

		if ((*it)->color == color && (*it)->noteType == Note::NoteType::HOLD_END)
		{
			nonPoppedNoteBuffer.erase(it);
			totalMissTimeInMs += hitTime;
			return;
		}
		it++;

	}

	it = notes.begin();
	while (it != notes.end())
	{

		if ((*it)->color == color && (*it)->noteType == Note::NoteType::HOLD_END)
		{
			notes.erase(it);
			totalMissTimeInMs += hitTime;
			return;
		}
		it++;

	}
}

float SongPlayer::getMiddleRotation(int time)
{
	return static_cast<float>(static_cast<double>(time % 5000) / 5000 * 2 * M_PI);
}

sf::Vector2f SongPlayer::getMiddleColorPositionAtTime(int time, int color)
{
	float rotation = getMiddleRotation(time);
	return sf::Vector2f(static_cast<float>(std::sin(rotation + skin.angleBetweenColors * (static_cast<double>(color) + 0.5f))), static_cast<float>(std::cos(rotation + skin.angleBetweenColors * (static_cast<double>(color) + 0.5f)))) * skin.getMiddleRadius();
}
