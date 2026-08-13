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

        // detects and resolves a collision between a particle and the walls of the simulation
        // Boundaries is an Array of floats arranged clockwise for the walls of the simulation:
        // boundaries[0] is top wall, boundaries[1] is right-most wall, boundaries[2] is bottom wall
        // boundaries[3] is left-most wall
        void detect_resolve(Particle& A, float boundaries[4]);

        // Handles the post collision velocities of both particle A and B
        void resolve(Particle& A, Particle& B);
};

#endif