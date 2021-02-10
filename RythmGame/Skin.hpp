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
	Skin(float middleRadius, std::vector<sf::Color> colors, std::vector<sf::Color> hitMarkerColors, sf::Vector2f screenSize);
	~Skin();
	void renderNote(int time, sf::RenderWindow &window, sf::Vector2f position, sf::Vector2f prevPos, int colorId);
	void prepareMiddleForSong(int amountOfColors);
	void renderMiddle(int time, sf::RenderWindow &window, float rotation);
	void showHitMark(HitType hitType);
	float getMiddleRadius();
private:
	std::vector<sf::Color> colors;
	std::vector<sf::Color> hitMarkerColors;
	float middleRadius;
	sf::Shape* noteShape;
	std::vector <sf::Shape*> middleShapes;
	sf::Shape* middleHitMarkerShape;
	sf::Vector2f screenCenter;
};