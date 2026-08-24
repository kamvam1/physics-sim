#include <iostream>
#include "Vector2D.h"
#include "Particle.h"
#include "Naive-Collisions.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <chrono>

using namespace sf;
using namespace std;

// For Performance Measurement
class Timer
{
    public:
        Timer()
        {
            m_StartTimePoint = chrono::high_resolution_clock::now();
        }

        long Stop()
        {
            auto endTimePoint = chrono::high_resolution_clock::now();

            auto start = chrono::time_point_cast<chrono::microseconds>(this->m_StartTimePoint).time_since_epoch().count();
            auto end = chrono::time_point_cast<chrono::microseconds>(endTimePoint).time_since_epoch().count();

            auto duration = end - start;

            cout << duration << " us, " << duration * 0.001 << " ms" << endl; 

            return duration;
        }
    private:
        chrono::time_point<chrono::high_resolution_clock> m_StartTimePoint;
};

// Actually a Text_Box, but it is mostly used for rendering buttons on screen, has a RectangleShape and a Text member
struct Button
{
    RectangleShape rect;
    Text text;
};

// Creates a RectangleShape object and returns it
// height: length of the RectangleShape
// width: width of the RectangleShape
// fill_color: the inner color of the RectangleShape.
RectangleShape CreateRectangle(const float& height,const float& width,const Color& fill_color)
{   
    RectangleShape rect;
    rect.setSize(Vector2f(height, width));
    rect.setFillColor(fill_color);

    rect.setOrigin(height / 2.f, width / 2.f);

    return rect;
}

// Creates a Text object and returns it
// display_text: The text that is to be shown in the screen
// font: Text font
// char_size: Size of the Characters
// fill_color: The color of the text
Text CreateText(const string& display_text, const Font& font,  const int& char_size, const Color& fill_color)
{
    Text t;
    t.setString(display_text);
    t.setFillColor(fill_color);
    t.setCharacterSize(char_size);
    t.setFont(font);
    FloatRect bounds = t.getLocalBounds();
    t.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);

    return t;
}

// Creates a Text object and returns it, without the display_text param
// to_init: Text that already has a display string.
// font: Text font
// char_size: Size of the Characters
// fill_color: The color of the text
void InitText(Text& to_init, const Font& font,  const int& char_size, const Color& fill_color)
{
    to_init.setFont(font);
    to_init.setFillColor(fill_color);
    to_init.setCharacterSize(char_size);
    FloatRect bounds = to_init.getLocalBounds();
    to_init.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
}

// Checks whether user clicked on a particle to delete it
bool ParticleClicked(const Particle& p,const Vector2i& click_position)
{
    CircleShape obj = p.getObject();

    float radius = obj.getRadius();
    float x_click_pos = float(click_position.x);
    float y_click_pos = float(click_position.y);
    
    // Calculating the distance in both x and y axis.
    float dist_x = max(x_click_pos,obj.getPosition().x) - min(x_click_pos, obj.getPosition().x);
    float dist_y = max(y_click_pos,obj.getPosition().y) - min(y_click_pos, obj.getPosition().y);

    // Checks whether the radius is larger than both distances
    if (dist_x <= radius && dist_y <= radius)
    {
        return true;
    }

    return false;
}

// Makes sure there isnt a Particle where the user clicked 
// Used in Particle Creation so that a Particle isnt created on top of another
// radius_B: Radius of the Particle to be Created
// particles: All current particles in the simulation
// x_pos: X coordinate of the mouse click
// Y_pos: Y coordinate of the mouse click
bool ParticleObstructionCheck(float radius_B, vector<Particle>& particles, float x_pos, float y_pos)
{
    // Loop to find the nearest particle to the click position
    for (int i = 0; i < particles.size(); i++)
    {
        CircleShape obj = particles[i].getObject();
        float radius_A = obj.getRadius();
    
        float dist_x = max(x_pos,obj.getPosition().x) - min(x_pos, obj.getPosition().x);
        float dist_y = max(y_pos,obj.getPosition().y) - min(y_pos, obj.getPosition().y);

        if (dist_x <= radius_A + radius_B && dist_y <= radius_A + radius_B)
        {
            return true;
        }
    }

    return false;
}

// Checks whether the user pressed a button in the simulation
// x_pos and y_pos are both positions where the mouse click happened.
bool ButtonPressed(Button b, float x_pos, float y_pos)
{
    float length = b.rect.getSize().x;
    float width = b.rect.getSize().y;
    float button_x_position = b.rect.getPosition().x;
    float button_y_position = b.rect.getPosition().y;

    float right_boundary = button_x_position + length / 2;
    float left_boundary = button_x_position - length / 2;
    
    float upper_boundary = (button_y_position - width / 2);
    float lower_boundary = (button_y_position + width / 2);

    if (x_pos >= left_boundary && x_pos <= right_boundary)
    {
        if (y_pos >= upper_boundary && y_pos <= lower_boundary)
        {
            return true;
        }
    }

    return false;
}

