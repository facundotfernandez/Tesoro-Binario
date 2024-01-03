#include "Turno.h"
#include "Interacciones.h"
#include "ManejoDeArchivos.h"
#include <string>

class Juego {

private:

    Tablero *tablero;
    Lista<Casillero *> *destruidos;
    Turno *turno;
    Lista<Jugador *> *jugadores;
    Interacciones *interacciones;
    ManejoDeArchivos *manejoDeArchivos;
    bool juegoEnCurso;
    unsigned int objetivoRobados;

    /*
     * pre : La estructura que contiene el tamaño de las dimensiones debe existir y contener tres valores (@throws out_of_range).
     * post: Pide los datos iniciales para comenzar el Juego.
     *       A cantidadDeJugadores se le va a asignar un numero entre MINIMO_JUGADORES y MAXIMO_JUGADORES.
     *       A cantidadDeTesoros se le va a asignar un numero entre MINIMO_TESOROS y MAXIMO_TESOROS.
     *       A dimensiones se le asignaran coordenadas entre MINIMO_DIMENSION y MAXIMO_DIMENSION.
     */
    void establecerDatosIniciales(unsigned int &cantidadDeJugadores, unsigned int &cantidadDeTesoros,
                                  Vector<unsigned int> *dimensiones);

    /*
     * pre : -
     * post: Reemplaza el dato dado por el ingreso por el Jugador, una vez haya ingresado un dato válido.
     */
    void establecerCantidadDeJugadores(unsigned int &jugadores);

    /*
     * pre : -
     * post: Reemplaza el dato dado por el ingreso por el Jugador, una vez haya ingresado un dato válido.
     */
    void establecerCantidadDeTesoros(unsigned int &tesoros);

    /*
     * pre : El puntero a la estructura NO debe ser nulo (@throws logic_error).
     * 		 La estructura dada tiene la longitud buscada (@throws out_of_range).
     * post: Reemplaza el dato dado por el ingreso por el Jugador, una vez haya ingresado un dato válido.
     */
    void establecerDimensiones(Vector<unsigned int> *dimensiones);

    /*
     * pre : -
     * post: Pide al Jugador coordenadas. Se asegura de que no pertenezca al jugador y que sea una coordenada dentro del
     *       tablero. Si no lo es, pide reingreso de datos.
     */
    Vector<unsigned int> *obtenerCoordenadasEnemigas(Jugador *jugador);

    /*
     * pre : Debe haber al menos un jugador en la partida (@throws out_of_range).
     * post: Obtiene el siguiente jugador en la lista de jugadores.
     * 		 Si el jugador actual es el ultimo de la lista, se devuelve el primero de la lista como el siguiente.
     * 		 Si el jugador actual es cualquier otro, se devuelve el siguiente jugador (con id mayor al actual).
     */
    Jugador *obtenerSiguienteJugador(unsigned int idJugadorActual);

    /*
     * pre : Debe haber al menos un jugador en la partida (@throws out_of_range).
     * post: Retorna el jugador con más tesoros robados.
     */
    Jugador *obtenerJugadorConMasTesoros();

    /*
     * pre : -
     * post: Verifica que NO haya un jugador que completó la condición de victoria por tesoros robados ni solo un jugador en partida.
     */
    bool obtenerJuegoEnCurso();

    /*
     * pre : Debe haber al menos un jugador para crear (@throws out_of_range).
     *   	 El directorio de cada jugador debe poder ser creado (@throws logic_error).
     * post: Inicializa la lista de jugadores con la cantidad de jugadores dada, con su ID desde 1.
     */
    Lista<Jugador *> *inicializarJugadores(unsigned int cantidadDeJugadores);

    /* pre : El puntero a la estructura de dimensiones NO debe ser nulo (@throws logic_error).
     * 		 La estructura de dimensiones dada tiene la longitud buscada (@throws out_of_range).
     * 		 Debe haber al menos un tesoro para asignar (@throws out_of_range).
     * post: Inicializa el tablero con las dimensiones dadas y la cantidad de tesoros dada.
     *       Si se indica que NO es partida rápida, los tesoros serán de tipo protegido.
     */
    Tablero *
    inicializarTablero(Vector<unsigned int> *dimensiones, unsigned int cantidadDeTesoros, bool esPartidaRapida = false);

