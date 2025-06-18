#include "JugadorIA.h"
#include <iostream>
#include <limits>

// Constructor
JugadorIA::JugadorIA(const std::string& rutaIdle, sf::Vector2f posicionInicial, const std::string& rutaAtaque, const std::string& rutaDanio) {
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
// Dibujar el sprirte del jugador IA
void JugadorIA::dibujar(sf::RenderWindow& ventana) const {
    ventana.draw(sprite);
}

// Movimiento
void JugadorIA::mover(const sf::Vector2f& desplazamiento) {
    sprite.move(desplazamiento);
}
// Mover a la izquierda
void JugadorIA::moverIzquierda() {
    mover(sf::Vector2f(-50.0f, 0.0f));
}
// Mover a la derecha
void JugadorIA::moverDerecha() {
    mover(sf::Vector2f(50.0f, 0.0f));
}

// Animacion idle
void JugadorIA::idle() {
    if (!atacando) {
        sprite.setTexture(texturaIdle);
    }
}

// Animacion de ataque
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

// Animacion cuando recibe danio
void JugadorIA::recibirDanio(int cantidad) {
    if (vida > 0) vida -= cantidad;
    recibiendoDanio = true;
    danioClock.restart();
    sprite.setTexture(texturaDanio);
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
}

// Actualiza la animacion según el estado actual
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
    } else {
        if (animClock.getElapsedTime().asSeconds() > frameTimeIdle) {
            frameIdle = (frameIdle + 1) % frameCountIdle;
            sprite.setTextureRect(sf::IntRect(frameIdle * frameWidth, 0, frameWidth, frameHeight));
            animClock.restart();
        }
    }
}

// Devuelve el sprite del jugador IA
sf::Sprite& JugadorIA::getSprite() {
    return sprite;
}

// Devuelve la hitbox del personaje para colisiones
sf::FloatRect JugadorIA::getHitbox() const {
    sf::Vector2f pos = sprite.getPosition();
    float ancho = frameWidth * sprite.getScale().x;
    float alto = frameHeight * sprite.getScale().y;
    float margenX = ancho * 0.2f;
    float margenY = alto * 0.1f;
    return sf::FloatRect(pos.x + margenX, pos.y + margenY, ancho * 0.6f, alto * 0.8f);
}

// IA - Minimax (estructura basica)
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

// Algoritmo minimax con poda alfa-beta para explorar el arbol de decisiones
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

// Heuristica: evalua el estado del juego (vida y distancia)
int JugadorIA::evaluarEstado(const EstadoJuego& estado) {
    int vidaScore = estado.vidaIA - estado.vidaJugador;
    float distancia = std::abs(estado.posX_IA - estado.posX_Jugador);

    // Penaliza estar lejos del oponente (por ejemplo, si quieres priorizar acercarse)
    const float distanciaIdeal = 200.0f;
    int distanciaScore = -static_cast<int>(std::abs(distancia - distanciaIdeal));

    // Puedes ponderar cada factor según lo que quieras priorizar
    return vidaScore + distanciaScore;

}
// Genera las acciones posibles para el estado actual
std::vector<JugadorIA::Accion> JugadorIA::generarAccionesPosibles(const EstadoJuego& estado) {
    return { MOVER_IZQ, MOVER_DER, ATACAR, BLOQUEAR };
}
// Simula el resultado de realizar una accion en el estado actual
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

// Funcion aux para actualizar la IA en el juego
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
