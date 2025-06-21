#include "MainMenu.h"

MainMenu::MainMenu(float width, float height){
    
    font.loadFromFile("resources/upheavtt.ttf");

    mainMenu[0].setFont(font);
    mainMenu[0].setFillColor(sf::Color::White);
    mainMenu[0].setString("Play PvP");
    mainMenu[0].setCharacterSize(70);
    mainMenu[0].setPosition(sf::Vector2f(400.0f, 200.0f));

    mainMenu[1].setFont(font);
    mainMenu[1].setFillColor(sf::Color::White);
    mainMenu[1].setString("Play PvE");
    mainMenu[1].setCharacterSize(70);
    mainMenu[1].setPosition(sf::Vector2f(400.0f, 300.0f));

    mainMenu[2].setFont(font);
    mainMenu[2].setFillColor(sf::Color::White);
    mainMenu[2].setString("Options");
    mainMenu[2].setCharacterSize(70);
    mainMenu[2].setPosition(sf::Vector2f(400.0f, 400.0f));

    mainMenu[3].setFont(font);
    mainMenu[3].setFillColor(sf::Color::White);
    mainMenu[3].setString("About");
    mainMenu[3].setCharacterSize(70);
    mainMenu[3].setPosition(sf::Vector2f(400.0f, 500.0f));

    mainMenu[4].setFont(font);
    mainMenu[4].setFillColor(sf::Color::White);
    mainMenu[4].setString("Exit");
    mainMenu[4].setCharacterSize(70);
    mainMenu[4].setPosition(sf::Vector2f(400.0f, 600.0f));

    MainMenuSelected = -1;

}

MainMenu::~MainMenu(){

}

void MainMenu::draw(sf::RenderWindow& Window){
    for (int i = 0; i < MaxMainMenu; i++){
        Window.draw(mainMenu[i]);
    }
}

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

void MainMenu::MoveDown(){
    if(MainMenuSelected + 1 <= MaxMainMenu){
        mainMenu[MainMenuSelected].setFillColor(sf::Color::White);

        MainMenuSelected++;
        if(MainMenuSelected == 5){
            MainMenuSelected = 0;
        }
        mainMenu[MainMenuSelected].setFillColor(sf::Color::Cyan);
    } 
}

void MainMenu::resetSeleccion() {
    mainMenu[MainMenuSelected].setFillColor(sf::Color::White);
    MainMenuSelected = 0;
    mainMenu[MainMenuSelected].setFillColor(sf::Color::Cyan);
}