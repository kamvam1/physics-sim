#include "Vector2D.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>

#if !defined(PARTICLE_H)
#define PARTICLE_H

class Particle
{
    public:

        // Default Constructor:
        // Takes in X and Y components of Velocity
        // Takes in radius to pass to CircleShape Constructor
        Particle(float v_x = 0.0f, float v_y = 0.0f, float radius = 0.0f, float mass = 0.0f);
        
        // Overloaded Default Constructor that takes in Vector2D vel, instead of components
        Particle(Vector2D vel, float radius = 0.0f, float mass = 0.0f);

        // Copy Constructor
        Particle(const Particle& other);
       
        // Returns the Velocity of the Particle
        Vector2D getVelocity() const;
        
        // Returns the object of the Particle
        const sf::CircleShape& getObject() const;

        // Returns the mass of the Particle
        float getMass() const;

        // Returns whether or not rendering *this is required
        bool getRender() const;
        
        // Sets the Velocity of the Particle
        // to_vel is the new velocity
        void setVelocity(const Vector2D& to_vel);  
        
        // Sets the render member of the Particle.
        // to_render is the new render
        void setRender(const bool& to_render);
        
        // Moves the Particle as described by its velocity, changes object's position in place
        // Takes in delta_time which is seconds per frame to compute distance moved.
        void Update_Position(const float delta_time, const Vector2D acceleration);

        // Updates the Particles Velocity as descirbed by Acceleration in place
        // Takes in delta_time which is seconds per frame
        // Takes in acceleration which has a default value of 5.0f
        void Update_Velocity(const float delta_time, const Vector2D acceleration);
    private:    
        // Class Members
        Vector2D velocity;
        sf::CircleShape object;
        float mass;
        bool render;
};


#endif // PARTICLE_H