#include "Skin.hpp"
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>

Skin::Skin(float middleRadius, std::vector<sf::Color> colors, sf::Vector2f screenSize) : middleRadius{ middleRadius }, noteShape(), middleShapes(), colors{ colors }, screenCenter(screenSize.x/2, screenSize.y/2)
{
	sf::ConvexShape* noteShapeTri = new sf::ConvexShape(3);
	noteShapeTri->setPoint(0, sf::Vector2f(10, 20));
	noteShapeTri->setPoint(1, sf::Vector2f(20, 0));
	noteShapeTri->setPoint(2, sf::Vector2f(0, 0));

	noteShapeTri->setOrigin(sf::Vector2f(10, 20));

	noteShape = noteShapeTri;
}

Skin::~Skin()
{
	delete noteShape;
	for (sf::Shape* shape : middleShapes)
	{
		delete shape;
	}
}

void Skin::renderNote(long time, sf::RenderWindow &window, sf::Vector2f position, float rotation, int colorId)
{
	noteShape->setPosition(screenCenter);
	noteShape->setRotation(rotation);
	noteShape->setFillColor(colors[colorId]);
	window.draw(*noteShape);
}

void Skin::prepareMiddleForSong(int amountOfColors)
{
	float radius = 20;
	float degreeDistance = 0;
	if (amountOfColors == 0)
	{
		sf::CircleShape* middle = new sf::CircleShape(radius, 20);
		middle->setOrigin(sf::Vector2f(middle->getLocalBounds().width/2, middle->getLocalBounds().height/2));
		middle->setPosition(screenCenter);

		//middle->setPosition(sf::Vector2f(screenCenter.x - middle->getLocalBounds().width/2, screenCenter.y - middle->getLocalBounds().height/2));
		middleShapes.push_back(middle);
	}
	else
	{
		degreeDistance = 1.0f / amountOfColors;
		for (int i = 0; i < amountOfColors; i++)
		{
			
		}
	}
}

void Skin::renderMiddle(long time, sf::RenderWindow &window, float rotation)
{
	
	for (sf::Shape *shape : middleShapes)
	{
		window.draw(*shape);
	}
}
