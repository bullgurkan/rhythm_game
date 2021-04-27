#pragma once
#include <SFML/Graphics.hpp>
#include "Skin.hpp"

class Panel
{
	
public:
	enum class Direciton
	{
		UP,
		LEFT,
		DOWN,
		RIGHT
	};

	Panel(sf::Vector2i posRelativeToParent, Panel* parent, bool interactable);
	virtual Panel* getSubPanelInDirection(Direciton dir, sf::Vector2i posRelativeToThis) = 0;
	virtual Panel* getHoveredPanel(sf::Vector2i posRelativeToParent);
	virtual Panel* getDefaultSelectedPanel();
	virtual void render(int time, sf::RenderWindow& window, Skin &skin) = 0;
	virtual void onClick() = 0;
	virtual void onBack();
	Panel* getPanelInDirection(Direciton dir);
	sf::Vector2i pos;
	bool interactable;
	bool selected;
	//bool held;
protected:
	virtual bool inBounds(sf::Vector2i posRelativeToParent) = 0;
	Panel* parent;


	
}; 