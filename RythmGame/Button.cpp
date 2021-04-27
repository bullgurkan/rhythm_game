#include "Button.hpp"
#include <iostream>

Button::Button(sf::Vector2i posRelativeToParent, sf::Vector2i size, Panel* parent, sf::Color colors[2], std::string text) : Panel(posRelativeToParent, parent, true), size{ size }, onClickFunc{ nullptr }, colors()
{
	for (size_t i = 0; i < 2; i++)
		this->colors[i] = colors[i];
}

Panel* Button::getSubPanelInDirection(Direciton dir, sf::Vector2i posRelativeToThis)
{
	return &*this;
}

void Button::render(int time, sf::RenderWindow& window, Skin& skin)
{
	
	shape.setSize(static_cast<sf::Vector2f>(size));
	shape.setFillColor(colors[selected ? 1 : 0]);

	sf::Vector2f acutalPos;
	if(parent != nullptr)
		acutalPos = static_cast<sf::Vector2f>(parent->pos + pos);
	else
		acutalPos = static_cast<sf::Vector2f>(pos);
	shape.setPosition(acutalPos);
	window.draw(shape);
}

void Button::onClick()
{
	if (onClickFunc != nullptr)
		onClickFunc();
}

bool Button::inBounds(sf::Vector2i posRelativeToParent)
{
	sf::Vector2i posRelativeToThis = posRelativeToParent - pos;
	return posRelativeToThis.x > 0 && posRelativeToThis.x < size.x&& posRelativeToThis.y > 0 && posRelativeToThis.y < size.y;
}
