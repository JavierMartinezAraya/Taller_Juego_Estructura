#include <iostream>
#include "Jugador.h"

Jugador::Jugador(const std::string& rutaIdle, sf::Vector2f posicionInicial, const std::string& rutaAtaque, const std::string& rutaDanio, const std::string& rutaBloqueo) {
    if (!texturaIdle.loadFromFile(rutaIdle)) {
        std::cerr << "No se pudo cargar el idle: " << rutaIdle << std::endl;
    }
    if (!texturaAtaque.loadFromFile(rutaAtaque)) {
        std::cerr << "No se pudo cargar el ataque: " << rutaAtaque << std::endl;
    }
    if (!texturaDanio.loadFromFile(rutaDanio)) {
        std::cerr << "No se pudo cargar el daño: " << rutaDanio << std::endl;
    }
    if (!texturaBloqueo.loadFromFile(rutaBloqueo)) {
        std::cerr << "No se pudo cargar el bloqueo: " << rutaBloqueo << std::endl;
    }
    sprite.setTexture(texturaIdle);
    sprite.setPosition(posicionInicial);
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
}


void Jugador::mover(const sf::Vector2f& desplazamiento) {
    sprite.move(desplazamiento);
}

void Jugador::dibujar(sf::RenderWindow& ventana) const {
    ventana.draw(sprite);
}

void Jugador::idle() {
    if (!atacando) {
        sprite.setTexture(texturaIdle);
    }
}

void Jugador::bloquear() {
    if (!bloqueando) {
        bloqueando = true;
        frameBloqueo = 0;
        animClock.restart();
        sprite.setTexture(texturaBloqueo);
        sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    }
}

void Jugador::atacar() {
    if (!atacando) {
        atacando = true;
        frameAtaque = 0;
        danioAplicado = false;
        animClock.restart();
        sprite.setTexture(texturaAtaque);
        sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    }
}

void Jugador::recibirDanio(int cantidad) {
    if (vida > 0) vida -= cantidad;
    recibiendoDanio = true;
    danioClock.restart();
    sprite.setTexture(texturaDanio);
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
}

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
    }
    if (bloqueando){
        if (animClock.getElapsedTime().asSeconds() > frameTimeBloqueo) {
            frameBloqueo++;
            if (frameBloqueo >= frameCountBloqueo) {
                bloqueando = false;
                sprite.setTexture(texturaIdle);
                frameIdle = 0;
                sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
            } else {
                sprite.setTextureRect(sf::IntRect(frameBloqueo * frameWidth, 0, frameWidth, frameHeight));
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

void Jugador::moverIzquierda() {
    mover(sf::Vector2f(-50.0f, 0.0f));
}

void Jugador::moverDerecha() {
    mover(sf::Vector2f(50.0f, 0.0f));
}

sf::Sprite& Jugador::getSprite() {
    return sprite;
}

sf::FloatRect Jugador::getHitbox() const {
    sf::Vector2f pos = sprite.getPosition();
    float ancho = frameWidth * sprite.getScale().x;
    float alto = frameHeight * sprite.getScale().y;
    float margenX = ancho * 0.2f;
    float margenY = alto * 0.1f;

    return sf::FloatRect(
        pos.x + margenX,
        pos.y + margenY, 
        ancho * 0.6f,
        alto * 0.8f 
    );
}