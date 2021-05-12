#include "Panel.hpp"

Panel::Panel(sf::Vector2i posRelativeToParent, Panel* parent, bool interactable) : pos{ posRelativeToParent }, parent{ parent }, interactable{ interactable }
{}

Panel* Panel::getSubPanelInDirection(Direciton dir, sf::Vector2i posRelativeToThis)
{
	return nullptr;
}

void Panel::onBack()
{
	parent->onBack();
}


void Panel::onSceneLoad()
{}

Panel* Panel::getHoveredPanel(sf::Vector2i posRelativeToParent)
{
	return nullptr;
}

Panel* Panel::getDefaultSelectedPanel()
{
	return &*this;
}

Panel* Panel::getPanelInDirection(Direciton dir)
{
	if (parent == nullptr)
		return nullptr;
	Panel* panel = parent->getSubPanelInDirection(dir, pos);
	if (panel == nullptr)
		panel = parent->getPanelInDirection(dir);
	return panel;
}
