#include "Opciones.h"

Opciones::Opciones(float width, float height) : volumen(50.0f) { // Se establece un volumen inicial en 50.
    font.loadFromFile("resources/upheavtt.ttf"); // Se carga la fuente desde un archivo perteneciente a la carpeta resources.
    // Se configura el texto del volumen.
    textoVolumen.setFont(font);
    textoVolumen.setFillColor(sf::Color::Yellow);
    textoVolumen.setCharacterSize(70);
    textoVolumen.setPosition(sf::Vector2f(400.0f, 300.0f));
    textoVolumen.setString("Volumen: " + std::to_string((int)volumen));
}

void Opciones::draw(sf::RenderWindow& window) { // Se dibuja el texto del volumen en la pantalla.
    textoVolumen.setString("Volumen: " + std::to_string((int)volumen));
    window.draw(textoVolumen);
}

void Opciones::aumentarVolumen() { //Aumenta el volumen de 5 en 5.
    if (volumen < 100) {
        volumen += 5;
    }
}

void Opciones::disminuirVolumen() {//Disminuye el volumen de 5 en 5.
    if (volumen > 0) {
        volumen -= 5;
    }
}

float Opciones::getVolumen() const {// Devuelve el volumen actual.
    return volumen;
}