#pragma once
#include "Panel.hpp"
#include "SongLoader.hpp"
#include "SongDataPanel.hpp"
#include "GameManager.hpp"

class SongList : public Panel
{
public:
	SongList(sf::Vector2i posRelativeToParent, sf::Vector2i size, Panel* parent, int songBufferSize, GameManager& gm, sf::Color boxColors[2], sf::Color textColor, sf::Font& font, int spacing);
	~SongList();
	Panel* getPanelInDirection(Direciton dir) override;
	Panel* getHoveredPanel(sf::Vector2i posRelativeToParent) override;

	void render(int time, sf::RenderWindow& window, Skin& skin) override;
	void onClick() override;
	void onSceneLoad() override;

private:
	bool inBounds(sf::Vector2i posRelativeToParent) override;
	std::vector<SongDataPanel*> subPanels;
	sf::Vector2i size;
	//std::vector<SongData*> songsInMenu;
	int selectedSongId;
	int songBufferSize;
	GameManager& gm;
};