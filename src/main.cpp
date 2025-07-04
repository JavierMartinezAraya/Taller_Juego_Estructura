#include <iostream>
#include <SFML/Graphics.hpp>
#include "MainMenu.h"
#include "Jugador.h"
#include "JugadorIA.h"
#include "Opciones.h"
#include <memory>

// Posibles estados del juego
enum class GameState {
    MENU,
    PLAY_PVP,
    PLAY_PVE,
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

    // Los jugadores son punteros porque se crean dinanmicamente y se destruyen al final del juego
    std::unique_ptr<Jugador> jugador;
    std::unique_ptr<Jugador> jugador2;
    std::unique_ptr<JugadorIA> jugadorIA;
    
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
                    if (x == 0) {
                        // Creacion de los jugadores
                        jugador = std::make_unique<Jugador>("resources/Deivid_Idle-Sheet.png", sf::Vector2f(300.0f, 600.0f),"resources/Deivid_Attack_L-Sheet.png","resources/Deivid_Danio-Sheet.png","resources/Deivid_Block-Sheet.png");
                        jugador2 = std::make_unique<Jugador>("resources/Paulo_Idle-Sheet.png", sf::Vector2f(900.0f, 600.0f), "resources/Paulo_Attack_L-Sheet.png","resources/Paulo_Danio-Sheet.png","resources/Paulo_Block-Sheet.png");
                        // Se modifica el tamaño de los sprites
                        jugador->getSprite().setScale(0.65f, 0.65f);
                        jugador2->getSprite().setScale(0.65f, 0.65f);
                        state = GameState::PLAY_PVP;     // Jugar
                    }
                    if (x == 1) {
                        // Creacion de los jugadores
                        jugador = std::make_unique<Jugador>("resources/Deivid_Idle-Sheet.png", sf::Vector2f(300.0f, 600.0f),"resources/Deivid_Attack_L-Sheet.png","resources/Deivid_Danio-Sheet.png","resources/Deivid_Block-Sheet.png");// falta implementar bloqueo
                        jugadorIA = std::make_unique<JugadorIA>("resources/Paulo_Idle-Sheet.png", sf::Vector2f(900.0f, 600.0f), "resources/Paulo_Attack_L-Sheet.png","resources/Paulo_Danio-Sheet.png","resources/Paulo_Block-Sheet.png");// falta implementar bloqueo
                        // Se modifica el tamaño de los sprites
                        jugador->getSprite().setScale(0.65f, 0.65f);
                        jugadorIA->getSprite().setScale(0.65f, 0.65f);
                        state = GameState::PLAY_PVE;     // Jugar
                    }

