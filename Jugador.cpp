#include <iostream>
#include "Jugador.h"

// Constructor
Jugador::Jugador(const std::string& rutaIdle, sf::Vector2f posicionInicial, const std::string& rutaAtaque, const std::string& rutaDanio) {
    if (!texturaIdle.loadFromFile(rutaIdle)) {
        std::cerr << "No se pudo cargar el idle: " << rutaIdle << std::endl;
    }
    if (!texturaAtaque.loadFromFile(rutaAtaque)) {
        std::cerr << "No se pudo cargar el ataque: " << rutaAtaque << std::endl;
    }
    if (!texturaDanio.loadFromFile(rutaDanio)) {
        std::cerr << "No se pudo cargar el daño: " << rutaDanio << std::endl;
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
// Resta vida si el jugador recibe danio y tiene mas de 0 de vida
void Jugador::recibirDanio(int cantidad) {
    if (vida > 0) vida -= cantidad;
    recibiendoDanio = true;
    danioClock.restart();
    sprite.setTexture(texturaDanio);
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
}
// Actualiza la animacion
void Jugador::actualizarAnimacion() {

    if (recibiendoDanio) {
        if (danioClock.getElapsedTime().asSeconds() > duracionDanio) {
            recibiendoDanio = false;
            sprite.setTexture(texturaIdle);
            sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
        }
        return;
    }
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
// Mover el jugador a la izquierda
void Jugador::moverIzquierda() {
    mover(sf::Vector2f(-50.0f, 0.0f));
}
// Mover el jugador a la derecha
void Jugador::moverDerecha() {
    mover(sf::Vector2f(50.0f, 0.0f));
}
// Devuelve el sprite del jugador
sf::Sprite& Jugador::getSprite() {
    return sprite;
}
// Devuelve la hitbox del jugador
sf::FloatRect Jugador::getHitbox() const {
    sf::Vector2f pos = sprite.getPosition();
    float ancho = frameWidth * sprite.getScale().x;
    float alto = frameHeight * sprite.getScale().y;
    float margenX = ancho * 0.2f; // 20% a cada lado
    float margenY = alto * 0.1f;  // 10% arriba y abajo
    // Ajusta la hitbox para que sea más pequeña
    return sf::FloatRect(
        pos.x + margenX,
        pos.y + margenY, 
        ancho * 0.6f, // 60% del ancho
        alto * 0.8f   // 80% de la altura
    );
}