#ifndef VECTOR2_H
#define VECTOR2_H

#include <iostream>
#include <string>

class Vector2 {
public:
    double x;
    double y;

    // Costruttori
    Vector2();
    Vector2(double x, double y);

    std::string toString() const;

    double mod() const;
    Vector2 norm() const;

    // Operatori aritmetici
    Vector2 operator+(const Vector2& other) const;
    Vector2 operator-(const Vector2& other) const;
    Vector2 operator-() const; // Negazione unaria
    Vector2 operator*(double scalar) const;
    Vector2 operator/(double scalar) const;

    friend Vector2 operator*(double scalar, const Vector2& v);

    // Operatori di assegnamento composto
    Vector2& operator+=(const Vector2& other);
    Vector2& operator-=(const Vector2& other);
    Vector2& operator*=(double scalar);
    Vector2& operator/=(double scalar);

    // Operatori di confronto
    bool operator==(const Vector2& other) const;
    bool operator!=(const Vector2& other) const;

    // Operatori di I/O stream
    friend std::ostream& operator<<(std::ostream& os, const Vector2& v);
    friend std::istream& operator>>(std::istream& is, Vector2& v);
};

std::ostream& operator<<(std::ostream& os, const Vector2& v);
std::istream& operator>>(std::istream& is, Vector2& v);

#endif // VECTOR2_H
