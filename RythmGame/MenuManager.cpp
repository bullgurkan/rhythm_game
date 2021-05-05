#include "MenuManager.hpp"
#include <iostream>

MenuManager::MenuManager() : currentlySelected{ nullptr }, inputRoot{ nullptr }, renderRoot{ nullptr }
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
	if (currentlySelected == nullptr)
		return;
	if (state == KeyBind::KeyState::Pressed)
	{
		switch (action)
		{
		case KeyBind::KeyAction::Up:
		case KeyBind::KeyAction::Left:
		case KeyBind::KeyAction::Down:
		case KeyBind::KeyAction::Right:

		{
			// UP is the first dir so removing it from a dir will give a value between UP - RIGHT 0-3 (this will work even if  KeyBind::KeyAction::UP is not 0)
			Panel* newSelection = currentlySelected->getPanelInDirection(static_cast<Panel::Direciton>(static_cast<int>(action) - static_cast<int>(KeyBind::KeyAction::Up)));
			if (newSelection != nullptr)
			{
				currentlySelected->selected = false;
				newSelection->selected = true;
				currentlySelected = newSelection;
			}
				
		}
			break;
		case KeyBind::KeyAction::Click:
			currentlySelected->onClick();
			break;
		case KeyBind::KeyAction::Back:
			currentlySelected->onBack();
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
		if(currentlySelected != nullptr)
			currentlySelected->selected = false;
		currentlySelected = inputRoot->getDefaultSelectedPanel();
		currentlySelected->selected = true;
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
