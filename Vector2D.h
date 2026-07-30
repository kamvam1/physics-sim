#if !defined(VECTOR2D_H)
#define VECTOR2D_H

class Vector2D {
public:
    // Default Constructor
    Vector2D(float x = 0.0, float y = 0.0);

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
private:
    float x;
    float y;  
};

#endif 