#include "Naive-Collisions.h"
#include "Particle.h"
#include "Vector2D.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>

using namespace std;
using namespace sf;


// detects a collision between a particle and the walls of the simulation
void Naive_Collisions::detect_resolve(Particle& A, float boundaries[4])
{
    Vector2D pos = A.getObject().getPosition();
    float radius = A.getObject().getRadius();
    Vector2D vel = A.getVelocity();
    
    // Handles colliding with top wall
    if (vel.getY() < 0)
    {
        if ((pos.getY() - radius) <= boundaries[0])
        {
            vel.setY(vel.getY() * -1);
            A.setVelocity(vel);
        }
    }

    
    if (vel.getY() > 0)
    {
        if ((pos.getY() + radius) >= boundaries[2])
        {
            vel.setY(vel.getY() * -1);
            A.setVelocity(vel);
        }
    }

    
    if (vel.getX() > 0)
    {
        if ((pos.getX() - radius) >= boundaries[1])
        {
            vel.setX(vel.getX() * -1);
            A.setVelocity(vel);
        }
    }

    if (vel.getX() < 0)
    {
        if ((pos.getX() + radius) >= boundaries[3])
        {
            vel.setX(vel.getX() * -1);
            A.setVelocity(vel);
        }
    }
}


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
    normalBA = normalBA.Normalize();
    
    // Line of impact from B to A
    Vector2D normalAB = A.getObject().getPosition() - B.getObject().getPosition();
    normalAB = normalAB.Normalize();

    // initial velocity of A
    Vector2D init_vel_A = A.getVelocity();

    // initial velocity of B
    Vector2D init_vel_B = B.getVelocity();

    // Difference in velocities: B - A
    Vector2D vel_BA = init_vel_B - init_vel_A;

    Vector2D vel_AB = init_vel_A - init_vel_B;

    // Masses of the particles
    float mass_A = A.getMass();
    float mass_B = B.getMass();

    float alphaBA = 2 * mass_B / (mass_A + mass_B);
    float alphaAB = 2 * mass_A / (mass_A + mass_B);

    Vector2D final_vel_A = init_vel_A + normalBA * ( alphaBA * vel_BA.Dot(normalBA) );
    Vector2D final_vel_B = init_vel_B + normalAB * ( alphaAB * vel_AB.Dot(normalAB) );

    A.setVelocity(final_vel_A);
    B.setVelocity(final_vel_B);
}
