#include <iostream>
#include <SFML/Graphics.hpp>

int main(){

        sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Kight");
    window.setFramerateLimit(144);

    float width = window.getSize().x;
    float height = window.getSize().y;

    sf::CircleShape circle(100.f);
    circle.setFillColor(sf::Color::Green);
    sf::RectangleShape rectangle(sf::Vector2f(100.f, 50.f));
    rectangle.setFillColor(sf::Color::Yellow);
    rectangle.setOrigin(rectangle.getSize() / 2.0f);
    rectangle.setPosition(sf::Vector2f(width / 2.0f, height / 2.0f));

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();            
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            circle.move(sf::Vector2f(1.0f, 0.5f)); // Se mueve a la derecha
        }    
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
            circle.move(sf::Vector2f(-0.5f, -1.0f)); // Se mueve a la derecha
        }       
        rectangle.move(sf::Vector2f(0.0f, 0.0f));

        if(circle.getGlobalBounds().findIntersection(rectangle.getGlobalBounds())){
            circle.setFillColor(sf::Color::White);
        }

        window.clear();
        window.draw(circle);
        window.draw(rectangle);
        window.display();
    }
    
}