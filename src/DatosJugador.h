#ifndef DATOSJUGADOR_H_
#define DATOSJUGADOR_H_

#include "Pila.h"
#include "Lista.h"
#include "Casillero.h"

class DatosJugador {

private:

    unsigned int id;

    /* Cantidad de tesoros que controla el jugador.*/
    unsigned int cantidadDeTesorosActivos;
    unsigned int cantidadDeTesorosRobados;

    /* Cambios que han afectado al Jugador mientras no era su turno. */
    Pila<string> *cambios;

public:

    /*
     * pre : -
     * post: Inicializa una descripción con el id dado.
     * 		 El resto de sus atributos serán inicializados en cero y el registro de cambios estará vacío.
     */
    explicit DatosJugador(unsigned int id);

    /*
     * pre : -
     * post: Libera los recursos asociados a la descripción.
     */
    virtual ~DatosJugador();

    /*
     * pre : -
     * post: Devuelve el identificador del Jugador.
     */
    unsigned int obtenerId() const;

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
     * pre : -
     * post: Aumenta la cantidad de tesoros activos del Jugador en 1.
     */
    void aumentarTesorosActivos();

    /*
     * pre : -
     * post: Aumenta la cantidad de tesoros robados por el Jugador en 1.
     */
    void aumentarTesorosRobados();

    /*
     * pre : -
     * post: Disminuye la cantidad de tesoros activos del Jugador en 1.
     */
    void disminuirTesorosActivos();

    /*
     * Pre: -
     * Post: Agrega un cambio al registro de cambios.
     */
    void agregarCambio(string cambio);

    /*
     * pre : -
     * post: Devuelve el registro de cambios.
     */
    Pila<string> *obtenerCambios() const;

};

#endif /* DATOSJUGADOR_H_ */
