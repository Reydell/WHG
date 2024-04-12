#include <SFML/Graphics.hpp>
#include "class/Manager.hpp"
#include "class/List.hpp"
#include "class/GameObject.hpp"
#include "class/Player.hpp"
#include "class/Enemy.hpp"
#include "class/Storage.hpp"
#include "class/Tile.hpp"
#include "class/Coin.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <SFML/Audio.hpp>
#include <filesystem>
#include <mach-o/dyld.h>

const float cell_side = 40.f;
const int setup_maxlevel = 10;

bool Player::checkCollectedCoins;

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
    int WIDTH = 32 * CELL, HEIGHT = 22 * CELL;

    int deats = Player::deaths;
    int levelsCompleted = 0;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "World's Hardest Game", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);
    Manager manager = Manager(window);

    sf::Music soundtrack;
    soundtrack.openFromFile("resources/soundtrack.ogg");
    soundtrack.setLoop(true);
    soundtrack.setVolume(50);
    soundtrack.play();

    sf::SoundBuffer sound_buffer;
    sf::Sound sound;
    sound.setBuffer(sound_buffer);

    while (window.isOpen())
    {
        manager.update();

        if (Player::deaths > deats)
        {
            sound_buffer.loadFromFile("resources/deathSound.wav");
            sound.play();
            deats++;
        }   

        if (Player::checkCollectedCoins)
        {
            sound_buffer.loadFromFile("resources/smb_coin.wav");
            sound.play();
        }

        Player::checkCollectedCoins = false;
        

        if (Player::victory)
        {
            if (levelsCompleted < manager.maxlevel - 1)
            {
                sound_buffer.loadFromFile("resources/smb_jump-super.wav");
                sound.play();
                levelsCompleted++;
            }
            if (manager.wheelchair == 101)
            {
                soundtrack.stop();
                sound_buffer.loadFromFile("resources/smb_stage_clear.wav");
                sound.play();
            }
            
        }        
    }
}