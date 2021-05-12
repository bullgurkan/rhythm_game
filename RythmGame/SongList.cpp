#include "SongList.hpp"
#include <iostream>

SongList::SongList(sf::Vector2i posRelativeToParent, sf::Vector2i size, Panel* parent, int songBufferSize, GameManager& gm, sf::Color boxColors[2], sf::Color textColor, sf::Font& font, int spacing) : Panel(posRelativeToParent, parent, true), size{ size }, songBufferSize{ songBufferSize }, gm{ gm }, selectedSongId{0}
{
	int songIndex = 0;
	for (int i = 0; i < songBufferSize; i++)
	{
		subPanels.push_back(new SongDataPanel(sf::Vector2i(0, i * (size.y / songBufferSize + spacing)), sf::Vector2i(size.x, size.y / songBufferSize - spacing * songBufferSize), this, boxColors[i == songBufferSize / 2 ? 1 : 0], textColor, font));
		gm.songLoader.loadImage(gm.songs[songIndex]);
		subPanels[subPanels.size() - 1]->setSongData(&gm.songs[songIndex]);

		songIndex++;
		if (songIndex >= gm.songs.size())
			songIndex = 0;


	}
	
	gm.songLoader.loadMusic(*subPanels[songBufferSize / 2]->getSongData());
	sf::Sound* currentMusic = subPanels[songBufferSize / 2]->getSongData()->music;
	currentMusic->setPitch(gm.playbackSpeed);
	currentMusic->setPlayingOffset(sf::milliseconds(static_cast<int>(subPanels[songBufferSize / 2]->getSongData()->previewTime * gm.playbackSpeed)));
	currentMusic->play();
	
}

SongList::~SongList()
{

}

Panel* SongList::getPanelInDirection(Direciton dir)
{
	if (selected)
	{
		SongData* songDataTemp;
		int songIndex;
		sf::Sound* currentMusic;
		switch (dir)
		{

		case Direciton::LEFT: break;
		case Direciton::RIGHT: break;

		case Direciton::DOWN:
			selectedSongId++;
			if (selectedSongId >= gm.songs.size())
				selectedSongId = 0;
			songIndex = selectedSongId + static_cast<int>(subPanels.size()) - 1;
			while (songIndex >= gm.songs.size())
				songIndex -= static_cast<int>(gm.songs.size());

			subPanels[songBufferSize / 2]->getSongData()->music->stop();

			for (int i = 0; i < subPanels.size(); i++)
			{
				songDataTemp = subPanels[i]->getSongData();
				if (static_cast<unsigned long long>(i) + 1 < subPanels.size())
					subPanels[i]->setSongData(subPanels[static_cast<int64_t>(i) + 1]->getSongData());
				else
				{
					gm.songLoader.loadImage(gm.songs[songIndex]);
					subPanels[i]->setSongData(&gm.songs[songIndex]);
				}
					
			}
			gm.songLoader.loadMusic(*subPanels[songBufferSize / 2]->getSongData());
			currentMusic = subPanels[songBufferSize / 2]->getSongData()->music;
			currentMusic->setPitch(gm.playbackSpeed);
			currentMusic->setPlayingOffset(sf::milliseconds(static_cast<int>(subPanels[songBufferSize / 2]->getSongData()->previewTime * gm.playbackSpeed)));
			currentMusic->play();

			break;
		case Direciton::UP:
			selectedSongId--;
			if (selectedSongId < 0)
				selectedSongId = static_cast<int>(gm.songs.size()) - 1;

			songIndex = selectedSongId;
			while (songIndex >= gm.songs.size())
				songIndex -= static_cast<int>(gm.songs.size());

			subPanels[songBufferSize / 2]->getSongData()->music->stop();

			for (int i = static_cast<int>(subPanels.size()) - 1; i >= 0; i--)
			{
				songDataTemp = subPanels[i]->getSongData();
				if (static_cast<int64_t>(i) - 1 >= 0)
					subPanels[i]->setSongData(subPanels[static_cast<int64_t>(i) - 1]->getSongData());
				else
				{
					gm.songLoader.loadImage(gm.songs[songIndex]);
					subPanels[i]->setSongData(&gm.songs[songIndex]);
				}
			}
			gm.songLoader.loadMusic(*subPanels[songBufferSize / 2]->getSongData());
			currentMusic = subPanels[songBufferSize / 2]->getSongData()->music;
			currentMusic->setPitch(gm.playbackSpeed);
			currentMusic->setPlayingOffset(sf::milliseconds(static_cast<int>(subPanels[songBufferSize / 2]->getSongData()->previewTime * gm.playbackSpeed)));
			currentMusic->play();

			break;

		}
	}



	return this;
}

Panel* SongList::getHoveredPanel(sf::Vector2i posRelativeToParent)
{

	return nullptr;
}

void SongList::render(int time, sf::RenderWindow& window, Skin& skin)
{
	for (Panel* child : subPanels)
		child->render(time, window, skin);

	sf::Sound* currentMusic = subPanels[songBufferSize / 2]->getSongData()->music;

	if (currentMusic != nullptr && currentMusic->getStatus() == sf::SoundSource::Status::Stopped)
	{
		currentMusic->setPitch(gm.playbackSpeed);
		currentMusic->setPlayingOffset(sf::milliseconds(static_cast<int>(subPanels[songBufferSize / 2]->getSongData()->previewTime * gm.playbackSpeed)));
		currentMusic->play();
	}

}

void SongList::onClick()
{
	gm.LoadSong(subPanels[subPanels.size() / 2]->getSongData());
}

void SongList::onSceneLoad()
{}

bool SongList::inBounds(sf::Vector2i posRelativeToParent)
{
	return false;
}
