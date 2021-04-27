#include "AutoPanel.hpp"
#include <iostream>

AutoPanel::AutoPanel(sf::Vector2i posRelativeToParent, sf::Vector2i size, Panel* parent) : Panel(posRelativeToParent, parent, true), size{ size }
{}

Panel* AutoPanel::getSubPanelInDirection(Direciton dir, sf::Vector2i posRelativeToThis)
{
	Panel* closest = nullptr;
	int distance = INT32_MAX;

	for (Panel* child : subPanels)
	{
		if (!child->selected)
		{
			sf::Vector2i relativePos = posRelativeToThis - child->pos;
			int dist = -1;


			switch (dir)
			{
			case Panel::Direciton::UP:
				if (relativePos.y < 0)
					dist = std::abs(relativePos.x) * 2 - relativePos.y;
				else
					dist = -1;
				break;
			case Panel::Direciton::LEFT:
				if (relativePos.x < 0)
					dist = std::abs(relativePos.y) * 2 - relativePos.x;
				else
					dist = -1;
				break;
			case Panel::Direciton::DOWN:
				if (relativePos.y > 0)
					dist = std::abs(relativePos.x) * 2 + relativePos.y;
				else
					dist = -1;
				break;
			case Panel::Direciton::RIGHT:
				if (relativePos.x > 0)
					dist = std::abs(relativePos.y) * 2 + relativePos.x;
				else
					dist = -1;
				break;
			default:
				break;
			}


			if (dist < distance && dist >= 0)
			{
				closest = child;
				distance = dist;
			}
		}
		

	}
	return closest;
}

Panel* AutoPanel::getHoveredPanel(sf::Vector2i posRelativeToParent)
{
	return nullptr;
}

Panel* AutoPanel::getDefaultSelectedPanel()
{
	return subPanels.front();
}

void AutoPanel::render(int time, sf::RenderWindow& window, Skin& skin)
{
	for (Panel* child : subPanels)
	{
		child->render(time, window, skin);
	}
}

bool AutoPanel::inBounds(sf::Vector2i posRelativeToParent)
{
	sf::Vector2i posRelativeToThis = posRelativeToParent - pos;
	return posRelativeToThis.x > 0 && posRelativeToThis.x < size.x&& posRelativeToThis.y > 0 && posRelativeToThis.y < size.y;
}

void AutoPanel::onClick()
{
	std::cout << "AutoPanel should not be selected and thus not clicked" << std::endl;
}

void AutoPanel::addSubPanel(Panel* subPanel)
{
	subPanels.push_back(subPanel);
}
