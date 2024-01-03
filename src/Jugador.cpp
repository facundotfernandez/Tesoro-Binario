#ifndef JUGADOR_H
#define JUGADOR_H

#include <cstdlib>
#include "Lista.h"
#include "Casillero.h"
#include "Carta.h"
#include "DatosJugador.h"

class Jugador {

private:

    /* Casilleros ocupados por el Jugador. */
    Lista<Casillero *> *casilleros;

    /* Cartas en la mano del Jugador. */
    Lista<Carta *> *cartas;

    /* Datos del Jugador. */
    DatosJugador *descripcion;

    /* Robos en proceso. */
    Lista<Casillero *> *inactivos;

    /* Tesoros protegidos. */
    Lista<Casillero *> *protegidos;

    /* El Jugador no ha sido eliminado. */
    bool enJuego;

    /* Ha contratado mercenarios. */
    bool mercenariosContratados;

public:

    /*
     * pre : -
     * post: Inicializa un jugador identificado con el identificador dado.
     * 		 No posee Cartas, tesoros, Casilleros, ni cambios registrados.
     */
    explicit Jugador(unsigned int id);

    /*
     * pre : -
     * post: Libera los recursos asociados al Jugador.
     */
    virtual ~Jugador();

    /*
     * pre : -
     * post: Devuelve los Casilleros ocupados por el Jugador.
     */
    Lista<Casillero *> *obtenerCasilleros() const;

    /*
     * pre : -
     * Post: Devuelve las Cartas en la mano del Jugador.
     */
    Lista<Carta *> *obtenerCartas() const;

    /*
     * pre : -
     * post: Devuelve el identificador del Jugador.
     */
    unsigned int obtenerId() const;

    /*
     * pre : -
     * post: Devuelve la cantidad de Cartas en la mano del Jugador.
     */
    unsigned int obtenerCantidadDeCartas() const;

    /*
     * pre : -
     * post: Devuelve la cantidad de tesoros activos del Jugador.
     */
    unsigned int obtenerCantidadDeTesorosActivos() const;

    /*
     * pre : -
     * post: Devuelve la cantidad de tesoros robados por el Jugador.
     */
    unsigned int obtenerCantidadDeTesorosRobados() const;

    /*
     * pre : Las coordenadas deben corresponder a un Casillero ocupado por el Jugador (@throws out_of_range).
     * 		El Vector debe existir y debe tener exactamente 3 elementos (@throws logic_error).
     * post: Devuelve el Casillero del Jugador que corresponda a las coordenadas dadas.
     */
    Casillero *obtenerCasillero(Vector<unsigned int> *coordenadas) const;

    /*
     * pre : El Casillero debe existir (@throws logic_error).
     *       El Casillero debe estar siendo ocupado por el Jugador (@throws logic_error).
     * post: Agrega el Casillero al registro de ocupados por el Jugador.
     *       Actualiza el registro de tesoros activos y/o de protegidos/inactivos según corresponda.
     */
    void agregarCasillero(Casillero *aAgregar);

    /*
     * pre : El Casillero debe existir (@throws logic_error).
     *       El Casillero debe estar siendo ocupado por el Jugador (@throws logic_error).
     *       El casillero, si es de tipo protegido, debe estar en el registro de protegidos (@throws out_of_range).
     * post: Remueve el Casillero del registro de ocupados por el Jugador.
     *       Actualiza el registro de tesoros activos y/o de protegidos/inactivos según corresponda.
     */
    void removerCasillero(Casillero *aRemover);

    /*
     * pre : La Carta debe existir (@throws logic_error).
     *       La mano del jugador NO puede estar llena (@throws out_of_range).
     * post: Agrega una carta a la mano.
     */
    void agregarCarta(Carta *carta);

    /*
     * pre : La mano NO puede estar vacía (@throws out_of_range).
     * post: Elimina la carta de la mano, libera los recursos asociados a la carta dependiendo de 'eliminar'.
     */
    bool usarCarta(unsigned int tipo, bool eliminar = true);

    /*
     * pre : Debe tener cartas en su mano (@throws out_of_range).
     * post: Obtiene una carta aleatoria de la mano del Jugador.
     */
    Carta *obtenerCartaAleatoria() const;

    /*
     * Pre: -
     * Post: Agrega un cambio al registro de cambios.
     *       Actualiza el registro de tesoros robados según corresponda.
     */
    void agregarCambio(string cambio);

    /*
     * pre : -
     * post: Devuelve el registro de cambios.
     */
    Pila<string> *obtenerCambios();

    /*
     * pre : -
     * post: Establece el estado del jugador como eliminado, sin borrar su información.
     */
    void dejaDeJugar();

    /*
     * pre : -
     * post: Devuelve el estado del jugador.
     */
    bool sigueEnJuego() const;

    /*
     * pre : -
     * post: Indica si tiene mercenarios contratados.
     */
    bool tieneMercenarios() const;

    /*
     * pre : NO debe tener mercenarios contratados (@throws logic_error).
     * post: Asigna una tropa de mercenarios al Jugador.
     */
    void contratarMercenarios();

    /*
     * pre : Debe tener mercenarios contratados (@throws logic_error).
     * post: Elimina la tropa de mercenarios del Jugador.
     */
    void usarMercenarios();

    /*
     * pre : -
     * post: Devuelve el registro de robos en proceso.
     */
    Lista<Casillero *> *obtenerInactivos();

    /*
     * pre : -
     * post: Devuelve el registro de tesoros protegidos.
     */
    Lista<Casillero *> *obtenerProtegidos();

    /*
     * Pre: El Casillero debe existir (@throws logic_error).
     *      El Casillero debe ser de tipo protegido (@throws logic_error).
     * Post: Agrega el casillero dado al registro de protegidos.
     */
    void agregarProtegido(Casillero *aAgregar);

    /*
     * Pre: El Casillero debe existir (@throws logic_error).
     *      El Casillero debe ser de tipo protegido (@throws logic_error).
     * Post: Remueve el casillero dado al registro de protegidos.
     */
    void removerProtegido(Casillero *aRemover);

    /*
     * Pre: El Casillero debe existir (@throws logic_error).
     *      El Casillero debe ser de tipo inactivo (@throws logic_error).
     * Post: Remueve el casillero dado al registro de robos en proceso.
     */
    void removerInactivo(Casillero *aRemover);

    /*
     * Pre: La estructura debe existir (@throws logic_error).
     *      El Casillero debe ser de tipo inactivo (@throws logic_error).
     * Post: Reemplaza el registro de tesoros protegidos.
     */
    void reemplazarProtegidos(Lista<Casillero *> *inactivos);

    /*
     * Pre: La estructura debe existir (@throws logic_error).
     *      El Casillero debe ser de tipo inactivo (@throws logic_error).
     * Post: Reemplaza el registro de robos en proceso.
     */
    void reemplazarInactivos(Lista<Casillero *> *inactivos);

};

#endif
