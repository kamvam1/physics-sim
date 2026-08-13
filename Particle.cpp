#include "Particle.h"
#include <cmath>

using namespace std;
using namespace sf;


// Default Constructor:
// Takes in X and Y components of Velocity
// Takes in radius to pass to CircleShape Constructor
Particle::Particle(float v_x, float v_y, float radius, float mass)
{
    this->velocity = Vector2D(v_x, v_y);
    this->object = CircleShape(radius);
    this->object.setOrigin(Vector2f(radius, radius));
    this->mass = mass;
}

// Overloaded Default Constructor that takes in Vector2D vel, instead of components
Particle::Particle(Vector2D vel, float radius, float mass)
{
    this->velocity = vel;
    this->object = CircleShape(radius);
    this->object.setOrigin(Vector2f(radius, radius));
    this->mass = mass;
}

// Copy Constructor
Particle::Particle(const Particle& other)
{
    this->velocity = other.velocity;
    this->object = other.object;
    this->mass = other.mass;
}

// Returns the Velocity of the Particle
Vector2D Particle::getVelocity() const
{
    return this->velocity;
}

// Returns the object of the Particle
const CircleShape& Particle::getObject() const
{
    return this->object;
}

// Returns the mass of the Particle
float Particle::getMass() const
{
    return this->mass;
}

// Sets the Velocity of the Particle
// to_vel is the new velocity
void Particle::setVelocity(const Vector2D& to_vel)
{
    this->velocity = to_vel;
}

// Moves the Particle as described by its velocity, changes object's position in place
// Takes in delta_time which is seconds per frame to compute distance moved.
void Particle::Update_Position(const float delta_time, const Vector2D acceleration)
{
    Vector2f offset;
    Vector2D delta_vel = acceleration * 0.5f * (delta_time * delta_time);
    offset.x = this->velocity.getX() * delta_time + delta_vel.getX();
    offset.y = this->velocity.getY() * delta_time + delta_vel.getY();

    this->object.move(offset);
}

// Updates the Particles Velocity as descirbed by Acceleration in place
// Takes in delta_time which is seconds per frame
// Takes in acceleration which has a default value of 5.0f
void Particle::Update_Velocity(const float delta_time, const Vector2D acceleration)
{
    Vector2D delta_vel = acceleration * delta_time;
    this->velocity += delta_vel;
    if (abs(this->velocity.getX()) >= 50.f)
    {
        float limit = copysign(50.f, this->velocity.getX());
        this->velocity.setX(limit); // Limiting X component to not cause rendering problems
    }

    if (abs(this->velocity.getY()) >= 50.f)
    {
        float limit = copysign(50.f, this->velocity.getY());
        this->velocity.setY(limit); // Limiting Y component to not cause rendering problems
    }
}