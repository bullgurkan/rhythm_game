#pragma once
#include <unordered_map>
#include "KeyBind.hpp"

const std::unordered_map<sf::Keyboard::Key, KeyBind> keyBinds = std::unordered_map<sf::Keyboard::Key, KeyBind>(
	{
		std::pair<sf::Keyboard::Key, KeyBind>
		(
			sf::Keyboard::Key::D,
			KeyBind(
				std::vector<KeyBind::KeyAction>
				(
					{KeyBind::KeyAction::InGameColor, KeyBind::KeyAction::Right}
				),
				new std::unordered_map<int, int>
				(
					{std::pair<int, int>(4,0)}
				)
			)
		),
	std::pair<sf::Keyboard::Key, KeyBind>
		(
			sf::Keyboard::Key::F,
			KeyBind(
				std::vector<KeyBind::KeyAction>
				(
					{KeyBind::KeyAction::InGameColor}
				),
				new std::unordered_map<int, int>
				(
					{std::pair<int, int>(4,1)}
				)
			)
		),
	std::pair<sf::Keyboard::Key, KeyBind>
		(
			sf::Keyboard::Key::J,
			KeyBind(
				std::vector<KeyBind::KeyAction>
				(
					{KeyBind::KeyAction::InGameColor}
				),
				new std::unordered_map<int, int>
				(
					{std::pair<int, int>(4,2)}
				)
			)
		),
	std::pair<sf::Keyboard::Key, KeyBind>
		(
			sf::Keyboard::Key::K,
			KeyBind(
				std::vector<KeyBind::KeyAction>
				(
					{KeyBind::KeyAction::InGameColor}
				),
				new std::unordered_map<int, int>
				(
					{std::pair<int, int>(4,3)}
				)
			)
		),
	std::pair<sf::Keyboard::Key, KeyBind>
		(
			sf::Keyboard::Key::W,
			KeyBind(
				std::vector<KeyBind::KeyAction>
				(
					{KeyBind::KeyAction::Up}
				),
				nullptr
			)
		),
	std::pair<sf::Keyboard::Key, KeyBind>
		(
			sf::Keyboard::Key::A,
			KeyBind(
				std::vector<KeyBind::KeyAction>
				(
					{KeyBind::KeyAction::Left}
				),
				nullptr
			)
		),
	std::pair<sf::Keyboard::Key, KeyBind>
		(
			sf::Keyboard::Key::S,
			KeyBind(
				std::vector<KeyBind::KeyAction>
				(
					{KeyBind::KeyAction::Down}
				),
				nullptr
			)
		),
	std::pair<sf::Keyboard::Key, KeyBind>
		(
			sf::Keyboard::Key::Escape,
			KeyBind(
				std::vector<KeyBind::KeyAction>
				(
					{KeyBind::KeyAction::Back}
				),
				nullptr
			)
		),
	std::pair<sf::Keyboard::Key, KeyBind>
		(
			sf::Keyboard::Key::Up,
			KeyBind(
				std::vector<KeyBind::KeyAction>
				(
					{KeyBind::KeyAction::Up}
				),
				nullptr
			)
		),
	std::pair<sf::Keyboard::Key, KeyBind>
		(
			sf::Keyboard::Key::Left,
			KeyBind(
				std::vector<KeyBind::KeyAction>
				(
					{KeyBind::KeyAction::Left}
				),
				nullptr
			)
		),
	std::pair<sf::Keyboard::Key, KeyBind>
		(
			sf::Keyboard::Key::Down,
			KeyBind(
				std::vector<KeyBind::KeyAction>
				(
					{KeyBind::KeyAction::Down}
				),
				nullptr
			)
		),
	std::pair<sf::Keyboard::Key, KeyBind>
		(
			sf::Keyboard::Key::Right,
			KeyBind(
				std::vector<KeyBind::KeyAction>
				(
					{KeyBind::KeyAction::Right}
				),
				nullptr
			)
		),
	std::pair<sf::Keyboard::Key, KeyBind>
		(
			sf::Keyboard::Key::Space,
			KeyBind(
				std::vector<KeyBind::KeyAction>
				(
					{ KeyBind::KeyAction::Click }
				),
				nullptr
			)
		),
		std::pair<sf::Keyboard::Key, KeyBind>
		(
			sf::Keyboard::Key::Enter,
			KeyBind(
				std::vector<KeyBind::KeyAction>
				(
					{ KeyBind::KeyAction::Click }
					),
				nullptr
			)
		)
	});

const std::unordered_map<sf::Mouse::Button, KeyBind> mouseButtonBinds = std::unordered_map<sf::Mouse::Button, KeyBind>({ std::pair<sf::Mouse::Button, KeyBind>(sf::Mouse::Button::Left, KeyBind({KeyBind::KeyAction::Click}, nullptr)) });