#include <iostream>
#include <vector>
#include "SFML/Graphics.hpp"

constexpr uint16_t gridSizeCols = 400;
constexpr uint16_t gridSizeRows = 400;

constexpr uint16_t screenWidth = 800;
constexpr uint16_t screenHeight = 800;

constexpr uint16_t cellWidth = screenWidth / gridSizeCols;
constexpr uint16_t cellHeight = screenHeight / gridSizeRows;

class Grid
{
public:
    Grid() : cellBuffer1(gridSizeRows, std::vector<uint8_t>(gridSizeCols)),
             cellBuffer2(gridSizeRows, std::vector<uint8_t>(gridSizeCols)),
             curr(&cellBuffer1),
             next(&cellBuffer2){};
    std::vector<std::vector<uint8_t>> cellBuffer1;
    std::vector<std::vector<uint8_t>> cellBuffer2;

    std::vector<std::vector<uint8_t>> *curr;
    std::vector<std::vector<uint8_t>> *next;

    void calcNextState()
    {
        for (size_t row = 0; row < gridSizeRows; row++)
        {
            for (size_t col = 0; col < gridSizeCols; col++)
            {
                // Change next based on current
                (*next)[row][col] = 1;

                /* Rules 
                - if none is directly below: clear the current cell, activate the one below
                - if directly below is blocked: check if left down is free or else if right down is free


                */
            }
        }
    }

    void swapBuffers()
    {
        const auto tmp = curr;
        curr = next;
        next = tmp;
    }
};

int main()
{
    Grid grid;

    std::cout << grid.cellBuffer1.size() << ' ' << grid.cellBuffer1[0].size() << '\n';
    std::cout << +grid.cellBuffer1[17][0] << '\n';

    // Window setup
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Sand Physics with Cellular Automata");
    window.setFramerateLimit(10);

    // Rectangle to visualize cells
    sf::RectangleShape rect({cellWidth, cellHeight});
    rect.setFillColor(sf::Color::White);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Calculate
        grid.calcNextState();
        grid.swapBuffers();

        // Render
        window.clear();

        for (size_t row = 0; row < gridSizeRows; row++)
        {
            for (size_t col = 0; col < gridSizeCols; col++)
            {
                rect.setPosition(col * cellWidth, row * cellHeight);
                if ((*grid.curr)[row][col] == 1)
                    rect.setFillColor(sf::Color::White);
                else
                    rect.setFillColor(sf::Color::Black);
                window.draw(rect);
            }
        }
        window.display();
    }
}