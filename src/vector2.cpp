#include "../include/vector2.h"
#include <cmath>
#include <sstream>
#include <stdexcept>

Vector2::Vector2() : x(0.0), y(0.0) {}

Vector2::Vector2(double x, double y) : x(x), y(y) {}

std::string Vector2::toString() const {
    std::ostringstream ss;
    ss << "(" << x << ", " << y << ")";
    return ss.str();
}

double Vector2::mod() const {
    return std::sqrt(x * x + y * y);
}

Vector2 Vector2::norm() const {
    double m = mod();
    if (m < 1e-12) {
        throw std::invalid_argument("Normalizzazione di un vettore nullo in Vector2");
    }
    return Vector2(x / m, y / m);
}

Vector2 Vector2::operator+(const Vector2& other) const {
    return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator-(const Vector2& other) const {
    return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator-() const {
    return Vector2(-x, -y);
}

Vector2 Vector2::operator*(double scalar) const {
    return Vector2(x * scalar, y * scalar);
}

Vector2 Vector2::operator/(double scalar) const {
    if (std::abs(scalar) < 1e-12) {
        throw std::invalid_argument("Divisione per zero in Vector2");
    }
    return Vector2(x / scalar, y / scalar);
}

Vector2 operator*(double scalar, const Vector2& v) {
    return v * scalar;
}

Vector2& Vector2::operator+=(const Vector2& other) {
    x += other.x;
    y += other.y;
    return *this;
}

Vector2& Vector2::operator-=(const Vector2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Vector2& Vector2::operator*=(double scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector2& Vector2::operator/=(double scalar) {
    if (std::abs(scalar) < 1e-12) {
        throw std::invalid_argument("Divisione per zero in Vector2");
    }
    x /= scalar;
    y /= scalar;
    return *this;
}

bool Vector2::operator==(const Vector2& other) const {
    return (std::abs(x - other.x) < 1e-9) && (std::abs(y - other.y) < 1e-9);
}

bool Vector2::operator!=(const Vector2& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const Vector2& v) {
    os << v.toString();
    return os;
}

std::istream& operator>>(std::istream& is, Vector2& v) {
    char c = 0;
    if (!(is >> std::ws)) return is;
    if (is.peek() == '(') {
        is >> c; // consume '('
        is >> v.x;
        if (is >> std::ws && is.peek() == ',') {
            is >> c; // consume ','
        }
        is >> v.y;
        if (is >> std::ws && is.peek() == ')') {
            is >> c; // consume ')'
        }
    } else {
        is >> v.x >> v.y;
    }
    return is;
}
