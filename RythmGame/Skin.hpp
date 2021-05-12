#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

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
	Skin(float middleRadius, std::vector<sf::Color> colors, std::vector<sf::Color> hitMarkerColors, sf::Vector2f screenSize, sf::Color notPressedColorMult);
	~Skin();
	void renderNote(int time, sf::RenderWindow &window, sf::Vector2f position, sf::Vector2f prevPos, int colorId, bool isNextNote);
	void prepareMiddleForSong(int amountOfColors);
	void renderMiddle(int time, sf::RenderWindow &window, float rotation);
	void renderHoldLine(int time, sf::RenderWindow& window, sf::Vector2f pos1, sf::Vector2f pos2, int colorId, bool heldOrNextNote);
	void showHitMark(HitType hitType);
	void setColorPressed(bool pressed, int colorId);
	float getMiddleRadius();
	sf::Vector2f getNoteMiddle(sf::Vector2f pos, sf::Vector2f prevPos);
	double angleBetweenColors;
private:
	std::vector<sf::Color> colors;
	std::vector<sf::Color> hitMarkerColors;
	float middleRadius;
	sf::Shape* noteShape;
	std::vector <sf::Shape*> middleShapes;
	sf::RectangleShape* gridShape;
	sf::Shape* middleHitMarkerShape;
	sf::Vector2f screenCenter;
	sf::Color notPressedColorMult;
};