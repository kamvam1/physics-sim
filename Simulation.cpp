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

// Allows user to change certain constants used in the simulation.
void settings_menu(RenderWindow& window, Font& font, Vector2D& acceleration, int collision_detection = 1)
{
    window.setTitle("Settings");
    
    // Current Acceleration
    string acc_x = to_string(acceleration.getX());
    string acc_y = to_string(acceleration.getY());

    Button interactable_buttons[3];
    Button text_boxes[5];

    interactable_buttons[0].text.setString("Back to Menu");
    interactable_buttons[1].text.setString("Brute Force");
    interactable_buttons[2].text.setString("Tree (experimental)");

    for (int i = 0; i < 3; i++)
    {
        interactable_buttons[i].rect.setSize(Vector2f(450.f, 150.f));
        interactable_buttons[i].rect.setOrigin(interactable_buttons[i].rect.getSize().x / 2.f, interactable_buttons[i].rect.getSize().y / 2.f);

        interactable_buttons[i].text.setFont(font);
        interactable_buttons[i].text.setCharacterSize(28);
        FloatRect bounds = interactable_buttons[i].text.getLocalBounds();
        interactable_buttons[i].text.setOrigin(bounds.width / 2.f + bounds.left, bounds.top + bounds.height / 2.f);
    }

    // Back to Menu Button
    interactable_buttons[0].rect.setFillColor(Color::Red);
    interactable_buttons[0].text.setFillColor(Color::Black);
    interactable_buttons[0].rect.setPosition(Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f + 620.f));
    interactable_buttons[0].text.setPosition(interactable_buttons[0].rect.getPosition());

    // Brute Force Algorithm: Green means currently chosen.
    //Red means currently not chosen
    interactable_buttons[1].rect.setFillColor(Color::Green);
    interactable_buttons[1].text.setFillColor(Color::Black);
    interactable_buttons[1].rect.setPosition(Vector2f(window.getSize().x / 2.f, window.getSize().y / 5.f + 650.f));
    interactable_buttons[1].text.setPosition(interactable_buttons[1].rect.getPosition());
    
    // Tree Algorithm button 
    interactable_buttons[2].rect.setFillColor(Color::Red);
    interactable_buttons[2].text.setFillColor(Color::Black);
    interactable_buttons[2].rect.setPosition(Vector2f(window.getSize().x * 0.75f, window.getSize().y / 5.f + 650.f));
    interactable_buttons[2].text.setPosition(interactable_buttons[2].rect.getPosition());

    float right_most = interactable_buttons[2].rect.getPosition().x + interactable_buttons[2].rect.getSize().x / 2.f;
    float left_most = interactable_buttons[1].rect.getPosition().x + interactable_buttons[1].rect.getSize().x / 2.f;
    float length = right_most - left_most;

    text_boxes[0].text.setString("X-Axis Acceleration:");
    text_boxes[1].text.setString("Y-Axis Acceleration:");
    text_boxes[2].text.setString("Collision Detection Algorithm:");
    text_boxes[3].text.setString(acc_x);
    text_boxes[4].text.setString(acc_y);

    for (int i = 0; i < 5; i++)
    {
        text_boxes[i].rect.setSize(Vector2f(450.f, 150.f));
        text_boxes[i].rect.setOrigin(text_boxes[i].rect.getSize().x / 2.f, text_boxes[i].rect.getSize().y / 2.f);
        text_boxes[i].rect.setFillColor(Color(180,180,180));
        if (i == 2)
        {
            text_boxes[i].rect.setSize(Vector2f(550.f, 150.f));
            text_boxes[i].rect.setOrigin(text_boxes[i].rect.getSize().x / 2.f, text_boxes[i].rect.getSize().y / 2.f);
        }
        if (i == 3 || i == 4)
        {
            text_boxes[i].rect.setSize(Vector2f(200.f, 200.f));
            text_boxes[i].rect.setOrigin(text_boxes[i].rect.getSize().x / 2.f, text_boxes[i].rect.getSize().y / 2.f);
        }
        text_boxes[i].text.setFont(font);
        text_boxes[i].text.setCharacterSize(28);
        FloatRect bounds = text_boxes[i].text.getLocalBounds();
        text_boxes[i].text.setOrigin(bounds.width / 2.f + bounds.left, bounds.top + bounds.height / 2.f);
        text_boxes[i].text.setFillColor(Color::Black);
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
                
                text_boxes[3].rect.setPosition(window.getSize().x * 0.8f, window.getSize().y / 5.f);
                text_boxes[3].text.setPosition(text_boxes[3].rect.getPosition());
                        
                text_boxes[4].rect.setPosition(window.getSize().x * 0.8f, window.getSize().y / 5.f + 350.f);
                text_boxes[4].text.setPosition(text_boxes[4].rect.getPosition());
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
        buttons[i].rect.setSize(Vector2f(600.f, 150.f));
        buttons[i].rect.setOrigin(300.f, 75.f);

        buttons[i].text.setFont(font);
        buttons[i].text.setCharacterSize(28);
        FloatRect bounds = buttons[i].text.getLocalBounds();
        buttons[i].text.setOrigin(bounds.width / 2.f + bounds.left, bounds.top + bounds.height / 2.f);
    }

    buttons[0].rect.setFillColor(Color::Green);
    buttons[0].text.setFillColor(Color::Black);
    buttons[0].rect.setPosition(Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f - 300.f));
    buttons[0].text.setPosition(buttons[0].rect.getPosition());

    buttons[1].rect.setFillColor(Color::Black);
    buttons[1].text.setFillColor(Color::White);
    buttons[1].rect.setPosition(Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f));
    buttons[1].text.setPosition(buttons[1].rect.getPosition());

    buttons[2].rect.setFillColor(Color::Red);
    buttons[2].text.setFillColor(Color::Black);
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
    
    return 0;
}