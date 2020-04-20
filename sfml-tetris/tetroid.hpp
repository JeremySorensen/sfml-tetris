#pragma once

#include <array>
#include <memory>
#include <limits>
#include <SFML/Graphics.hpp>
#include "square.hpp"

using std::array;

const int BIG = std::numeric_limits<int>::max();

struct Offsets {
	int row_0;
	int col_0;
	int row_1;
	int col_1;
	int row_2;
	int col_2;
	int row_3;
	int col_3;

	Offsets(int row_0, int col_0, int row_1, int col_1, int row_2, int col_2, int row_3, int col_3) :
		row_0(row_0), col_0(col_0), row_1(row_1), col_1(col_1), row_2(row_2), col_2(col_2), row_3(row_3), col_3(col_3) { }
};

struct Bounds {
    int left;
    int right;
    int top;
    int bottom;

    Bounds(): left(BIG), right(-BIG), top(BIG), bottom(-BIG) { }
};

enum class Shape {
    T, L, LFlip, Line, Box, S, Z
};

class Tetroid {
private:
    array<Square, 4> squares;
    sf::Color color;
    Shape shape;
    int rotation;

public:
    Tetroid(sf::Color color, Shape shape, int row, int col) :
        squares(setup(shape, row, col)), color(color), shape(shape), rotation(0) { }

    void move(int row_off, int col_off) {
        for (auto&& square : squares) {
            square.row += row_off;
            square.col += col_off;
        }
    }

    array<Square, 4> get_next_move(int row_off, int col_off) {
        auto squares = array<Square, 4>(this->squares);
        for (auto&& square : squares) {
            square.row += row_off;
            square.col += col_off;
        }
        return squares;
    }

    void rotate() {
        apply_offsets(squares, get_offsets());
        ++rotation;
        if (rotation > 3) { rotation = 0; }
        
    }

    array<Square, 4> get_next_rotate() {
        auto squares = array<Square, 4>(this->squares);
        apply_offsets(squares, get_offsets());
        return squares;
    }

    array<Square, 4> get_squares() const {
        return squares;
    }

    sf::Color get_color() const { return color; }

    static Bounds get_bounds(array<Square, 4> squares) {
        Bounds bounds;
        for (auto&& square : squares) {
            if (square.row < bounds.top) { bounds.top = square.row; }
            if (square.row > bounds.bottom) { bounds.bottom = square.row; }
            if (square.col < bounds.left) { bounds.left = square.col; }
            if (square.col > bounds.right) { bounds.right = square.col; }
        }
        return bounds;
    }

    Bounds get_bounds() {
        return get_bounds(squares);
    }

private:
    void apply_offsets(array<Square, 4>& squares, const Offsets& offsets) {
        squares[0].row += offsets.row_0;
        squares[0].col += offsets.col_0;
        squares[1].row += offsets.row_1;
        squares[1].col += offsets.col_1;
        squares[2].row += offsets.row_2;
        squares[2].col += offsets.col_2;
        squares[3].row += offsets.row_3;
        squares[3].col += offsets.col_3;
    }

    Offsets get_offsets() {
        switch (shape) {
        case Shape::T: return get_offsets_t();
        case Shape::L: return get_offsets_l();
        case Shape::LFlip: return get_offsets_l_flip();
        case Shape::Line: return get_offsets_line();
        case Shape::Box: return get_offsets_box();
        case Shape::S: return get_offsets_s();
        case Shape::Z:
        default:
            return get_offsets_z();
        }
    }

    static array<Square, 4> setup(Shape shape, int row, int col) {
        switch (shape) {
        case Shape::T: return setup_t(row, col);
        case Shape::L: return setup_l(row, col);
        case Shape::LFlip: return setup_l_flip(row, col);
        case Shape::Line: return setup_line(row, col);
        case Shape::Box: return setup_box(row, col);
        case Shape::S: return setup_s(row, col);
        case Shape::Z:
        default:
            return setup_z(row, col);
        }
    }

