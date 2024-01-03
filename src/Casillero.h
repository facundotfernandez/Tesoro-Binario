#ifndef CASILLERO_H_
#define CASILLERO_H_

#include <cstdio>
#include "DatosCasillero.h"

/* Declaración anticipada de la clase Plano. */
class Plano;

/* Declaración anticipada de la clase Tablero. */
class Tablero;

class Casillero {

private:

    /* Coordenadas (x,y,z) del Casillero en el Tablero. */
    Vector<unsigned int> *coordenadas;

    /* Descripción del Casillero. */
    DatosCasillero *descripcion;

    /* Perspectiva XY del Casillero en el Tablero. */
    Plano *planoXY;

    /* Perspectiva XZ del Casillero en el Tablero. */
    Plano *planoXZ;

    /* Perspectiva YZ del Casillero en el Tablero. */
    Plano *planoYZ;

    /*
     * pre : El Casillero debe ser del tipo buscado (@throws logic_error).
     * post: -
     */
    void validarTipo(unsigned int tipo) const;

    /*
     * pre : El Plano debe existir (@throws logic_error).
     * post: -
     */
    void validarExistencia(Plano *plano) const;

    /*
     * pre : El Casillero debe existir (@throws logic_error).
     * post: -
     */
    void validarExistencia(Casillero *casillero) const;

    /*
     * pre : -
     * post: Asigna el dato de las coordenadas del Casillero.
     */
    void asignarCoordenadas(unsigned int x, unsigned int y, unsigned int z);

    /*
     * pre : -
     * post: Asigna los datos de tipo y idJugador del Casillero, en su descripción.
     */
    void asignarDescripcionCorta(unsigned int tipo, Jugador *jugador);

    /*
     * pre : El Casillero debe ser de tipo inactivo o destruido (@throws logic_error).
     * post: Asigna el dato inactivoHasta del Casillero, en su descripción.
     */
    void asignarInactivoHasta(unsigned int inactivoHasta);

    /*
     * pre : El Casillero debe ser de tipo protegido (@throws logic_error).
     * post: Asigna el dato protegidoHasta del Casillero, en su descripción.
     */
    void asignarProtegidoHasta(unsigned int protegidoHasta);

    /*
     * pre : El Casillero debe ser de tipo partido (@throws logic_error).
     * post: Asigna el dato de las coordenadas del tesoro partido asociado al Casillero, en su descripción.
     */
    void asignarAsociado(Casillero *asociado);

public:

    /*
     * pre : -
     * post: Se inicializa un Casillero con las coordenadas (x,y,z) dadas y la descripción corresponde a un Casillero vacío.
     */
    Casillero(unsigned int x, unsigned int y, unsigned int z);

    /*
     * pre : -
     * post: Libera los recursos asociados al Casillero.
     */
    virtual ~Casillero();

    /*
     * pre : -
     * post: Devuelve la coordenada X del Casillero.
     */
    unsigned int obtenerX() const;

    /*
     * pre : -
     * post: Devuelve la coordenada Y del Casillero.
     */
    unsigned int obtenerY() const;

    /*
     * pre : -
     * post: Devuelve la coordenada Z del Casillero.
     */
    unsigned int obtenerZ() const;

    /*
     * pre : -
     * post: Devuelve el puntero al Vector de coordenadas del Casillero.
     */
    Vector<unsigned int> *obtenerCoordenadas() const;

    /*
     * pre : -
     * post: Devuelve el tipo del Casillero, dato que figura en su descripción.
     */
    unsigned int obtenerTipo() const;

    /*
     * pre : -
     * post: Devuelve el jugador del Casillero, dato que figura en su descripción.
     */
    Jugador *obtenerJugador() const;

    /*
     * pre : El Casillero debe ser de tipo inactivo o destruido (@throws logic_error).
     * post: Devuelve el inactivoHasta del Casillero, dato que figura en su descripción.
     */
    unsigned int obtenerInactivoHasta() const;

    /*
     * pre : El Casillero debe ser de tipo protegido (@throws logic_error).
     * post: Devuelve el protegidoHasta del Casillero, dato que figura en su descripción.
     */
    unsigned int obtenerProtegidoHasta() const;

    /*
     * pre : El Casillero debe ser de tipo partido (@throws logic_error).
     * post: Devuelve un puntero al Vector con las coordenadas del tesoro partido asociado al Casillero, dato que figura en su descripción.
     */
    Casillero *obtenerAsociado() const;

    /*
     * pre : El Plano debe existir (@throws logic_error).
     * post: Devuelve un puntero al plano de tamaño 5x5 con perspectiva XY asociado al Casillero.
     */
    Plano *obtenerPlanoXY() const;

    /*
     * pre : El Plano debe existir (@throws logic_error).
     * post: Devuelve un puntero al plano de tamaño 5x5 con perspectiva XZ asociado al Casillero.
     */
    Plano *obtenerPlanoXZ() const;

    /*
     * pre : El Plano debe existir (@throws logic_error).
     * post: Devuelve un puntero al plano de tamaño 5x5 con perspectiva YZ asociado al Casillero.
     */
    Plano *obtenerPlanoYZ() const;

    /*
     * pre : El Plano debe existir (@throws logic_error).
     * 		 El Casillero no debe tener un plano ya asignado (@throws logic_error).
     * post: Asigna el plano de tamaño 5x5 con perspectiva XY asociado al Casillero.
     */
    void asignarPlanoXY(Plano *planoXY);

    /*
     * pre : El Plano debe existir (@throws logic_error).
     * 		 El Casillero no debe tener un plano ya asignado (@throws logic_error).
     * post: Asigna el plano de tamaño 5x5 con perspectiva XZ asociado al Casillero.
     */
    void asignarPlanoXZ(Plano *planoXZ);

    /*
     * pre : El Plano debe existir (@throws logic_error).
     * 		 El Casillero no debe tener un plano ya asignado (@throws logic_error).
     * post: Asigna el plano de tamaño 5x5 con perspectiva YZ asociado al Casillero.
     */
    void asignarPlanoYZ(Plano *planoYZ);

    /*
     * pre : No admite conversión a tipo partido (@throws logic_error).
     *       Casilleros de tipo vacio o destruido no pueden pertenecer a un jugador en particular (@throws logic_error).
     * post: Reemplaza la descripción del Casillero y lo convierte a Casillero de tipo dado.
     */
    void convertirA(unsigned int tipo, Jugador *ocupante = NULL, unsigned int hastaTurno = 0);

    /*
     * pre : El Casillero asociado debe existir (@throws logic_error).
     *       No admite conversión a tipos distintos de partido (@throws logic_error).
     * post: Reemplaza la descripción del Casillero y lo convierte a Casillero de tipo dado.
     */
    void convertirA(unsigned int tipo, Jugador *ocupante, Casillero *asociado);

};

#endif /* CASILLERO_H_ */
