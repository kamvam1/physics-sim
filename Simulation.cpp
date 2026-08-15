#include <iostream>
#include "Vector2D.h"
#include "Particle.h"
#include "Naive-Collisions.h"
#include <SFML/Graphics.hpp>
#include <vector>

using namespace sf;
using namespace std;

struct Button
{
    RectangleShape rect;
    Text text;
};

// Checks whether the user pressed a button in the simulation
bool ButtonPressed(Button b, Vector2f click_position)
{
    float length = b.rect.getSize().x;
    float width = b.rect.getSize().y;
    float button_x_position = b.rect.getPosition().x;
    float button_y_position = b.rect.getPosition().y;

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


void start_menu(RenderWindow& window)
{
    Font font;
    if (!font.loadFromFile("../fonts/0xProtoNerdFont-Regular.ttf"))
    {
        cout << "Could not load font" << endl;
        window.close();
        return;
    }

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
        cout << buttons[i].text.getOrigin().x << ", " << buttons[i].text.getOrigin().y << endl; 
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
                
            }

            window.clear(Color(190, 190, 190));

            for (int i = 0; i < 3; i++)
            {
                window.draw(buttons[i].rect);
                window.draw(buttons[i].text);
            }

            window.display();
        }
    }
}


int main()
{
    RenderWindow window(VideoMode(2000, 1500),"Menu");
    window.setFramerateLimit(30);
    start_menu(window);

    return 0;
}