#pragma once
#include "Boton.h"
#include <iostream>
#include <SFML/Graphics.hpp>

#define MaxMainMenu 4
class MainMenu{

    public:
        MainMenu(float width, float height);

        void draw(sf::RenderWindow& windows);
        void MoveUp();
        void MoveDown();

        int MainMenuPressed(){
            return MainMenuSelected;
        }
        void resetSeleccion();
        ~MainMenu();

    private:
        Boton botonJugar; // Botón de jugar
        int MainMenuSelected;
        sf::Font font;
        sf::Text mainMenu[MaxMainMenu];
};