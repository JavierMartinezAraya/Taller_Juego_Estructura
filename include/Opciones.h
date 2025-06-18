#pragma once
#include <SFML/Graphics.hpp>

class Opciones { 
public:
    Opciones(float width, float height);

    void draw(sf::RenderWindow& window);; //Dibuja el texto del volumen.
    void aumentarVolumen(); // Sube el volumen.
    void disminuirVolumen(); // Baja el volumen.

    float getVolumen() const; //Devuelve el valor actual del volumen.

private:
    float volumen;
    sf::Font font; // Se carga la fuente a través de un objeto Font.
    sf::Text textoVolumen;  // Se carga el texto del volumen.
};