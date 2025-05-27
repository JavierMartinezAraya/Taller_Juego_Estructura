#pragma once
#include <stack>

class PilaJugador {
public:
    PilaJugador(int vidaTotal = 10); // Constructor que inicializa la vida del jugdaor
    void recibirGolpe(int cantidadDañoRecibido); // El jugador pierde vida al recibir un golpe
    void curar();  // El jugador se cura sin sobrepasar su vida maxia
    int vidaActual() const; // Devuelve la vida actual del jugador
    bool estaMuerto() const; // Verifica si es que el jugador esta muerto

private:
    std::stack<int> pila;  // Pila que representa los puntos de vida del jugador
    int vidaTotal;         // Vida maxima que puede tener le jugador
};