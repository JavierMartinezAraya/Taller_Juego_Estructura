#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class JugadorIA {

    public:
    struct EstadoJuego {
        int vidaIA;
        int vidaJugador;
        float posX_IA;
        float posX_Jugador;
        bool turnoIA;
    };
    JugadorIA(const std::string& rutaIdle, sf::Vector2f posicionInicial, const std::string& rutaAtaque, const std::string& rutaDanio);

    void moverIzquierda();
    void moverDerecha();
    void mover(const sf::Vector2f& desplazamiento);
    void dibujar(sf::RenderWindow& ventana) const;
    void idle();
    void atacar();
    void actualizarAnimacion();
    void recibirDanio(int cantidad);

    bool estaAtacando() const { return atacando; }
    bool estaRecibiendoDanio() const { return recibiendoDanio; }
    bool estaVivo() const { return vida > 0; }
    int getVida() const { return vida; }

    bool getDanioAplicado() const { return danioAplicado; }
    void setDanioAplicado(bool valor) { danioAplicado = valor; }

    sf::FloatRect getHitbox() const;
    sf::Sprite& getSprite();

    // IA
    enum Accion { MOVER_IZQ, MOVER_DER, ATACAR, BLOQUEAR };
    Accion decidirAccion(const EstadoJuego& estadoActual, int profundidad);


private:
    int evaluarEstado(const EstadoJuego& estado); // Evalua el estado actual (heuritica)
    int minimax(EstadoJuego estado, int profundidad, int alpha, int beta, bool esTurnoIA);
    std::vector<Accion> generarAccionesPosibles(const EstadoJuego& estado);
    EstadoJuego simularAccion(const EstadoJuego& estado, Accion accion, bool esTurnoIA);
    void simularAccion(Accion accion);
    void deshacerAccion(Accion accion);

    // Atributos del jugador IA
    int vida = 100;
    bool danioAplicado = false;
    sf::Texture texturaIdle;
    sf::Texture texturaAtaque;
    sf::Texture texturaDanio;
    sf::Sprite sprite;

    int frame = 0;
    int frameCountIdle = 6;
    int frameCountAtaque = 16;
    int frameWidth = 640;
    int frameHeight = 640;

    sf::Clock animClock;
    float frameTimeIdle = 0.15f;
    float frameTimeAtaque = 0.08f;

    bool atacando = false;
    bool recibiendoDanio = false;

    int frameAtaque = 0;
    int frameIdle = 0;

    sf::Clock danioClock;
    float duracionDanio = 0.8f;
};