#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.hpp"
#include <string>

extern const float cell_side;

class Tile : public GameObject
{
public:
    Tile (sf::Vector2f position, sf::Color color, std::string name) : GameObject (position, color, name, 0)
    {}

    virtual void draw(sf::RenderWindow& window)
    {
        sf::RectangleShape entity(sf::Vector2f(cell_side, cell_side));    // corner coordinate
        entity.setPosition(_position);
        entity.setFillColor(_color);

        window.draw(entity);
    }
};

class FinishTile : public Tile
{
public:
    FinishTile (sf::Vector2f position) : Tile (position, sf::Color(195, 249, 179), std::string("finish"))
    {}
};

class StandardTile : public Tile
{
public:
    StandardTile (sf::Vector2f position) : Tile (position, sf::Color::Black /* /// */, std::string("standard"))
    {
        _color = sf::Color(232, 226, 250);

        if (((int)(_position.x / cell_side) + (int)(_position.y / cell_side)) % 2)
        {
            _color = sf::Color(252, 248, 254);
        }
    }

    void draw(sf::RenderWindow& window)
    {
        sf::RectangleShape entity(sf::Vector2f(cell_side, cell_side));
        entity.setPosition(_position);
        entity.setFillColor(_color);
        window.draw(entity);
    }
};

class StartTile : public Tile
{
public:
    StartTile (sf::Vector2f position) : Tile (position, sf::Color(195, 249, 179), std::string("start"))
    {}
};

class BoundaryTile : public Tile
{
public:
    BoundaryTile (sf::Vector2f position) : Tile (position, sf::Color(152, 153, 203)/*sf::Color(191, 169, 249)*/, std::string("boundary"))
    {}
};