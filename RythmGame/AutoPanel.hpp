#pragma once
#include "Panel.hpp"

class AutoPanel : public Panel
{
public:
	AutoPanel(sf::Vector2i posRelativeToParent, sf::Vector2i size, Panel* parent);
	Panel* getSubPanelInDirection(Direciton dir, sf::Vector2i posRelativeToThis) override;
	Panel* getHoveredPanel(sf::Vector2i posRelativeToParent) override;
	Panel* getDefaultSelectedPanel() override;

	void render(int time, sf::RenderWindow& window, Skin& skin) override;
	void onClick() override;
	void addSubPanel(Panel* subPanel);

private:
	virtual bool inBounds(sf::Vector2i posRelativeToParent) override;
	std::vector<Panel*> subPanels;
	sf::Vector2i size;
};