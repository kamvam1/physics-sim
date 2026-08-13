#include "Particle.h"
#include "Vector2D.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>

#if !defined(NAIVE_COLLISIONS_H)
#define NAIVE_COLLISIONS_H

class Naive_Collisions
{
    public:
        // detects a collision between Particles A and B.
        bool detect(const Particle& A, const Particle& B);

        // detects a collision between a particle and the walls of the simulation
        bool detect(const Particle& A, float boundaries[4]);

        // Handles the post boundary collision velocities of particle A
        void resolve(Particle& A);

        // Handles the post collision velocities of both particle A and B
        void resolve(Particle& A, Particle& B);
};

#endif