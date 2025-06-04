#include "Boton.h"
#include <iostream>

Boton::Boton(const std::string& ruta, sf::Vector2f posicion) {
    if (!textura.loadFromFile(ruta)) {
        std::cerr << "No se pudo cargar la textura del boton: " << ruta << std::endl;
    }
    sprite.setTexture(textura);
    sprite.setPosition(posicion);
}

void Boton::framesConfig(int ancho, int alto, int cantidadFrames, float tiempoEntreFrames) {
    frameWidth = ancho;
    frameHeight = alto;
    frameCount = cantidadFrames;
    frameTime = tiempoEntreFrames;
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    hitbox = sprite.getGlobalBounds(); // Inicializa la hitbox
}

void Boton::resetearFrame() {
    frameActual = 0; // Resetea al primer frame
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    animClock.restart(); // Reinicia el reloj de animación
}

void Boton::actualizar() {
    if (animClock.getElapsedTime().asSeconds() > frameTime) {
        frameActual = (frameActual + 1) % frameCount; // Cicla a través de los frames
        sprite.setTextureRect(sf::IntRect(frameActual * frameWidth, 0, frameWidth, frameHeight));
        animClock.restart();
    }
}

void Boton::dibujar(sf::RenderWindow& ventana) const {
    ventana.draw(sprite);
}

sf::FloatRect Boton::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}