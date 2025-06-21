#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

/**
 * @brief Clase que representa el menu principal del juego
 *
 * Usando SFML se crea un menu principal con opciones
 */
#define MaxMainMenu 5
class MainMenu{

    public:
        /**
         * @brief Constructor de MainMenu
         * @param width Ancho de la ventana
         * @param height Alto de la ventana
         */
        MainMenu(float width, float height);

        /**
         * @brief Dibuja el menu principal en la ventana
         * @param windows Referencia a la ventana SFML
         */
        void draw(sf::RenderWindow& windows);

        /**
         * @brief Mueve la seleccion del menu hacia arriba
         */
        void MoveUp();

        /**
         * @brief Mueve la seleccion del menu hacia abajo
         */
        void MoveDown();

        /**
         * @brief Devuelve el indice de la opcion seleccionada actualmente
         * @return Indice de la opcion seleccionada
         */
        int MainMenuPressed(){
            return MainMenuSelected;
        }
        /**
         * @brief Reinicia la seleccion del menu a la opcion inicial
         */
        void resetSeleccion();

        /**
         * @brief Destructor de MainMenu
         */
        ~MainMenu();

    private:
        int MainMenuSelected;           // Opcion seleccionada actualmente
        sf::Font font;                  // Fuente para el texto
        sf::Text mainMenu[MaxMainMenu]; // Array con las opciones del menu
};