// Overload of ButtonPressed that takes RectangleShape instead of Button
bool ButtonPressed(RectangleShape rect, float x_pos, float y_pos)
{
    float length = rect.getSize().x;
    float width = rect.getSize().y;
    float button_x_position = rect.getPosition().x;
    float button_y_position = rect.getPosition().y;

    float right_boundary = button_x_position + length / 2;
    float left_boundary = button_x_position - length / 2;
    
    float upper_boundary = (button_y_position - width / 2);
    float lower_boundary = (button_y_position + width / 2);

    if (x_pos >= left_boundary && x_pos <= right_boundary)
    {
        if (y_pos >= upper_boundary && y_pos <= lower_boundary)
        {
            return true;
        }
    }

    return false;
}

// Checks every particle with every other particle to see if a collision occured
// if a collision happened it gets resolved
void brute_force(vector<Particle>& particles)
{
    Naive_Collisions physics;

    for (int i = 0; i < particles.size() - 1; i++)
    {
        for (int j = i + 1; j < particles.size(); j++)
        {
            if (physics.detect(particles[i], particles[j]))
            {
                physics.resolve(particles[i], particles[j]);
            }
        }
    }
}

// Recursively builds a tree where particles in a node decrease as you get deeper, leading to faster brute_force in the deepest nodes.
// since there are less particles
void TreeBuilder(vector<Particle>& particles, vector<int>& indices, int max_depth, int min_particles, int curr_depth = 0)
{
    // Base Case
    if (curr_depth >= max_depth || indices.size() <= min_particles)
    {
        Naive_Collisions physics;

        for (int i = 0; i < indices.size() - 1; i++)
        {
            int A = indices[i];
            for (int j = i + 1; j < indices.size(); j++)
            {
                int B = indices[j];
                if (physics.detect(particles[A], particles[B]))
                {
                    physics.resolve(particles[A], particles[B]);
                }
            }
        }

        return;
    }

    // Finding Variation in both axis
    float min_x = 100000;
    float max_x = -100000;
    
    float min_y = 100000;
    float max_y = -100000;

    for (auto it = indices.begin();  it < indices.end(); it++)
    {
        min_x = min(min_x, particles[*it].getPosition().x);
        max_x = max(max_x, particles[*it].getPosition().x);

        min_y = min(min_y, particles[*it].getPosition().y);
        max_y = max(max_y, particles[*it].getPosition().y);
    }

    float variance_x = max_x - min_x;
    float variance_y = max_y - min_y;

    vector<int> left_indices;
    vector<int> right_indices;

    // Finding the best and splitting across the best axis 
    if (variance_x > variance_y)
    {    
        for (auto it = indices.begin(); it < indices.end(); it++)
        {
            if (particles[*it].getPosition().x <= min_x + variance_x / 2.f)
            {
                left_indices.push_back(*it);
            }
            else 
            {
                right_indices.push_back(*it);
            }
        }
    }
    else 
    {
        for (auto it = indices.begin(); it < indices.end(); it++)
        {
            if (particles[*it].getPosition().y <= min_y + variance_y / 2.f)
            {
                left_indices.push_back(*it);
            }
            else 
            {
                right_indices.push_back(*it);
            }
        }
    }

    // Base case
    if (left_indices.empty() || right_indices.empty())
    {
        Naive_Collisions physics;

        for (int i = 0; i < indices.size() - 1; i++)
        {
            int A = indices[i];
            for (int j = i + 1; j < indices.size(); j++)
            {
                int B = indices[j];
                if (physics.detect(particles[A], particles[B]))
                {
                    physics.resolve(particles[A], particles[B]);
                }
            }
        }

        return;
    }

    // Recursive Calls
    TreeBuilder(particles, left_indices, max_depth, min_particles, curr_depth + 1);
    TreeBuilder(particles, right_indices, max_depth, min_particles, curr_depth + 1);
}

// Initializes the indices and then calls TreeBuilder with them
void Tree(vector<Particle>& particles, int max_depth, int min_particles)
{
    vector<int> indices;

    for (int i = 0; i < particles.size(); i++)
    {
        indices.push_back(i);
    }

    TreeBuilder(particles, indices, max_depth, min_particles, 0);
}

// Handles Collisions of particles with the wall of the simulation
// particles: All current Particles in the Simulation
// boundaries: The 4 wall positions of the simulation
void boundary_collision(vector<Particle>& particles, float boundaries[4])
{
    Naive_Collisions physics;

    for (int i = 0; i < particles.size(); i++)
    {
        physics.detect_resolve(particles[i], boundaries);
    }
}

