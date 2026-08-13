#include "Naive-Collisions.h"
#include "Particle.h"
#include "Vector2D.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>

using namespace std;
using namespace sf;


// Function that detects a collision between Particles A and B.
bool Naive_Collisions::detect(const Particle& A,const Particle& B)
{
    Vector2D p1 = A.getObject().getPosition();
    Vector2D p2 = B.getObject().getPosition();
    float A_radius = A.getObject().getRadius();
    float B_radius = B.getObject().getRadius();
    
    float dist = p1.Dist(p2);
    if (A_radius + B_radius < dist)
    {
        return false;
    }

    return true;
}

// Handles the post collision velocities
void Naive_Collisions::resolve(Particle& A, Particle& B)
{
    // Line of impact for A and B, specifically from A to B
    Vector2D normalBA = B.getObject().getPosition() - A.getObject().getPosition(); 
    
    // Line of impact from B to A
    Vector2D normalAB = A.getObject().getPosition() - B.getObject().getPosition();

    // initial velocity of A
    Vector2D init_vel_A = A.getVelocity();

    // initial velocity of B
    Vector2D init_vel_B = B.getVelocity();

    
}
