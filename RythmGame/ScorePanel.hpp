#pragma once
#include "Panel.hpp"
#include "GameManager.hpp"


class ScorePanel : public Panel
{
public:
	ScorePanel(sf::Vector2i posRelativeToParent, sf::Vector2i size, Panel* parent, sf::Color boxColor, sf::Color textColor, sf::Font& font, GameManager &gm);
	~ScorePanel();
	void render(int time, sf::RenderWindow& window, Skin& skin) override;
	void onClick() override;
	void onSceneLoad() override;

private:
	bool inBounds(sf::Vector2i posRelativeToParent) override;
	sf::Text scoreDisplay;
	sf::RectangleShape shape;
	GameManager &gm;
	sf::Vector2i size;


};