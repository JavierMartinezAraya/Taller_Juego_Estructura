#include <iostream>
#include <SFML/Graphics.hpp>
#include "MainMenu.h"
#include "Jugador.h"
#include "Opciones.h"

// Posibles estados del juego
enum class GameState {
    MENU,
    PLAY,
    OPTIONS,
    ABOUT
};

int main() {
    // Se crea la ventana principal del juego de tamaño 1920x1080
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Kight");
    window.setFramerateLimit(90);

    // Se carga la imagen de fondo que se vera en el menu principal de juego
    sf::Texture texture;
    if (!texture.loadFromFile("resources/Sala.png")) {
        std::cout << "Error: no se pudo cargar la imagen Sala.png" << std::endl;
        return 1;
    }
    sf::Sprite sprite(texture);

    float width = window.getSize().x;
    float height = window.getSize().y;
    // Se crea el menu principal y las opciones
    MainMenu mainMenu(width, height);
    Opciones opciones(width, height);

    // Creacion del jugador
    Jugador jugador("resources/Ramos.png", sf::Vector2f(300.0f, 600.0f),"resources/Ramos_Idle-Sheet.png");

    // Estado del juego
    GameState state = GameState::MENU;

    sf::Event event;
    while (window.isOpen()) {
        // Se procesa los eventos
        while (window.pollEvent(event)) { 
            if (event.type == sf::Event::Closed)
                window.close();

            // Opciones del menu principal
            if (state == GameState::MENU && event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Up)
                    mainMenu.MoveUp();
                if (event.key.code == sf::Keyboard::Down)
                    mainMenu.MoveDown();
                if (event.key.code == sf::Keyboard::Return) {
                    int x = mainMenu.MainMenuPressed();
                    if (x == 0) state = GameState::PLAY;     // Jugar
                    if (x == 1) state = GameState::OPTIONS;  // Opciones
                    if (x == 2) state = GameState::ABOUT;    // Acerca de
                    if (x == 3) window.close();
                }
            }
            // Volver al menu principal con la tecla Esc
            if (state != GameState::MENU && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                state = GameState::MENU;
                if (event.key.code == sf::Keyboard::Return) {
                    state = GameState::MENU;
                    mainMenu.resetSeleccion();
                }
            }
            // Manejo de las opciones
            if (state == GameState::OPTIONS && event.type == sf::Event::KeyReleased) { 
                if (event.key.code == sf::Keyboard::Right) opciones.aumentarVolumen();
                if (event.key.code == sf::Keyboard::Left) opciones.disminuirVolumen(); 
            }
        }
        // Se limpia la pantlla y el dibujo del fondo
        window.clear();
        window.draw(sprite);
        
        // Si el estado es menu, se dibuja el menui principal
        if (state == GameState::MENU) {
            mainMenu.draw(window);
        }
        else if (state == GameState::PLAY) {
            // Se realiza el movimiento del jugador
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                jugador.mover(sf::Vector2f(1.0f, 0.0f));
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
                jugador.mover(sf::Vector2f(-1.0f, 0.0f));
            else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && !sf::Mouse::isButtonPressed(sf::Mouse::Right)) jugador.idle();
            

            jugador.dibujar(window);
        }
        else if (state == GameState::OPTIONS) {
            // Se dibuja la pantalla de opciones
            sf::Font font;
            font.loadFromFile("resources/upheavtt.ttf");
            sf::Text text("OPTIONS\nPresiona ESC para volver", font, 65);
            text.setFillColor(sf::Color::Cyan);
            text.setPosition(100, 160);
            window.draw(text);
            opciones.draw(window); // Se dibuja el texto del volumen.
        }
        else if (state == GameState::ABOUT) {
            // Se dibuja la pantalla de "Acerca de"
            sf::Font font;
            font.loadFromFile("resources/upheavtt.ttf");
            sf::Text text("ABOUT\nPresiona ESC para volver", font, 65);
            text.setFillColor(sf::Color::Cyan);
            text.setPosition(100, 160);
            window.draw(text);
        }

        window.display();
    }
    return 0;
}
