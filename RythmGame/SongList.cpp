#include "SongList.hpp"

SongList::SongList(sf::Vector2i posRelativeToParent, sf::Vector2i size, Panel* parent, int songBufferSize, GameManager& gm, sf::Color boxColors[2], sf::Color textColor, sf::Font& font, int spacing) : Panel(posRelativeToParent, parent, true), size{ size }, songBufferSize{ songBufferSize }, gm{ gm }
{
	selectedSongId = songBufferSize / 2;
	for (int i = 0; i < songBufferSize; i++)
	{
		subPanels.push_back(new SongDataPanel(sf::Vector2i(0, i * (size.y / songBufferSize + spacing)), sf::Vector2i(size.x, size.y / songBufferSize - spacing * songBufferSize), this, boxColors[i == songBufferSize / 2 ? 1 : 0], textColor, font));
		if (i < gm.songs.size())
		{
			gm.songLoader.loadGeneral(gm.songs[i]);
			subPanels[subPanels.size() - 1]->setSongData(&gm.songs[i]);
		}

	}
}

SongList::~SongList()
{

}

Panel* SongList::getSubPanelInDirection(Direciton dir, sf::Vector2i posRelativeToThis)
{
	if (selected)
	{
		switch (dir)
		{

		case Direciton::LEFT: break;
		case Direciton::RIGHT: break;

		case Direciton::DOWN:
			selectedSongId++;
			break;
		case Direciton::UP:
			selectedSongId--;
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
{}

bool SongList::inBounds(sf::Vector2i posRelativeToParent)
{
	return false;
}
