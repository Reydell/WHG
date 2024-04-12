#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include <cmath> ///
#include "Storage.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Audio.hpp>

void f(int a)
{
    std::cout << a << std::endl;
}

void f(bool a)
{
    std::cout << a << std::endl;
}

extern const float cell_side;
extern const int setup_maxlevel;

bool Player::victory;
int  Player::deaths;
int  Player::checkDeathIncrement;

class Manager
{
public:
    int maxlevel; 
    sf::Font font;
    sf::RenderWindow& window;
    Storage storage; //was a ref

    std::vector<std::string> savefiles;
    int currentLevel;
    int wheelchair;

private:
    sf::Color background = sf::Color(152, 153, 203);//sf::Color(191, 169, 249);
    sf::Event evnt;
    double timestamp;
    double levelTime;   

public:
    GameObject* pPlayer = nullptr;

public:
    Manager (sf::RenderWindow& renderWindow/*, Storage& gameStorage*/):
        timestamp(0.),
        levelTime(0.),
        window(renderWindow),
        maxlevel(setup_maxlevel),
        wheelchair(100)
    {
        Player::deaths = 0;

        font.loadFromFile("resources/font.ttf");
        // storage.load();
        currentLevel = 0;

        for (int i = 1; i <= maxlevel; i++)
        {
            std::string path("saves/level");
            path += std::to_string(i);
            savefiles.push_back(path);
        }

        storage.loadFromFile(savefiles[currentLevel]);
        // storage.loadFromFile("saves/a");
        pPlayer = storage.findByName();

        // for (size_t i = 0; i < count; i++)
        // {
        //     /* code */
        // }
        
    }

    void launchVictoryScreen()
    {
        background = sf::Color(143, 145, 201);

        sf::Text message;
        message.setString(sf::String("cooliti"));
        message.setFont(font);
        message.setFillColor(sf::Color::Black);
        message.setScale(.125f * cell_side, .125f * cell_side);
        message.setPosition(11.25f * cell_side, 6.25f * cell_side);
        window.draw(message);

        sf::Text time;
        std::string clock("TIME:");
        clock += std::to_string((int)timestamp / 60);
        clock += std::string(":");
        if ((int)timestamp % 60 < 10)
        {
            clock += std::string("0");
        }
        clock += std::to_string((int)timestamp % 60);
        clock += std::string(".");
        
        if (100. * fmod(timestamp, 1.) < 10)
        {
            clock += std::string("0");
        }
        
        clock += std::to_string((int)(100. * fmod(timestamp, 1.)));

        time.setString(sf::String(clock));
        time.setFont(font);
        time.setFillColor(sf::Color::Black);
        time.setScale(.0875f * cell_side, .0875f * cell_side);
        time.setPosition(11.25f * cell_side, 10.f * cell_side);
        window.draw(time);

        sf::Text deaths;
        std::string counter("DEATHS: ");
        counter += std::to_string(Player::deaths);

        deaths.setString(sf::String(counter));
        deaths.setFont(font);
        deaths.setFillColor(sf::Color::Black);
        deaths.setScale(.0875f * cell_side, .0875f * cell_side);
        deaths.setPosition(11.25 * cell_side, 13.75f * cell_side);

        window.draw(deaths);
    }

    void updateDeath()
    {
        sf::Text text;
        std::string counter("DEATHS: ");
        counter += std::to_string(Player::deaths);

        text.setString(sf::String(counter));
        text.setFont(font);
        text.setFillColor(sf::Color::Black);
        text.setScale(.0375 * cell_side, .0375 * cell_side);
        text.setPosition(.25f * cell_side, .25f * cell_side);

        window.draw(text);
    }

    void updateLevelNumber()
    {
        sf::Text text;
        std::string levelNumber("LEVEL: ");

        if ((currentLevel + 1) < 10)
        {
            levelNumber += std::string(" ");
        }

        levelNumber += std::to_string(currentLevel + 1);
        text.setString(sf::String(levelNumber));
        text.setFont(font);
        text.setFillColor(sf::Color::Black);
        text.setScale(.0375 * cell_side, .0375 * cell_side);
        text.setPosition(28.5f * cell_side, .25f * cell_side);

        window.draw(text);
    }

    void updateClock()
    {
        sf::Text text;
        std::string clock("");
        clock += std::to_string((int)timestamp / 60);
        clock += std::string(":");
        if ((int)timestamp % 60 < 10)
        {
            clock += std::string("0");
        }
        clock += std::to_string((int)timestamp % 60);
        clock += std::string(".");
        
        if (100. * fmod(timestamp, 1.) < 10)
        {
            clock += std::string("0");
        }
        
        clock += std::to_string((int)(100. * fmod(timestamp, 1.)));

        text.setString(sf::String(clock));
        text.setFont(font);
        text.setFillColor(sf::Color::Black);
        text.setScale(.0375f * cell_side, .0375f * cell_side);
        text.setPosition(14.5f * cell_side, .25f * cell_side);

        window.draw(text);
    }

    void restoreCoins()
    {
        for (int i = 1; i <= storage.objects.len(); i++)
        {
            GameObject* tmpPtr = storage.objects[i];
            if (tmpPtr->_name == std::string("coin"))
            {
                static_cast<Coin*>(tmpPtr)->_isCollected = false; 
            }
        }
    }

    void update()
    {
        while (window.pollEvent(evnt))
        {
            switch (evnt.type)
            {
            case sf::Event::Closed:
                window.close();

            case sf::Event::KeyPressed:
                if (evnt.key.code == sf::Keyboard::Tab)
                    window.close();
                else if (evnt.key.code == sf::Keyboard::L) // level skip
                {
                    Player::victory = true;
                }
            }
        }

        window.clear(background);

        if (!Player::victory)
        {
            for (int i = 1; i <= storage.objects.len(); i++)
            {
                storage.objects[i]->draw(window);
            }

            for (int i = 1; i <= storage.objects.len(); i++)
            {
                storage.objects[i]->move(levelTime);
            }

            for (int i = 1; i <= storage.objects.len(); i++)
            {
                Player::checkDeathIncrement = Player::deaths;
                pPlayer->checkCollision(storage.objects[i]);  
                
                if (Player::checkDeathIncrement < Player::deaths)
                {
                    this->restoreCoins();
                }
            }
            
            // static_cast<Player*>(pPlayer)->_position += static_cast<Player*>(pPlayer)->_velocity;

            this->updateDeath();
            this->updateClock();
            this->updateLevelNumber();
            timestamp += 1. / 60.;
            levelTime += 1. / 60.;
        }

        else  // victory
        {
            storage.clear();

            if (currentLevel == maxlevel - 1)
            {
                Player::victory = true;
                wheelchair += 1;
                this->launchVictoryScreen();
            }
            else
            {
                levelTime = 0.;
                currentLevel++;
                Player::victory = false;
                storage.loadFromFile(savefiles[currentLevel]);
                pPlayer = storage.findByName();
                Player::coinsRemaining = Player::totalCoins;
            }            
        }
        
        window.display();
    } 
};