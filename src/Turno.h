#ifndef TURNO_H
#define TURNO_H

#include "Jugador.h"

class Turno {

private:

    unsigned int turno;
    Jugador *jugador;

public:

    /*
     * pre : -
     * post: Inicializa un turno con el turno en 1 y con el jugador dado.
     */
    explicit Turno(Jugador *jugador);

    /*
     * pre : -
     * post: Libera los recursos asociados al Turno.
     */
    ~Turno();

    /*
     * pre : -
     * post: Devuelve el turno actual.
     */
    unsigned int obtenerTurno() const;

    /*
     * pre : Debe haber un jugador asignado al turno actual (@throws logic_error).
     * post: Devuelve un puntero al jugador asignado al turno actual.
     */
    Jugador *obtenerJugador() const;

    /*
     * pre : El jugador dado debe existir (@throws logic_error).
     * post: Avanza al siguiente turno y reemplaza al jugador asignado, por el dado.
     */
    void avanzarTurno(Jugador *jugador);
};

#endif //TURNO_H
