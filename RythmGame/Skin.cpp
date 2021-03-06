#include "Skin.hpp"
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

Skin::Skin(float middleRadius, std::vector<sf::Color> colors, std::vector<sf::Color> hitMarkerColors, sf::Vector2f screenSize, sf::Color notPressedColorMult) : middleRadius{ middleRadius }, noteShape{ nullptr }, middleShapes(), middleHitMarkerShape{ nullptr }, colors{ colors }, hitMarkerColors{ hitMarkerColors }, notPressedColorMult{ notPressedColorMult }, screenCenter(screenSize.x / 2, screenSize.y / 2)
{
	sf::ConvexShape* noteShapeTri = new sf::ConvexShape(3);
	noteShapeTri->setPoint(0, sf::Vector2f(-15, 0));
	noteShapeTri->setPoint(1, sf::Vector2f(15, -10));
	noteShapeTri->setPoint(2, sf::Vector2f(15, 10));

	noteShapeTri->setOrigin(sf::Vector2f(0, 0));

	//noteShapeTri->setOutlineColor(sf::Color::White);

	noteShape = noteShapeTri;

	gridShape = new sf::RectangleShape();
	gridShape->setFillColor(sf::Color::Transparent);

	gridShape->setOutlineThickness(2);
}


Skin::~Skin()
{
	delete noteShape;
	for (auto shape : middleShapes)
	{
		delete shape.shape;
	}

	if (middleHitMarkerShape != nullptr)
		delete middleHitMarkerShape;
	hitSounds.clear();

}

void Skin::renderNote(int time, sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f prevPos, int colorId, bool isNextNote)
{
	noteShape->setPosition(screenCenter + position);

	float rotation = std::atan2f(prevPos.y - position.y, prevPos.x - position.x) * 180 / (float)M_PI;
	noteShape->setRotation(rotation);
	//noteShape->setFillColor(isNextNote ? colors[colorId] : colors[colorId] * notPressedColorMult);
	noteShape->setFillColor(colors[colorId]);

	float dist = std::sqrt(position.x * position.x + position.y * position.y);
	gridShape->setSize(sf::Vector2f(dist * 2, dist * 2));
	gridShape->setOrigin(sf::Vector2f(dist, dist));
	gridShape->setRotation(rotation);
	gridShape->setPosition(screenCenter);
	float i = (1 - dist / 500);
	if (i < 0)
		i = 0;

	gridShape->setOutlineColor(sf::Color(255, 255, 255, i * 80));

	window.draw(*gridShape);

	window.draw(*noteShape);

}

void Skin::prepareMiddleForSong(int amountOfColors)
{
	if (middleHitMarkerShape != nullptr)
		delete middleHitMarkerShape;
	for (auto shape : middleShapes)
		delete shape.shape;
	middleShapes.clear();

	if (amountOfColors == 1)
	{
		sf::CircleShape* middle = new sf::CircleShape(middleRadius, 20);
		middle->setOrigin(sf::Vector2f(middle->getLocalBounds().width / 2, middle->getLocalBounds().height / 2));
		middle->setPosition(screenCenter);
		middle->setFillColor(colors[0]);
		//middle->setPosition(sf::Vector2f(screenCenter.x - middle->getLocalBounds().width/2, screenCenter.y - middle->getLocalBounds().height/2));
		middleShapes.push_back(MiddleShape(middle));

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

		angleBetweenColors = 2 * M_PI / amountOfColors;
		float radiusForEdge = static_cast<float>(middleRadius / std::sin(angleBetweenColors / 2));
		for (int i = 0; i < amountOfColors; i++)
		{
			sf::ConvexShape* colorShape = new sf::ConvexShape(3);

			colorShape->setPoint(0, sf::Vector2f(0, 0));
			colorShape->setPoint(1, sf::Vector2f(static_cast<float>(std::sin(angleBetweenColors * i) * radiusForEdge), static_cast<float>(std::cos(angleBetweenColors * i) * radiusForEdge)));
			colorShape->setPoint(2, sf::Vector2f(static_cast<float>(std::sin(angleBetweenColors * (static_cast<int64_t>(i) + 1))) * radiusForEdge, static_cast<float>(std::cos(angleBetweenColors * (static_cast<int64_t>(i) + 1l)) * radiusForEdge)));
			colorShape->setFillColor(colors[i] * notPressedColorMult);

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
	for (int i = 0; i < middleShapes.size(); i++)
	{
		middleShapes[i].shape->setRotation(rotation);
		window.draw(*middleShapes[i].shape);

		if (middleShapes[i].lastHitTime != 0)
		{
			float scale = 1.3 - (static_cast<float>(time - middleShapes[i].lastHitTime) / 100) * 0.3;
			if (scale < 1)
				scale = 1;
			middleShapes[i].shape->setScale(sf::Vector2f(scale, scale));
		}
		


	}
	middleHitMarkerShape->setRotation(rotation);
	window.draw(*middleHitMarkerShape);

}

void Skin::renderHoldLine(int time, sf::RenderWindow& window, sf::Vector2f pos1, sf::Vector2f pos2, int colorId, bool heldOrNextNote)
{
	float width = 2;
	sf::Vector2f normal = sf::Vector2f(pos1.y, -pos1.x) / std::sqrt(pos1.x * pos1.x + pos1.y * pos1.y);
	sf::ConvexShape line = sf::ConvexShape(4);
	line.setPoint(0, pos1 - normal * width);
	line.setPoint(1, pos1 + normal * width);
	line.setPoint(2, pos2 + normal * width);
	line.setPoint(3, pos2 - normal * width);

	line.setPosition(screenCenter);
	line.setFillColor(heldOrNextNote ? colors[colorId] : colors[colorId] * notPressedColorMult);
	window.draw(line);
}

void Skin::showHitMark(HitType hitType, int colorId, int time)
{
	if (hitType != HitType::EARLY_MISS && hitType != HitType::LATE_MISS)
	{
		auto sb = hitSounds.find(hitType);
		if (sb != hitSounds.end())
			soundPlayer.setBuffer(*sb->second);
		soundPlayer.setPitch(0.8f + (static_cast<float>(std::rand()) / RAND_MAX) / 2);
		soundPlayer.play();
		middleShapes[colorId].lastHitTime = time;
	}


	middleHitMarkerShape->setFillColor(hitMarkerColors[(int)hitType]);

}

void Skin::setColorPressed(bool held, int colorId)
{
	middleShapes[colorId].shape->setFillColor(held ? colors[colorId] : colors[colorId] * notPressedColorMult);
}

float Skin::getMiddleRadius()
{
	return middleRadius;
}

sf::Vector2f Skin::getNoteMiddle(sf::Vector2f pos, sf::Vector2f prevPos)
{
	sf::Vector2f delta = (prevPos - pos);
	float dist = std::sqrt(delta.x * delta.x + delta.y * delta.y);
	delta /= dist;
	return pos + delta * (noteShape->getLocalBounds().width / 2);
}

void Skin::setHitSounds(std::unordered_map<HitType, sf::SoundBuffer*>& hitSounds)
{
	this->hitSounds = hitSounds;
}
