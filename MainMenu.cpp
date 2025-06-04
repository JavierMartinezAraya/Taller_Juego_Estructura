#include "MainMenu.h"


// Constructor del menu
MainMenu::MainMenu(float width, float height)
:botonJugar("resources/Jugar_Button-Sheet.png", sf::Vector2f(400.0f, 100.0f)){
    
    // Cargar tipo de letra a usar
    font.loadFromFile("resources/upheavtt.ttf");

    // Jugar
    mainMenu[0];{
        botonJugar.framesConfig(400, 160, 49, 0.015f); // Configura los frames del boton
        botonJugar.actualizar();
    }
    
    // Opciones
    mainMenu[1].setFont(font);
    mainMenu[1].setFillColor(sf::Color::White);
    mainMenu[1].setString("Options");
    mainMenu[1].setCharacterSize(70);
    mainMenu[1].setPosition(sf::Vector2f(400.0f, 300.0f));

    // Acerca de
    mainMenu[2].setFont(font);
    mainMenu[2].setFillColor(sf::Color::White);
    mainMenu[2].setString("About");
    mainMenu[2].setCharacterSize(70);
    mainMenu[2].setPosition(sf::Vector2f(400.0f, 400.0f));

    // Salir
    mainMenu[3].setFont(font);
    mainMenu[3].setFillColor(sf::Color::White);
    mainMenu[3].setString("Exit");
    mainMenu[3].setCharacterSize(70);
    mainMenu[3].setPosition(sf::Vector2f(400.0f, 500.0f));

    MainMenuSelected = -1;

}

// Destructor del menu
MainMenu::~MainMenu(){

}
// Dibujar el menu
void MainMenu::draw(sf::RenderWindow& Window){
    // Detectar si el mouse está sobre el botón
    sf::Vector2i mousePos = sf::Mouse::getPosition(Window);
    sf::FloatRect bounds = botonJugar.getGlobalBounds();
    if (bounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
        botonJugar.actualizar(); // Solo anima si el mouse está encima
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            MainMenuSelected = 0; // Asignar el índice del botón "Jugar"
        }
    } else {
        botonJugar.resetearFrame(); // Si no, vuelve al primer frame
    }
    botonJugar.dibujar(Window);

    for (int i = 1; i < MaxMainMenu; i++){
        Window.draw(mainMenu[i]);
    }
}

// Mover eleccion hacia arriba
void MainMenu::MoveUp(){

    if(MainMenuSelected - 1 >= 0){
        mainMenu[MainMenuSelected].setFillColor(sf::Color::White);

        MainMenuSelected--;
        if(MainMenuSelected == -1){
            MainMenuSelected = 2;
        }
        mainMenu[MainMenuSelected].setFillColor(sf::Color::Cyan);
    } 
}

// Mover eleccion hacia abajo
void MainMenu::MoveDown(){
    if(MainMenuSelected + 1 <= MaxMainMenu){
        mainMenu[MainMenuSelected].setFillColor(sf::Color::White);

        MainMenuSelected++;
        if(MainMenuSelected == 4){
            MainMenuSelected = 0;
        }
        mainMenu[MainMenuSelected].setFillColor(sf::Color::Cyan);
    } 
}

// Reestablecer la eleccion del menu
void MainMenu::resetSeleccion() {
    mainMenu[MainMenuSelected].setFillColor(sf::Color::White);
    MainMenuSelected = 0;
    mainMenu[MainMenuSelected].setFillColor(sf::Color::Cyan);
}

