#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fstream>
#include <vector>
#include <string>
#include "Line.hpp"
#include <cmath>
#include "Editor.hpp"
#include "Object.hpp"
#include <fstream>
#include <filesystem>
#include <mach-o/dyld.h>

const float cell_side = 40.;
const float half_cell = cell_side / 2.;

std::string GetExecutablePath() {
    char path[PATH_MAX];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0) {
        return std::string(path);
    }
}

int main()
{
    std::filesystem::path currentPath = GetExecutablePath();
    currentPath = currentPath.parent_path();
    std::filesystem::current_path(currentPath);

    int CELL = (int)cell_side;
    int WIDTH = 35 * CELL, HEIGHT = 22 * CELL;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Level Editor", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);
    sf::Event evnt;

    Editor editor(window, evnt);

    sf::Music soundtrack;
    soundtrack.openFromFile("editor_music.ogg");
    soundtrack.setLoop(true);
    soundtrack.setVolume(50);
    soundtrack.play();

    while (window.isOpen())
    {
        while (window.pollEvent(evnt))
        {
            switch (evnt.type)
            {
            case sf::Event::Closed:
                window.close();

            case sf::Event::KeyPressed:
                if (evnt.key.code == sf::Keyboard::Tab)
                {
                    window.close();
                    editor.save();
                }
            }
        }

        window.clear(sf::Color(201, 220, 255));

        editor.update();

        window.display();
    }
}