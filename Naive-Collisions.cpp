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
    
}
