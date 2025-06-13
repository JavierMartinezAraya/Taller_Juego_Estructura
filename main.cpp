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
enum class FaseRonda {
    TURNO_J1,
    TURNO_J2
};

int main() {
    bool mouseLiberado = true;
    bool teclaLiberadaJ2 = true;
    bool mitadAnimacionJ1 = false;
    bool mitadAnimacionJ2 = false;
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
    FaseRonda faseRonda = FaseRonda::TURNO_J1;

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
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !jugador.estaAtacando()) {
                jugador.atacar();
                esperandoAccion = false;
            }
            // Ataque del jugador2 (tecla L)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::L) && !jugador2.estaAtacando()) {
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
            switch (faseRonda) {
                case FaseRonda::TURNO_J1:
                    if (esperandoAccion) {
                        // Movimiento
                        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && !sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                            mouseLiberado = true;
                        }
                        if (mouseLiberado) {
                            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                                jugador.moverDerecha();
                                esperandoAccion = false;
                                mouseLiberado = false;
                            } else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                                sf::FloatRect nuevaHitbox = jugador.getHitbox();
                                nuevaHitbox.left -= 50.0f;
                                if (!nuevaHitbox.intersects(jugador2.getHitbox())) {
                                    jugador.moverIzquierda();
                                    esperandoAccion = false;
                                    mouseLiberado = false;
                                }
                            }
                        }
                        // Ataque
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !jugador.estaAtacando()) {
                            jugador.atacar();
                            esperandoAccion = false;
                        }
                        if (jugador.estaAtacando() && jugador.getFrameAtaque() >= 8) { //cambiar los frames de ataque
                            mitadAnimacionJ1 = true;
                        }
                        if (!esperandoAccion && !jugador.estaAtacando() && mitadAnimacionJ1) {
                            faseRonda = FaseRonda::TURNO_J2;
                            esperandoAccion = true;
                            mitadAnimacionJ1 = false;
                        }
                    }
                    // Aplica daño si corresponde
                    if (jugador.estaAtacando() && !jugador.getDanioAplicado() &&
                        jugador.getFrameAtaque() == 11 &&
                        jugador.getHitbox().intersects(jugador2.getHitbox())) {
                        jugador2.recibirDanio(20);
                        jugador.setDanioAplicado(true);
                    }
                    // Cuando termina la acción, pasa al turno del jugador 2
                    if (!esperandoAccion && !jugador.estaAtacando()) {
                        faseRonda = FaseRonda::TURNO_J2;
                        esperandoAccion = true;
                    }
                    break;

                case FaseRonda::TURNO_J2:
                    if (esperandoAccion) {
                        // Movimiento
                        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                            teclaLiberadaJ2 = true;
                        }
                        if (teclaLiberadaJ2) {
                            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                                jugador2.moverDerecha();
                                esperandoAccion = false;
                                teclaLiberadaJ2 = false;
                            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                                sf::FloatRect nuevaHitbox = jugador2.getHitbox();
                                nuevaHitbox.left -= 50.0f;
                                if (!nuevaHitbox.intersects(jugador.getHitbox())) {
                                    jugador2.moverIzquierda();
                                    esperandoAccion = false;
                                    teclaLiberadaJ2 = false;
                                }
                            }
                        }
                        // Ataque
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L) && !jugador2.estaAtacando()) {
                            jugador2.atacar();
                            esperandoAccion = false;
                        }
                        if (jugador2.estaAtacando() && jugador2.getFrameAtaque() >= 8) { //cambiar los frames de ataque
                            mitadAnimacionJ2 = true;
                        }
                        if (!esperandoAccion && !jugador2.estaAtacando() && mitadAnimacionJ2) {
                            faseRonda = FaseRonda::TURNO_J1;
                            esperandoAccion = true;
                            mitadAnimacionJ2 = false;
                        }
                    }
                    // Aplica daño si corresponde
                    if (jugador2.estaAtacando() && !jugador2.getDanioAplicado() &&
                        jugador2.getFrameAtaque() == 11 &&
                        jugador2.getHitbox().intersects(jugador.getHitbox())) {
                        jugador.recibirDanio(20);
                        jugador2.setDanioAplicado(true);
                    }
                    // Cuando termina la acción, pasa al turno del jugador 1
                    if (!esperandoAccion && !jugador2.estaAtacando()) {
                        faseRonda = FaseRonda::TURNO_J1;
                        esperandoAccion = true;
                    }
                    break;
            }
        // Actualiza animaciones y dibuja jugadores
        if (!esperandoAccion) {
            jugador.actualizarAnimacion();
            jugador2.actualizarAnimacion();
        }
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
