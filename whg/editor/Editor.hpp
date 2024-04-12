#pragma once
#include <SFML/Graphics.hpp>
#include "Line.hpp"
#include <vector>
#include <string>
#include "Object.hpp"
#include <fstream>
#include <iostream>
#include <cmath>

extern const float cell_side;
extern const float half_cell;

float smartRound(float f)
{
    return cell_side / 2 * round(f / cell_side * 2);
}

std::string vector_to_line(std::vector<std::string> vector, bool add_new_line=true)
{
    std::string res("");

    for (std::string s : vector)
    {
        res += s;
        res += std::string(" ");
    }

    if (add_new_line)
        res.replace(res.length() - 1, 1, "\n");

    return res;
}

class Editor
{
public:
    sf::Font font;
    sf::Vector2f mousePosition;
    std::vector<std::string> objectTypes;
    int currentClickType;
    sf::RenderWindow& editorWindow;
    sf::Event& editorEvent;
    bool wasMouseButtonReleased;
    // bool waitingForInput;  ///
    std::vector<std::string> objectBuffer;  ///
    sf::Vector2f prevMousePosition;  ///
    std::vector<Object*> storage;

public:
    Editor(sf::RenderWindow& window, sf::Event& event):
        editorWindow(window),
        editorEvent(event),
        wasMouseButtonReleased(true)
        //waitingForInput(false)
    {   
        font.loadFromFile("../resources/font.ttf");

        mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(editorWindow));
        prevMousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(editorWindow));  ///

        objectTypes.push_back(std::string("standard"));
        objectTypes.push_back(std::string("boundary"));
        objectTypes.push_back(std::string("start"));
        objectTypes.push_back(std::string("finish"));
        objectTypes.push_back(std::string("player"));
        // objectTypes.push_back(std::string("enemy"));
        //objectTypes.push_back(std::string("waiting"));
        objectTypes.push_back(std::string("Ienemy"));
        objectTypes.push_back(std::string("enemyI"));

        objectTypes.push_back(std::string("Renemy"));
        objectTypes.push_back(std::string("enemyR"));

        objectTypes.push_back(std::string("Senemy"));
        objectTypes.push_back(std::string("enemyS"));

        objectTypes.push_back(std::string("coin"));

        currentClickType = 0;
    }

    void fillGrid()
    {
        for (float y = 1.f; y < 22.f; y++)
        {
            drawLine(sf::Vector2f(0.f, y * cell_side), sf::Vector2f(cell_side * 32.f, y * cell_side), editorWindow);
        }

        drawLine(sf::Vector2f(0.f, 11.f * cell_side + 1.f), sf::Vector2f(cell_side * 32.f, 11.f * cell_side + 1.f), editorWindow);
        drawLine(sf::Vector2f(0.f, 11.f * cell_side - 1.f), sf::Vector2f(cell_side * 32.f, 11.f * cell_side - 1.f), editorWindow);

        for (float x = 1.f; x < 33.f; x++)
        {
            drawLine(sf::Vector2f(x * cell_side, 0.f), sf::Vector2f(x * cell_side, cell_side * 22.f), editorWindow);
        }

        drawLine(sf::Vector2f(16.f * cell_side + 1.f, 0.f), sf::Vector2f(16.f * cell_side + 1.f, cell_side * 22.f), editorWindow);
        drawLine(sf::Vector2f(16.f * cell_side - 1.f, 0.f), sf::Vector2f(16.f * cell_side - 1.f, cell_side * 22.f), editorWindow);

        sf::RectangleShape square(sf::Vector2f(cell_side, cell_side));
        square.setPosition(sf::Vector2f(cell_side * 33.f, cell_side));
        square.setFillColor(sf::Color(0, 0, 0, 0));
        square.setOutlineColor(sf::Color(0, 0, 0, 128));
        square.setOutlineThickness(1);
        
        editorWindow.draw(square);

        sf::Text clickTypeCaption;
        clickTypeCaption.setString(sf::String(objectTypes[currentClickType].data()));
        clickTypeCaption.setFont(font);
        clickTypeCaption.setFillColor(sf::Color(0, 0, 0, 50));
        clickTypeCaption.setScale(cell_side / 40.f, cell_side / 40.f);
        clickTypeCaption.setPosition(cell_side * 32.3f, 0.f);
        editorWindow.draw(clickTypeCaption);
    }

    void drawTrajectories()
    {
        if (currentClickType == 6)
        {
            drawLine(prevMousePosition, mousePosition, editorWindow);
        }

        else if (currentClickType == 8)
        {
            float contourRadius = norm(mousePosition - prevMousePosition);

            sf::CircleShape contour(contourRadius);
            contour.setFillColor(sf::Color(0, 0, 0, 0));
            contour.setOutlineColor(sf::Color(0, 0, 0, 128));
            contour.setOutlineThickness(2);
            contour.setPosition(sf::Vector2f(prevMousePosition - sf::Vector2f(contourRadius, contourRadius)));

            editorWindow.draw(contour);
        }

        else if (currentClickType == 10)
        {
            float contourRadius = std::max(abs((mousePosition - prevMousePosition).x), abs((mousePosition - prevMousePosition).y));

            sf::RectangleShape contour(sf::Vector2f(2. * contourRadius, 2. * contourRadius));
            contour.setFillColor(sf::Color(0, 0, 0, 0));
            contour.setOutlineColor(sf::Color(0, 0, 0, 128));
            contour.setOutlineThickness(2);
            contour.setPosition(sf::Vector2f(prevMousePosition - sf::Vector2f(contourRadius, contourRadius)));

            editorWindow.draw(contour);
        }
    }

    void highlight(sf::Color color = sf::Color(30, 200, 30, 40))
    {
        mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(editorWindow));

        if (mousePosition.x > cell_side * 32.f)
            return;

        float X = cell_side * floor(mousePosition.x / cell_side);
        float Y = cell_side * floor(mousePosition.y / cell_side);
        
        sf::RectangleShape entity(sf::Vector2f(cell_side, cell_side));
        entity.setPosition(sf::Vector2f(X, Y));
        entity.setFillColor(color);

        editorWindow.draw(entity);
    }

    void createObject()
    {
        Object* tmp = nullptr;

        int speed = 2;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            speed = 1;
        }

        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            speed = 3;
        }

        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            speed = 4;
        }
        
        if (currentClickType == 0)
        {
            std::vector<std::string> args;
            args.push_back(objectTypes[currentClickType]);
            args.push_back(std::to_string(cell_side * floor(mousePosition.x / cell_side)));
            args.push_back(std::to_string(cell_side * floor(mousePosition.y / cell_side)));
            
            tmp = new Standard(args);
        }

        else if (currentClickType == 1)
        {
            std::vector<std::string> args;
            args.push_back(objectTypes[currentClickType]);
            args.push_back(std::to_string(cell_side * floor(mousePosition.x / cell_side)));
            args.push_back(std::to_string(cell_side * floor(mousePosition.y / cell_side)));
            
            tmp = new Boundary(args);
        }

        else if (currentClickType == 2)
        {
            std::vector<std::string> args;
            args.push_back(objectTypes[currentClickType]);
            args.push_back(std::to_string(cell_side * floor(mousePosition.x / cell_side)));
            args.push_back(std::to_string(cell_side * floor(mousePosition.y / cell_side)));
            
            tmp = new Start(args);
        }

        else if (currentClickType == 3)
        {
            std::vector<std::string> args;
            args.push_back(objectTypes[currentClickType]);
            args.push_back(std::to_string(cell_side * floor(mousePosition.x / cell_side)));
            args.push_back(std::to_string(cell_side * floor(mousePosition.y / cell_side)));
            
            tmp = new Finish(args);
        }

        else if (currentClickType == 4)
        {
            std::vector<std::string> args;
            args.push_back(objectTypes[currentClickType]);
            args.push_back(std::to_string(mousePosition.x));
            args.push_back(std::to_string(mousePosition.y));
            args.push_back(std::to_string(195));
            args.push_back(std::to_string(53));
            args.push_back(std::to_string(29));
            
            tmp = new Player(args);
        }

        else if (currentClickType == 5)
        {
            objectBuffer.push_back(std::string("enemylinear"));
            objectBuffer.push_back(std::to_string(smartRound(mousePosition.x)));
            objectBuffer.push_back(std::to_string(smartRound(mousePosition.y)));
            objectBuffer.push_back(std::to_string(speed));
            objectBuffer.push_back(std::to_string(smartRound(mousePosition.x) / cell_side));
            objectBuffer.push_back(std::to_string(smartRound(mousePosition.y) / cell_side));
            prevMousePosition = mousePosition;
            currentClickType = 6;
            return;
        }

        else if (currentClickType == 6)
        {
            objectBuffer.push_back(std::to_string(smartRound(mousePosition.x) / cell_side));
            objectBuffer.push_back(std::to_string(smartRound(mousePosition.y) / cell_side));
            
            tmp = new Enemy(objectBuffer);
            objectBuffer.clear();
            currentClickType = 5;
        }

        else if (currentClickType == 7)
        {
            objectBuffer.push_back(std::string("enemycircular"));
            objectBuffer.push_back(std::to_string(smartRound(mousePosition.x)));
            objectBuffer.push_back(std::to_string(smartRound(mousePosition.y)));
            objectBuffer.push_back(std::to_string(speed));
            objectBuffer.push_back(std::to_string(smartRound(mousePosition.x)/ cell_side));
            objectBuffer.push_back(std::to_string(smartRound(mousePosition.y) / cell_side));
            prevMousePosition = mousePosition;
            currentClickType = 8;
            return;
        }

        else if (currentClickType == 8)
        {
            objectBuffer.push_back(std::to_string(smartRound(mousePosition.x) / cell_side));
            objectBuffer.push_back(std::to_string(smartRound(mousePosition.y) / cell_side));
            
            tmp = new EnemyCircular(objectBuffer);
            objectBuffer.clear();
            currentClickType = 7;
        }

        else if (currentClickType == 9)
        {
            objectBuffer.push_back(std::string("enemysquare"));
            objectBuffer.push_back(std::to_string(smartRound(mousePosition.x)));
            objectBuffer.push_back(std::to_string(smartRound(mousePosition.y)));
            objectBuffer.push_back(std::to_string(speed));
            objectBuffer.push_back(std::to_string(smartRound(mousePosition.x)/ cell_side));
            objectBuffer.push_back(std::to_string(smartRound(mousePosition.y) / cell_side));
            prevMousePosition = mousePosition;
            currentClickType = 10;
            return;
        }

        else if (currentClickType == 10)
        {
            objectBuffer.push_back(std::to_string(smartRound(mousePosition.x) / cell_side));
            objectBuffer.push_back(std::to_string(smartRound(mousePosition.y) / cell_side));
            
            tmp = new EnemySquare(objectBuffer);
            objectBuffer.clear();
            currentClickType = 9;
        }

        else if (currentClickType == 11)
        {
            std::vector<std::string> args;
            args.push_back(objectTypes[currentClickType]);
            args.push_back(std::to_string(smartRound(mousePosition.x) / cell_side * 80.f));
            args.push_back(std::to_string(smartRound(mousePosition.y) / cell_side * 80.f));
            
            tmp = new Coin(args);
        }

        storage.push_back(tmp);
    }

    void processClick()
    {
        if (editorEvent.type == sf::Event::MouseButtonReleased && !wasMouseButtonReleased)
            wasMouseButtonReleased = true;

        if (editorEvent.type == sf::Event::MouseButtonPressed && wasMouseButtonReleased)
        {
            wasMouseButtonReleased = false;

            if (editorEvent.mouseButton.button == sf::Mouse::Right)
            {
                currentClickType = (currentClickType + 1) % (objectTypes.size());
                if (currentClickType == 6 || currentClickType == 8 || currentClickType == 10)
                    currentClickType = (currentClickType + 1) % (objectTypes.size());
            }

            else if (editorEvent.mouseButton.button == sf::Mouse::Left)
            {
                this->createObject();
            }
        }
    }

    void save()
    {
        std::string path("../saves/");
        
        std::string filename;
        std::cout << "Enter File Name: ";
        std::cin >> filename;
        path += filename;

        std::fstream file(path, /*std::ios_base::out | */std::ios_base::out);   ///

        for (int i = storage.size() - 1; i > 0; i-=1)
        {
            std::string line = vector_to_line(storage[i]->_args);
            file << line;
        }

        std::string line = vector_to_line(storage[0]->_args, false);
        file << line;

        file.close();
    }

    void displayObjects()
    {
        for (Object* ptr : storage)
        {
            ptr->draw(editorWindow);
        }

        // if (waitingForInput)
        // {
        //     drawLine(prevMousePosition, mousePosition, editorWindow);
        // }
    }

    void update()
    {   
        this->processClick();
        this->fillGrid();
        this->highlight();
        this->displayObjects();
        this->drawTrajectories();
    }    
};