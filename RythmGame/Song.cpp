#include "Song.hpp"

const sf::Vector2f zeroVector(0, 0);
const float tickSize = 0.01f;

Song::Song(std::vector<Note> notes, float perfectTime, float hitTime, float missTime, Skin& skin) : notes{ notes }, perfectTime{ perfectTime }, hitTime{ hitTime }, missTime{ missTime }, skin{ skin }, inputs()
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

void Song::render(long time, sf::RenderWindow& window)
{
	skin.renderMiddle(time, window, 0);
	std::vector<Note>::iterator it = notes.begin();
	while (it != notes.end())
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



	for (std::vector<Note>::iterator it = nonPoppedNoteBuffer.begin(); it != nonPoppedNoteBuffer.end(); ++it)
	{
		if (it->color == inputData.colorToPop)
		{

			float hitDelta = inputData.time - it->hitTime;
			nonPoppedNoteBuffer.erase(it);
			noteHitUpdate(hitDelta, inputData.colorToPop, it->noteType);

		}
	}

	for (std::vector<Note>::iterator it = notes.begin(); it != notes.end(); ++it)
	{


		if (it->color == inputData.colorToPop)
		{
			float hitDelta = inputData.time - it->hitTime;
			switch (inputData.inputType)
			{
			case InputManager::InputType::PRESSED:
				if ((it->noteType == Note::NoteType::HOLD_START || it->noteType == Note::NoteType::PRESS) && hitDelta < -missTime)
				{
					notes.erase(it);

					noteHitUpdate(hitDelta, inputData.colorToPop, it->noteType);
				}
			case InputManager::InputType::RELEASED:
				if (it->noteType == Note::NoteType::HOLD_END && hitDelta < -it->holdNoteLength)
				{
					notes.erase(it);
					noteHitUpdate(hitDelta, inputData.colorToPop, it->noteType);


				}
			default:
				break;
			}


		}


	}
}

void Song::clearOldNotesInPopBuffer(long time)
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
	if (hitDelta < perfectTime)
	{
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
