#include <iostream>
#include "Jugador.h"

// Constructor
Jugador::Jugador(const std::string& rutaImagen, sf::Vector2f posicionInicial) {
    // Se carga la imagen del jugador
    if (!textura.loadFromFile("resources/Ramos.png")) {
        std::cerr << "No se pudo cargar la imagen del jugador: " << rutaImagen << std::endl;
    }
    sprite.setTexture(textura);             // Se asigna la textura al sprite
    sprite.setPosition(posicionInicial);    // Se coloca el juagdor en la posicion inicial
}
//
void Jugador::mover(const sf::Vector2f& desplazamiento) {
    sprite.move(desplazamiento);
}
// Se dibuja al jugador en la ventana
void Jugador::dibujar(sf::RenderWindow& ventana) const {
    ventana.draw(sprite);
}

// Se devuelve una referencia al sprite del jugador
sf::Sprite& Jugador::getSprite() {
    return sprite;
}