#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shape.hpp>

class Skin
{
public:
	enum class HitType
	{
		EARLY_MISS,
		EARLY_HIT,
		PERFECT,
		LATE_HIT,
		LATE_MISS
	};
	Skin(float middleRadius, std::vector<sf::Color> colors, sf::Vector2f screenSize);
	~Skin();
	void renderNote(long time, sf::RenderWindow &window, sf::Vector2f position, sf::Vector2f prevPos, int colorId);
	void prepareMiddleForSong(int amountOfColors);
	void renderMiddle(long time, sf::RenderWindow &window, float rotation);
	void showHitMark(HitType hitType);
private:
	std::vector<sf::Color> colors;
	float middleRadius;
	sf::Shape* noteShape;
	std::vector <sf::Shape*> middleShapes;
	sf::Vector2f screenCenter;
};