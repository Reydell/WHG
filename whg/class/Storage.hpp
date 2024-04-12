#pragma once
#include "List.hpp" 
#include "GameObject.hpp"
#include <vector>
#include <string>
#include "Player.hpp"
#include "Enemy.hpp"
#include "Tile.hpp"
#include "Coin.hpp"
#include <fstream>
#include <iostream>

extern const float cell_side;
int Player::totalCoins;
int Player::coinsRemaining;

std::vector<std::string> split(const std::string& s)
{
   std::vector<std::string> res;
   typedef std::string::size_type string_size;
   string_size i = 0;

   while (i != s.size()) 
   {
      while (i != s.size() && isspace(s[i]))
        ++i;

      string_size j = i;
      
      while (j != s.size() && !isspace(s[j]))
        j++;
 
      if (i != j) 
      {
        res.push_back(s.substr(i, j - i));
        i = j;
      }
   }
   return res;
}

class Storage 
{
public:
    List<GameObject> objects;
    
public:
    Storage ()
    {
        objects = List<GameObject>();
    }
    
    // ~Storage ()
    // {
    //     delete objects;
    // }

    GameObject* findByName(std::string name = std::string("player"))
    {
        for (int i = 1;; i++)
        {
            if (objects[i]->_name == name)
                return objects[i];
        }
    }

    void add(GameObject* pObject)    // add layers later
    {
        objects.pushFront(pObject);
    }

    void createObject(std::vector<std::string> args)
    {
        if (args[0] == std::string("enemy"))
        {
            GameObject* pObject = new Enemy (sf::Vector2f(std::stof(args[1]), std::stof(args[2])), std::stof(args[3])
            , sf::Vector2f(std::stof(args[4]), std::stof(args[5])) * cell_side
            , sf::Vector2f(std::stof(args[6]), std::stof(args[7])) * cell_side);
            this->add(pObject);
        }

        else if (args[0] == std::string("enemylinear"))
        {
            GameObject* pObject = new EnemyLinear (sf::Vector2f(std::stof(args[1]), std::stof(args[2])), std::stof(args[3])
            , sf::Vector2f(std::stof(args[4]), std::stof(args[5])) * cell_side
            , sf::Vector2f(std::stof(args[6]), std::stof(args[7])) * cell_side);
            this->add(pObject);
        }

        else if (args[0] == std::string("enemycircular"))
        {
            GameObject* pObject = new EnemyCircular (sf::Vector2f(std::stof(args[1]), std::stof(args[2])), std::stof(args[3])
            , sf::Vector2f(std::stof(args[4]), std::stof(args[5])) * cell_side
            , sf::Vector2f(std::stof(args[6]), std::stof(args[7])) * cell_side);
            this->add(pObject);
        }

        else if (args[0] == std::string("enemysquare"))
        {
            GameObject* pObject = new EnemySquare (sf::Vector2f(std::stof(args[1]), std::stof(args[2])), std::stof(args[3])
            , sf::Vector2f(std::stof(args[4]), std::stof(args[5])) * cell_side
            , sf::Vector2f(std::stof(args[6]), std::stof(args[7])) * cell_side);
            this->add(pObject);
        }

        else if (args[0] == std::string("finish"))
        {
            GameObject* pObject = new FinishTile (sf::Vector2f(std::stof(args[1]), std::stof(args[2])));
            this->add(pObject);
        } 

        else if (args[0] == std::string("standard"))
        {
            GameObject* pObject = new StandardTile (sf::Vector2f(std::stof(args[1]), std::stof(args[2])));
            this->add(pObject);
        } 

        else if (args[0] == std::string("boundary"))
        {
            GameObject* pObject = new BoundaryTile (sf::Vector2f(std::stof(args[1]), std::stof(args[2])));
            this->add(pObject);
        } 

        else if (args[0] == std::string("coin"))
        {
            GameObject* pObject = new Coin (sf::Vector2f(std::stof(args[1]), std::stof(args[2])));
            this->add(pObject);
        } 

        else if (args[0] == std::string("player"))
        {
            GameObject* pObject = new Player (sf::Vector2f(std::stof(args[1]), std::stof(args[2]))
            , sf::Color(stoi(args[3]), stoi(args[4]), stoi(args[5])));
            this->add(pObject);
        } 
        
        else if (args[0] == std::string("start"))
        {
            GameObject* pObject = new StartTile (sf::Vector2f(std::stof(args[1]), std::stof(args[2])));
            this->add(pObject);
        } 
    }

    void load()
    {
        std::string path("saves/");
        std::string filename;
        std::cout << "Enter File Name: ";
        std::cin >> filename;
        
        path += filename;

        std::vector<std::string> buffer;  
        std::ifstream file(path);

        for (std::string line; getline(file, line);)
        {
            buffer = split(line);
            this->createObject(buffer);
        }

        file.close();
    }

    void loadFromFile(std::string path)
    {
        std::vector<std::string> buffer;  
        std::ifstream file(path);
        for (std::string line; getline(file, line);)
        {
            buffer = split(line);
            this->createObject(buffer);
        }

        file.close();

        Player::totalCoins = 0;
        Player::coinsRemaining = 0;

        for (int i = 1; i <= this->objects.len(); i++)
        {
            if (this->objects[i]->_name == std::string("coin"))
            {
                Player::totalCoins ++;
                Player::coinsRemaining ++;
            } 
        }
    }

    void clear()
    {
        objects.clear();
    }
};