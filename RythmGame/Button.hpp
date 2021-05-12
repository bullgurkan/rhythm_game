#pragma once
#include "Panel.hpp"
#include <functional>

class Button : public Panel
{
public:
	Button(sf::Vector2i posRelativeToParent, sf::Vector2i size, Panel* parent, sf::Color colors[2], std::string text, sf::Font &font, std::function<void()> onClickFunc = nullptr);
	Panel* getSubPanelInDirection(Direciton dir, sf::Vector2i posRelativeToThis) override;

	void render(int time, sf::RenderWindow& window, Skin& skin) override;
	void onClick() override;
	void onSceneLoad() override;
	std::function<void()> onClickFunc;
private:
	virtual bool inBounds(sf::Vector2i posRelativeToParent) override;
	std::vector<Panel*> subPanels;
	sf::Vector2i size;
	sf::Color colors[2];
	sf::RectangleShape shape;
	sf::Text text;
};