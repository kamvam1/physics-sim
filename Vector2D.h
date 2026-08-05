#include <cmath>

#if !defined(VECTOR2D_H)
#define VECTOR2D_H

class Vector2D {
public:
    // Default Constructor
    Vector2D(float x = 0.0f, float y = 0.0f);

    // Copy Constructor
    Vector2D(const Vector2D& other);

    // Basic Vector Math Operations
    float Dot(const Vector2D& other) const;
    float Magnitude() const;
    Vector2D Normalize() const;
    Vector2D Project(const Vector2D& to_vec) const;


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
    float getX();

    // Returns y component of Vector2D
    float getY();

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