// Allows user to change certain constants used in the simulation.
void settings_menu(RenderWindow& window, Font& font, Vector2D& acceleration, int& collision_detection)
{
    window.setTitle("Settings");

    // Current Acceleration
    string acc_x = to_string(acceleration.getX());
    string acc_y = to_string(acceleration.getY());

    // 3 Buttons in this menu: 2 for collision detection algorithm, 1 for exit
    Button interactable_buttons[3];

    // 5 Text Boxes: 2 for Acceleration, 1 for Collision Detection, 2 for displaying current accelerations
    // in x and y
    Button text_boxes[5];

    // Sliders and their draggable components for X and Y accelerations
    RectangleShape lines[2];
    RectangleShape draggables[2];

    interactable_buttons[0].text.setString("Back to Menu");
    interactable_buttons[1].text.setString("Brute Force");
    interactable_buttons[2].text.setString("Tree (experimental)");

    
    for (int i = 0; i < 3; i++)
    {
        interactable_buttons[i].rect = CreateRectangle(450.f, 150.f, Color::Red);
        
        InitText(interactable_buttons[i].text, font, 28, Color::Black); 
    }

    // Back to Menu Button
    interactable_buttons[0].rect.setFillColor(Color::Red);
    interactable_buttons[0].rect.setPosition(Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f + 620.f));
    interactable_buttons[0].text.setPosition(interactable_buttons[0].rect.getPosition());

    // Brute Force Algorithm: Green means currently chosen.
    //Red means currently not chosen
    interactable_buttons[1].rect.setFillColor(Color::Green);
    interactable_buttons[1].rect.setPosition(Vector2f(window.getSize().x / 2.f, window.getSize().y / 5.f + 650.f));
    interactable_buttons[1].text.setPosition(interactable_buttons[1].rect.getPosition());
    
    // Tree Algorithm button 
    interactable_buttons[2].rect.setFillColor(Color::Red);
    interactable_buttons[2].rect.setPosition(Vector2f(window.getSize().x * 0.75f, window.getSize().y / 5.f + 650.f));
    interactable_buttons[2].text.setPosition(interactable_buttons[2].rect.getPosition());
    
    for (int i = 0; i < 2; i++)
    {
        lines[i] = CreateRectangle(950.f, 15.f, Color::Black);
        
        draggables[i] = CreateRectangle(20.f, 70.f, Color::White);
    }
    
    text_boxes[0].text.setString("X-Axis Acceleration:");
    text_boxes[1].text.setString("Y-Axis Acceleration:");
    text_boxes[2].text.setString("Collision Detection Algorithm:");
    text_boxes[3].text.setString(acc_x);
    text_boxes[4].text.setString(acc_y);

    for (int i = 0; i < 5; i++)
    {
        text_boxes[i].rect = CreateRectangle(450.f, 150.f, Color(180,180,180));
        if (i == 2)
        {
            text_boxes[i].rect = CreateRectangle(550.f,150.f,Color(180,180,180));
        }
        if (i == 3 || i == 4)
        {
            text_boxes[i].rect = CreateRectangle(200.f, 200.f, Color(180,180,180));
        }

        InitText(text_boxes[i].text, font, 27, Color::Black);
    }

    text_boxes[0].rect.setPosition(window.getSize().x / 6.f, window.getSize().y / 5.f);
    text_boxes[0].text.setPosition(text_boxes[0].rect.getPosition());
    
    text_boxes[1].rect.setPosition(window.getSize().x / 6.f, window.getSize().y / 5.f + 350.f);
    text_boxes[1].text.setPosition(text_boxes[1].rect.getPosition());
    
    text_boxes[2].rect.setPosition(window.getSize().x / 6.f, window.getSize().y / 5.f + 650.f);
    text_boxes[2].text.setPosition(text_boxes[2].rect.getPosition());
    
    text_boxes[3].rect.setPosition(window.getSize().x * 0.93f, window.getSize().y / 5.f);
    text_boxes[3].text.setPosition(text_boxes[3].rect.getPosition());
    
    text_boxes[4].rect.setPosition(window.getSize().x * 0.93f, window.getSize().y / 5.f + 350.f);
    text_boxes[4].text.setPosition(text_boxes[4].rect.getPosition());
    
    lines[0].setPosition(window.getSize().x * 0.62f, window.getSize().y / 5.f);
    lines[1].setPosition(window.getSize().x * 0.62f, window.getSize().y / 5.f + 350.f);
    
    float right_most = lines[0].getPosition().x + lines[0].getSize().x / 2.f;
    float left_most = lines[0].getPosition().x - lines[0].getSize().x / 2.f;
    float length = right_most - left_most;
    
    draggables[0].setPosition(left_most + (acceleration.getX() + 25.f) / 50.f * length, window.getSize().y / 5.f);
    draggables[1].setPosition(left_most + (acceleration.getY() + 25.f) / 50.f * length, window.getSize().y / 5.f + 350.f);

    while (window.isOpen())
    {
        Event event;
        // Event Handler loop
        while (window.pollEvent(event))
        {
            // Allows users to close the window if prompted
            if (event.type == Event::Closed)
            {
                window.close();
            }
            
            // Ensures Objects stay in their initial positions despite resizing
            if (event.type == Event::Resized)
            {
                View view( FloatRect( Vector2f( 0.f,0.f ), Vector2f( window.getSize() ) ) );
                window.setView(view);

                interactable_buttons[0].rect.setPosition(Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f + 620.f));
                interactable_buttons[0].text.setPosition(interactable_buttons[0].rect.getPosition());
                    
                interactable_buttons[1].rect.setPosition(Vector2f(window.getSize().x / 2.f, window.getSize().y / 5.f + 650.f));
                interactable_buttons[1].text.setPosition(interactable_buttons[1].rect.getPosition());
                
                interactable_buttons[2].rect.setPosition(Vector2f(window.getSize().x * 0.75f, window.getSize().y / 5.f + 650.f));
                interactable_buttons[2].text.setPosition(interactable_buttons[2].rect.getPosition());
                
                text_boxes[0].rect.setPosition(window.getSize().x / 6.f, window.getSize().y / 5.f);
                text_boxes[0].text.setPosition(text_boxes[0].rect.getPosition());
    
                text_boxes[1].rect.setPosition(window.getSize().x / 6.f, window.getSize().y / 5.f + 350.f);
                text_boxes[1].text.setPosition(text_boxes[1].rect.getPosition());
    
                text_boxes[2].rect.setPosition(window.getSize().x / 6.f, window.getSize().y / 5.f + 650.f);
                text_boxes[2].text.setPosition(text_boxes[2].rect.getPosition());
                
                text_boxes[3].rect.setPosition(window.getSize().x * 0.93f, window.getSize().y / 5.f);
                text_boxes[3].text.setPosition(text_boxes[3].rect.getPosition());
                        
                text_boxes[4].rect.setPosition(window.getSize().x * 0.93f, window.getSize().y / 5.f + 350.f);
                text_boxes[4].text.setPosition(text_boxes[4].rect.getPosition());
                
                lines[0].setPosition(window.getSize().x * 0.62f, window.getSize().y / 5.f);
                lines[1].setPosition(window.getSize().x * 0.62f, window.getSize().y / 5.f + 350.f);
                
                right_most = lines[0].getSize().x / 2.f + lines[0].getPosition().x;
                left_most = lines[0].getPosition().x - lines[0].getSize().x / 2.f;
                length = right_most - left_most;
                
                draggables[0].setPosition(left_most + acceleration.getX() / 15.f * length, window.getSize().y / 5.f);
                draggables[1].setPosition(left_most + acceleration.getY() / 15.f * length, window.getSize().y / 5.f + 350.f);
            }
        }
        
        // Button checks
        if (Mouse::isButtonPressed(Mouse::Left))
        {
            Vector2i click_position = Mouse::getPosition(window);
            
            // Back to Menu Button
            if (ButtonPressed(interactable_buttons[0], click_position.x, click_position.y))
            {
                return;
            }
            // Brute Force Collision Detection Button
            else if (ButtonPressed(interactable_buttons[1], click_position.x, click_position.y))
            {
                collision_detection = 1;
                interactable_buttons[1].rect.setFillColor(Color::Green);
                interactable_buttons[2].rect.setFillColor(Color::Red);
            }
            // Tree Collision Detection Button
            else if (ButtonPressed(interactable_buttons[2], click_position.x, click_position.y))
            {
                collision_detection = 2;
                interactable_buttons[1].rect.setFillColor(Color::Red);
                interactable_buttons[2].rect.setFillColor(Color::Green);
            }
            // X axis acceleration draggable
            else if (ButtonPressed(draggables[0], click_position.x, click_position.y)
                    || ButtonPressed(lines[0], click_position.x, click_position.y)
                    )
            {
                draggables[0].setPosition(click_position.x, window.getSize().y / 5.f);
                
                // Calculates new X-Acceleration based on draggable position
                acceleration.setX((click_position.x - left_most) / length * 50.f - 25.f);

                acc_x = to_string(acceleration.getX());

                // Handles Extreme positions for the draggable
                if (draggables[0].getPosition().x >= right_most)
                {
                    draggables[0].setPosition(right_most, window.getSize().y / 5.f);
                    acceleration.setX(25.f);
                    acc_x = to_string(acceleration.getX());
                }
                else if (draggables[0].getPosition().x <= left_most)
                {
                    draggables[0].setPosition(left_most, window.getSize().y / 5.f);
                    acceleration.setX(-25.f);
                    acc_x = to_string(acceleration.getX());
                }
                
                text_boxes[3].text.setString(acc_x);
            }
            // Y axis acceleration draggable
            else if (ButtonPressed(draggables[1], click_position.x, click_position.y)
            || ButtonPressed(lines[1], click_position.x, click_position.y)
                    )
            {
                draggables[1].setPosition(click_position.x, window.getSize().y / 5.f + 350.f);
                
                // Calculated new Y-Accelerations based on draggable position
                acceleration.setY((click_position.x - left_most) / length * 50.f - 25.f);
                
                acc_y = to_string(acceleration.getY());
                
                // Handles Extreme positions for the draggable
                if (draggables[1].getPosition().x >= right_most)
                {
                    draggables[1].setPosition(right_most, window.getSize().y / 5.f + 350.f);
                    acceleration.setY(25.f);
                    acc_y = to_string(acceleration.getY());
                }
                else if (draggables[1].getPosition().x <= left_most)
                {
                    draggables[1].setPosition(left_most, window.getSize().y / 5.f + 350.f);
                    acceleration.setY(-25.f);
                    acc_y = to_string(acceleration.getY());
                }
                
                text_boxes[4].text.setString(acc_y);
            }
        }
        
        window.clear(Color(190,190,190));

        // Drawing
        for (int i = 0; i < 3; i++)
        {
            window.draw(interactable_buttons[i].rect);
            window.draw(interactable_buttons[i].text);

        }
        
        for (int i = 0; i < 5; i++)
        {
            window.draw(text_boxes[i].rect);
            window.draw(text_boxes[i].text);
        }

        for (int i = 0; i < 2; i++)
        {
            window.draw(lines[i]);
            window.draw(draggables[i]);
        }

        window.display();
    }
}

