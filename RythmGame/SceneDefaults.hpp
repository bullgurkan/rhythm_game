#pragma once
#include "AutoPanel.hpp"
#include "Button.hpp"
#include "MenuManager.hpp"
#include <functional>

void GenerateScenes(MenuManager &menuManager, sf::Font &font)
{
	sf::Color buttonColors[2] = { sf::Color::Cyan, sf::Color::White };

	//Main menu
	AutoPanel* root = new AutoPanel(sf::Vector2i(0, 0), sf::Vector2i(100, 100), nullptr, true);
	root->addSubPanel(new Button(sf::Vector2i(100, 100), sf::Vector2i(100, 100), root, buttonColors, "text", font));
	root->addSubPanel(new Button(sf::Vector2i(300, 100), sf::Vector2i(100, 100), root, buttonColors, "text", font));
	menuManager.assignScene(MenuManager::Scene::MAIN_MENU, root);

	//End game screen
	root = new AutoPanel(sf::Vector2i(0, 0), sf::Vector2i(100, 100), nullptr, true);
	
	root->addSubPanel(new Button(sf::Vector2i(100, 500), sf::Vector2i(300, 100), root, buttonColors, "Back To Menu", font, static_cast<std::function<void()>>([&menuManager]() { menuManager.setActiveScene(MenuManager::Scene::MAIN_MENU); })));

	root->addSubPanel(new Button(sf::Vector2i(300, 100), sf::Vector2i(100, 100), root, buttonColors, "text", font));
	root->addSubPanel(new Button(sf::Vector2i(300, 800), sf::Vector2i(100, 100), root, buttonColors, "text", font));
	menuManager.assignScene(MenuManager::Scene::SONG_SELECT, root);


}