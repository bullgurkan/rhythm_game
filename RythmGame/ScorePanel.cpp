#include "ScorePanel.hpp"
ScorePanel::ScorePanel(sf::Vector2i posRelativeToParent, sf::Vector2i size, Panel* parent, sf::Color boxColor, sf::Color textColor, sf::Font& font, GameManager& gm) : Panel(posRelativeToParent, parent, false), gm{ gm }
{
	shape.setSize(static_cast<sf::Vector2f>(size));
	shape.setFillColor(boxColor);
	this->scoreDisplay.setFont(font);
	this->scoreDisplay.setFillColor(textColor);
}

ScorePanel::~ScorePanel()
{}

void ScorePanel::render(int time, sf::RenderWindow & window, Skin & skin)
{
	sf::Vector2f actualPos;
	if (parent != nullptr)
		actualPos = static_cast<sf::Vector2f>(parent->pos + pos);
	else
		actualPos = static_cast<sf::Vector2f>(pos);
	shape.setPosition(actualPos);


	scoreDisplay.setPosition(actualPos + (sf::Vector2f(shape.getGlobalBounds().width, shape.getGlobalBounds().height) - sf::Vector2f(scoreDisplay.getGlobalBounds().width, scoreDisplay.getGlobalBounds().height * 2)) / static_cast<float>(2));

	window.draw(shape);
	window.draw(scoreDisplay);
}

void ScorePanel::onClick()
{}

void ScorePanel::onSceneLoad()
{
	int acc = gm.getSongAccuracy();
	std::string grade;
	if (acc == 100)
		grade = "SS";
	else if (acc >= 95)
		grade = "S";
	else if (acc >= 90)
		grade = "A";
	else if (acc >= 80)
		grade = "B";
	else if (acc >= 70)
		grade = "C";
	else if (acc > 0)
		grade = "D";
	else
		grade = "F";


	this->scoreDisplay.setString(grade + " " + std::to_string(acc) + "%");
}

bool ScorePanel::inBounds(sf::Vector2i posRelativeToParent)
{
	return false;
}
