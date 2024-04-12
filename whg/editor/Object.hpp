#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>

extern const float cell_side;

float norm(sf::Vector2f v)
{
    return sqrt(v.x*v.x + v.y*v.y);
}

class Object
{
public:
    std::vector<std::string> _args;
    std::string _name;
    sf::Vector2f _position;

public:
    Object(std::vector<std::string> args):
        _args(args)
    {
        _name = _args[0];
        _position = sf::Vector2f(std::stof(_args[1]), std::stof(_args[2]));
    }

    // maybe add constructor from file

    virtual void draw(sf::RenderWindow& window)
    {
        return;
    }
};

class Player : public Object
{
public:
    Player (std::vector<std::string> args) : Object(args)
    {}

    void draw(sf::RenderWindow& window)
    {
        sf::RectangleShape entity(sf::Vector2f(cell_side * .7f, cell_side * .7f));
        entity.setPosition(_position - sf::Vector2f(cell_side * .35f, cell_side * .35f));
        entity.setFillColor(sf::Color::Red);
        window.draw(entity);
    }
};

class Standard : public Object
{
public:
    Standard (std::vector<std::string> args) : Object(args)
    {}

    void draw(sf::RenderWindow& window)
    {
        sf::RectangleShape entity(sf::Vector2f(cell_side, cell_side));
        entity.setPosition(_position);

        sf::Color col = sf::Color(232, 226, 250);

        if (((int)(_position.x / cell_side) + (int)(_position.y / cell_side)) % 2)
        {
            col = sf::Color(252, 248, 254);
        }

        entity.setFillColor(col);
        window.draw(entity);
    }
};

class Finish : public Object
{
public:
    Finish (std::vector<std::string> args) : Object(args)
    {}

    void draw(sf::RenderWindow& window)
    {
        sf::RectangleShape entity(sf::Vector2f(cell_side, cell_side));
        entity.setPosition(_position);
        entity.setFillColor(sf::Color(195, 249, 179));
        window.draw(entity);
    }
};

class Start : public Object
{
public:
    Start (std::vector<std::string> args) : Object(args)
    {}

    void draw(sf::RenderWindow& window)
    {
        sf::RectangleShape entity(sf::Vector2f(cell_side, cell_side));
        entity.setPosition(_position);
        entity.setFillColor(sf::Color(195, 249, 179));
        window.draw(entity);
    }
};

class Enemy : public Object
{
public:
    Enemy (std::vector<std::string> args) : Object(args)
    {}

    void draw(sf::RenderWindow& window)
    {
        sf::CircleShape entity(cell_side / 4.f);
        entity.setPosition(_position - sf::Vector2f(cell_side / 4.f, cell_side / 4.f));
        entity.setFillColor(sf::Color::Blue);
        window.draw(entity);

        drawLine(sf::Vector2f(cell_side * std::stof(_args[4]), cell_side * std::stof(_args[5]))
        , sf::Vector2f(cell_side * std::stof(_args[6]), cell_side * std::stof(_args[7])), window);
    }
};

class EnemyCircular : public Object
{
public:
    EnemyCircular (std::vector<std::string> args) : Object(args)
    {}

    void draw(sf::RenderWindow& window)
    {
        sf::CircleShape entity(cell_side / 4.f);
        entity.setPosition(_position - sf::Vector2f(cell_side / 4.f, cell_side / 4.f));
        entity.setFillColor(sf::Color::Blue);
        window.draw(entity);

        float contourRadius = cell_side * norm(sf::Vector2f(std::stof(_args[6]), std::stof(_args[7])) 
        - sf::Vector2f(std::stof(_args[4]), std::stof(_args[5])));

        sf::CircleShape contour(contourRadius);
        contour.setFillColor(sf::Color(0, 0, 0, 0));
        contour.setOutlineColor(sf::Color(0, 0, 0, 128));
        contour.setOutlineThickness(2);
        contour.setPosition(cell_side * sf::Vector2f(std::stof(_args[4]), std::stof(_args[5])) - sf::Vector2f(contourRadius, contourRadius));

        window.draw(contour);
    }
};

class EnemySquare : public Object
{
public:
    EnemySquare (std::vector<std::string> args) : Object(args)
    {}

    void draw(sf::RenderWindow& window)
    {
        sf::CircleShape entity(cell_side / 4.f);
        entity.setPosition(_position - sf::Vector2f(cell_side / 4.f, cell_side / 4.f));
        entity.setFillColor(sf::Color::Blue);
        window.draw(entity);

        float contourRadius = cell_side * std::max
        (
            abs
            (
                (sf::Vector2f(std::stof(_args[6]), std::stof(_args[7])) 
                - sf::Vector2f(std::stof(_args[4]), std::stof(_args[5]))).x
            )
            
            , abs
            (
                (sf::Vector2f(std::stof(_args[6]), std::stof(_args[7]))
                - sf::Vector2f(std::stof(_args[4]), std::stof(_args[5]))).y
            )
        );

        sf::RectangleShape contour(sf::Vector2f(2. * contourRadius, 2. * contourRadius));
        contour.setFillColor(sf::Color(0, 0, 0, 0));
        contour.setOutlineColor(sf::Color(0, 0, 0, 128));
        contour.setOutlineThickness(2);
        contour.setPosition(cell_side * sf::Vector2f(std::stof(_args[4]), std::stof(_args[5])) - sf::Vector2f(contourRadius, contourRadius));

        window.draw(contour);
    }
};

class Boundary : public Object
{
public:
    Boundary (std::vector<std::string> args) : Object(args)
    {}

    void draw(sf::RenderWindow& window)
    {
        sf::RectangleShape entity(sf::Vector2f(cell_side, cell_side));
        entity.setPosition(_position);
        entity.setFillColor(sf::Color(191, 169, 249));
        window.draw(entity);
    }
};

class Coin : public Object
{
public:
    Coin (std::vector<std::string> args) : Object(args)
    {}

    void draw(sf::RenderWindow& window)
    {
        sf::CircleShape entity((.1875f + .0875) * cell_side - .0875f * cell_side);
        entity.setPosition((sf::Vector2f(std::stof(_args[1]), std::stof(_args[2]))
         - sf::Vector2f((.1875f + .0875) * cell_side - cell_side * .0875f
        , (.1875f + .0875) * cell_side - cell_side * .0875f)) / 80.f * cell_side);

        entity.setFillColor(sf::Color(209, 200, 67));
        entity.setOutlineColor(sf::Color::Black);
        entity.setOutlineThickness(.0875 * cell_side);
        window.draw(entity);
    }
};