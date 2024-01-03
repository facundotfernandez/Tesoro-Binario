#ifndef MANEJODEARCHIVOS_H_
#define MANEJODEARCHIVOS_H_

#include <sstream>
#include <sys/stat.h>
#include <unistd.h>
#include "../easy-bmp/EasyBMP_Font.h"
#include "Constantes.h"
#include "Plano.h"

/* Declaración anticipada de la clase Jugador. */
class Jugador;

class ManejoDeArchivos {

private:

    /*
     * pre : -
     * post: Genera la cadena de texto correspondiente a la ruta relativa al tablero del jugador, con la perspectiva y idJugador dados.
     *       Además indica si los enemigos están ocultos.
     */
    string generarRutaTablero(unsigned int idJugador, unsigned int perspectiva, unsigned int fila, unsigned int columna,
                              unsigned int profundidad, bool enemigosOcultos);

    /*
     * pre : -
     * post: Genera la cadena de texto correspondiente a la ruta relativa al directorio que contiene los tableros del jugador, con el idJugador dado.
     */
    string generarRutaJugador(unsigned int idJugador);

    /*
     * pre : -
     * post: Indica si existe el directorio dado.
     */
    bool existeDirectorio(const char *ruta);

public:

    /*
     * pre : -
     * post: Inicializa una instancia de ManejoDeArchivos.
     */
    ManejoDeArchivos();

    /*
     * pre : -
     * post: Libera los recursos asociados a ManejoDeArchivos.
     */
    virtual ~ManejoDeArchivos();

    /*
     * pre : El directorio del jugador en cuestión ya debe existir dentro del directorio 'tableros/', con formato 'jugador-<idJugador>' (@throws EasyBMPwarnings).
     *       El tipo de casillero de todos los casilleros a mostrar debe existir (@throws logic_error).
     *       Las imagenes deben existir y estar guardadas en el directorio 'assets/' (@throws).
     *       Las imagenes deben tener la misma dimension que el casillero (@throws logic_error).
     *       El Jugador ocupante del Casillero central debe existir (@throws).
     * post: Crea tres tableros (Planos XY,XZ,YZ con el Casillero en el centro) en formato BMP, a partir del Tablero y Casillero dados, y lo guarda en el directorio del jugador correspondiente.
     * 		 Oculta los casilleros de enemigos según lo indicado, determina el jugador seleccionado según el Jugador que ocupa el Casillero central.
     */
    void exportar(Plano *plano, bool ocultarEnemigos = true);

    /*
     * pre : El directorio 'tableros/' debe existir (@throws).
     * post: Crea el directorio del jugador correspondiente dentro del directorio 'tableros/'.
     */
    bool crearDirectorioJugador(unsigned int idJugador);

    /*
     * pre : -
     * post: Crea el directorio 'tableros/'.
     */
    bool crearDirectorioTableros();

    /*
     * pre : -
     * post: Elimina el directorio 'tableros/' y todos los directorios o archivos que tenga dentro.
     */
    void eliminarDirectorioTableros();
};

#endif /* MANEJODEARCHIVOS_H_ */
