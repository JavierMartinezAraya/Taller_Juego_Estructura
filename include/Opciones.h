#pragma once
#include <SFML/Graphics.hpp>

/**
 * @brief Clase que gestiona las opciones de configuracion, como el volumen, en el juego
 */
class Opciones { 
public:
    /**
     * @brief Constructor de Opciones
     * @param width Ancho de la ventana
     * @param height Alto de la ventana
     */
    Opciones(float width, float height);

    /**
     * @brief Dibuja el texto del volumen en la ventana
     * @param window Referencia a la ventana SFML
     */
    void draw(sf::RenderWindow& window);;

    /**
     * @brief Sube el volumen
     */
    void aumentarVolumen();

    /**
     * @brief Baja el volumen
     */
    void disminuirVolumen();

    /**
     * @brief Devuelve el valor actual del volumen
     * @return Valor del volumen
     */
    float getVolumen() const;

private:
    float volumen;          // Volumen actual del juego
    sf::Font font;          // Fuente para el texto
    sf::Text textoVolumen;  // Texto que muestra el volumen actual
};