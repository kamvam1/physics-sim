#include <iostream>
#include "Vector2D.h"
#include "Particle.h"
#include "Naive-Collisions.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

using namespace sf;
using namespace std;

struct Button
{
    RectangleShape rect;
    Text text;
};

// Creates a RectangleShape object and returns it
RectangleShape CreateRectangle(const float& height,const float& width,const Color& fill_color)
{   
    RectangleShape rect;
    rect.setSize(Vector2f(height, width));
    rect.setFillColor(fill_color);

    rect.setOrigin(height / 2.f, width / 2.f);

    return rect;
}

// Creates a Text object and returns it
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
    
    float dist_x = max(x_click_pos,obj.getPosition().x) - min(x_click_pos, obj.getPosition().x);
    float dist_y = max(y_click_pos,obj.getPosition().y) - min(y_click_pos, obj.getPosition().y);

    if (dist_x <= radius && dist_y <= radius)
    {
        return true;
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

// Checks every particle with every other particle
void brute_force(vector<Particle>& particles)
{
    Naive_Collisions physics;

    for (int i = 0; i < particles.size() - 1; i++)
    {
        Particle A = particles[i];
        for (int j = i + 1; j < particles.size(); i++)
        {
            Particle B = particles[j];
            if (physics.detect(A, B))
            {
                physics.resolve(A, B);
            }
        }
    }
}

// Handles Collisions of particles with the wall of the simulation
void boundary_collision(vector<Particle>& particles, float boundaries[4])
{
    Naive_Collisions physics;

    for (int i = 0; i < particles.size(); i++)
    {
        physics.detect_resolve(particles[i], boundaries);
    }
}

// Allows user to change certain constants used in the simulation.
void settings_menu(RenderWindow& window, Font& font, Vector2D& acceleration, int collision_detection = 1)
{
    window.setTitle("Settings");

    // Current Acceleration
    string acc_x = to_string(acceleration.getX());
    string acc_y = to_string(acceleration.getY());

    Button interactable_buttons[3];
    Button text_boxes[5];
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
    
    draggables[0].setPosition(left_most + acceleration.getX() / 15.f * length, window.getSize().y / 5.f);
    draggables[1].setPosition(left_most + acceleration.getY() / 15.f * length, window.getSize().y / 5.f + 350.f);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            
            
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
        
        if (Mouse::isButtonPressed(Mouse::Left))
        {
            Vector2i click_position = Mouse::getPosition(window);
            if (ButtonPressed(interactable_buttons[0], click_position.x, click_position.y))
            {
                return;
            }
            else if (ButtonPressed(interactable_buttons[1], click_position.x, click_position.y))
            {
                collision_detection = 1;
                interactable_buttons[1].rect.setFillColor(Color::Green);
                interactable_buttons[2].rect.setFillColor(Color::Red);
            }
            else if (ButtonPressed(interactable_buttons[2], click_position.x, click_position.y))
            {
                collision_detection = 2;
                interactable_buttons[1].rect.setFillColor(Color::Red);
                interactable_buttons[2].rect.setFillColor(Color::Green);
            }
            else if (ButtonPressed(draggables[0], click_position.x, click_position.y)
                    || ButtonPressed(lines[0], click_position.x, click_position.y)
                    )
            {
                draggables[0].setPosition(click_position.x, window.getSize().y / 5.f);
                
                acceleration.setX((click_position.x - left_most) / length * 15.f);

                acc_x = to_string(acceleration.getX());

                if (draggables[0].getPosition().x >= right_most)
                {
                    draggables[0].setPosition(right_most, window.getSize().y / 5.f);
                    acceleration.setX(15.f);
                    acc_x = to_string(acceleration.getX());
                }
                else if (draggables[0].getPosition().x <= left_most)
                {
                    draggables[0].setPosition(left_most, window.getSize().y / 5.f);
                    acceleration.setX(0.f);
                    acc_x = to_string(acceleration.getX());
                }
                
                text_boxes[3].text.setString(acc_x);
            }
            else if (ButtonPressed(draggables[1], click_position.x, click_position.y)
            || ButtonPressed(lines[1], click_position.x, click_position.y)
                    )
            {
                draggables[1].setPosition(click_position.x, window.getSize().y / 5.f + 350.f);
                
                acceleration.setY((click_position.x - left_most) / length * 15.f);
                
                acc_y = to_string(acceleration.getY());
                
                if (draggables[1].getPosition().x >= right_most)
                {
                    draggables[1].setPosition(right_most, window.getSize().y / 5.f + 350.f);
                    acceleration.setY(15.f);
                    acc_y = to_string(acceleration.getY());
                }
                else if (draggables[1].getPosition().x <= left_most)
                {
                    draggables[1].setPosition(left_most, window.getSize().y / 5.f + 350.f);
                    acceleration.setY(0.f);
                    acc_y = to_string(acceleration.getY());
                }
                
                text_boxes[4].text.setString(acc_y);
            }
        }
        
        window.clear(Color(190,190,190));

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
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            
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
        if (Mouse::isButtonPressed(Mouse::Left))
        {
            Vector2i click_position = Mouse::getPosition(window);
            if (ButtonPressed(buttons[2], click_position.x, click_position.y))
            {
                window.close();
            }
            else if (ButtonPressed(buttons[1], click_position.x, click_position.y))
            {
                return 1;
            }
            else if (ButtonPressed(buttons[0], click_position.x, click_position.y))
            {
                return 2;
            }
        }

        window.clear(Color(190, 190, 190));

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
    window.setFramerateLimit(30);
    
    Font font;
    if (!font.loadFromFile("../fonts/0xProtoNerdFont-Regular.ttf"))
    {
        cout << "Could not load font" << endl;
        window.close();
        return -1;
    }   

    Vector2D acceleration(0.f, 10.f); // gravity 
    int collision_detection = 1; // 1 for brute force, 2 for tree (experimental).
    int option = start_menu(window, font);
    while (option != 2)
    {
        option = start_menu(window, font);
        
        if (option == 0)
        {
            break;
        }
        else if (option == 1)
        {
            settings_menu(window, font, acceleration, collision_detection);
        }
    }
    
    Clock time;
    vector<Particle> particles;
    Button buttons[3];
    RectangleShape sim_boundary; // Box where the particles will be simulated.
    bool create_particle = false; // When Create Particle Button is pressed, this will be true
    bool delete_particle = false; // When Delete Particle Button is pressed, this will be true
    Text frames;

    frames = CreateText("FPS: ", font, 28, Color::Black);

    frames.setPosition(window.getSize().x * 0.88f, window.getSize().y * 0.02f);
    
    sim_boundary = CreateRectangle(window.getSize().x * 0.70f, window.getSize().y - 15.f, Color::White);
    
    sim_boundary.setOutlineThickness(10.f);
    sim_boundary.setOutlineColor(Color::Cyan);
    sim_boundary.setPosition(10.f + sim_boundary.getSize().x / 2.f, sim_boundary.getSize().y / 2.f + 8.f);


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
    
    while (window.isOpen())
    {
        float delta_time = time.getElapsedTime().asSeconds();
        if (delta_time >= 1.f / 30.f)
        {
            time.restart();
            float fps = 1 / delta_time;
            frames.setString("FPS: " + to_string(fps));

            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    window.close();
                }
    
                if (event.type == Event::Resized)
                {
                    View view( FloatRect( Vector2f( 0.f,0.f ), Vector2f( window.getSize() ) ) );
                    window.setView(view);
    
                    sim_boundary.setSize(Vector2f(window.getSize().x * 0.70f, window.getSize().y - 15.f));
    
                    buttons[0].rect.setPosition(window.getSize().x * 0.85f, window.getSize().y * 0.3f);
                    buttons[1].rect.setPosition(window.getSize().x * 0.85f, window.getSize().y * 0.5f);
                    buttons[2].rect.setPosition(window.getSize().x * 0.85f, window.getSize().y * 0.9f);
    
                    buttons[0].text.setPosition(buttons[0].rect.getPosition());
                    buttons[1].text.setPosition(buttons[1].rect.getPosition());
                    buttons[2].text.setPosition(buttons[2].rect.getPosition());
                }
            }
    
            for (int i = 0; i < particles.size(); i++)
            {
                particles[i].Update_Velocity(delta_time, acceleration);
                particles[i].Update_Position(delta_time, acceleration);
            }
            
            if (particles.size())
            {
                if (collision_detection == 1)
                {
                    brute_force(particles);
                }
                // else 
                // {

                // }
            }

            window.clear(Color(170,170,170));
    
            window.draw(sim_boundary);
            window.draw(frames);
    
            for (int i = 0; i < particles.size(); i++)
            {
                window.draw(particles[i].getObject());
            }
            
            if (create_particle == false && delete_particle == false)
            {
                for (int i = 0; i < 3; i++)
                {
                    window.draw(buttons[i].rect);
                    window.draw(buttons[i].text);
                }
    
                if (Mouse::isButtonPressed(Mouse::Left))
                {
                    Vector2i click_position = Mouse::getPosition(window);
                    if (ButtonPressed(buttons[0], click_position.x, click_position.y))
                    {
                        create_particle = true;
                    }
                    else if (ButtonPressed(buttons[1], click_position.x, click_position.y))
                    {   
                        delete_particle = true;
                    }
                    else if (ButtonPressed(buttons[2], click_position.x, click_position.y))
                    {
                        window.close();
                    }
                }
            }
    
            if (create_particle)
            {
                Button text_boxes[3];
                RectangleShape lines[2];
                RectangleShape draggables[2];
                float mass = 1.f;
                float radius = 0.f;

                text_boxes[0].rect = CreateRectangle(450.f, 150.f, Color::Green);
                text_boxes[2].rect = CreateRectangle(400.f, 150.f, Color(180,180,180));
                text_boxes[1].rect = CreateRectangle(400.f, 150.f, Color(180,180,180));

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

                for (int i = 0; i < 2; i++)
                {
                    lines[i] = CreateRectangle(window.getSize().x * 0.2, 15.f, Color::Black);
                    draggables[2] = CreateRectangle(20.f, 70.f, Color::White);
                }

                lines[0].setPosition(window.getSize().x * 0.85f, window.getSize().y * 0.48f);
                lines[1].setPosition(window.getSize().x * 0.85f, window.getSize().y * 0.78f);
                
                // float right_most = lines[0].getPosition().x + lines[0].getSize().x / 2.f;
                // float left_most = lines[0].getPosition().x - lines[0].getSize().x / 2.f;
                // float length = right_most - left_most;
                
                // draggables[0].setPosition(left_most + radius / 25.f * length, window.getSize().y * 0.475f);
                // draggables[1].setPosition(left_most + mass / 100.f * length, window.getSize().y * 0.775f);

                for (int i = 0; i < 2; i++)
                {
                    window.draw(lines[i]);
                    // window.draw(draggables[i]);
                }

                for (int i = 0; i < 3; i++)
                {
                    window.draw(text_boxes[i].rect);
                    window.draw(text_boxes[i].text);
                }
                    
                if (Mouse::isButtonPressed(Mouse::Left))
                {
                    Vector2i click_position = Mouse::getPosition(window);
                    // if (click_position.x >= )
                    //     Particle to_create(Vector2D(), 10.f, 10.f);
                    // if (ButtonPressed(draggables[0], click_position.x, click_position.y)
                    // || ButtonPressed(lines[0], click_position.x, click_position.y)
                    // )
                    // {
                    //     draggables[0].setPosition(click_position.x, window.getSize().y * 0.475f);

                    //     if (draggables[0].getPosition().x >= right_most)
                    //     {
                    //         draggables[0].setPosition(right_most, window.getSize().y * 0.475f);
                    //     }
                    //     else if (draggables[0].getPosition().x <= left_most)
                    //     {
                    //         draggables[0].setPosition(left_most, window.getSize().y * 0.475f);
                    //     }

                    //     text_boxes[1].text.setString("Radius: " + to_string(radius));
                    // }
                    // else if (ButtonPressed(draggables[1], click_position.x, click_position.y)
                    // || ButtonPressed(lines[1], click_position.x, click_position.y)
                    //         )
                    // {
                    //         draggables[1].setPosition(click_position.x, window.getSize().y * 0.775);

                    //         if (draggables[1].getPosition().x >= right_most)
                    //         {
                    //             draggables[1].setPosition(right_most, window.getSize().y * 0.775);
                    //         }
                    //         else if (draggables[1].getPosition().x <= left_most)
                    //         {
                    //             draggables[1].setPosition(left_most, window.getSize().y * 0.775);
                    //         }

                    //         text_boxes[2].text.setString("Mass: " + to_string(mass));
                    // }
                    create_particle = false;
                }
            }
    
            if (delete_particle)
            {
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