// Renders the Start menu, if start is pressed, the function returns 2, if settings is pressed it returns 1.
// if exit is pressed window closes and function returns 0.
int start_menu(RenderWindow& window, Font& font)
{
    window.setTitle("Menu");
    // 3 Buttons: Start, Settings and Exit
    Button buttons[3];
    
    buttons[0].text.setString("Start Simulation");
    buttons[1].text.setString("Change Simulation Constants");
    buttons[2].text.setString("Exit");

    for (int i = 0; i < 3; i++)
    {
        buttons[i].rect = CreateRectangle(600.f, 150.f, Color::White);

        InitText(buttons[i].text, font, 28, Color::Black);
    }

    buttons[0].rect.setFillColor(Color::Green);
    buttons[0].rect.setPosition(Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f - 300.f));
    buttons[0].text.setPosition(buttons[0].rect.getPosition());

    buttons[1].rect.setFillColor(Color::Black);
    buttons[1].text.setFillColor(Color::White);
    buttons[1].rect.setPosition(Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f));
    buttons[1].text.setPosition(buttons[1].rect.getPosition());

    buttons[2].rect.setFillColor(Color::Red);
    buttons[2].rect.setPosition(Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f + 300.f));
    buttons[2].text.setPosition(buttons[2].rect.getPosition());

    while (window.isOpen())
    {
        Event event;

        // Event Handler Loop
        while (window.pollEvent(event))
        {
            // Allows user to close the Menu if prompted
            if (event.type == Event::Closed)
            {
                window.close();
            }
            
            // Ensures objects stay in their initial positions despite resizing
            if (event.type == Event::Resized)
            {
                View view( FloatRect( Vector2f( 0.f,0.f ), Vector2f( window.getSize() ) ) );
                window.setView(view);
                
                buttons[0].rect.setPosition(Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f - 300.f));
                buttons[0].text.setPosition(buttons[0].rect.getPosition());
                
                buttons[1].rect.setPosition(Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f));
                buttons[1].text.setPosition(buttons[1].rect.getPosition());
                
                buttons[2].rect.setPosition(Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f + 300.f));
                buttons[2].text.setPosition(buttons[2].rect.getPosition());
            }
        }

        // Handles Button pressing logic
        if (Mouse::isButtonPressed(Mouse::Left))
        {
            Vector2i click_position = Mouse::getPosition(window);

            // Exit Button
            if (ButtonPressed(buttons[2], click_position.x, click_position.y))
            {
                window.close();
            }
            // Settings Button
            else if (ButtonPressed(buttons[1], click_position.x, click_position.y))
            {
                return 1;
            }
            // Start Simulation
            else if (ButtonPressed(buttons[0], click_position.x, click_position.y))
            {
                return 2;
            }
        }

        window.clear(Color(190, 190, 190));

        // Drawing
        for (int i = 0; i < 3; i++)
        {
            window.draw(buttons[i].rect);
            window.draw(buttons[i].text);
        }

        window.display();
    }

    return 0;
}

