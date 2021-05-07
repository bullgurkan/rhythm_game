#include "AutoPanel.hpp"
#include <iostream>

AutoPanel::AutoPanel(sf::Vector2i posRelativeToParent, sf::Vector2i size, Panel* parent, bool loopSelectorAtEdge) : Panel(posRelativeToParent, parent, true), size{ size }, loopSelectorAtEdge{ loopSelectorAtEdge }
{}

AutoPanel::~AutoPanel()
{
	for (auto child : subPanels)
		delete child;
}

Panel* AutoPanel::getSubPanelInDirection(Direciton dir, sf::Vector2i posRelativeToThis)
{
	Panel* closest = getSubPanelInDirectionNonRecursive(dir, posRelativeToThis, true);

	if (loopSelectorAtEdge && closest == nullptr)
		switch (dir)
		{
		case Panel::Direciton::UP:
			return getSubPanelInDirectionNonRecursive(dir, sf::Vector2i(posRelativeToThis.x, INT32_MIN), false);
		case Panel::Direciton::LEFT:
			return getSubPanelInDirectionNonRecursive(dir, sf::Vector2i(INT32_MAX, posRelativeToThis.y), false);
		case Panel::Direciton::DOWN:
			return getSubPanelInDirectionNonRecursive(dir, sf::Vector2i(posRelativeToThis.x, INT32_MAX), false);
		case Panel::Direciton::RIGHT:
			return getSubPanelInDirectionNonRecursive(dir, sf::Vector2i(INT32_MIN, posRelativeToThis.y), false);
		}


	return closest;
}

Panel* AutoPanel::getSubPanelInDirectionNonRecursive(Direciton dir, sf::Vector2i posRelativeToThis, bool ignoreNegativeDistance)
{
	Panel* closest = nullptr;
	int distance = INT32_MAX;

	for (Panel* child : subPanels)
	{
		if (!child->selected)
		{
			sf::Vector2i relativePos =  child->pos - posRelativeToThis;
			int dist = INT32_MAX;


			switch (dir)
			{
			case Panel::Direciton::UP:

				if (!ignoreNegativeDistance || relativePos.y < 0)
					dist = std::abs(relativePos.x)  - relativePos.y;
				else
					continue;
				break;
			case Panel::Direciton::LEFT:
				if (!ignoreNegativeDistance || relativePos.x < 0)
					dist = std::abs(relativePos.y)  - relativePos.x;
				else
					continue;
				break;
			case Panel::Direciton::DOWN:
				if (!ignoreNegativeDistance || relativePos.y > 0)
					dist = std::abs(relativePos.x) + relativePos.y;
				else
					continue;
				break;
			case Panel::Direciton::RIGHT:
				if (!ignoreNegativeDistance || relativePos.x > 0)
					dist = std::abs(relativePos.y)  + relativePos.x;
				else
					continue;
				break;
			default:
				continue;
			}
		

			if (dist < distance && (dist >= 0 ||(dist != 0 && !ignoreNegativeDistance)))
			{
				//std::cout << posRelativeToThis.y << " " <<dist << std::endl;
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
	if (subPanels.size() > 0)
		return subPanels.front();
	else
		return nullptr;
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
