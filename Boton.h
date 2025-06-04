#pragma once
#include <SFML/Graphics.hpp>

class Boton {
    public:
        Boton(const std::string& ruta, sf::Vector2f posicion);

        void dibujar(sf::RenderWindow& ventana) const;          // Dibuja el botón
        void framesConfig(int ancho, int alto, int cantidadFrames, float tiempoEntreFrames); // Configura los frames de la animación
        void resetearFrame();
        void actualizar();
        sf::FloatRect getGlobalBounds() const;
        

    
    private:
        sf::Texture textura;          // Textura del botón
        sf::Sprite sprite;            // Sprite del botón
        int frameWidth;               // Ancho de cada frame
        int frameHeight;              // Alto de cada frame
        int frameCount;               // Número total de frames
        int frameActual = 0;          // Frame actual
        float frameTime;              // Tiempo entre frames (segundos)
        sf::Clock animClock;          // Reloj para animación
        sf::FloatRect hitbox;         // Hitbox del botón
};