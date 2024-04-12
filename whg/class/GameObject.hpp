#pragma once
#include <SFML/Graphics.hpp>
#include <cmath> ///
#include <string>

extern const float cell_side;

class GameObject
{
public:
    std::string _name;
    sf::Vector2f _position;
    sf::Color _color;    // can be sprite instead
    int _layer;    // small integer

public:
    GameObject ():
        _position(sf::Vector2f()),
        _color(sf::Color::Black),
        _layer(0),
        _name(std::string(""))
    {}

    GameObject (sf::Vector2f position, sf::Color color, std::string name, int layer=0):
        _position(position / 80.f * cell_side),
        _color(color),
        _layer(layer),
        _name(name)
    {}

    virtual void draw(sf::RenderWindow& window)
    {
        return;
    }

    virtual void move(double t)
    {
        return;
    }

    virtual bool checkCollision(GameObject* p)
    {
        return 0;
    }
};