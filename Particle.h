#include "Vector2D.h"
#include <SFML/Graphics/CircleShape.hpp>

using namespace sf;

#if !defined(PARTICLE)
#define PARTICLE

class Particle
{
    public:

        // Default Constructor:
        // Takes in X and Y components of Velocity
        // Takes in radius to pass to CircleShape Constructor
        Particle(float v_x = 0.0f, float v_y = 0.0f, float radius = 0.0f);

        // Copy Constructor
        Particle(const Particle& other);
       
        // Returns the Velocity of the Particle
        Vector2D getVelocity();
        
        // Returns the object of the Particle
        CircleShape getObject();
        
        // Sets the Velocity of the Particle
        // to_vel is the new velocity
        void setVelocity(Vector2D to_vel);                
        
        // Moves the Particle as described by its velocity, changes object's position 
        // Takes in the change in time to compute distance moved.
        void Update(float delta_time);
    private:    
        // Class Members
        Vector2D velocity;
        CircleShape object;
};


#endif // PARTICLE