#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cstdint>
#include <vector>


// Classes
class BasicPair;

class Position;

class Vector;

class Rectangle;

class Rectangles;

// Usings
using grid_t = int32_t;
using area_t = uint64_t;
using recs_t = std::vector<Rectangle>;


class BasicPair {
protected:
    grid_t x_val, y_val;

public:
    BasicPair(grid_t x, grid_t y);


    grid_t x() const;

    grid_t y() const;
};

class Position : public BasicPair {
public:
    Position(grid_t x, grid_t y);

    explicit Position(const Vector& v);


    static const Position& origin();

    Position reflection() const;


    bool operator==(const Position& other) const;

    Position& operator+=(const Vector& rhs);


};

class Vector : public BasicPair {
public:
    Vector(grid_t x, grid_t y);

    explicit Vector(const Position& p);


    Vector reflection() const;


    bool operator==(const Vector& other) const;

    Vector& operator+=(const Vector& rhs);
};

class Rectangle {
    Position lower_left;
    grid_t width_val, height_val;

public:
    Rectangle(grid_t width, grid_t height);

    Rectangle(grid_t width, grid_t height, const Position& pos);


    area_t area() const;

    grid_t width() const;

    grid_t height() const;

    Position pos() const;

    Rectangle reflection() const;


    bool operator==(const Rectangle& other) const;

    Rectangle& operator+=(const Vector& rhs);
};

class Rectangles {
    recs_t recs;

public:
    Rectangles() = default;

    Rectangles(std::initializer_list<Rectangle> list);

    Rectangles(const Rectangles& rectangles) = default;

    Rectangles(Rectangles&& rectangles) noexcept;


    size_t size() const;


    bool operator==(const Rectangles& other) const;

    Rectangles& operator+=(const Vector& rhs);

    Rectangle& operator[](size_t i);

    const Rectangle& operator[](size_t i) const;

    Rectangles& operator=(Rectangles&& rectangles) noexcept = default;

    Rectangles& operator=(const Rectangles& rectangles) = default;
};

// Non-member functions
Position operator+(const Position& p, const Vector& v);

Position operator+(const Vector& v, const Position& p);

Vector operator+(const Vector& v1, const Vector& v2);

Rectangle operator+(const Rectangle& rec, const Vector& v);

Rectangle operator+(const Vector& v, const Rectangle& r);

Rectangles operator+(const Vector& v, const Rectangles& recs);

Rectangles operator+(const Vector& v, Rectangles&& recs);

Rectangles operator+(const Rectangles& recs, const Vector& v);

Rectangles operator+(Rectangles&& recs, const Vector& v);

Rectangle merge_horizontally(const Rectangle& r1, const Rectangle& r2);

Rectangle merge_vertically(const Rectangle& r1, const Rectangle& r2);

Rectangle merge_all(const Rectangles& recs);

#endif