// Simulation window
int main()
{
    RenderWindow window(VideoMode(2000, 1500),"Menu");
    window.setFramerateLimit(60);
    
    Font font;
    if (!font.loadFromFile("../fonts/0xProtoNerdFont-Regular.ttf"))
    {
        cout << "Could not load font" << endl;
        window.close();
        return -1;
    }   

    Vector2D acceleration(0.f, 15.f); // just the gravity 
    int collision_detection = 1; // 1 for brute force, 2 for tree (experimental).
    int option = start_menu(window, font);
    
    // Infinite loop until user decides to either start or exit program
    while (option != 2)
    {
        option = start_menu(window, font);
        
        if (option == 0)
        {
            return 0;
        }
        else if (option == 1)
        {
            settings_menu(window, font, acceleration, collision_detection);
        }
    }
    
    // Used for Delta Time
    Clock time;
    
    // Contains all the particles in the simulation
    vector<Particle> particles;

    // 3 Buttons: Create, Delete Particle, Exit Simulation
    Button buttons[3];

    // Box where all the particles are rendered
    RectangleShape sim_boundary;

    bool create_particle = false; // When Create Particle Button is pressed, this will be true
    bool delete_particle = false; // When Delete Particle Button is pressed, this will be true
    
    // Displays current fps
    Text frames;
    
    // Displays Number of Particles currently in the simulation
    Text particle_count;

    // Initial Mass and Radius of the Particles
    float mass = 1.f;
    float radius = 20.f;

    // Initializing and Creating Text, RectangleShapes, and Buttons
    particle_count = CreateText("Particle Count: " + to_string(particles.size()), font, 25, Color::Black);
    frames = CreateText("FPS: ", font, 28, Color::Black);

    particle_count.setPosition(window.getSize().x * 0.075f, window.getSize().y * 0.02f);
    frames.setPosition(window.getSize().x * 0.88f, window.getSize().y * 0.02f);
    
    sim_boundary = CreateRectangle(window.getSize().x * 0.70f, window.getSize().y - 15.f, Color::White);
    
    sim_boundary.setOutlineThickness(10.f);
    sim_boundary.setOutlineColor(Color::Cyan);
    sim_boundary.setPosition(10.f + sim_boundary.getSize().x / 2.f, sim_boundary.getSize().y / 2.f + 8.f);
    
    // Boundaries of the Simulation for resolving collisions against them
    float boundaries[4];
    boundaries[0] = 0.0f + 10.f;
    boundaries[1] = sim_boundary.getOrigin().x + sim_boundary.getSize().x / 2.f + 10.f;
    boundaries[2] = sim_boundary.getOrigin().y + sim_boundary.getSize().y / 2.f + 5.f;
    boundaries[3] = 0.0f + 10.f;

    buttons[0].text.setString("Create Particle");
    buttons[1].text.setString("Delete Particle");
    buttons[2].text.setString("Exit");
    
    for (int i = 0; i < 3; i++)
    {   
        buttons[i].rect = CreateRectangle(400.f, 150.f, Color::Red);
        
        InitText(buttons[i].text, font, 28, Color::Black);
    }

    buttons[0].rect.setFillColor(Color::Green);

    buttons[0].rect.setPosition(window.getSize().x * 0.85f, window.getSize().y * 0.3f);
    buttons[1].rect.setPosition(window.getSize().x * 0.85f, window.getSize().y * 0.5f);
    buttons[2].rect.setPosition(window.getSize().x * 0.85f, window.getSize().y * 0.9f);

    buttons[0].text.setPosition(buttons[0].rect.getPosition());
    buttons[1].text.setPosition(buttons[1].rect.getPosition());
    buttons[2].text.setPosition(buttons[2].rect.getPosition());
    
    window.setTitle("Simulation");

    // Used for Performance Checks 
    // int iteration = 0;
    // long total_time = 0;

    while (window.isOpen())
    {
        float delta_time = time.getElapsedTime().asSeconds();
        
        // Used to get accurate times
        // Changing the Denominator changes the frames per second
        if (delta_time >= 1.f / 60.f)
        {   
            time.restart();
            float fps = 1 / delta_time;
            frames.setString("FPS: " + to_string(fps));
            particle_count.setString("Particle Count: " + to_string(particles.size()));

            Event event;

            // Event Handler
            while (window.pollEvent(event))
            {
                // Allows user to close the Simulation if prompted
                if (event.type == Event::Closed)
                {
                    window.close();
                }
                
                // Ensures objects stay in their initial positions despite resizing
                if (event.type == Event::Resized)
                {
                    View view( FloatRect( Vector2f( 0.f,0.f ), Vector2f( window.getSize() ) ) );
                    window.setView(view);
    
                    sim_boundary.setSize(Vector2f(window.getSize().x * 0.70f, window.getSize().y - 15.f));
                    sim_boundary.setOrigin(sim_boundary.getSize().x / 2.f, sim_boundary.getSize().y / 2.f);

                    sim_boundary.setPosition(10.f + sim_boundary.getSize().x / 2.f, sim_boundary.getSize().y / 2.f + 8.f);

                    boundaries[0] = 0.0f + 10.f;
                    boundaries[1] = sim_boundary.getOrigin().x + sim_boundary.getSize().x / 2.f + 10.f;
                    boundaries[2] = sim_boundary.getOrigin().y + sim_boundary.getSize().y / 2.f + 5.f;
                    boundaries[3] = 0.0f + 10.f;
                    
                    particle_count.setPosition(window.getSize().x * 0.075f, window.getSize().y * 0.02f);
                    frames.setPosition(window.getSize().x * 0.88f, window.getSize().y * 0.02f);
    
                    buttons[0].rect.setPosition(window.getSize().x * 0.85f, window.getSize().y * 0.3f);
                    buttons[1].rect.setPosition(window.getSize().x * 0.85f, window.getSize().y * 0.5f);
                    buttons[2].rect.setPosition(window.getSize().x * 0.85f, window.getSize().y * 0.9f);
    
                    buttons[0].text.setPosition(buttons[0].rect.getPosition());
                    buttons[1].text.setPosition(buttons[1].rect.getPosition());
                    buttons[2].text.setPosition(buttons[2].rect.getPosition());
                }
            }
    
            // Particle Update Loop
            for (int i = 0; i < particles.size(); i++)
            {
                particles[i].Update_Velocity(delta_time, acceleration);
                particles[i].Update_Position(delta_time, acceleration);
            }
            
            // For Performance checking only.
            // if (particles.size() >= 100 && iteration < 500)
            // {
            //     if (collision_detection == 1)
            //     {
            //         Timer t;
            //         cout << "Iteration: " << iteration + 1 << endl;
            //         iteration++;
            //         brute_force(particles);
            //         total_time += t.Stop();
            //     }
            //     else if (collision_detection == 2)
            //     {
            //         Timer t;
            //         cout << "Iteration: " << iteration + 1 << endl;
            //         iteration++;
            //         Tree(particles, 9, 5);
            //         total_time += t.Stop();
            //     }
            // }
            // else 
            
            // Handles Collisions in between Particles
            if (particles.size() > 1)
            {
                if (collision_detection == 1)
                {
                    brute_force(particles);
                }
                else if (collision_detection == 2)
                {
                    Tree(particles, 9, 5);
                }
            }

            // if (iteration == 500)
            // {
            //     cout << "Average: " << total_time / 500 << " us, " << total_time * 0.001 / 500 << " ms" << endl;
            //     iteration = 501;
            //     if (collision_detection == 1)
            //     {
            //         cout << "Brute Force" << endl;
            //     }
            //     else if (collision_detection == 2)
            //     {
            //         cout << "Tree" << endl;
            //     }
            // }
            
            // Handles Collisions with the Walls of the Simulation Box
            if (particles.size())
            {
                boundary_collision(particles, boundaries);
            }

            window.clear(Color(170,170,170));
            
            // Drawing 
            window.draw(sim_boundary);
            
            window.draw(particle_count);
            window.draw(frames);
            
            for (int i = 0; i < particles.size(); i++)
            {
                window.draw(particles[i].getObject());
            }
            
            // This is the basic simulation menu with only 3 buttons, only 
            // rendered if the user hasnt pressed create and delete particles 
            if (create_particle == false && delete_particle == false)
            {
                // Drawing the buttons
                for (int i = 0; i < 3; i++)
                {
                    window.draw(buttons[i].rect);
                    window.draw(buttons[i].text);
                }
                
                // Handles the pressing of buttons
                if (Mouse::isButtonPressed(Mouse::Left))
                {
                    Vector2i click_position = Mouse::getPosition(window);
                    // Create a Particle Button
                    if (ButtonPressed(buttons[0], click_position.x, click_position.y))
                    {
                        create_particle = true;
                    }
                    // Delete a Particle Button
                    else if (ButtonPressed(buttons[1], click_position.x, click_position.y))
                    {   
                        delete_particle = true;
                    }
                    // Exit Button
                    else if (ButtonPressed(buttons[2], click_position.x, click_position.y))
                    {
                        window.close();
                    }
                }
            }
            
            // Particle Creation Menu
            if (create_particle)
            {
                // 3 Text Boxes: Instruction on how to create, Current Radius and Mass boxes.
                Button text_boxes[3];

                // Slider Components
                RectangleShape lines[2];
                RectangleShape draggables[2];

                // Initializing and Creating Text, RectangleShapes, and Buttons
                text_boxes[0].rect = CreateRectangle(450.f, 150.f, Color::Green);
                text_boxes[1].rect = CreateRectangle(400.f, 150.f, Color(180,180,180));
                text_boxes[2].rect = CreateRectangle(400.f, 150.f, Color(180,180,180));

                text_boxes[0].text = CreateText("Click In the Simulation Box \n   To Create the Particle",
                                            font, 25, Color::Black);
                text_boxes[1].text = CreateText("Radius: " + to_string(radius), font, 24, Color::Black);
                text_boxes[2].text = CreateText("Mass: " + to_string(mass), font, 24, Color::Black);

                text_boxes[0].rect.setPosition(window.getSize().x * 0.85f, window.getSize().y * 0.10f);
                text_boxes[0].text.setPosition(text_boxes[0].rect.getPosition());
                
                text_boxes[1].rect.setPosition(window.getSize().x * 0.85f, window.getSize().y * 0.3f);
                text_boxes[1].text.setPosition(text_boxes[1].rect.getPosition());
                
                text_boxes[2].rect.setPosition(window.getSize().x * 0.85f, window.getSize().y * 0.65f);
                text_boxes[2].text.setPosition(text_boxes[2].rect.getPosition());
                
                // Length of lines
                float length = window.getSize().x * 0.2;
                
                for (int i = 0; i < 2; i++)
                {
                    lines[i] = CreateRectangle(length, 15.f, Color::Black);
                    draggables[i] = CreateRectangle(20.f, 70.f, Color::White);
                }

                lines[0].setPosition(window.getSize().x * 0.85f, window.getSize().y * 0.475f);
                lines[1].setPosition(window.getSize().x * 0.85f, window.getSize().y * 0.775f);
                
                // For handling extreme positions of the draggable
                float right_most = lines[0].getPosition().x + lines[0].getSize().x / 2.f;
                float left_most = lines[0].getPosition().x - lines[0].getSize().x / 2.f;
                
                draggables[0].setPosition(left_most + (radius - 20.f)/ (70.f - 20.f) * length, window.getSize().y * 0.475f);
                draggables[1].setPosition(left_most + (mass - 1.f) / (100.f - 1.f) * length, window.getSize().y * 0.775f);

                // Handles Slider logic and particle creation
                if (Mouse::isButtonPressed(Mouse::Left))
                {
                    Vector2i click_position = Mouse::getPosition(window);
                    // Radius Draggable
                    if (ButtonPressed(draggables[0], click_position.x, click_position.y)
                    || ButtonPressed(lines[0], click_position.x, click_position.y)
                        )
                    {
                        draggables[0].setPosition(click_position.x, window.getSize().y * 0.475f);
                        
                        // Calculates new Radius based on draggable position
                        radius = (click_position.x - left_most) / length * 50.f + 20.f;
                        
                        // Handles Extremes positions of the draggables
                        if (draggables[0].getPosition().x >= right_most)
                        {
                            draggables[0].setPosition(right_most, window.getSize().y * 0.475f);
                            radius = 70.f;
                        }
                        else if (draggables[0].getPosition().x <= left_most)
                        {
                            draggables[0].setPosition(left_most, window.getSize().y * 0.475f);
                            radius = 20.f;
                        }

                        text_boxes[1].text.setString("Radius: " + to_string(radius));
                    }
                    // Mass Draggable
                    else if (ButtonPressed(draggables[1], click_position.x, click_position.y)
                    || ButtonPressed(lines[1], click_position.x, click_position.y)
                            )
                    {
                        draggables[1].setPosition(click_position.x, window.getSize().y * 0.775);

                        // Calculates new mass based on draggable position
                        mass = (click_position.x - left_most) / length * 100.f;

                        // Handles Extreme postions for the draggable
                        if (draggables[1].getPosition().x >= right_most)
                        {
                            draggables[1].setPosition(right_most, window.getSize().y * 0.775);
                            mass = 100.f;
                        }
                        else if (draggables[1].getPosition().x <= left_most)
                        {
                            draggables[1].setPosition(left_most, window.getSize().y * 0.775);
                            mass = 1.f;
                        }
                        
                        text_boxes[2].text.setString("Mass: " + to_string(mass));
                    }
                    // Checks whether the region clicked by the user is within the simulation boundaries
                    // and checks no particle is obstructing that position the user clicked on
                    else if ((click_position.x >= radius && click_position.x <= sim_boundary.getSize().x - radius)
                    && (click_position.y >= radius && click_position.y <= sim_boundary.getSize().y)
                    &&  !ParticleObstructionCheck(radius, particles, click_position.x, click_position.y))
                    {
                        Particle to_create(Vector2D(0.f,0.f), radius, mass);
                        to_create.setPosition(Vector2f(click_position.x, click_position.y));
                        int red = rand() % 255 + 10;
                        int green = rand() % 255 + 10;
                        int blue = rand() % 255 + 10;
                        to_create.setColor(Color(red,green,blue));
                        particles.push_back(to_create);
                        create_particle = false;
                    }
                }
                
                // Drawing
                for (int i = 0; i < 3; i++)
                {
                    window.draw(text_boxes[i].rect);
                    window.draw(text_boxes[i].text);
                }

                for (int i = 0; i < 2; i++)
                {
                    window.draw(lines[i]);
                    window.draw(draggables[i]);
                }
            }
    
            // Deleting a particle
            if (delete_particle)
            {
                // Ensures user is not able to delete when there are no particles
                if (!particles.size())
                {
                    Text error;
                    error.setFont(font);
                    error.setString("Cannot Delete Particles \n    There are None");
                    error.setCharacterSize(27);
                    FloatRect bounds = error.getLocalBounds();
                    error.setOrigin(bounds.width / 2.f + bounds.left, bounds.top + bounds.height / 2.f);    
                    error.setFillColor(Color::Black);
    
                    error.setPosition(window.getSize().x * 0.85f, window.getSize().y * 0.5f);
    
                    Button back;
                    back.rect.setSize(Vector2f(400.f, 150.f));
                    back.rect.setOrigin(back.rect.getSize().x / 2.f, back.rect.getSize().y / 2.f);
                    back.rect.setFillColor(Color::Red);
            
                    back.text.setFillColor(Color::Black);
                    back.text.setFont(font);
                    back.text.setString("Back");
    
                    back.text.setCharacterSize(28);
                    bounds = back.text.getLocalBounds();
                    back.text.setOrigin(bounds.width / 2.f + bounds.left, bounds.top + bounds.height / 2.f);
    
                    back.rect.setPosition(window.getSize().x * 0.85f, window.getSize().y * 0.6f);
                    back.text.setPosition(back.rect.getPosition());
                    
                    window.draw(error);
                    window.draw(back.rect);
                    window.draw(back.text);
                    
                    if (Mouse::isButtonPressed(Mouse::Left))
                    {
                        Vector2i click_position = Mouse::getPosition(window);
                        if (ButtonPressed(back, click_position.x, click_position.y))
                        {
                            delete_particle = false;
                        }
                    }
                }
                // Allows user to delete a particle
                else
                {
                    Text del_text;
                    del_text.setFont(font);
                    del_text.setString("Click on a Particle \n    to Delete it");
                    del_text.setCharacterSize(28);
                    FloatRect bounds = del_text.getLocalBounds();
                    del_text.setOrigin(bounds.width / 2.f + bounds.left, bounds.top + bounds.height / 2.f);
                    del_text.setFillColor(Color::Black);
    
                    del_text.setPosition(window.getSize().x * 0.85f, window.getSize().y * 0.5f);
    
                    window.draw(del_text);
                    
                    if (Mouse::isButtonPressed(Mouse::Left))
                    {   
                        Vector2i click_position = Mouse::getPosition(window);

                        // Locates which particle was clicked on
                        for (int i = 0; i < particles.size(); i++)
                        {
                            if (ParticleClicked(particles[i], click_position))
                            {
                                auto it = particles.begin() + i;
                                particles.erase(it);
                                delete_particle = false;
                            }
                        }   
                    }
                }
            }
            
            window.display();
        }
    }
    
    return 0;
}