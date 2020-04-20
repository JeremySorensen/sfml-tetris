#include <SFML/Graphics.hpp>
#include <unordered_set>
#include <array>

#include "board.hpp"

using std::vector;
using std::array;

sf::VertexArray render(const Tetroid& tetroid, const unordered_set<Square, SquareHasher>& squares) {
    sf::VertexArray vertex_array(sf::Quads);
    for (auto&& square : tetroid.get_squares()) {
        square.add_to_vertex_array(vertex_array, tetroid.get_color());
    }
    for (auto&& square : squares) {
        square.add_to_vertex_array(vertex_array, sf::Color::White);
    }
    return vertex_array;
}

int main()
{
    Board board;
    sf::RenderWindow window(sf::VideoMode(BoardWidthPx, BoardHeightPx), "SFML Tetris");

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                board.move(event.key.code);
            }
        }

        sf::Time elapsed = clock.getElapsedTime();
        if (elapsed.asMilliseconds() > 250) {
            clock.restart();
            board.move(sf::Keyboard::Key::Down);
        }

        auto tetroid = board.get_tetroid();
        auto squares = board.get_squares();

        sf::RectangleShape background(sf::Vector2f(BoardWidthPx - SquarePx, BoardHeightPx - SquarePx));
        background.setPosition(sf::Vector2f(HalfSquarePx, HalfSquarePx));
        background.setFillColor(sf::Color(0x80, 0x80, 0x080));

        window.clear();
        window.draw(background);
        window.draw(render(tetroid, squares));
        window.display();
    }

    return 0;
}