#pragma once
#include "Panel.hpp"
#include <functional>

class AutoPanel : public Panel
{
public:
	AutoPanel(sf::Vector2i posRelativeToParent, sf::Vector2i size, Panel* parent, bool loopSelectorAtEdge);
	AutoPanel(sf::Vector2i posRelativeToParent, sf::Vector2i size, Panel* parent, bool loopSelectorAtEdge, std::function<void()> onBackFunction);
	~AutoPanel();
	Panel* getSubPanelInDirection(Direciton dir, sf::Vector2i posRelativeToThis) override;
	Panel* getHoveredPanel(sf::Vector2i posRelativeToParent) override;
	Panel* getDefaultSelectedPanel() override;

	void render(int time, sf::RenderWindow& window, Skin& skin) override;
	void onClick() override;
	void onSceneLoad() override;
	void addSubPanel(Panel* subPanel);
	void onBack() override;

private:
	bool inBounds(sf::Vector2i posRelativeToParent) override;
	Panel* getSubPanelInDirectionNonRecursive(Direciton dir, sf::Vector2i posRelativeToThis, bool useClosest);
	std::vector<Panel*> subPanels;
	sf::Vector2i size;
	bool loopSelectorAtEdge;
	std::function<void()> onBackFunction;
};