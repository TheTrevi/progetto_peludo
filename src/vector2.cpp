#include "../include/vector2.h"
#include <cmath>
#include <sstream>
#include <stdexcept>

// Costruttore default: inizializza a (0, 0)
Vector2::Vector2() : x(0.0), y(0.0) {}

// Costruttore da coordinate
Vector2::Vector2(double x, double y) : x(x), y(y) {}

// Formattazione a stringa "(x, y)"
std::string Vector2::toString() const {
    std::ostringstream ss;
    ss << "(" << x << ", " << y << ")";
    return ss.str();
}

// Norma euclidea
double Vector2::mod() const {
    return std::sqrt(x * x + y * y);
}

// Restituisce il versore associato; solleva eccezione se modulo < 1e-12
Vector2 Vector2::norm() const {
    double m = mod();
    if (m < 1e-12) {
        throw std::invalid_argument("Normalizzazione di un vettore nullo in Vector2");
    }
    return Vector2(x / m, y / m);
}

// Somma tra vettori
Vector2 Vector2::operator+(const Vector2& other) const {
    return Vector2(x + other.x, y + other.y);
}

// Sottrazione tra vettori
Vector2 Vector2::operator-(const Vector2& other) const {
    return Vector2(x - other.x, y - other.y);
}

// Negazione (inverte il verso)
Vector2 Vector2::operator-() const {
    return Vector2(-x, -y);
}

// Moltiplicazione per scalare
Vector2 Vector2::operator*(double scalar) const {
    return Vector2(x * scalar, y * scalar);
}

// Divisione per scalare (solleva eccezione per divisione per zero)
Vector2 Vector2::operator/(double scalar) const {
    if (std::abs(scalar) < 1e-12) {
        throw std::invalid_argument("Divisione per zero in Vector2");
    }
    return Vector2(x / scalar, y / scalar);
}

// Moltiplicazione scalare * vettore
Vector2 operator*(double scalar, const Vector2& v) {
    return v * scalar;
}

// Assegnamento con somma
Vector2& Vector2::operator+=(const Vector2& other) {
    x += other.x;
    y += other.y;
    return *this;
}

// Assegnamento con sottrazione
Vector2& Vector2::operator-=(const Vector2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

// Assegnamento con moltiplicazione scalare
Vector2& Vector2::operator*=(double scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

// Assegnamento con divisione scalare
Vector2& Vector2::operator/=(double scalar) {
    if (std::abs(scalar) < 1e-12) {
        throw std::invalid_argument("Divisione per zero in Vector2");
    }
    x /= scalar;
    y /= scalar;
    return *this;
}


// Output su stream
std::ostream& operator<<(std::ostream& os, const Vector2& v) {
    os << v.toString();
    return os;
}
