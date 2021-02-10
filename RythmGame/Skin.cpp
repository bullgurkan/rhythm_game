#include "Skin.hpp"
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

Skin::Skin(float middleRadius, std::vector<sf::Color> colors, std::vector<sf::Color> hitMarkerColors, sf::Vector2f screenSize) : middleRadius{ middleRadius }, noteShape{ nullptr }, middleShapes(), middleHitMarkerShape{ nullptr }, colors{ colors }, hitMarkerColors{ hitMarkerColors }, screenCenter(screenSize.x / 2, screenSize.y / 2)
{
	sf::ConvexShape* noteShapeTri = new sf::ConvexShape(3);
	noteShapeTri->setPoint(0, sf::Vector2f(0, 10));
	noteShapeTri->setPoint(1, sf::Vector2f(25, 0));
	noteShapeTri->setPoint(2, sf::Vector2f(25, 20));

	noteShapeTri->setOrigin(sf::Vector2f(0, 10));

	noteShapeTri->setOutlineColor(sf::Color::White);

	noteShape = noteShapeTri;


}

Skin::~Skin()
{
	delete noteShape;
	for (sf::Shape* shape : middleShapes)
	{
		delete shape;
	}

	if (middleHitMarkerShape != nullptr)
		delete middleHitMarkerShape;
}

void Skin::renderNote(int time, sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f prevPos, int colorId)
{
	noteShape->setPosition(screenCenter + position);

	float rotation = std::atan2f(prevPos.y - position.y, prevPos.x - position.x) * 180 / (float)M_PI;
	noteShape->setRotation(rotation);
	noteShape->setFillColor(colors[colorId]);
	window.draw(*noteShape);
}

void Skin::prepareMiddleForSong(int amountOfColors)
{
	float degreeDistance = 0;
	if (amountOfColors == 0)
	{
		sf::CircleShape* middle = new sf::CircleShape(middleRadius, 20);
		middle->setOrigin(sf::Vector2f(middle->getLocalBounds().width / 2, middle->getLocalBounds().height / 2));
		middle->setPosition(screenCenter);
		middle->setFillColor(colors[0]);
		//middle->setPosition(sf::Vector2f(screenCenter.x - middle->getLocalBounds().width/2, screenCenter.y - middle->getLocalBounds().height/2));
		middleShapes.push_back(middle);

		middleHitMarkerShape = new sf::CircleShape(middleRadius / 2, 20);
		middleHitMarkerShape->setOrigin(sf::Vector2f(middleHitMarkerShape->getLocalBounds().width / 2, middleHitMarkerShape->getLocalBounds().height / 2));
		middleHitMarkerShape->setPosition(screenCenter);

	}
	else if (amountOfColors == 1)
	{
		//Not implemnted yet :P
	}
	else if (amountOfColors > 2)
	{
		degreeDistance = 1.0f / amountOfColors;
		for (int i = 0; i < amountOfColors; i++)
		{
			sf::ConvexShape* colorShape = new sf::ConvexShape(3);
			noteShapeTri->setPoint(0, sf::Vector2f(0, 10));
			noteShapeTri->setPoint(1, sf::Vector2f(25, 0));
			noteShapeTri->setPoint(2, sf::Vector2f(25, 20));
		}
	}


}

void Skin::renderMiddle(int time, sf::RenderWindow& window, float rotation)
{

	for (sf::Shape* shape : middleShapes)
	{
		window.draw(*shape);
	}
	window.draw(*middleHitMarkerShape);
}

void Skin::showHitMark(HitType hitType)
{
	middleHitMarkerShape->setFillColor(hitMarkerColors[(int)hitType]);

}

float Skin::getMiddleRadius()
{
	return middleRadius;
}
