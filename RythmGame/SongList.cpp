#include "SongList.hpp"
#include <iostream>

SongList::SongList(sf::Vector2i posRelativeToParent, sf::Vector2i size, Panel* parent, int songBufferSize, GameManager& gm, sf::Color boxColors[2], sf::Color textColor, sf::Font& font, int spacing) : Panel(posRelativeToParent, parent, true), size{ size }, songBufferSize{ songBufferSize }, gm{ gm }
{
	//selectedSongId = songBufferSize / 2;
	int songIndex = 0;
	for (int i = 0; i < songBufferSize; i++)
	{
		subPanels.push_back(new SongDataPanel(sf::Vector2i(0, i * (size.y / songBufferSize + spacing)), sf::Vector2i(size.x, size.y / songBufferSize - spacing * songBufferSize), this, boxColors[i == songBufferSize / 2 ? 1 : 0], textColor, font));

		songIndex++;
		if (songIndex >= gm.songs.size())
			songIndex = 0;

		gm.songLoader.loadGeneral(gm.songs[songIndex]);
		subPanels[subPanels.size() - 1]->setSongData(&gm.songs[songIndex]);
	}
	
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
		switch (dir)
		{

		case Direciton::LEFT: break;
		case Direciton::RIGHT: break;

		case Direciton::DOWN:
			selectedSongId++;
			if (selectedSongId >= gm.songs.size())
				selectedSongId = 0;
			songIndex = selectedSongId + subPanels.size() - 1;
			while (songIndex >= gm.songs.size())
				songIndex -= gm.songs.size();

			for (int i = 0; i < subPanels.size(); i++)
			{
				songDataTemp = subPanels[i]->getSongData();
				if (static_cast<int64_t>(i) + 1 < subPanels.size())
					subPanels[i]->setSongData(subPanels[static_cast<int64_t>(i) + 1]->getSongData());
				else
					subPanels[i]->setSongData(&gm.songs[songIndex]);
			}

			break;
		case Direciton::UP:
			selectedSongId--;
			if (selectedSongId < 0)
				selectedSongId = gm.songs.size() - 1;

			songIndex = selectedSongId;
			while (songIndex >= gm.songs.size())
				songIndex -= gm.songs.size();

			for (int i = subPanels.size() - 1; i >= 0; i--)
			{
				songDataTemp = subPanels[i]->getSongData();
				if (static_cast<int64_t>(i) - 1 >= 0)
					subPanels[i]->setSongData(subPanels[static_cast<int64_t>(i) - 1]->getSongData());
				else
					subPanels[i]->setSongData(&gm.songs[songIndex]);
			}
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

}

void SongList::onClick()
{
	gm.LoadSong(subPanels[subPanels.size() / 2]->getSongData());
}

bool SongList::inBounds(sf::Vector2i posRelativeToParent)
{
	return false;
}
