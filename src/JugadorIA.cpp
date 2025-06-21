#include "JugadorIA.h"
#include <iostream>
#include <limits>

JugadorIA::JugadorIA(const std::string& rutaIdle, sf::Vector2f posicionInicial, const std::string& rutaAtaque, const std::string& rutaDanio, const std::string& rutaBloqueo) {
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

void JugadorIA::dibujar(sf::RenderWindow& ventana) const {
    ventana.draw(sprite);
}


void JugadorIA::mover(const sf::Vector2f& desplazamiento) {
    sprite.move(desplazamiento);
}

void JugadorIA::moverIzquierda() {
    mover(sf::Vector2f(-50.0f, 0.0f));
}

void JugadorIA::moverDerecha() {
    mover(sf::Vector2f(50.0f, 0.0f));
}

void JugadorIA::idle() {
    if (!atacando) {
        sprite.setTexture(texturaIdle);
    }
}

void JugadorIA::bloquear() {
    if (!bloqueando) {
        bloqueando = true;
        frameBloqueo = 0;
        animClock.restart();
        sprite.setTexture(texturaBloqueo);
        sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    }
}

void JugadorIA::atacar() {
    if (!atacando) {
        atacando = true;
        frameAtaque = 0;
        danioAplicado = false;
        animClock.restart();
        sprite.setTexture(texturaAtaque);
        sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    }
}

void JugadorIA::recibirDanio(int cantidad) {
    if (vida > 0) vida -= cantidad;
    recibiendoDanio = true;
    danioClock.restart();
    sprite.setTexture(texturaDanio);
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
}

void JugadorIA::actualizarAnimacion() {
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

sf::Sprite& JugadorIA::getSprite() {
    return sprite;
}

sf::FloatRect JugadorIA::getHitbox() const {
    sf::Vector2f pos = sprite.getPosition();
    float ancho = frameWidth * sprite.getScale().x;
    float alto = frameHeight * sprite.getScale().y;
    float margenX = ancho * 0.2f;
    float margenY = alto * 0.1f;
    return sf::FloatRect(pos.x + margenX, pos.y + margenY, ancho * 0.6f, alto * 0.8f);
}

JugadorIA::Accion JugadorIA::decidirAccion(const EstadoJuego& estadoActual, int profundidad) {
    int mejorValor = std::numeric_limits<int>::min();
    Accion mejorAccion = ATACAR;

    for (Accion accion : generarAccionesPosibles(estadoActual)) {
        EstadoJuego nuevoEstado = simularAccion(estadoActual, accion, true);
        int valor = minimax(nuevoEstado, profundidad - 1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), false);

        if (valor > mejorValor) {
            mejorValor = valor;
            mejorAccion = accion;
        }
    }
    return mejorAccion;
}

int JugadorIA::minimax(EstadoJuego estado, int profundidad, int alpha, int beta, bool esTurnoIA) {
    if (profundidad == 0 || estado.vidaIA <= 0 || estado.vidaJugador <= 0) {
        return evaluarEstado(estado);
    }

    if (esTurnoIA) {
        int mejor = std::numeric_limits<int>::min();
        for (Accion accion : generarAccionesPosibles(estado)) {
            EstadoJuego nuevoEstado = simularAccion(estado, accion, true);
            int valor = minimax(nuevoEstado, profundidad - 1, alpha, beta, false);
            mejor = std::max(mejor, valor);
            alpha = std::max(alpha, valor);
            if (beta <= alpha) break;
        }
        return mejor;
    } else {
        int peor = std::numeric_limits<int>::max();
        for (Accion accion : generarAccionesPosibles(estado)) {
            EstadoJuego nuevoEstado = simularAccion(estado, accion, false);
            int valor = minimax(nuevoEstado, profundidad - 1, alpha, beta, true);
            peor = std::min(peor, valor);
            beta = std::min(beta, valor);
            if (beta <= alpha) break;
        }
        return peor;
    }
}

int JugadorIA::evaluarEstado(const EstadoJuego& estado) {
    int vidaScore = estado.vidaIA - estado.vidaJugador;
    float distancia = std::abs(estado.posX_IA - estado.posX_Jugador);

    const float distanciaIdeal = 200.0f;
    int distanciaScore = -static_cast<int>(std::abs(distancia - distanciaIdeal));

    int bloqueoScore = 0;
    if (estado.vidaIA < 30) {
        bloqueoScore += 10; 
    }

    return vidaScore + distanciaScore + bloqueoScore;

}

std::vector<JugadorIA::Accion> JugadorIA::generarAccionesPosibles(const EstadoJuego& estado) {
    return { MOVER_IZQ, MOVER_DER, ATACAR, BLOQUEAR };
}

JugadorIA::EstadoJuego JugadorIA::simularAccion(const EstadoJuego& estado, Accion accion, bool esTurnoIA) {
    EstadoJuego nuevo = estado;
    if (esTurnoIA) {
        switch (accion) {
            case MOVER_IZQ: nuevo.posX_IA -= 50.f; break;
            case MOVER_DER: nuevo.posX_IA += 50.f; break;
            case ATACAR: nuevo.vidaJugador -= 10; break;
            case BLOQUEAR: nuevo.vidaIA += 1; break;
        }
    } else {
        switch (accion) {
            case MOVER_IZQ: nuevo.posX_Jugador -= 50.f; break;
            case MOVER_DER: nuevo.posX_Jugador += 50.f; break;
            case ATACAR: nuevo.vidaIA -= 10; break;
            case BLOQUEAR: nuevo.vidaJugador += 1; break;
        }
    }
    nuevo.turnoIA = !esTurnoIA;
    return nuevo;
}

void actualizarIA(JugadorIA& ia, int vidaEnemigo, float posicionEnemigoX) {
    JugadorIA::EstadoJuego estadoActual;
    estadoActual.vidaIA = ia.getVida();
    estadoActual.vidaJugador = vidaEnemigo;
    estadoActual.posX_IA = ia.getSprite().getPosition().x;
    estadoActual.posX_Jugador = posicionEnemigoX;
    estadoActual.turnoIA = true;

    JugadorIA::Accion accionElegida = ia.decidirAccion(estadoActual, 1);

    switch (accionElegida) {
        case JugadorIA::MOVER_IZQ: ia.moverIzquierda(); break;
        case JugadorIA::MOVER_DER: ia.moverDerecha(); break;
        case JugadorIA::ATACAR: ia.atacar(); break;
        case JugadorIA::BLOQUEAR: break;
    }
};
