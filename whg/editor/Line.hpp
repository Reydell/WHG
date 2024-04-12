#pragma once
#include <SFML/Graphics.hpp>

extern const float cell_side;

class Line
{
public:
    sf::Vector2f _p1, _p2;
    sf::Color _color;

public:
    Line (sf::Vector2f p1, sf::Vector2f p2, sf::Color color = sf::Color(0, 0, 0, 128)):
        _p1(p1),
        _p2(p2),
        _color(color)
    {}

    void draw(sf::RenderWindow& window)
    {
        sf::Vertex entity[] = {sf::Vertex(_p1, _color), sf::Vertex(_p2, _color)};
        window.draw(entity, 2, sf::Lines);
    }
};

void drawLine(sf::Vector2f p1, sf::Vector2f p2, sf::RenderWindow& window)
{
    Line entity(p1, p2);
    entity.draw(window);
}