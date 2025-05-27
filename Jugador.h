#pragma once
#include <SFML/Graphics.hpp>

class Jugador {
public:
    Jugador(const std::string& rutaImagen, sf::Vector2f posicionInicial);

    void mover(const sf::Vector2f& desplazamiento);

    void dibujar(sf::RenderWindow& ventana) const;
    sf::Sprite& getSprite();

private:
    sf::Texture textura;
    sf::Sprite sprite;
};