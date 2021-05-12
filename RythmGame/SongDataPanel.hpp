#pragma once
#include "Panel.hpp"
#include "SongLoader.hpp"

class SongDataPanel : public Panel
{
public:
	SongDataPanel(sf::Vector2i posRelativeToParent, sf::Vector2i size, Panel* parent, sf::Color boxColor, sf::Color textColor, sf::Font& font);
	~SongDataPanel();
	void render(int time, sf::RenderWindow& window, Skin& skin) override;
	void setSongData(SongData* songData);
	SongData* getSongData();
	void unloadSongData();
	void onClick() override;
	void onSceneLoad() override;

private:
	bool inBounds(sf::Vector2i posRelativeToParent) override;
	sf::RectangleShape selectedBox;
	sf::RectangleShape imageDisplay;
	sf::Text nameDisplay;

	SongData* songData;
	sf::Vector2i size;
	
	
};