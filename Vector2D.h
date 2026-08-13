#include <SFML/System/Vector2.hpp>

#if !defined(VECTOR2D_H)
#define VECTOR2D_H

class Vector2D {
public:
    // Default Constructor
    Vector2D(float x = 0.0f, float y = 0.0f);

    // Copy Constructor
    // Copies from Vector2D
    Vector2D(const Vector2D& other);

    // Copies from SFML Vector2f
    Vector2D(const sf::Vector2f& other);

    // Basic Vector Math Operations
    float Dot(const Vector2D& other) const;
    float Magnitude() const;
    Vector2D Normalize() const;
    Vector2D Project(const Vector2D& to_vec) const;
    float Dist(const Vector2D& other);

    // Overloaded Operators
    bool operator==(const Vector2D& other) const;
    Vector2D operator+(const Vector2D& other) const;
    Vector2D operator-(const Vector2D& other) const;
    Vector2D operator*(float scalar) const;
    Vector2D& operator=(const Vector2D& other);
    Vector2D& operator+=(const Vector2D& other);
    Vector2D& operator-=(const Vector2D& other);
    Vector2D& operator*=(float scalar);

    // Getters

    // Returns x component of Vector2D
    float getX() const;

    // Returns y component of Vector2D
    float getY() const;

    // Setters

    // Sets the x component of Vector2D 
    // to_x is the new x component
    void setX(float to_x);
    
    // Sets the y component of Vector2D
    // to_y is the new y component
    void setY(float to_y);

private:
    float x;
    float y;  
};

#endif 