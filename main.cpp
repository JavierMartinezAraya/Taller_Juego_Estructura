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
    Jugador jugador("resources/Ramos_Idle-Sheet.png", sf::Vector2f(300.0f, 600.0f),"resources/Ramos_Attack_L-Sheet.png","resources/Ramos_Danio-Sheet.png");
    Jugador jugador2("resources/Pablo_Idle-Sheet.png", sf::Vector2f(900.0f, 600.0f), "resources/Pablo_Attack_L-Sheet.png","resources/Pablo_Danio-Sheet.png");
    // Se establece el tamaño del sprite de los jugadores
    jugador.getSprite().setScale(0.65f, 0.65f);
    jugador2.getSprite().setScale(0.65f, 0.65f);
    
    // Controla de quien es el turno, true para el jugador y false para el jugador2
    bool turnoJugador = true;
    // Controla si se puede atacar 
    bool esperandoAccion = true; 
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
            // Ataque del jugador (tecla A)
            if (state == GameState::PLAY && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A && turnoJugador && esperandoAccion) {
                jugador.atacar();
                esperandoAccion = false;
            }
            // Ataque del jugador2 (tecla L)
            if (state == GameState::PLAY && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::L && !turnoJugador && esperandoAccion) {
                jugador2.atacar();
                esperandoAccion = false;
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
        
        // Si el estado es menu, se dibuja el menu principal
        if (state == GameState::MENU) {
            mainMenu.draw(window);
        }
        else if (state == GameState::PLAY) {
            // Solo permite una acción por turno para el jugador
            if (turnoJugador && esperandoAccion) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                    jugador.moverDerecha();
                    esperandoAccion = false;
                } else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
                    sf::FloatRect nuevaHitbox = jugador.getHitbox();
                    nuevaHitbox.left -= 50.0f;
                    if (!nuevaHitbox.intersects(jugador2.getHitbox())) {
                        jugador.moverIzquierda();
                        esperandoAccion = false;
                    }
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                    jugador.atacar();
                    esperandoAccion = false;
                } else if (!jugador.estaAtacando() && !jugador.estaRecibiendoDanio()) {
                    jugador.idle();
                }
            }
            // Solo permite una acción por turno para jugador 2
            if (!turnoJugador && esperandoAccion) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    jugador2.moverDerecha();
                    esperandoAccion = false;
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    sf::FloatRect nuevaHitbox = jugador2.getHitbox();
                    nuevaHitbox.left -= 50.0f;
                    if (!nuevaHitbox.intersects(jugador.getHitbox())) {
                        jugador2.moverIzquierda();
                        esperandoAccion = false;
                    }
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
                    jugador2.atacar();
                    esperandoAccion = false;
                } else if (!jugador2.estaAtacando() && !jugador2.estaRecibiendoDanio()) {
                    jugador2.idle();
                }
            }

        if (!turnoJugador && esperandoAccion) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
                jugador2.atacar();
                esperandoAccion = false;
            }
        }
            // Actualiza las animaciones de ambos jugadores
            jugador.actualizarAnimacion();
            jugador2.actualizarAnimacion();
            // Combate por turnos
            if (turnoJugador) {
                if (jugador.estaAtacando() && !jugador.getDanioAplicado() &&
                    jugador.getFrameAtaque() == 11 &&
                    jugador.getHitbox().intersects(jugador2.getHitbox())) {
                    jugador2.recibirDanio(20);
                    jugador.setDanioAplicado(true);
                }
                // Cambia de turno cuando termina la animacion de ataque
                if (!jugador.estaAtacando() && !esperandoAccion) {
                    turnoJugador = false;
                    esperandoAccion = true;
                }
            } else {
                if (jugador2.estaAtacando() && !jugador2.getDanioAplicado() &&
                    jugador2.getFrameAtaque() == 11 &&
                    jugador2.getHitbox().intersects(jugador.getHitbox())) {
                    jugador.recibirDanio(20);
                    jugador2.setDanioAplicado(true);
                }
                // Cambia de turno cuando termina la animacion de ataque
                if (!jugador2.estaAtacando() && !esperandoAccion) {
                    turnoJugador = true;
                    esperandoAccion = true;
                }
        }

            // Dibuja ambos jugadores
            if (jugador.estaVivo())
                jugador.dibujar(window);
            if (jugador2.estaVivo())
                jugador2.dibujar(window);
        }
        else if (state == GameState::OPTIONS) {
            // Se dibuja la pantalla de opciones
            sf::Font font;
            font.loadFromFile("resources/upheavtt.ttf");
            sf::Text text("OPTIONS\nPresiona ESC para volver", font, 65);
            text.setFillColor(sf::Color::Cyan);
            text.setPosition(100, 160);
            window.draw(text);
            opciones.draw(window); // Se dibuja el texto del volumen
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