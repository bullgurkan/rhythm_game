#include "Button.hpp"
#include <iostream>

Button::Button(sf::Vector2i posRelativeToParent, sf::Vector2i size, Panel* parent, sf::Color colors[2], std::string text, sf::Font &font, std::function<void()> onClickFunc) : Panel(posRelativeToParent, parent, true), size{ size }, onClickFunc{ onClickFunc }, colors()
{
	
	for (size_t i = 0; i < 2; i++)
		this->colors[i] = colors[i];
	shape.setSize(static_cast<sf::Vector2f>(size));
	this->text.setString(text);
	this->text.setFont(font);
	this->text.setFillColor(sf::Color::Magenta);
}

Panel* Button::getSubPanelInDirection(Direciton dir, sf::Vector2i posRelativeToThis)
{
	return &*this;
}

void Button::render(int time, sf::RenderWindow& window, Skin& skin)
{

	
	shape.setFillColor(colors[selected ? 1 : 0]);

	//should prob be cached(and updated when parent or panel is moved)
	sf::Vector2f actualPos;
	if (parent != nullptr)
		actualPos = static_cast<sf::Vector2f>(parent->pos + pos);
	else
		actualPos = static_cast<sf::Vector2f>(pos);
	shape.setPosition(actualPos);
	
	text.setPosition(actualPos + (sf::Vector2f(shape.getGlobalBounds().width, shape.getGlobalBounds().height) - sf::Vector2f(text.getGlobalBounds().width, text.getGlobalBounds().height * 2)) / static_cast<float>(2));

	window.draw(shape);
	window.draw(text);
}

void Button::onClick()
{
	if(onClickFunc != nullptr)
		onClickFunc();
}

void Button::onSceneLoad()
{}

bool Button::inBounds(sf::Vector2i posRelativeToParent)
{
	sf::Vector2i posRelativeToThis = posRelativeToParent - pos;
	return posRelativeToThis.x > 0 && posRelativeToThis.x < size.x&& posRelativeToThis.y > 0 && posRelativeToThis.y < size.y;
}
