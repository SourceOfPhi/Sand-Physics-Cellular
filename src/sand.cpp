#include <iostream>
#include <vector>
#include <random>
#include "SFML/Graphics.hpp"

constexpr uint16_t gridSizeCols = 100;
constexpr uint16_t gridSizeRows = 100;

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
                //(*next)[row][col] = 1;

                /* Rules 
                - if none is directly below: clear the current cell, activate the one below
                - if directly below is blocked: check if left down is free or else if right down is free
                */

                if ((*curr)[row][col] == 0)
                    continue;

                if (row == gridSizeRows - 1)
                {
                    (*next)[row][col] = (*curr)[row][col];
                    continue;
                }

                // Fall if possible
                if ((*curr)[row + 1][col] == 0)
                {
                    (*curr)[row][col] = 0;
                    (*next)[row + 1][col] = 1;
                }
                // Try slide left down
                else if (col > 0 && (*curr)[row + 1][col - 1] == 0)
                {
                    (*curr)[row][col] = 0;
                    (*next)[row + 1][col - 1] = 1;
                }
                // Try slide right down
                else if (col < (gridSizeCols - 1) && (*curr)[row + 1][col + 1] == 0)
                {
                    (*curr)[row][col] = 0;
                    (*next)[row + 1][col + 1] = 1;
                }
                else
                {
                    (*next)[row][col] = (*curr)[row][col];
                }
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

    // Window setup
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Sand Physics with Cellular Automata");
    window.setFramerateLimit(120);

    // Rectangle to visualize cells
    sf::RectangleShape rect({cellWidth, cellHeight});
    rect.setFillColor(sf::Color::White);

    uint64_t frameNum = 0;

    // Main loop
    while (window.isOpen())
    {
        frameNum++;
        if (frameNum % 5 == 0)
        {
            (*grid.curr)[0][rand() % 10 + 45] = 1;
        }

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