#include "MenuManager.hpp"
#include <iostream>
#include "GameManager.hpp"

MenuManager::MenuManager(GameManager& gm) : currentlySelected{ nullptr }, inputRoot{ nullptr }, renderRoot{ nullptr }, gm{ gm }
{}

MenuManager::~MenuManager()
{
	for (auto scene : loadedScenes)
		delete scene.second;
}

void MenuManager::render(int time, sf::RenderWindow& window, Skin& skin)
{
	renderRoot->render(time, window, skin);
}

void MenuManager::keyChanged(KeyBind::KeyAction action, KeyBind::KeyState state)
{

	if (state == KeyBind::KeyState::Pressed)
	{
		switch (action)
		{
		case KeyBind::KeyAction::Up:
		case KeyBind::KeyAction::Left:
		case KeyBind::KeyAction::Down:
		case KeyBind::KeyAction::Right:

		{
			if (currentlySelected == nullptr)
				break;
			// UP is the first dir so removing it from a dir will give a value between UP - RIGHT 0-3 (this will work even if  KeyBind::KeyAction::UP is not 0)
			Panel* newSelection = nullptr;
			if (currentlySelected != nullptr)
				newSelection = currentlySelected->getPanelInDirection(static_cast<Panel::Direciton>(static_cast<int>(action) - static_cast<int>(KeyBind::KeyAction::Up)));
			if (newSelection != nullptr)
			{
				currentlySelected->selected = false;
				newSelection->selected = true;
				currentlySelected = newSelection;
			}

		}
		break;
		case KeyBind::KeyAction::Click:
			if (currentlySelected != nullptr)
				currentlySelected->onClick();
			break;
		case KeyBind::KeyAction::Back:
			if (inputRoot != nullptr)
				inputRoot->onBack();
			break;
		default:
			break;
		}
	}
	/*else
	{
		if (action == KeyBind::KeyAction::SELECT)
		{
			currentlySelected->onClick();
		}
	}*/
}

void MenuManager::mouseMoved(int x, int y)
{}

void MenuManager::textEntered(int unicode)
{}

void MenuManager::setActiveScene(Scene scene)
{
	
	auto sceneSearch = loadedScenes.find(scene);
	if (sceneSearch != loadedScenes.end())
	{
		renderRoot = inputRoot = sceneSearch->second;
		inputRoot->onSceneLoad();
		if (currentlySelected != nullptr)
			currentlySelected->selected = false;
		currentlySelected = inputRoot->getDefaultSelectedPanel();
		if (currentlySelected != nullptr)
			currentlySelected->selected = true;
		if (scene != Scene::IN_GAME)
			gm.LoadSong(nullptr);
	}
	else
	{
		std::cout << "scene: " << static_cast<int>(scene) << " did not exitst" << std::endl;
	}

}

void MenuManager::assignScene(Scene scene, Panel* root)
{
	auto sceneSearch = loadedScenes.find(scene);
	if (sceneSearch != loadedScenes.end())
		sceneSearch->second = root;
	else
		loadedScenes.insert(std::pair<Scene, Panel*>(scene, root));
}

void MenuManager::setInputRoot(Panel* inputRoot)
{
	inputRoot = inputRoot;
}

Panel* MenuManager::getCurrentInputRoot()
{
	return inputRoot;
}
