#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

/**
 * @brief Clase que representa un jugador controlado por IA en el juego
 */
class JugadorIA {

    public:
    /**
     * @brief Estructura que representa el estado del juego para la IA
     */
    struct EstadoJuego {
        int vidaIA;
        int vidaJugador;
        float posX_IA;
        float posX_Jugador;
        bool turnoIA;
    };

     /**
     * @brief Constructor de JugadorIA
     * @param rutaIdle Ruta de la textura idle
     * @param posicionInicial Posicion inicial del sprite
     * @param rutaAtaque Ruta de la textura de ataque
     * @param rutaDanio Ruta de la textura de danio
     * @param rutaBloqueo Ruta de la textura de bloqueo
     */
    JugadorIA(const std::string& rutaIdle, sf::Vector2f posicionInicial, const std::string& rutaAtaque, const std::string& rutaDanio, const std::string& rutaBloqueo);

    /**
     * @brief Mueve la IA hacia la izquierda
     */
    void moverIzquierda();

    /**
     * @brief Mueve la IA hacia la derecha
     */
    void moverDerecha();

    /**
     * @brief Mueve la IA según un desplazamiento dado
     * @param desplazamiento Vector de desplazamiento
     */
    void mover(const sf::Vector2f& desplazamiento);

    /**
     * @brief Dibuja el sprite de la IA en la ventana
     * @param ventana Referencia a la ventana SFML
     */
    void dibujar(sf::RenderWindow& ventana) const;

    /**
     * @brief Cambia la animacion a idle si no esta atacando
     */
    void idle();

    /**
     * @brief Inicia la animacion de ataque
     */
    void atacar();

    /**
     * @brief Inicia la animacion de bloqueo
     */
    void bloquear();

    /**
     * @brief Actualiza la animacion segun el estado actual
     */
    void actualizarAnimacion();

    /**
     * @brief Aplica danio a la IA
     * @param cantidad Cantidad de danio recibido
     */
    void recibirDanio(int cantidad);

    /**
     * @brief Indica si la IA esta atacando
     * @return true si esta atacando, false en caso contrario
     */
    bool estaAtacando() const { return atacando; }

    /**
     * @brief Indica si la IA esta bloqueando
     * @return true si esta bloqueando, false en caso contrario
     */
    bool estaBloqueando() const { return bloqueando; } 

    /**
     * @brief Indica si la IA esta recibiendo daño
     * @return true si esta recibiendo danio, false en caso contrario
     */
    bool estaRecibiendoDanio() const { return recibiendoDanio; }

    /**
     * @brief Indica si la IA sigue viva
     * @return true si la vida es mayor a 0
     */
    bool estaVivo() const { return vida > 0; }

    /**
     * @brief Obtiene la vida actual de la IA
     * @return Valor de vida
     */
    int getVida() const { return vida; }

    /**
     * @brief Indica si el danio ya fue aplicado en el ataque actual
     * @return true si el danio fue aplicado
     */
    bool getDanioAplicado() const { return danioAplicado; }

    /**
     * @brief Establece si el danio fue aplicado en el ataque actual
     * @param valor true si el danio fue aplicado
     */
    void setDanioAplicado(bool valor) { danioAplicado = valor; }

    /**
     * @brief Devuelve la hitbox del personaje para colisiones
     * @return Rectangulo flotante con la hitbox
     */
    sf::FloatRect getHitbox() const;

    /**
     * @brief Devuelve el sprite de la IA
     * @return Referencia al sprite
     */
    sf::Sprite& getSprite();

    /**
     * @brief Acciones posibles de la IA
     */
    enum Accion { MOVER_IZQ, MOVER_DER, ATACAR, BLOQUEAR };

    /**
     * @brief Decide la mejor acción para la IA usando Minimax
     * @param estadoActual Estado actual del juego
     * @param profundidad Profundidad maxima de búsqueda
     * @return Accion optima para la IA
     */
    Accion decidirAccion(const EstadoJuego& estadoActual, int profundidad);

private:
    /**
     * @brief Evalua el estado actual del juego (heuristica)
     * @param estado Estado a evaluar
     * @return Valor heuristico del estado
     */
    int evaluarEstado(const EstadoJuego& estado);

    /**
     * @brief Algoritmo Minimax con poda alfa-beta
     * @param estado Estado actual
     * @param profundidad Profundidad restante
     * @param alpha Valor alfa para poda
     * @param beta Valor beta para poda
     * @param esTurnoIA Indica si es el turno de la IA
     * @return Valor heuristico del mejor movimiento
     */
    int minimax(EstadoJuego estado, int profundidad, int alpha, int beta, bool esTurnoIA);

    /**
     * @brief Genera las acciones posibles para el estado actual
     * @param estado Estado actual
     * @return Vector de acciones posibles
     */
    std::vector<Accion> generarAccionesPosibles(const EstadoJuego& estado);

    /**
     * @brief Simula el resultado de realizar una acción en el estado actual
     * @param estado Estado actual
     * @param accion Acción a simular
     * @param esTurnoIA Indica si es el turno de la IA
     * @return Nuevo estado tras aplicar la accion
     */
    EstadoJuego simularAccion(const EstadoJuego& estado, Accion accion, bool esTurnoIA);

    void simularAccion(Accion accion);
    void deshacerAccion(Accion accion);

    // Atributos del jugador IA
    int vida = 100;
    bool danioAplicado = false;
    sf::Texture texturaIdle;
    sf::Texture texturaAtaque;
    sf::Texture texturaDanio;
    sf::Texture texturaBloqueo;
    sf::Sprite sprite;

    int frame = 0;
    int frameCountIdle = 6;
    int frameCountAtaque = 16;
    int frameCountBloqueo = 16;
    int frameWidth = 640;
    int frameHeight = 640;

    sf::Clock animClock;
    float frameTimeIdle = 0.15f;
    float frameTimeAtaque = 0.08f;
    float frameTimeBloqueo = 0.08f;

    bool atacando = false;
    bool bloqueando = false;
    bool recibiendoDanio = false;

    int frameAtaque = 0;
    int frameBloqueo = 0; 
    int frameIdle = 0;

    sf::Clock danioClock;
    float duracionDanio = 0.8f;
};