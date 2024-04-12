#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.hpp"
#include <string>

class Coin : public GameObject
{
public:
    float _radius; 
    bool _isCollected;

public:
    Coin(sf::Vector2f position) : GameObject (position, sf::Color(209, 200, 67), std::string("coin"), 2)
    {
        _radius = 0.275f * cell_side;
        _isCollected = false;
    }

    void draw(sf::RenderWindow& window)
    {
        if (!_isCollected)
        {
            sf::CircleShape entity(_radius - .0875f * cell_side);
            entity.setPosition(_position - sf::Vector2f(_radius - cell_side * .0875f, _radius - cell_side * .0875f));
            entity.setFillColor(_color);
            entity.setOutlineColor(sf::Color::Black);
            entity.setOutlineThickness(.0875 * cell_side);
            window.draw(entity);
        }
    }
};