    /*
     * pre : Debe haber al menos un tesoro para asignar (@throws out_of_range).
     * post: Inicializa la cantidad de tesoros de cada jugador en posiciónes aleatorias del tablero.
     *       Si se indica que NO es partida rápida, los tesoros serán de tipo protegido.
     */
    void inicializarTesoros(Tablero *tablero, unsigned int cantidadDeTesoros, unsigned int ancho, unsigned int alto,
                            unsigned int profundidad, bool esPartidaRapida);

    /*
     * pre : El jugador debe existir (@throws logic_error).
     * post: Actualiza la lista de jugadores descartando los jugadores que NO siguen en juego.
     *       Actualiza el estado y casilleros del Jugador, y su registro de cambios, según corresponda.
     */
    void actualizarJugadores(Jugador *jugadorTurnoActual);

    /*
     * pre : El jugador debe existir (@throws logic_error).
     * post: Actualiza la lista de protegidos de cada Jugador.
     */
    void actualizarProtegidos(Jugador *jugador, unsigned int turnoActual);

    /*
     * pre : El jugador debe existir (@throws logic_error).
     * post: Actualiza la lista de inactivos de cada Jugador.
     */
    void actualizarInactivos(Jugador *jugador, unsigned int turnoActual);

    /*
     * pre :
     * post: Actualiza los Casilleros inactivos cuyo inactivoHasta es igual al turno actual.
     * 		 Registra los cambios en los jugadores, en caso de ser necesario.
     */
    void actualizarOcupados(unsigned int turnoActual);

    /*
     * pre : El turno en que deja de estar destruido no puede ser anterior al actual (@throws logic_error).
     * post: Actualiza los Casilleros destruidos cuyo inactivoHasta es igual al turno actual.
     */
    void actualizarDestruidos(unsigned int turnoActual);

    /*
     * pre : La estructura debe existir (@throws logic_error).
     * post: Convierte todos los Casilleros a tipo vacío.
     */
    void vaciarCasilleros(Lista<Casillero *> *casilleros);

    /*
     * pre : El Casillero debe existir (@throws logic_error).
     *       El Casillero debe ser de tipo destruido (@throws logic_error).
     * post: Agrega un Casillero a la lista de destruidos.
     */
    void agregarDestruido(Casillero *casillero);

    /*
     * pre : El Jugador debe tener al menos un tesoro disponible para mover (@throws logic_error).
     *   	 El Jugador debe elegir mover un tesoro y una opción valida para mover (@throws logic_error).
     * post: Mueve un tesoro seleccionado por jugador a la ubicación enemiga elegida, en caso de ser posible.
     */
    void moverTesoro(Jugador *jugador, unsigned int turnoActual);

    /*
     * pre : El jugador debe existir (@throws logic_error).
     * post: -
     */
    void validarExistencia(Jugador *jugador);

    /*
     * pre : La carta debe existir (@throws logic_error).
     * post: -
     */
    void validarExistencia(Carta *carta);

    /*
     * pre : La estructura debe existir (@throws logic_error).
     *       La estructura debe tener la longitud esperada (@throws out_of_range).
     * post: -
     */
    void validarExistencia(Vector<unsigned int> *estructura, unsigned int longitudEsperada = CANTIDAD_DIMENSIONES);/*

     * pre : La estructura debe existir (@throws logic_error).
     *       La estructura no puede estar vacía (@throws out_of_range).
     * post: -
     */
    void validarExistencia(Lista<Casillero *> *estructura);/*

     * pre : La estructura debe existir (@throws logic_error).
     *       La estructura no puede estar vacía (@throws out_of_range).
     * post: -
     */
    void validarExistencia(Lista<unsigned int> *estructura);

    /*
     * pre : Las estructuras deben existir (@throws logic_error).
     *       Las estructuras no pueden estar vacías (@throws out_of_range).
     * post: CREA una nueva lista que contenga solamente los casilleros filtrados según el o los tipos aceptados.
     */
    Lista<Casillero *> *filtrarCasilleros(Lista<Casillero *> *casilleros, Lista<unsigned int> *tiposAceptados);

    /*
     * pre : El jugador debe existir (@throws logic_error).
     * post: Coloca un espía en la ubicación enemiga elegida por el jugador, en caso de ser posible.
     */
    void agregarPirata(Jugador *jugador, unsigned int turnoActual);

