#include "PilaJugador.h"

PilaJugador::PilaJugador(int vidaTotal) {
    for (int i = 0; i < vidaTotal; ++i) {
        pila.push(1);
    }
}

// Recibe un tipo de golpe y pierde una cantidad de vida
void PilaJugador::recibirGolpe(int cantidadDañoRecibido) {
    for (int i = 0; i < cantidadDañoRecibido && !pila.empty(); ++i) {
        pila.pop();
    }
}

// Se cura el jugador y gana x cantidad de vida
void PilaJugador::curar() {
    if(pila.size() < vidaTotal)
        pila.push(1);
}

// Devuelve la vida actual de jugador
int PilaJugador::vidaActual() const {
    return pila.size();
}

// Verifica si es que el jugador esta muerto
bool PilaJugador::estaMuerto() const {
    return pila.empty();
}