#pragma once
#include "KeyBind.hpp"
#include "Panel.hpp"
#include <unordered_map>
class MenuManager
{
public:
	MenuManager();
	~MenuManager();

	enum class Scene
	{
		MAIN_MENU,
		SONG_SELECT,
		IN_GAME,
		POST_GAME
	};
	
	void render(int time, sf::RenderWindow& window, Skin &skin);
	void keyChanged(KeyBind::KeyAction action, KeyBind::KeyState state);
	void mouseMoved(int x, int y);
	void textEntered(int unicode);
	void setActiveScene(Scene scene);
	void assignScene(Scene scene, Panel* root);

	/** used to "lock" the input to a confined space. ex a popup menu (that menu saves a pointer to the original root and changes it back when closed) **/
	void setInputRoot(Panel* inputRoot);
	Panel* getCurrentInputRoot();

private:
	Panel* renderRoot;
	Panel* inputRoot;
	Panel* currentlySelected;

	std::unordered_map<Scene, Panel*> loadedScenes;

};