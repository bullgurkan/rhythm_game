#pragma once
#include "AutoPanel.hpp"
#include "Button.hpp"
#include "MenuManager.hpp"
#include "SongList.hpp"
#include "ScorePanel.hpp"
#include <functional>

void GenerateScenes(MenuManager &menuManager, sf::Font &font, GameManager &gm)
{
	sf::Color buttonColors[2] = { sf::Color::White, sf::Color::Cyan  };

	//Main menu
	AutoPanel* root = new AutoPanel(sf::Vector2i(0, 0), sf::Vector2i(100, 100), nullptr, true);
	root->addSubPanel(new Button(sf::Vector2i(100, 100), sf::Vector2i(100, 100), root, buttonColors, "text", font));
	root->addSubPanel(new Button(sf::Vector2i(300, 100), sf::Vector2i(100, 100), root, buttonColors, "text", font));
	menuManager.assignScene(MenuManager::Scene::MAIN_MENU, root);

	//Post game screen
	root = new AutoPanel(sf::Vector2i(0, 0), sf::Vector2i(100, 100), nullptr, true);

	root->addSubPanel(new ScorePanel(sf::Vector2i(100, 100), sf::Vector2i(500, 500), root, buttonColors[0], sf::Color::Black, font, gm));
	root->addSubPanel(new Button(sf::Vector2i(1400, 800), sf::Vector2i(500, 100), root, buttonColors, "Back To SongSelect", font, static_cast<std::function<void()>>([&menuManager]() { menuManager.setActiveScene(MenuManager::Scene::SONG_SELECT); })));

	//root->addSubPanel(new Button(sf::Vector2i(300, 100), sf::Vector2i(100, 100), root, buttonColors, "text", font));
	//root->addSubPanel(new Button(sf::Vector2i(300, 800), sf::Vector2i(100, 100), root, buttonColors, "text", font));
	menuManager.assignScene(MenuManager::Scene::POST_GAME, root);

	//Song select
	root = new AutoPanel(sf::Vector2i(0, 0), sf::Vector2i(100, 100), nullptr, true);
	
	//root->addSubPanel(new Button(sf::Vector2i(100, 500), sf::Vector2i(300, 100), root, buttonColors, "Back To Menu", font, static_cast<std::function<void()>>([&menuManager]() { menuManager.setActiveScene(MenuManager::Scene::MAIN_MENU); })));

	//root->addSubPanel(new Button(sf::Vector2i(300, 100), sf::Vector2i(100, 100), root, buttonColors, "text", font));
	root->addSubPanel(new SongList(sf::Vector2i(300, 100), sf::Vector2i(500, 800), root, 5, gm, buttonColors, sf::Color::Black, font, 10));
	
	menuManager.assignScene(MenuManager::Scene::SONG_SELECT, root);

	root = new AutoPanel(sf::Vector2i(0, 0), sf::Vector2i(0, 0), nullptr, false, static_cast<std::function<void()>>([&menuManager]() { menuManager.setActiveScene(MenuManager::Scene::SONG_SELECT); }));
	
	menuManager.assignScene(MenuManager::Scene::IN_GAME, root);

}