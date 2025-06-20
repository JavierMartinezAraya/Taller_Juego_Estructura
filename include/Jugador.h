#pragma once
#include <SFML/Graphics.hpp>

class Jugador {
public:
    Jugador(const std::string& rutaIdle, sf::Vector2f posicionInicial, const std::string& rutaAtaque, const std::string& rutaDanio, const std::string& rutaBloqueo);
    bool estaBloqueando() const { return bloqueando; }              // Devuelve true si el jugador está bloqueando
    bool estaAtacando() const { return atacando; }                  // Devuelve true si el jugador está atacando
    bool estaRecibiendoDanio() const { return recibiendoDanio; }
    bool estaVivo() const { return vida > 0; }                      // Devuelve true si el jugador tiene vida
    int getVida() const { return vida; }                            // Devuelve la vida actual del jugador
    bool esperandoAccion = true;                                    // Si el jugador está esperando una acción
    void moverIzquierda();                                          // Mueve al jugador a la izquierda
    void moverDerecha();                                            // Mueve al jugador a la derecha
    void mover(const sf::Vector2f& desplazamiento);                 // Se mueve al jugador
    void dibujar(sf::RenderWindow& ventana) const;                  // Dibuja al jugador
    void idle();                                                    // Jugador en reposo
    void bloquear();                                                // Jugador bloqueando
    void atacar();                                                  // Jugador atacando
    void actualizarAnimacion();                                     // Actualiza la animación del jugador
    bool getDanioAplicado() const { return danioAplicado; }         // Verifica si se aplico el danio
    void setDanioAplicado(bool valor) { danioAplicado = valor; }    // Establece si se aplicó el danio
    void recibirDanio(int cantidad);                                // Recibe danio, reduce la vida del jugador       
    int getFrameAtaque() const { return frameAtaque; }              // Resta la vida al jugador
    sf::FloatRect getHitbox() const;                                // Devuelve la hitbox del jugador
    sf::Sprite& getSprite();                                        // Devuelve el sprite del jugador

private:

    int vida = 100;                     // Vida del jugador
    bool danioAplicado = false;         // Indica si se aplico danio
    sf::Texture texturaIdle;            // Textura del estado en reposo
    sf::Texture texturaBloqueo;         // Textura del estado de bloqueo
    sf::Texture texturaAtaque;          // Textura del estado de ataque
    sf::Sprite sprite;                  // Sprite del jugador

    int frame = 0;                      // Frame actual de la animacion
    int frameCountIdle = 6;             // frames de idle (reposo)
    int frameCountAtaque = 16;          // frames de ataque
    int frameCountBloqueo = 16;          // frames de bloqueo
    int frameWidth = 640;               // ancho de frame (igual para ambos)
    int frameHeight = 640;              // alto de frame (igual para ambos)
    sf::Clock animClock;                // Reloj para controlar el tiempo de animacion
    float frameTimeIdle = 0.15f;        // Tiempo por frame en idle (reposo)
    float frameTimeAtaque = 0.08f;      // Tiempo por frame en ataque
    float frameTimeBloqueo = 0.08f;     // Tiempo por frame en bloqueo
    bool atacando = false;              // Si el jugador está atacando
    bool bloqueando = false;            // Si el jugador está bloqueando
    int frameAtaque = 0;                // Frame actual de la animacion de ataque
    int frameBloqueo = 0;               // Frame actual de la animacion de bloqueo
    int frameIdle = 0;                  // Frame actual de la animacion de idle (reposo)

    sf::Texture texturaDanio;           // Textura del estado de danio
    bool recibiendoDanio = false;       // Indica si el jugador está recibiendo danio
    sf::Clock danioClock;               // Reloj para controlar el tiempo de danio
    float duracionDanio = 0.8f;         // Duración del estado de danio

};
