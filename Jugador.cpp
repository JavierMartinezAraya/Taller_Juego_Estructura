#include <iostream>
#include "Jugador.h"

// Constructor
Jugador::Jugador(const std::string& rutaIdle, sf::Vector2f posicionInicial, const std::string& rutaAtaque) {
    if (!texturaIdle.loadFromFile(rutaIdle)) {
        std::cerr << "No se pudo cargar el idle: " << rutaIdle << std::endl;
    }
    if (!texturaAtaque.loadFromFile(rutaAtaque)) {
        std::cerr << "No se pudo cargar el ataque: " << rutaAtaque << std::endl;
    }
    sprite.setTexture(texturaIdle);
    sprite.setPosition(posicionInicial);
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
}

// Mueve al jugador
void Jugador::mover(const sf::Vector2f& desplazamiento) {
    sprite.move(desplazamiento);
}
// Dibuja al jugador en la ventana
void Jugador::dibujar(sf::RenderWindow& ventana) const {
    ventana.draw(sprite);
}
// Cambia el sprite a su estado idle (reposo)
void Jugador::idle() {
    if (!atacando) {
        sprite.setTexture(texturaIdle);
    }
}
// Animacion de ataque del jugador
void Jugador::atacar() {
    if (!atacando) {
        atacando = true;
        frameAtaque = 0;
        danioAplicado = false; // <-- Reinicia aquí
        animClock.restart();
        sprite.setTexture(texturaAtaque);
        sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    }
}
// Resta vida si el jugador recibe daño y tiene mas de 0 de vida
void Jugador::recibirDanio(int cantidad) {
    if (vida > 0) vida -= cantidad;
}
// Actualiza la animacion
void Jugador::actualizarAnimacion() {
    if (atacando) {
        if (animClock.getElapsedTime().asSeconds() > frameTimeAtaque) {
            frameAtaque++;
            if (frameAtaque >= frameCountAtaque) {
                atacando = false;
                sprite.setTexture(texturaIdle);
                frameIdle = 0;
                sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
            } else {
                sprite.setTextureRect(sf::IntRect(frameAtaque * frameWidth, 0, frameWidth, frameHeight));
            }
            animClock.restart();
        }
    } else {
        if (animClock.getElapsedTime().asSeconds() > frameTimeIdle) {
            frameIdle = (frameIdle + 1) % frameCountIdle;
            sprite.setTextureRect(sf::IntRect(frameIdle * frameWidth, 0, frameWidth, frameHeight));
            animClock.restart();
        }
    }
}

// Devuelve el sprite del jugador
sf::Sprite& Jugador::getSprite() {
    return sprite;
}
// Devuelve la hitbox del jugador
sf::FloatRect Jugador::getHitbox() const {
    return sprite.getGlobalBounds();
}