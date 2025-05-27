#include <iostream>
#include "Jugador.h"

// Constructor
Jugador::Jugador(const std::string& rutaImagen, sf::Vector2f posicionInicial, const std::string& rutaSheet)
    : rutaIdleSheet(rutaSheet) // Guardamos la ruta del sprite idle
{
    // Se carga la imagen del jugador
    if (!textura.loadFromFile(rutaImagen)) {
        std::cerr << "No se pudo cargar la imagen del jugador: " << rutaImagen << std::endl;
    }
    sprite.setTexture(textura);             // Se asigna la textura al sprite
    sprite.setPosition(posicionInicial);    // Se coloca el jugador en la posicion inicial
}
//
void Jugador::mover(const sf::Vector2f& desplazamiento) {
    sprite.move(desplazamiento);
}
// Se dibuja al jugador en la ventana
void Jugador::dibujar(sf::RenderWindow& ventana) const {
    ventana.draw(sprite);
}

void Jugador::idle() {
    // Carga el sprite sheet solo una vez
    static bool loaded = false;
    if (!loaded) {
        if (texturaIdle.loadFromFile(rutaIdleSheet)) {
            loaded = true;
        } else {
            std::cerr << "No se pudo cargar el sprite idle: " << rutaIdleSheet << std::endl;
            return;
        }
    }
    sprite.setTexture(texturaIdle);

    if (idleClock.getElapsedTime().asSeconds() > idleFrameTime) {
        idleFrame = (idleFrame + 1) % idleFrameCount;
        idleClock.restart();
    }

    // Muestra el frame actual
    sprite.setTextureRect(sf::IntRect(idleFrame * idleFrameWidth, 0, idleFrameWidth, idleFrameHeight));
}

// Se devuelve una referencia al sprite del jugador
sf::Sprite& Jugador::getSprite() {
    return sprite;
}