#pragma once

#include <unordered_set>
#include <random>
#include "square.hpp"
#include "tetroid.hpp"

using std::unordered_set;

const int BoardWidth = 10;
const int BoardHeight = 20;

const int BoardWidthPx = (BoardWidth + 1) * SquarePx;
const int BoardHeightPx = (BoardHeight + 1) * SquarePx;

class Board {
private:
    Tetroid tetroid;
    unordered_set<Square, SquareHasher> squares;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> color_dist{ 0, 5 };
    std::uniform_int_distribution<int> shape_dist{ 0, 6 };

public:
    Board(): tetroid(random_color(), random_shape(), 0, BoardWidth / 2) { }

    bool move(sf::Keyboard::Key key) {
        array<Square, 4> next;
        if (key == sf::Keyboard::Key::Left) {
            next = tetroid.get_next_move(0, -1);
        }
        else if (key == sf::Keyboard::Key::Right) {
            next = tetroid.get_next_move(0, 1);
        }
        else if (key == sf::Keyboard::Key::Down) {
            next = tetroid.get_next_move(1, 0);
        }
        else if (key == sf::Keyboard::Key::Up) {
            next = tetroid.get_next_rotate();
        }

        auto bounds = Tetroid::get_bounds(next);

        if (bounds.bottom >= BoardHeight) {
            return make_squares(bounds);
        }
        for (auto&& square : squares) {
            for (auto&& next_square : next) {
                if (square.row == next_square.row && square.col == next_square.col) {
                    return make_squares(bounds);
                }
            }
        }

        if (bounds.left < 0 || bounds.right >= BoardWidth) { return true; }

        if (key == sf::Keyboard::Key::Left) {
            tetroid.move(0, -1);
        }
        else if (key == sf::Keyboard::Key::Right) {
            tetroid.move(0, 1);
        }
        else if (key == sf::Keyboard::Key::Down) {
            tetroid.move(1, 0);
        }
        else if (key == sf::Keyboard::Key::Up) {
            tetroid.rotate();
        }
    }

    Tetroid get_tetroid() { return tetroid; }

    unordered_set<Square, SquareHasher> get_squares() { return squares;  }

private:
    bool make_squares(const Bounds& bounds) {
        if (bounds.top == 0) { return false; }
        for (auto&& square : tetroid.get_squares()) {
            squares.insert(square);
        }

        for (int row = BoardHeight - 1; row > 0; --row) {
            bool full = true;
            for (int col = 0; col < BoardWidth; ++col) {
                if (squares.find(Square(row, col)) == squares.end()) {
                    full = false;
                    break;
                }
            }
            if (full) {
                for (int col = 0; col < BoardWidth; ++col) {
                    squares.erase(Square(row, col));
                }

                unordered_set<Square, SquareHasher> new_squares;
                for (auto&& square : squares) {
                    if (square.row < row) {
                        new_squares.insert(Square(square.row + 1, square.col));
                    }
                    else {
                        new_squares.insert(square);
                    }
                }

                squares = new_squares;
            }
        }

        tetroid = Tetroid(random_color(), random_shape(), 0, BoardWidth / 2);
        return true;
    }

    Shape random_shape() {
        switch (shape_dist(generator)) {
        case 0: return Shape::T;
        case 1: return Shape::L;
        case 2: return Shape::LFlip;
        case 3: return Shape::Line;
        case 4: return Shape::Box;
        case 5: return Shape::S;
        case 6:
        default:
            return Shape::Z;
        }
    }

    sf::Color random_color() {
        switch (color_dist(generator)) {
        case 0: return sf::Color::Red;
        case 1: return sf::Color::Green;
        case 2: return sf::Color::Blue;
        case 3: return sf::Color::Yellow;
        case 4: return sf::Color::Magenta;
        case 5: return sf::Color::Cyan;
        }
    }
};
