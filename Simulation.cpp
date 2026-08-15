#include "Vector2D.h"
#include "Particle.h"
#include "Naive-Collisions.h"
#include <SFML/Graphics.hpp>
#include <vector>

using namespace sf;
using namespace std;

// Checks whether the user pressed a button in the simulation
bool ButtonPressed(RectangleShape button, Vector2f click_position)
{
    float length = button.getSize().x;
    float width = button.getSize().y;
    float button_x_position = button.getPosition().x;
    float button_y_position = button.getPosition().y;

    float right_boundary = button_x_position + length / 2;
    float left_boundary = button_x_position - length / 2;
    
    float upper_boundary = (button_y_position - width / 2);
    float lower_boundary = (button_y_position + width / 2);

    if (click_position.x >= left_boundary && click_position.x <= right_boundary)
    {
        if (click_position.y >= upper_boundary && click_position.y <= lower_boundary)
        {
            return true;
        }
    }

    return false;
}


int main()
{
      



    return 0;
}