#include "Vector2D.h"
#include <cmath>

float EPSILON = 1e-4;

// ------- Overloaded Constructors --------

Vector2D::Vector2D(float x = 0.0, float y = 0.0)
{
    this->x = x;
    this->y = y;
}

Vector2D::Vector2D(const Vector2D& other)
{
    this->x = other.x;
    this->y = other.y;
}

// ------- Overloaded Operators --------

// Linear scaling of a Vector
Vector2D Vector2D::operator*(float scalar) const
{
    return Vector2D(this->x * scalar, this->y * scalar);
}


// Checks if *this and other are equal via tolerance = EPSILON (1e-4). 
bool Vector2D::operator==(const Vector2D& other) const
{
    if (this == &other)
    {
        return true;
    }
    float x_tol = abs(this->x - other.x);
    float y_tol = abs(this->y - other.y);
    if (x_tol <= EPSILON && y_tol <= EPSILON)
    {
        return true;
    }
    else 
    {
        return false;
    }
}

// Addition of 2 vectors
Vector2D Vector2D::operator+(const Vector2D& other) const
{
    return Vector2D(this->x + other.x, this->y + other.y);
}
    
// Subtraction of 2 vectors
Vector2D Vector2D::operator-(const Vector2D& other) const
{
    return Vector2D(this->x - other.x, this->y - other.y);
}

// ------- Basic Math Operations --------

// Squared Norm of *this with other.
float Vector2D::Dot(const Vector2D& other) const
{
    return this->x * other.x + this->y * other.y;
}

// Euclidean Norm of *this
float Vector2D::Magnitude() const
{
    return sqrt(pow(this->x, 2.f) + pow(this->y, 2.f));
}

// Transforms a *this to a unit vector
Vector2D Vector2D::Normalize() const
{
    float mag = this->Magnitude();
    
    if (mag < EPSILON)
    {
        return Vector2D();
    }

    return Vector2D(this->x / mag, this->y / mag);
}

// Projects *this onto to_vec and returns a new scaled to_vec
Vector2D Vector2D::Project(const Vector2D& to_vec) const
{
    float projection_length = this->Dot(to_vec);
    return to_vec * projection_length;
}