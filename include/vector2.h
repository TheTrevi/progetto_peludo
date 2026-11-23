#ifndef VECTOR2_H
#define VECTOR2_H

#include <iostream>
#include <string>

// Classe per la rappresentazione e l'algebra di vettori/punti 2D (x, y).
class Vector2 {
public:
    double x;
    double y;

    // Costruttori: default all'origine (0, 0) o da coordinate fornite
    Vector2();
    Vector2(double x, double y);

    // Rappresentazione in stringa nel formato "(x, y)"
    std::string toString() const;

    // Norma euclidea sqrt(x^2 + y^2)
    double mod() const;

    // Versore normalizzato (lunghezza 1). Lancia eccezione se vettore nullo
    Vector2 norm() const;

    // Operatori aritmetici vettoriali e scalari
    Vector2 operator+(const Vector2& other) const;
    Vector2 operator-(const Vector2& other) const;
    Vector2 operator-() const; // Negazione unaria (-v)
    Vector2 operator*(double scalar) const;
    Vector2 operator/(double scalar) const;

    // Prodotto scalare * vettore (moltiplicazione a sinistra)
    friend Vector2 operator*(double scalar, const Vector2& v);

    // Operatori di assegnamento composto
    Vector2& operator+=(const Vector2& other);
    Vector2& operator-=(const Vector2& other);
    Vector2& operator*=(double scalar);
    Vector2& operator/=(double scalar);

    // XOutput
    friend std::ostream& operator<<(std::ostream& os, const Vector2& v);
};

std::ostream& operator<<(std::ostream& os, const Vector2& v);

#endif // VECTOR2_H
