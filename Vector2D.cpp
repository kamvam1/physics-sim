#include "Vector2D.h"
#include <cmath>

using namespace std;
using namespace sf;

float EPSILON = 1e-4f;

// ------- Overloaded Constructors --------

Vector2D::Vector2D(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vector2D::Vector2D(const Vector2D& other)
{
    this->x = other.x;
    this->y = other.y;
}

Vector2D::Vector2D(const Vector2f& other)
{
    this->x = other.x;
    this->y = other.y;
}

// ------- Getters --------

// Returns X component of *this
float Vector2D::getX() const
{
    return this->x;
}

// Returns Y component of *this
float Vector2D::getY() const
{
    return this->y;
}

// ------- Setters --------

// Sets the X component of *this to to_x
void Vector2D::setX(float to_x)
{
    this->x = to_x;
}

// Sets the Y component of *this to to_y
void Vector2D::setY(float to_y)
{
    this->y = to_y;
}
// ------- Overloaded Operators --------

// Linear scaling of a Vector
Vector2D Vector2D::operator*(float scalar) const
{
    return Vector2D(this->x * scalar, this->y * scalar);
}

// Addition of 2 vectors
Vector2D Vector2D::operator+(const Vector2D& other) const
{
    return Vector2D(this->x + other.x, this->y + other.y);
}

// Addition of self with another vector in place
Vector2D& Vector2D::operator+=(const Vector2D& other)
{
    this->x += other.x;
    this->y += other.y;
    return *this;
}

// Subtraction of 2 vectors
Vector2D Vector2D::operator-(const Vector2D& other) const
{
    return Vector2D(this->x - other.x, this->y - other.y);
}

// Subtraction of self with another vector in place
Vector2D& Vector2D::operator-=(const Vector2D& other)
{
    this->x -= other.x;
    this->y -= other.y;
    return *this;
}

// Linear Scaling of self with another vector in place
Vector2D& Vector2D::operator*=(float scalar)
{
    this->x *= scalar;
    this->y *= scalar;
    return *this;
}

// Assigns other to *this
Vector2D& Vector2D::operator=(const Vector2D& other)
{
    this->x = other.x;
    this->y = other.y;
    return *this;
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


// ------- Basic Math Operations --------

// Squared Norm of *this with other.
float Vector2D::Dot(const Vector2D& other) const
{
    return this->x * other.x + this->y * other.y;
}

// Euclidean Norm of *this
float Vector2D::Magnitude() const
{
    return sqrt(this->x * this->x + this->y * this->y);
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
    float projection_length = this->Dot(to_vec.Normalize());
    return to_vec * projection_length;
}

// Calculates distance between *this and other
float Vector2D::Dist(const Vector2D& other)
{
    Vector2D dist = other - *this;
    return dist.Magnitude();
}