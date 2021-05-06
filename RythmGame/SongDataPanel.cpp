#include "SongDataPanel.hpp"
#include <iostream>

SongDataPanel::SongDataPanel(sf::Vector2i posRelativeToParent, sf::Vector2i size, Panel* parent, sf::Color boxColor, sf::Color textColor, sf::Font& font) : Panel(posRelativeToParent, parent, true), size{ size }
{
	interactable = false;
	imageDisplay = sf::RectangleShape(sf::Vector2f(static_cast<float>(size.x) / 4, static_cast<float>(size.y)));
	selectedBox = sf::RectangleShape(static_cast<sf::Vector2f>(size) - sf::Vector2f(static_cast<float>(size.x)/4,0));
	selectedBox.setFillColor(boxColor);
	nameDisplay.setFillColor(textColor);
	nameDisplay.setFont(font);
	nameDisplay.setScale(sf::Vector2f(0.8f, 0.8f));
}

SongDataPanel::~SongDataPanel()
{

}


void SongDataPanel::render(int time, sf::RenderWindow& window, Skin& skin)
{
	sf::Vector2f actualPos;
	if (parent != nullptr)
		actualPos = static_cast<sf::Vector2f>(parent->pos + pos);
	else
		actualPos = static_cast<sf::Vector2f>(pos);

	selectedBox.setPosition(actualPos + sf::Vector2f(static_cast<float>(size.x) / 4, 0));
	imageDisplay.setPosition(actualPos);
	nameDisplay.setPosition(actualPos + sf::Vector2f(static_cast<float>(size.x) / 4, 0) +(sf::Vector2f(selectedBox.getGlobalBounds().width, selectedBox.getGlobalBounds().height) - sf::Vector2f(nameDisplay.getGlobalBounds().width, nameDisplay.getGlobalBounds().height * 2)) / static_cast<float>(2));

	window.draw(selectedBox);
	window.draw(imageDisplay);
	window.draw(nameDisplay);
}

void SongDataPanel::setSongData(SongData* songData)
{
	this->songData = songData;
	imageDisplay.setTexture(songData->image);

	auto metadataSearch = songData->metadata.find(L"Title");
	

	if (metadataSearch != songData->metadata.end())
		nameDisplay.setString(metadataSearch->second);
		

}

SongData* SongDataPanel::getSongData()
{
	return songData;
}

void SongDataPanel::unloadSongData()
{
	songData->~SongData();
}

void SongDataPanel::onClick()
{}

bool SongDataPanel::inBounds(sf::Vector2i posRelativeToParent)
{
	return false;
}



