#include <cassert>
#include "geometry.h"

// BasicPair::
BasicPair::BasicPair(grid_t x, grid_t y) {
    x_val = x;
    y_val = y;
}

grid_t BasicPair::x() const {
    return x_val;
}

grid_t BasicPair::y() const {
    return y_val;
}


//Position::
Position::Position(grid_t x, grid_t y) : BasicPair(x, y) {}

Position::Position(const Vector& v) : BasicPair(v.x(), v.y()) {}

Position Position::reflection() const {
    return { y_val, x_val };
}

const Position& Position::origin() {
    static const Position origin = Position(0, 0);
    return origin;
}

bool Position::operator==(const Position& other) const {
    return x_val == other.x_val && y_val == other.y_val;
}

Position& Position::operator+=(const Vector& rhs) {
    x_val += rhs.x();
    y_val += rhs.y();
    return *this;
}

//Vector::
Vector::Vector(grid_t x, grid_t y)
    : BasicPair(x, y) {}

Vector::Vector(const Position& p)
    : BasicPair(p.x(), p.y()) {}

Vector Vector::reflection() const {
    return { y_val, x_val };
}

bool Vector::operator==(const Vector& other) const {
    return x_val == other.x_val && y_val == other.y_val;
}

Vector& Vector::operator+=(const Vector& rhs) {
    x_val += rhs.x_val;
    y_val += rhs.y_val;

    return *this;
}

//Rectangle::
Rectangle::Rectangle(grid_t width, grid_t height)
    : lower_left(0, 0) {
    assert(width > 0 && height > 0);
    width_val = width;
    height_val = height;
}

Rectangle::Rectangle(grid_t width, grid_t height, const Position& pos)
    : lower_left(pos) {
    assert(width > 0 && height > 0);
    width_val = width;
    height_val = height;
}

grid_t Rectangle::width() const {
    return width_val;
}

grid_t Rectangle::height() const {
    return height_val;
}

Position Rectangle::pos() const {
    return lower_left;
}

area_t Rectangle::area() const {
    return width_val * height_val;
}

Rectangle Rectangle::reflection() const {
    return { height_val, width_val, lower_left.reflection() };
}

bool Rectangle::operator==(const Rectangle& other) const {
    return width_val == other.width_val &&
        height_val == other.height_val &&
        lower_left == other.lower_left;
}

Rectangle& Rectangle::operator+=(const Vector& rhs) {
    lower_left += rhs;
    return *this;
}


//Rectangles::
Rectangles::Rectangles(std::initializer_list<Rectangle> list)
    : recs{ list } {}

Rectangles::Rectangles(Rectangles&& rectangles) noexcept
    : recs(std::move(rectangles.recs)) {}

size_t Rectangles::size() const {
    return recs.size();
}

Rectangle& Rectangles::operator[](size_t i) {
    return recs[i];
}

const Rectangle& Rectangles::operator[](size_t i) const {
    return recs[i];
}

bool Rectangles::operator==(const Rectangles& other) const {
    if (recs.size() != other.size())
        return false;
    for (size_t i = 0; i < recs.size(); i++) {
        if (!(recs[i] == other.recs[i]))
            return false;
    }
    return true;
}

Rectangles& Rectangles::operator+=(const Vector& rhs) {
    for (Rectangle& r : recs)
        r += rhs;
    return *this;
}

//Non-member functions
Position operator+(const Position& p, const Vector& v) {
    Position result(p);
    result += v;
    return result;
}

Position operator+(const Vector& v, const Position& p) { return p + v; }

Vector operator+(const Vector& v1, const Vector& v2) {
    Vector result(v1);
    result += v2;
    return result;
}

Rectangle operator+(const Rectangle& r, const Vector& v) {
    Rectangle result(r);
    result += v;
    return result;
}

Rectangle operator+(const Vector& v, const Rectangle& r) {
    return r + v;
}

Rectangles operator+(const Vector& v, const Rectangles& recs) {
    Rectangles result(recs);
    result += v;
    return result;
}

Rectangles operator+(const Vector& v, Rectangles&& recs) {
    Rectangles result(std::move(recs));
    result += v;
    return result;
}

Rectangles operator+(const Rectangles& recs, const Vector& v) {
    return v + recs;
}

Rectangles operator+(Rectangles&& recs, const Vector& v) {
    return v + std::move(recs);
}

namespace {

    bool can_merge_horizontally(const Rectangle& r1, const Rectangle& r2) {
        return (r1.width() == r2.width() &&
            r1.pos() + Vector(0, r1.height()) == r2.pos());
    }

    bool can_merge_vertically(const Rectangle& r1, const Rectangle& r2) {
        return (r1.height() == r2.height() &&
            r1.pos() + Vector(r1.width(), 0) == r2.pos());
    }

    Rectangle merge(const Rectangle& r1, const Rectangle& r2) {
        if (can_merge_vertically(r1, r2))
            return merge_vertically(r1, r2);
        if (can_merge_horizontally(r1, r2))
            return merge_horizontally(r1, r2);
        assert(false);
    }
}

Rectangle merge_horizontally(const Rectangle& r1, const Rectangle& r2) {
    assert(can_merge_horizontally(r1, r2));
    return { r1.width(), r1.height() + r2.height(), r1.pos() };
}

Rectangle merge_vertically(const Rectangle& r1, const Rectangle& r2) {
    assert(can_merge_vertically(r1, r2));
    return { r1.width() + r2.width(), r1.height(), r1.pos() };
}

Rectangle merge_all(const Rectangles& recs) {
    assert(recs.size() > 0);
    Rectangle result(recs[0]);
    for (size_t i = 1; i < recs.size(); i++)
        result = merge(result, recs[i]);
    return result;
}
