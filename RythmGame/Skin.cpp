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
	if (amountOfColors == 1)
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
	else if (amountOfColors == 2)
	{
		//Not implemnted yet :P
	}
	else if (amountOfColors > 3)
	{
		sf::ConvexShape* middleHitMarker = new sf::ConvexShape(amountOfColors);

		degreeDistance = 2 * M_PI / amountOfColors;
		float radiusForEdge = static_cast<float>(middleRadius / std::sin(degreeDistance / 2));
		for (int i = 0; i < amountOfColors; i++)
		{
			sf::ConvexShape* colorShape = new sf::ConvexShape(3);

			colorShape->setPoint(0, sf::Vector2f(0, 0));
			colorShape->setPoint(1, sf::Vector2f(static_cast<float>(std::sin(degreeDistance * i) * radiusForEdge), static_cast<float>(std::cos(degreeDistance * i) * radiusForEdge)));
			colorShape->setPoint(2, sf::Vector2f((float)std::sin(degreeDistance * (static_cast<int64_t>(i) + 1)) * radiusForEdge, (float)std::cos(degreeDistance * (static_cast<int64_t>(i) + 1l)) * radiusForEdge));
			colorShape->setFillColor(colors[i]);

			colorShape->setPosition(screenCenter);
			middleShapes.push_back(colorShape);



			middleHitMarker->setPoint(i, colorShape->getPoint(1));

		}

		middleHitMarker->setPosition(screenCenter);
		middleHitMarkerShape = middleHitMarker;
		middleHitMarkerShape->setScale(sf::Vector2f(0.5f, 0.5f));

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

void Skin::renderHoldLine(int time, sf::RenderWindow& window, sf::Vector2f pos1, sf::Vector2f pos2, int colorId)
{
	float width = 2;
	sf::Vector2f normal = sf::Vector2f(pos1.y, -pos1.x) / std::sqrt(pos1.x * pos1.x + pos1.y * pos1.y);
	sf::ConvexShape line = sf::ConvexShape(4);
	line.setPoint(0, pos1 - normal * width);
	line.setPoint(1, pos1 + normal * width);
	line.setPoint(2, pos2 + normal * width);
	line.setPoint(3, pos2 - normal * width);

	line.setPosition(screenCenter);
	//line.setFillColor(colors[colorId]);
	window.draw(line);
}

void Skin::showHitMark(HitType hitType)
{
	std::cout << (int)hitType << std::endl;
	middleHitMarkerShape->setFillColor(hitMarkerColors[(int)hitType]);

}

float Skin::getMiddleRadius()
{
	return middleRadius;
}

sf::Vector2f Skin::getNoteMiddle(sf::Vector2f pos, sf::Vector2f prevPos)
{
	float dist = std::atan2((prevPos - pos).y, (prevPos - pos).x);
	return pos + sf::Vector2f(noteShape->getLocalBounds().width / dist, noteShape->getLocalBounds().height / dist);
}
