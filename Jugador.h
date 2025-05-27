#pragma once
#include <SFML/Graphics.hpp>

class Jugador {
public:
    Jugador(const std::string& rutaImagen, sf::Vector2f posicionInicial, const std::string& rutaSheet);
    void mover(const sf::Vector2f& desplazamiento);
    void dibujar(sf::RenderWindow& ventana) const;
    void idle();
    sf::Sprite& getSprite();

private:
    sf::Texture textura;
    sf::Texture texturaIdle;
    sf::Sprite sprite;
    std::string rutaIdleSheet;
    int idleFrame = 0;                // Frame actual
    int idleFrameCount = 6;           // Total de frames (ajusta según tu sprite sheet)
    int idleFrameWidth = 384;          // Ancho de cada frame (ajusta según tu sprite sheet)
    int idleFrameHeight = 384;         // Alto de cada frame (ajusta según tu sprite sheet)
    sf::Clock idleClock;              // Reloj para controlar el tiempo entre frames
    float idleFrameTime = 0.15f;      // Tiempo entre frames en segundos

};