    /*
     * pre : El jugador debe existir (@throws logic_error).
     * post: Coloca un barril en la ubicación enemiga elegida por el jugador, en caso de ser posible.
     */
    void agregarBarril(Jugador *jugador, unsigned int turnoActual);

    /*
     * pre : El jugador debe existir (@throws logic_error).
     *       El jugador debe elegir jugar una carta (@throws out_of_range).
     * 		 El jugador debe elegir una carta que esté en su mano (@throws logic_error).
     * post: Permite al jugador elegir una carta de su mano y jugarla.
     */
    void jugarCarta(Jugador *jugador, unsigned int turnoActual);

    /*
     * pre : El jugador debe existir (@throws logic_error).
     *       Si la mano está llena, debe elegir descartar una carta (@throws out_of_range).
     * 		 El jugador debe elegir descartar una carta que esté en su mano (@throws logic_error).
     * post: Crea una carta aleatoria y la agrega a la mano del jugador.
     */
    void sacarCarta(Jugador *jugador);

    /*
     * pre : El jugador debe existir (@throws logic_error).
     *       El Jugador debe tener al menos un tesoro disponible para partir (@throws logic_error).
     *   	 El Jugador debe elegir una opción valida para partir (@throws logic_error).
     *   	 El Casillero de destino debe estar vacio (@throws logic_error).
     * post: Convierte el tesoro elegido a tipo Partido y ubica la otra parte en la ubicación elegida, en caso de ser posible.
     */
    void jugarPartirTesoro(Jugador *jugador);

    /*
     * pre : El jugador debe existir (@throws logic_error).
     *       El Jugador debe elegir una opción valida (@throws logic_error).
     * post: Exporta el casillero elegido revelando todas las posiciónes enemigas de su alrededor.
     */
    void jugarRadar(Jugador *jugador);

    /*
     * pre : El jugador debe existir (@throws logic_error).
     *       El Jugador debe tener al menos un tesoro disponible para proteger (@throws logic_error).
     *   	 El Jugador debe elegir una opción valida para proteger (@throws logic_error).
     * post: Convierte el tesoro elegido a tipo Protegido, en caso de ser posible.
     */
    void jugarProteger(Jugador *jugador, unsigned int turnoActual);

    /*
     * pre : El jugador debe existir (@throws logic_error).
     *       El Jugador debe elegir un jugador enemigo que siga en partida (@throws logic_error).
     *  	 El Jugador enemigo debe tener al menos una carta disponible en su mano (@throws logic_error).
     * post: Roba una carta de un jugador enemigo, en caso de ser posible.
     */
    void jugarRobarCarta(Jugador *jugador);

    /*
     * pre : El jugador debe existir (@throws logic_error).
     * post: Reinicia el turno con las acciones posibles en un turno duplicado.
     */
    void jugarDuplicarTurno(Jugador *jugador, unsigned int turnoActual);

    /*
     * pre : El jugador debe existir (@throws logic_error).
     * post: Contrata mercenarios, en caso de ser posible.
     */
    void jugarContratarMercenarios(Jugador *jugador);

    /*
     * pre : El jugador debe existir (@throws logic_error).
     *       La estructura debe existir (@throws logic_error).
     *       La estructura no puede estar vacía (@throws out_of_range).
     *       El jugador debe tener mercenarios contratados (@throws logic_error).
     * post: Usa los mercenarios del jugador, guarda el cambio y muestra la notificación correspondiente.
     */
    void defenderConMercenarios(Jugador *jugador, Vector<unsigned int> *coordenadas);

    /*
     * pre : El jugador debe existir (@throws logic_error).
     * post: Convierte a vacio el casillero y lo remueve del registro de ocupados del jugador.
     *       Agrega la nueva ubicación al registro de ocupados del jugador según corresponda.
     */
    void vaciarCasillero(Jugador *jugador, Casillero *aVaciar, Casillero *moverHacia = NULL);

    /*
     * pre : El jugador debe existir (@throws logic_error).
     * post: Convierte a destruido el casillero y lo remueve del registro de ocupados del jugador.
     *       Actualiza el registro de destruidos.
     */
    void destruirCasillero(Jugador *jugador, Casillero *aDestruir, unsigned int destruidoHasta);

    /*
     * pre : El jugador debe existir (@throws logic_error).
     *       El casillero debe existir (@throws logic_error).
     *       El jugador debe elegir descartar (@throws out_of_range).
     *       El jugador debe elegir una opción válida (@throws logic_error).
     * post: Coloca un espía en la ubicación enemiga elegida por el jugador, en caso de ser posible.
     */
    void descartar(Jugador *jugador, Carta *carta);

