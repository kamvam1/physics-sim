#include "Particle.h"
#include "Vector2D.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>

#if !defined(NAIVE_COLLISIONS_H)
#define NAIVE_COLLISIONS_H

class Naive_Collisions
{
    public:
        // Function that detects a collision between Particles A and B.
        bool detect(const Particle& A,const Particle& B);

        // Handles the post collision velocities
        void resolve(Particle& A, Particle& B);
};

#endif