#include <SFML/Graphics.hpp>
#include "Song.hpp"
#include <chrono>
#include <iostream>

int main()
{
    sf::Vector2f screenSize(1920, 1080);
    sf::RenderWindow window(sf::VideoMode((unsigned int)screenSize.x, (unsigned int)screenSize.y), "SFML works!");

    std::vector<sf::Color> colors;
    colors.push_back(sf::Color::Red);
    colors.push_back(sf::Color::Green);
    colors.push_back(sf::Color::Blue);
    colors.push_back(sf::Color::Cyan);

    Skin skin (1, colors, screenSize);

    std::vector<Note> notes;
    notes.push_back(Note(10000, sf::Vector2f(0.01f, 0), 0));

    Song song (notes, 1.0f, skin);

    long time = 0;
    long lastTime = (long)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    long currentTime = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        currentTime = (long)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        if (currentTime - lastTime != 0)
        {
            time += currentTime - lastTime;
            lastTime = currentTime;
        }
        

        

        window.clear();
        song.render(time, window);
        window.display();
    }

    return 0;
}