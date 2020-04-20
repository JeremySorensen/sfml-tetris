#pragma once

const int SquarePx = 40;
const int HalfSquarePx = SquarePx / 2;
const int Pad = 2;

struct Square {
    int row;
    int col;

    Square() : row(0), col(0) { }
    Square(int row, int col) : row(row), col(col) { }

    bool operator==(const Square& other) const
    {
        return (row == other.row) && (col == other.col);
    }

    void add_to_vertex_array(sf::VertexArray& vertex_array, sf::Color color) const {
        int x_left = col * SquarePx + HalfSquarePx;
        int x_right = x_left + SquarePx;
        int y_top = row * SquarePx + HalfSquarePx;
        int y_bot = y_top + SquarePx;
        vertex_array.append(sf::Vertex(sf::Vector2f(x_left + Pad, y_top + Pad), color));
        vertex_array.append(sf::Vertex(sf::Vector2f(x_right - Pad, y_top + Pad), color));
        vertex_array.append(sf::Vertex(sf::Vector2f(x_right - Pad, y_bot - Pad), color));
        vertex_array.append(sf::Vertex(sf::Vector2f(x_left + Pad, y_bot - Pad), color));

    }
};

class SquareHasher {
public:
    // id is returned as hash function 
    size_t operator()(const Square& square) const
    {
        return square.row << 8 + square.col;
    }
};
