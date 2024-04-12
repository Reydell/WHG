#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.hpp"
#include "Enemy.hpp"
#include <cmath>
#include <iostream>
#include <string>
#include "Tile.hpp"
#include "Coin.hpp"

extern const float cell_side;

sf::Vector2f abs(sf::Vector2f v)
{
    return sf::Vector2f(abs(v.x), abs(v.y));
}

// void normalize(sf::Vector2f &v)
// {
//     v = v / sqrt(v.x * v.x + v.y * v.y);
// }

class Player : public GameObject
{
public:
    static bool victory;
    static int deaths;
    static int totalCoins;
    static int coinsRemaining;
    static int checkDeathIncrement;
    static bool checkCollectedCoins;

public:
    double _side;
    sf::Vector2f _size;
    float _speed;
    sf::Vector2f _velocity;
    sf::Vector2f _respawn;    // resp pos
    
public:
    Player (sf::Vector2f position, sf::Color color=sf::Color(195, 53, 29)) : GameObject(position, color, std::string("player"), 1)
    {
        // _side = 60.;
        _side = .525 * cell_side; //celled 625
        _size = sf::Vector2f(_side, _side);
        _speed = .05875f * cell_side; //celled
        _velocity = sf::Vector2f();
        _respawn = _position;

        victory = false;
        checkCollectedCoins = false;
    }

    void draw(sf::RenderWindow& window)
    {
        sf::RectangleShape entity(_size);
        entity.setPosition(_position - _size / 2.f/* - sf::Vector2f(5.f, 5.f)*/);
        entity.setFillColor(_color);
        entity.setOutlineColor(sf::Color::Black);
        entity.setOutlineThickness(.1f * cell_side);

        window.draw(entity);
    }

    void move(double t)    /// get rid of this time
    {
        _velocity = sf::Vector2f();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            _velocity -= sf::Vector2f(_speed, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            _velocity += sf::Vector2f(_speed, 0.f);
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            _velocity += sf::Vector2f(0.f, _speed);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            _velocity -= sf::Vector2f(0.f, _speed);   

        _position += _velocity; 
        
    }

    void kill()
    {
        _position = _respawn;
        deaths ++;
        coinsRemaining = totalCoins;
    }

    bool checkCollision(GameObject* obj)
    {
        if (obj->_name == std::string("enemy") || obj->_name == std::string("enemylinear") || obj->_name == std::string("enemycircular") || obj->_name == std::string("enemysquare"))
        {
            // if (!_isDashing)
            // {
                Enemy* tmpPtr;
                tmpPtr = static_cast<Enemy*>(obj); 

                sf::Vector2f relativePosition = (_position - tmpPtr->_position);
                double distance = sqrt(pow(relativePosition.y, 2) + pow(relativePosition.x, 2));

                if (distance <= tmpPtr->_radius + _side / 2.)
                {
                    this->kill();
                }

                return (distance <= tmpPtr->_radius + _side / 2.);   ///
            // }

            // return false;
        }

        else if (obj->_name == std::string("boundary"))
        {
            BoundaryTile* tmpPtr;
            tmpPtr = static_cast<BoundaryTile*>(obj);

            double criticalDistance = (_side + cell_side) / 2.;

            sf::Vector2f collisionDirection = _position - tmpPtr->_position - sf::Vector2f(cell_side / 2., cell_side / 2.);

            if ((abs(_position.x - tmpPtr->_position.x - cell_side / 2.) < criticalDistance)
             && (abs(_position.y - tmpPtr->_position.y - cell_side / 2.) < criticalDistance))
            {
                if (abs(collisionDirection.y) >= abs(collisionDirection.x))
                {
                    if (collisionDirection.y >= 0.)    // down
                    {
                        _position += sf::Vector2f(0.f, _speed);
                    }

                    else if (collisionDirection.y < 0.)    // up
                    {
                        _position -= sf::Vector2f(0.f, _speed);
                    }
                }

                else if (abs(collisionDirection.y) < abs(collisionDirection.x))
                {
                    if (collisionDirection.x >= 0.)    // right
                    {
                        _position += sf::Vector2f(_speed, 0.f);
                    }

                    else if (collisionDirection.x < 0.)    // left
                    {
                        _position -= sf::Vector2f(_speed, 0.f);
                    }
                }
            }

            // if ((abs(_position.x - tmpPtr->_position.x - cell_side / 2.) <= criticalDistance)
            //  && (abs(_position.y - tmpPtr->_position.y - cell_side / 2.) <= criticalDistance))
            // {
            //     if (abs(_position.x - tmpPtr->_position.x - cell_side / 2.) <= criticalDistance)
            //         _position -= _velocity;
            //     if (abs(_position.y - tmpPtr->_position.y - cell_side / 2.) <= criticalDistance)
            //         _position.y -= _velocity.y;

            // }

            return ((abs(_position.x - tmpPtr->_position.x - cell_side / 2.) <= criticalDistance)
             && (abs(_position.y - tmpPtr->_position.y - cell_side / 2.) <= criticalDistance));
        }

        else if (obj->_name == std::string("finish"))
        {
            if (coinsRemaining == 0)
            {
                FinishTile* tmpPtr;
                tmpPtr = static_cast<FinishTile*>(obj);

                double criticalDistance = (_side + cell_side) / 2.;

                if ((abs(_position.x - tmpPtr->_position.x - cell_side / 2.) <= criticalDistance) 
                && (abs(_position.y - tmpPtr->_position.y - cell_side / 2.) <= criticalDistance))
                {
                    victory = true;
                }
                return ((abs(_position.x - tmpPtr->_position.x - cell_side / 2.) <= criticalDistance) 
                && (abs(_position.y - tmpPtr->_position.y - cell_side / 2.) <= criticalDistance));
            }

            return 0;
        }

        else if (obj->_name == std::string("coin"))
        {
            Coin* tmpPtr;
            tmpPtr = static_cast<Coin*>(obj); 

            if (!tmpPtr->_isCollected)
            {
                sf::Vector2f relativePosition = (_position - tmpPtr->_position);
                double distance = sqrt(pow(relativePosition.y, 2) + pow(relativePosition.x, 2));

                if (distance <= tmpPtr->_radius + _side / 2.)
                {
                    tmpPtr->_isCollected = true; 
                    coinsRemaining --;
                    checkCollectedCoins = true;
                }

                return (distance <= tmpPtr->_radius + _side / 2.);   ///
            }

            return 0;
        }
        

        else
            return 0;
    }
};