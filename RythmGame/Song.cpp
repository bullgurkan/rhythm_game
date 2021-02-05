#include "Song.hpp"
#include <iostream>

const sf::Vector2f zeroVector(0, 0);

Song::Song(std::vector<Note> notes, int perfectTime, int hitTime, int missTime, Skin& skin) : notes{ notes }, perfectTime{ perfectTime }, hitTime{ hitTime }, missTime{ missTime }, skin{ skin }, inputs(), totalMissTimeInMs{ 0 }
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

void Song::render(int time, sf::RenderWindow& window)
{
	skin.renderMiddle(time, window, 0);
	std::vector<Note>::iterator it = notes.begin();
	while (it != notes.end())
	{
		sf::Vector2f pos = (*it).getPosition(time);
		if (pos != zeroVector)
		{

			//std::cout << "render" << std::endl;
			skin.renderNote(time, window, pos, (*it).getPosition(time - 1), 0);
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


	std::cout << "pop note" << std::endl;
	std::cout << nonPoppedNoteBuffer.begin()._Ptr << std::endl;
	for (std::vector<Note>::iterator it = nonPoppedNoteBuffer.begin(); it != nonPoppedNoteBuffer.end(); ++it)
	{

		if (it->color == inputData.colorToPop)
		{
			std::cout << it._Ptr << " 1 " << nonPoppedNoteBuffer.end()._Ptr << std::endl;
			int hitDelta = inputData.time - it->hitTime;
			noteHitUpdate(hitDelta, inputData.colorToPop, it->noteType);
			it = nonPoppedNoteBuffer.erase(it);
			std::cout << it._Ptr << " " << nonPoppedNoteBuffer.end()._Ptr << std::endl;
		}
	}

	for (std::vector<Note>::iterator it = notes.begin(); it != notes.end(); ++it)
	{
		std::cout << inputData.time - it->hitTime << std::endl;

		if (it->color == inputData.colorToPop)
		{
			int hitDelta = inputData.time - it->hitTime;
			switch (inputData.inputType)
			{
			case InputManager::InputType::PRESSED:
				if ((it->noteType == Note::NoteType::HOLD_START || it->noteType == Note::NoteType::PRESS) && hitDelta > -missTime)
				{
					notes.erase(it);
					noteHitUpdate(hitDelta, inputData.colorToPop, it->noteType);
					return;
				}
			case InputManager::InputType::RELEASED:
				if (it->noteType == Note::NoteType::HOLD_END && hitDelta < -it->holdNoteLength)
				{
					notes.erase(it);
					noteHitUpdate(hitDelta, inputData.colorToPop, it->noteType);
					return;
				}
			default:
				break;
			}


		}


	}
}

void Song::clearOldNotesInPopBuffer(int time)
{
	for (std::vector<Note>::iterator it = nonPoppedNoteBuffer.begin(); it != nonPoppedNoteBuffer.end(); ++it)
	{
		if (time - it->hitTime > missTime)
		{
			noteHitUpdate(missTime, it->color, it->noteType);
			it = nonPoppedNoteBuffer.erase(it);
		}
		else
			break;

	}
}

void Song::noteHitUpdate(int hitDelta, int color, Note::NoteType noteType)
{
	if (hitDelta < -hitTime)
	{
		if (hitDelta < -missTime)
			return;
		skin.showHitMark(Skin::HitType::EARLY_MISS);
		totalMissTimeInMs += missTime;
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
		std::cout << "perfect?" << std::endl;
		skin.showHitMark(Skin::HitType::PERFECT);
	}
	else if (hitDelta < hitTime)
	{
		skin.showHitMark(Skin::HitType::LATE_HIT);
		totalMissTimeInMs += hitDelta;
	}
	else if(hitDelta <= missTime)
	{
		skin.showHitMark(Skin::HitType::LATE_MISS);
		totalMissTimeInMs += missTime;
		if (noteType == Note::NoteType::HOLD_START)
			onHoldStartMiss(color);
	}
}

void Song::onHoldStartMiss(int color)
{

}