    Offsets get_offsets_t() {
        if (rotation == 0) {
            return Offsets(-1, 1, 0, 0, 1, -1, -1, -1);
        }
        else if (rotation == 1) {
            return Offsets(1, 1, 0, 0, -1, -1, -1, 1);
        }
        else if (rotation == 2) {
            return Offsets(1, -1, 0, 0, -1, 1, 1, 1);
        }
        else {
            return Offsets(-1, -1, 0, 0, 1, 1, 1, -1);
        }
    }

    static array<Square, 4> setup_t(int row, int col) {
        return array<Square, 4> {
            Square(row, col),
            Square(row, col + 1),
            Square(row, col + 2),
            Square(row + 1, col + 1)
        };
    }

    Offsets get_offsets_l() {
        if (rotation == 0) {
            return Offsets(0, 2, -1, 1, -2, 0, -1, -1);
        }
        else if (rotation == 1) {
            return Offsets(2, 0, 1, 1, 0, 2, -1, 1);
        }
        else if (rotation == 2) {
            return Offsets(0,-2, 1, -1, 2, 0, 1, 1);
        }
        else {
            return Offsets(-2, 0, -1, -1, 0, -2, 1, -1);
        }
    }

    static array<Square, 4> setup_l(int row, int col) {
        return array<Square, 4> { Square(row, col), Square(row + 1, col), Square(row + 2, col), Square(row + 2, col + 1) };
    }

    Offsets get_offsets_l_flip() {
        if (rotation == 0) {
            return Offsets(2, 0, 1, -1, 0, -2, -1, -1);
        }
        else if (rotation == 1) {
            return Offsets(0, -2, -1, -1, -2, 0, -1, 1);
        }
        else if (rotation == 2) {
            return Offsets(-2, 0, -1, 1, 0, 2, 1, 1);
        }
        else {
            return Offsets(0, 2, 1, 1, 2, 0, 1, -1);
        }
    }

    static array<Square, 4> setup_l_flip(int row, int col) {
        return array<Square, 4> { Square(row, col + 1), Square(row + 1, col + 1), Square(row + 2, col + 1), Square(row + 2, col) };
    }

    Offsets get_offsets_line() {
        if (rotation == 0 || rotation == 2) {
            return Offsets(-1, 1, 0, 0, 1, -1, 2, -2);
        }
        else {
            return Offsets(1, -1, 0, 0, -1, 1, -2, 2);
        }
    }

    static array<Square, 4> setup_line(int row, int col) {
        return array<Square, 4> { Square(row, col), Square(row, col + 1), Square(row, col + 2), Square(row, col + 3) };
    }

    Offsets get_offsets_box() {
        return Offsets(0, 0, 0, 0, 0, 0, 0, 0);
    }

    static array<Square, 4> setup_box(int row, int col) {
        return array<Square, 4> { Square(row, col), Square(row, col  + 1), Square(row + 1, col), Square(row + 1, col + 1) };
    }

    Offsets get_offsets_s() {
        if (rotation == 0 || rotation == 2) {
            return Offsets(0, 0, 0, 0, -2, 0, 0, 2);
        }
        else {
            return Offsets(0, 0, 0, 0, 2, 0, 0, -2);
        }
    }

    static array<Square, 4> setup_s(int row, int col) {
        return array<Square, 4> { Square(row, col), Square(row, col + 1), Square(row + 1, col), Square(row + 1, col - 1) };
    }

    Offsets get_offsets_z() {
        if (rotation == 0 || rotation == 2) {
            return Offsets(-1, 2, 0, 0, 0, 0, -1, 0);
        }
        else {
            return Offsets(1, -2, 0, 0, 0, 0, 1, 0);
        }
    }

    static array<Square, 4> setup_z(int row, int col) {
        return array<Square, 4> { Square(row, col - 1), Square(row, col), Square(row + 1, col), Square(row + 1, col + 1) };
    }
};


