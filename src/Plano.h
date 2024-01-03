#ifndef PLANO_H_
#define PLANO_H_

#include "Tablero.h"

class Plano {

private:

    /* Fila del Plano correspondiente al origen de coordenadas del mismo. */
    int filaOrigen;

    /* Columna del Plano correspondiente al origen de coordenadas del mismo. */
    int columnaOrigen;

    /* Fila del Casillero central del Plano. */
    int filaCentral;

    /* Columna del Casillero central del Plano. */
    int columnaCentral;

    /* Dimensiones del Tablero. Siempre deben ser iguales a 5. */
    int ancho, alto;

    /* Coordenada invariable de los Casilleros del Plano según la perspectiva. */
    int profundidad;

    /* Perspectiva (XY,XZ,YZ) del Plano. */
    int perspectiva;

    /* Plano bidimensional implementado a base de Listas. */
    Lista<Lista<Casillero *> *> *plano;

    /* Tablero al que pertenece el Plano. */
    Tablero *tablero;

    /*
     * pre : La posición del Casillero pertenece al Plano (@throws out_of_range).
     * post: -
     */
    void validarPosicion(int fila, int columna) const;

    /*
     * pre : Las dimensiones del Tablero deben ser mayores o iguales al mínimo permitido (@throws out_of_range).
     * post: -
     */
    void validarDimensiones(Tablero *tablero);

    /*
     * pre : -
     * post: Inicializa los punteros a Casilleros del Tablero.
     */
    void inicializarCasilleros();

    /*
     * pre : -
     * post: Corrige la coordenada de Fila/Columna para obtener el origen de coordenadas del Plano.
     */
    int corregir(int coordenada, int limite);

public:

    /*
     * pre : El Casillero debe pertenecer al Tablero y el Tablero debe tener todas sus dimensiones mayores o iguales al mínimo permitido (@throws out_of_range).
     *       La perspectiva debe ser XY, XZ o YZ (@throws logic_error).
     * post: Inicializa el Plano con dimensiones de tamaño DIMENSION_PLANO, alrededor del Casillero dado.
     */
    Plano(Tablero *tablero, Casillero *casillero, int perspectiva);

    /*
     * pre : -
     * post: Libera los recursos asociados al Plano.
     */
    virtual ~Plano();

    /*
     * pre : -
     * post: Devuelve la fila de origen del Plano, según su ubicación en el Tablero.
     */
    int obtenerFilaOrigen() const;

    /*
     * pre : -
     * post: Devuelve la columna de origen del Plano, según su ubicación en el Tablero.
     */
    int obtenerColumnaOrigen() const;

    /*
     * pre : -
     * post: Devuelve la fila de central del Plano, según su ubicación en el Tablero.
     */
    int obtenerFilaCentral() const;

    /*
     * pre : -
     * post: Devuelve la columna de central del Plano, según su ubicación en el Tablero.
     */
    int obtenerColumnaCentral() const;

    /*
     * pre : -
     * post: Devuelve la profundidad del Plano.
     */
    int obtenerProfundidad() const;

    /*
     * pre : -
     * post: Devuelve la perspectiva del Plano.
     */
    int obtenerPerspectiva() const;

    /*
     * pre : Las coordenadas no pueden ser negativas (@throws out_of_range).
     *       La posición del Casillero debe pertenecer al Plano (@throws out_of_range).
     * post: Devuelve un puntero al Casillero según la fila y columna dadas.
     */
    Casillero *obtenerCasillero(int fila, int columna) const;

};

#endif /* PLANO_H_ */
