#pragma once
#include <SFML/Graphics.hpp>

/**
 * @brief Clase que representa a un jugador controlado por el usuario en el juego
 */
class Jugador {
public:
    /**
     * @brief Constructor de Jugador
     * @param rutaIdle Ruta de la textura idle
     * @param posicionInicial Posicion inicial del sprite
     * @param rutaAtaque Ruta de la textura de ataque
     * @param rutaDanio Ruta de la textura de danio
     * @param rutaBloqueo Ruta de la textura de bloqueo
     */
    Jugador(const std::string& rutaIdle, sf::Vector2f posicionInicial, const std::string& rutaAtaque, const std::string& rutaDanio, const std::string& rutaBloqueo);
    
    /**
     * @brief Indica si el jugador esta bloqueando
     * @return true si esta bloqueando, false en caso contrario
     */
    bool estaBloqueando() const { return bloqueando; }
    
    /**
     * @brief Indica si el jugador esta atacando
     * @return true si esta atacando, false en caso contrario
     */
    bool estaAtacando() const { return atacando; }
    
    /**
     * @brief Indica si el jugador esta recibiendo danio
     * @return true si está recibiendo danio, false en caso contrario
     */    
    bool estaRecibiendoDanio() const { return recibiendoDanio; }
    
    /**
     * @brief Indica si el jugador esta vivo
     * @return true si la vida es mayor a 0
     */   
    bool estaVivo() const { return vida > 0; }
    
    /**
     * @brief Obtiene la vida actual del jugdor
     * @return Valor de vida
     */
    int getVida() const { return vida; }

    /**
     * @brief Indica si el jugador esta esperando una accion
     * @return true si esta esperando, false en caso contrario
     */
    bool esperandoAccion = true;

    /**
     * @brief Mueve al jugador hacia la izquierda
     */
    void moverIzquierda();

    /**
     * @brief Mueve al jugador hacia la derecha
     */
    void moverDerecha();

    /**
     * @brief Mueve al jugador segun un desplazamiento dado
     * @param desplazamiento Vector de desplazamiento
     */
    void mover(const sf::Vector2f& desplazamiento);

    /**
     * @brief Dibuja el sprite del jugador en la ventana
     * @param ventana Referencia a la ventana SFML
     */
    void dibujar(sf::RenderWindow& ventana) const;

    /**
     * @brief Cambia la animación a idle si no está atacando o bloqueando
     */
    void idle();

    /**
     * @brief Inicia la animacion de bloqueo
     */
    void bloquear();

    /**
     * @brief Inicia la animacion de ataque
     */
    void atacar();

    /**
     * @brief Actualiza la animacion segun el estado actual
     */
    void actualizarAnimacion();
    
    /**
     * @brief Obtiene el estado de danio aplicado
     * @return true si el danio fue aplicado, false en caso contrario
     */
    bool getDanioAplicado() const { return danioAplicado; }

    /**
     * @brief Establece si el danio fue aplicado en el ataque actual
     * @param valor true si el danio fue aplicado
     */
    void setDanioAplicado(bool valor) { danioAplicado = valor; }

    /**
     * @brief Aplica danio al jugador
     * @param cantidad Cantidad de danio a aplicar
     */
    void recibirDanio(int cantidad);      

    /**
     * @brief Obtiene el frame actual de la animacion de ataque.
     * @return Frame de ataque.
     */
    int getFrameAtaque() const { return frameAtaque; }

    /**
     * @brief Devuelve la hitbox del personaje para colisiones
     * @return Rectangulo flotante con la hitbox.
     */
    sf::FloatRect getHitbox() const;

    /**
     * @brief Devuelve el sprite del jugador
     * @return Referencia al sprite
     */
    sf::Sprite& getSprite();

private:

    int vida = 100;
    bool danioAplicado = false;
    sf::Texture texturaIdle;
    sf::Texture texturaBloqueo;
    sf::Texture texturaAtaque;
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
    int frameAtaque = 0;
    int frameBloqueo = 0;
    int frameIdle = 0;    

    sf::Texture texturaDanio;
    bool recibiendoDanio = false;
    sf::Clock danioClock;
    float duracionDanio = 0.8f;

};