    /*
     * pre : El jugador debe existir (@throws logic_error).
     *       El casillero debe existir (@throws logic_error).
     * post: Genera y guarda el cambio del jugador con el formato correspondiente.
     */
    void agregarCambio(Jugador *jugador, Casillero *casillero, unsigned int tipo);

    /*
     * pre : El jugador debe existir (@throws logic_error).
     *       El jugador debe elegir exportar (@throws logic_error).
     * post: Muestra un menú desplegable con los casilleros dados y exporta cada uno de los elegidos, según lo desee.
     */
    void mostrarCasilleros(Jugador *jugador);

    /*
     * pre : El jugador debe existir (@throws logic_error).
     * post: Se mostraran los cambios recientes que afectaron al jugador en turnos de otros jugadores.
     */
    void mostrarCambios(Jugador *jugador);

    /*
     * pre : El jugador debe existir (@throws logic_error).
     * post: Realiza las acciones destinadas al turno de un jugador.
     */
    void jugarTurno(Jugador *jugador, unsigned int turnoActual);

    /*
     * pre : El identificador dado no puede ser cero (@throws logic_error).
     * post: Avanza el turno y asigna como jugador actual al siguiente jugador, según el idJugador dado.
     */
    void avanzarTurno(unsigned int idJugador);

    /*
     * pre : -
     * post: Mientras el juega siga en curso: Actualiza el tablero; Juega el turno del jugador actual; Actualiza el tablero la lista de jugadores activos.
     * 		 Luego avanza al siguiente turno.
     * 		 Si el juego finalizó, muestra un mensaje de fin de juego con el ganador.
     */
    void actualizarJuego();

    /*
     * pre : -
     * post: Exporta Los 3 planos ortogonales centrados en un casillero (Elegido por el Jugador) a el directorio del jugadorc correspondiente.
     */
    void exportar(Lista<Casillero *> *casilleros, string indicacion = "Selecciona un casillero para exportar",
                  bool ocultarEnemigos = true);

    /*
     * pre : El jugador debe existir (@throws logic_error).
     * post: Muestra el progreso de los robos que está llevando a cabo el jugador del turno actual.
     */
    void mostrarProgresoRobos(Jugador *jugador, unsigned int turnoActual);

    /*
     * pre : El jugador debe existir (@throws logic_error).
     *       El jugador debe tener tesoros disponibles para elegir (@throws logic_error).
     * post: Muestra un menú desplegable con los tesoros del jugador y devuelve el elegido.
     */
    Casillero *obtenerTesoro(Jugador *jugador, string indicacion, bool cualquierTipo = false);

    /*
     * pre : El jugador debe existir (@throws logic_error).
     * post: Pide coordenadas al jugador hasta que ingrese unas correspondientes a una ubicación que no ocupa.
     */
    Casillero *obtenerDestino(Jugador *jugador, string indicacion);

    /*
     * pre : El jugador debe existir (@throws logic_error).
     * post: Muestra el progreso de robos, permite exportar casilleros ocupados, muestra casilleros ocupados y cartas en mano.
     *       Si el turno NO es duplicado, el jugador saca una carta.
     */
    void mostrarInformacion(Jugador *jugador, unsigned int turnoActual, bool esTurnoDuplicado = false);

    /*
     * pre : El jugador debe existir (@throws logic_error).
     * post: Realiza los llamados necesarios para realizar las acciones propias del juego del turno.
     */
    void jugar(Jugador *jugador, unsigned int turnoActual);

public:

    /*
     * pre : -
     * post: Inicializa la cantidad de jugadores, y establece ID en cada jugador. Inicializa el tablero y coloca los tesoros de forma aleatoria para cada jugador
     */
    explicit Juego(bool inicializado = false);

    /*
     * pre : -
     * post: Libera los recursos asociados a la partida.
     */
    virtual ~Juego();

    /*
     * pre : -
     * post: Devuelve la cantidad de jugadores de la partida.
     */
    unsigned int obtenerCantidadJugadores();

    /*
     * pre : Debe haber al menos un jugador en la partida (@throws out_of_range).
     * post: Devuelve el jugador especificado por su identificador.
     */
    Jugador *obtenerJugador(unsigned int idJugadorActual);

};
