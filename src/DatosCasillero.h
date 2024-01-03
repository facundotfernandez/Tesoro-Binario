#ifndef DATOSCASILLERO_H_
#define DATOSCASILLERO_H_

#include "Vector.h"
#include "Constantes.h"

/* Declaración anticipada de la clase Casillero. */
class Casillero;

/* Declaración anticipada de la clase Jugador. */
class Jugador;

class DatosCasillero {

private:

    unsigned int tipo;

    /* Jugador que lo ocupa */
    Jugador *jugador;

    /* Turno en que deja de estar inactivo. */
    unsigned int inactivoHasta;

    /* Turno en que deja de estar protegido. */
    unsigned int protegidoHasta;

    /* Casillero partido asociado a este. */
    Casillero *asociado;

public:

    /*
     * pre : -
     * post: Inicializa una descripción con tipo VACIO y idJugador en ID_GLOBAL.
     * 		 El resto de sus atributos y coordenadas del Casillero asociado serán inicializados en cero o NULL.
     */
    DatosCasillero();

    /*
     * pre : -
     * post: Libera los recursos asociados a la descripción.
     */
    virtual ~DatosCasillero();

    /*
     * pre : -
     * post: Devuelve el tipo del Casillero.
     */
    unsigned int obtenerTipo() const;

    /*
     * pre : -
     * post: Devuelve el jugador que ocupa el Casillero.
     */

    Jugador *obtenerJugador() const;

    /*
     * pre : -
     * post: Devuelve el inactivoHasta del Casillero.
     */
    unsigned int obtenerInactivoHasta() const;

    /*
     * pre : -
     * post: Devuelve el protegidoHasta del Casillero.
     */
    unsigned int obtenerProtegidoHasta() const;

    /*
     * pre : -
     * post: Devuelve un puntero al Casillero asociado a este.
     */
    Casillero *obtenerAsociado() const;

    /*
     * pre : -
     * post: Reemplaza el tipo del Casillero, con el  dado.
     */
    void reemplazarTipo(unsigned int tipo);

    /*
     * pre : -
     * post: Reemplaza el Jugador ocupante, con el dado.
     */
    void reemplazarJugador(Jugador *jugador);

    /*
     * pre : -
     * post: Reemplaza el inactivoHasta del Casillero, con el dado.
     */
    void reemplazarInactivoHasta(unsigned int inactivoHasta);

    /*
     * pre : -
     * post: Reemplaza el protegidoHasta del Casillero, con el dado.
     */
    void reemplazarProtegidoHasta(unsigned int protegidoHasta);

    /*
     * pre : El Casillero debe existir (@throws logic_error).
     * post: Reemplaza el Casillero asociado, con el dado.
     */
    void reemplazarAsociado(Casillero *asociado);

};

#endif /* DATOSCASILLERO_H_ */