                    if (x == 2) state = GameState::OPTIONS;  // Opciones
                    if (x == 3) state = GameState::ABOUT;    // Acerca de
                    if (x == 4) window.close();              // Salir
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
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !jugador->estaAtacando()) {
                jugador->atacar();
                esperandoAccion = false;
            }
            // Ataque del jugador2 (tecla L)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::L) && !jugador2->estaAtacando()) {
                jugador2->atacar();
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
        // Si el estado play PvP, se realiza el juego PvP
        else if (state == GameState::PLAY_PVP && jugador && jugador2) {
            //Actualiza las animaciones de los jugadores solo si no estan a mitad de animación
            if (!mitadAnimacionJ1 && ((faseRonda == FaseRonda::TURNO_J1 && esperandoAccion) || jugador->estaAtacando())) {
                jugador->actualizarAnimacion();
            }
            if (!mitadAnimacionJ2 && ((faseRonda == FaseRonda::TURNO_J2 && esperandoAccion) || jugador2->estaAtacando())) {
                jugador2->actualizarAnimacion();
            }
            switch (faseRonda) {
                case FaseRonda::TURNO_J1:
                    if(mitadAnimacionJ1){
                        // Permite que J2 actúe mientras J1 está pausado
                        if (esperandoAccion) {
                            // Movimiento J2
                            if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                                teclaLiberadaJ2 = true;
                            }
                            if (teclaLiberadaJ2) {
                                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                                    jugador2->moverDerecha();
                                    esperandoAccion = false;
                                    teclaLiberadaJ2 = false;
                                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                                    sf::FloatRect nuevaHitbox = jugador2->getHitbox();
                                    nuevaHitbox.left -= 50.0f;
                                    if (!nuevaHitbox.intersects(jugador->getHitbox())) {
                                        jugador2->moverIzquierda();
                                        esperandoAccion = false;
                                        teclaLiberadaJ2 = false;
                                    }
                                }
                                mitadAnimacionJ1 = false;
                                esperandoAccion = false;
                            }
                            // Bloqueo J2
                            if (sf::Keyboard::isKeyPressed(sf::Keyboard::K) && !jugador2->estaBloqueando()) {
                                jugador2->bloquear();
                                esperandoAccion = false;
                            }
                            // Ataque J2
                            if (sf::Keyboard::isKeyPressed(sf::Keyboard::L) && !jugador2->estaAtacando()) {
                                jugador2->atacar();
                                esperandoAccion = false;
                            }
                        }
                        // Cuando J2 termina su acción, reanuda animación de J1
                        if (!esperandoAccion) {
                            mitadAnimacionJ1 = false;
                            faseRonda = FaseRonda::TURNO_J2; 
                            esperandoAccion = true;
                        }
                        break;
                    }
                    if (esperandoAccion) {
                        // Movimiento
                        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && !sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                            mouseLiberado = true;
                        }
                        if (mouseLiberado) {
                            if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                                jugador->moverIzquierda();
                                esperandoAccion = false;
                                mouseLiberado = false;
                            } else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                                sf::FloatRect nuevaHitbox = jugador->getHitbox();
                                nuevaHitbox.left -= 50.0f;
                                if (!nuevaHitbox.intersects(jugador2->getHitbox())) {
                                    jugador->moverDerecha();
                                    esperandoAccion = false;
                                    mouseLiberado = false;
                                }
                            }
                        }
                        // Bloqueo
                        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !jugador->estaBloqueando()){
                            jugador->bloquear();
                            esperandoAccion = false;
                        }
                        // Ataque
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !jugador->estaAtacando()) {
                            jugador->atacar();
                            esperandoAccion = false;
                        }
                    }
                    // Pausa a mitad de animación
                    if (jugador->estaAtacando() && jugador->getFrameAtaque() >= 11 && !mitadAnimacionJ1) { //frame
                        mitadAnimacionJ1 = true;
                        esperandoAccion = true;
                        break;
                    }
                    // Aplica daño si corresponde
                    if (jugador->estaAtacando() && !jugador->getDanioAplicado()&& jugador->getFrameAtaque() == 10 && jugador->getHitbox().intersects(jugador2->getHitbox()) && !jugador2->estaBloqueando()) {
                        jugador2->recibirDanio(20);
                        jugador->setDanioAplicado(true);
                    }
                    if (!jugador->estaAtacando() && !mitadAnimacionJ1 && !esperandoAccion) {
                        faseRonda = FaseRonda::TURNO_J2;
                        esperandoAccion = true;
                    }
                    break;
                    
                    case FaseRonda::TURNO_J2:
                    if(mitadAnimacionJ2){
                    // Permite que J1 actúe mientras J2 está pausado
                        if (esperandoAccion) {
                            // Movimiento J1
                            if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && !sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                            mouseLiberado = true;
                        }
                        if (mouseLiberado) {
                            if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                                jugador->moverIzquierda();
                                esperandoAccion = false;
                                mouseLiberado = false;
                            } else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                                sf::FloatRect nuevaHitbox = jugador->getHitbox();
                                nuevaHitbox.left -= 50.0f;
                                if (!nuevaHitbox.intersects(jugador2->getHitbox())) {
                                    jugador->moverDerecha();
                                    esperandoAccion = false;
                                    mouseLiberado = false;
                                    }
                                }
                                esperandoAccion = false;
                            }
                            // Bloqueo J1
                            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !jugador->estaBloqueando()) {
                                jugador->bloquear();
                                esperandoAccion = false;
                            }
                            // Ataque J1
                            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !jugador->estaAtacando()) {
                                jugador->atacar();
                                esperandoAccion = false;
                            }
                        }
                        // Cuando J1 termina su acción, reanuda animación de J2
                        if (!esperandoAccion) {
                            faseRonda = FaseRonda::TURNO_J1;
                            esperandoAccion = true;
                            mitadAnimacionJ2 = false;
                        }
                        mitadAnimacionJ2 = false;
                        break;
                    }
                    if (esperandoAccion) {
                        // Movimiento
                        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                            teclaLiberadaJ2 = true;
                        }
                        if (teclaLiberadaJ2) {
                            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                                jugador2->moverDerecha();
                                esperandoAccion = false;
                                teclaLiberadaJ2 = false;
                            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                                sf::FloatRect nuevaHitbox = jugador2->getHitbox();
                                nuevaHitbox.left -= 50.0f;
                                if (!nuevaHitbox.intersects(jugador->getHitbox())) {
                                    jugador2->moverIzquierda();
                                    esperandoAccion = false;
                                    teclaLiberadaJ2 = false;
                                }
                            }
                        }
                        // Bloqueo
                        if(sf::Keyboard::isKeyPressed(sf::Keyboard::K) && !jugador2->estaBloqueando()){
                            jugador2->bloquear();
                            esperandoAccion = false;
                        }
                        // Ataque
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L) && !jugador2->estaAtacando()) {
                            jugador2->atacar();
                            esperandoAccion = false;
                        }

                    }
                    // Pausa a mitad de animación
                    if (jugador2->estaAtacando() && jugador2->getFrameAtaque() >= 11 && !mitadAnimacionJ2) { //frame
                        mitadAnimacionJ2 = true;
                        esperandoAccion = true;
                        break;
                    }
                    // Aplica daño si corresponde
                    if (jugador2->estaAtacando() && !jugador2->getDanioAplicado() && jugador2->getFrameAtaque() == 10 && jugador2->getHitbox().intersects(jugador->getHitbox()) && !jugador->estaBloqueando()) {
                        jugador->recibirDanio(20);
                        jugador2->setDanioAplicado(true);
                    }
                    if (!jugador2->estaAtacando() && !mitadAnimacionJ2 && !esperandoAccion) {
                        faseRonda = FaseRonda::TURNO_J1;
                        esperandoAccion = true;
                    }
                    break;
            }

        if (jugador->estaVivo())
            jugador->dibujar(window);
        if (jugador2->estaVivo())
            jugador2->dibujar(window);
    }
        // Si el estado play PvE, se realiza el juego PvE
        else if (state == GameState::PLAY_PVE && jugador && jugador2) {
            jugador->actualizarAnimacion();
            jugadorIA->actualizarAnimacion();
            switch (faseRonda) {
                case FaseRonda::TURNO_J1:
                    if (esperandoAccion) {
                        // Movimiento
                        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && !sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                            mouseLiberado = true;
                        }
                        if (mouseLiberado) {
                            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                                jugador->moverDerecha();
                                esperandoAccion = false;
                                mouseLiberado = false;
                            } else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                                sf::FloatRect nuevaHitbox = jugador->getHitbox();
                                nuevaHitbox.left -= 50.0f;
                                if (!nuevaHitbox.intersects(jugador2->getHitbox())) {
                                    jugador->moverIzquierda();
                                    esperandoAccion = false;
                                    mouseLiberado = false;
                                }
                            }
                        }
                        // Ataque
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !jugador->estaAtacando()) {
                            jugador->atacar();
                            esperandoAccion = false;
                        }
                        if (jugador->estaAtacando() && jugador->getFrameAtaque() >= 8) { //cambiar los frames de ataque
                            mitadAnimacionJ1 = true;
                        }
                        if (!esperandoAccion && !jugador->estaAtacando() && mitadAnimacionJ1) {
                            faseRonda = FaseRonda::TURNO_J2;
                            esperandoAccion = true;
                            mitadAnimacionJ1 = false;
                        }
                    }
                    // Aplica daño si corresponde
                    if (jugador->estaAtacando() && !jugador->getDanioAplicado() && jugador->getFrameAtaque() == 11 && jugador->getHitbox().intersects(jugador2->getHitbox())) {
                        jugador2->recibirDanio(20);
                        jugador->setDanioAplicado(true);
                    }
                    // Cuando termina la acción, pasa al turno del jugador 2
                    if (!esperandoAccion && !jugador->estaAtacando()) {
                        faseRonda = FaseRonda::TURNO_J2;
                        esperandoAccion = true;
                    }
                    break;

                case FaseRonda::TURNO_J2:
                    // Lógica de la IA
                    if (jugadorIA->estaVivo()) {
                        JugadorIA::EstadoJuego estadoActual;
                        estadoActual.vidaIA = jugadorIA->getVida();
                        estadoActual.vidaJugador = jugador->getVida();
                        estadoActual.posX_IA = jugadorIA->getSprite().getPosition().x;
                        estadoActual.posX_Jugador = jugador->getSprite().getPosition().x;
                        estadoActual.turnoIA = true;

                        JugadorIA::Accion accionElegida = jugadorIA->decidirAccion(estadoActual, 2); // Puedes ajustar la profundidad

                        switch (accionElegida) {
                            case JugadorIA::MOVER_IZQ: jugadorIA->moverIzquierda(); break;
                            case JugadorIA::MOVER_DER: jugadorIA->moverDerecha(); break;
                            case JugadorIA::ATACAR: jugadorIA->atacar(); break;
                            case JugadorIA::BLOQUEAR: jugadorIA->bloquear(); break;
                        }
                        faseRonda = FaseRonda::TURNO_J1;
                    }
                    break;
            }

            if (jugador->estaVivo())
                jugador->dibujar(window);
            if (jugadorIA->estaVivo())
                jugadorIA->dibujar(window);

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
