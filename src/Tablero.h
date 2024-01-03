#ifndef TABLERO_H_
#define TABLERO_H_

#include "Lista.h"
#include "Casillero.h"

/* Declaración anticipada de la clase Plano. */
class Plano;

class Tablero {

private:

    /* Dimensiones (ancho, alto, profundidad) del Tablero */
    Vector<unsigned int> *dimensiones;

    /* Tablero tridimensional implementado a base de Listas */
    Lista<Lista<Lista<Casillero *> *> *> *tablero;

    /*
     * pre : Las dimensiones dadas pertenecen al rango de dimensiones permitido (@throws out_of_range).
     * post: -
     */
    void validarDimensiones(unsigned int ancho, unsigned int alto, unsigned int profundidad) const;

    /*
     * pre : La posición del Casillero pertenece al Tablero (@throws out_of_range).
     * post: -
     */
    void validarPosicion(unsigned int x, unsigned int y, unsigned int z) const;

    /*
     * pre : El puntero a la estructura NO debe ser nulo (@throws logic_error).
     * 		 La estructura dada tiene la longitud buscada (@throws out_of_range).
     * post: -
     */
    void validarLongitud(Vector<unsigned int> *estructura, unsigned int longitud = CANTIDAD_DIMENSIONES) const;

    /*
     * pre : -
     * post: Inicializa todos los Casilleros del Tablero asignandoles
     * 		 la descripcion correspondiente al tipo de Casillero vacío.
     * 		 Inicializa los Planos vecinos correspondientes a cada Casillero.
     */
    void inicializarCasilleros();

    /*
     * pre : -
     * post: Inicializa los Planos vecinos de cada Casillero del tablero.
     */
    void inicializarPlanos();

public:

    /*
     * pre : -
     * post: Inicializa el Tablero con dimensiones de tamaño MINIMO_DIMENSION,
     *       puede inicializar los Casilleros con tipo vacio según se indique.
     */
    explicit Tablero(bool inicializado = false);

    /*
     * pre : Las dimensiones dadas pertenecen al rango de dimensiones permitido (@throws out_of_range).
     * post: Inicializa el Tablero con dimensiones de tamaño dado,
     *       puede inicializar los Casilleros con tipo vacio según se indique.
     */
    Tablero(unsigned int ancho, unsigned int alto, unsigned int profundidad, bool inicializado = false);

    /*
     * pre : -
     * post: Libera los recursos asociados al Tablero.
     */
    virtual ~Tablero();

    /*
     * pre : -
     * post: Devuelve el ancho del Tablero.
     */
    unsigned int obtenerAncho() const;

    /*
     * pre : -
     * post: Devuelve el alto del Tablero.
     */
    unsigned int obtenerAlto() const;

    /*
     * pre : -
     * post: Devuelve la profundidad del Tablero.
     */
    unsigned int obtenerProfundidad() const;

    /*
     * pre : La posición (x,y,z) del Casillero pertenece al Tablero (@throws out_of_range).
     * post: Devuelve el puntero al Casillero según las coordenadas dadas.
     */
    Casillero *obtenerCasillero(unsigned int x, unsigned int y, unsigned int z) const;

    /*
     * pre : La posición del Casillero pertenece al Tablero (@throws out_of_range).
     *       El puntero a la estructura de coordenadas NO debe ser nulo (@throws logic_error).
     * 		 La estructura de coordenadas dada tiene la longitud buscada (@throws out_of_range).
     * post: Devuelve el puntero al Casillero según las coordenadas dadas.
     */
    Casillero *obtenerCasillero(Vector<unsigned int> *coordenadas) const;

};

#endif /* TABLERO_H_ */
