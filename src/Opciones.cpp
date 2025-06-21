#include "Opciones.h"

Opciones::Opciones(float width, float height) : volumen(50.0f) { 
    font.loadFromFile("resources/upheavtt.ttf"); 

    textoVolumen.setFont(font);
    textoVolumen.setFillColor(sf::Color::Yellow);
    textoVolumen.setCharacterSize(70);
    textoVolumen.setPosition(sf::Vector2f(400.0f, 300.0f));
    textoVolumen.setString("Volumen: " + std::to_string((int)volumen));
}

void Opciones::draw(sf::RenderWindow& window) {
    textoVolumen.setString("Volumen: " + std::to_string((int)volumen));
    window.draw(textoVolumen);
}

void Opciones::aumentarVolumen() {
    if (volumen < 100) {
        volumen += 5;
    }
}

void Opciones::disminuirVolumen() {
    if (volumen > 0) {
        volumen -= 5;
    }
}

float Opciones::getVolumen() const {
    return volumen;
}