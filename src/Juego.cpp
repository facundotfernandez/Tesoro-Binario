#include "Juego.h"

Juego::Juego(bool inicializado) {

    this->interacciones = new Interacciones();
    this->manejoDeArchivos = new ManejoDeArchivos();

    bool esPartidaRapida = false;
    try {
        esPartidaRapida = this->interacciones->preguntar("Querés jugar una partida rápida?", "Si", "No");
    } catch (const logic_error &e) {
    }

    unsigned int cantidadDeJugadores = (esPartidaRapida) ? (MINIMO_JUGADORES) : (0);
    unsigned int cantidadDeTesoros = (esPartidaRapida) ? (MINIMO_TESOROS) : (0);
    Vector<unsigned int> *dimensiones = new Vector<unsigned int>(CANTIDAD_DIMENSIONES);

    if (esPartidaRapida) {
        dimensiones->reemplazarElemento(0, MINIMO_DIMENSION);
        dimensiones->reemplazarElemento(1, MINIMO_DIMENSION);
        dimensiones->reemplazarElemento(2, MINIMO_DIMENSION);
    }

    this->establecerDatosIniciales(cantidadDeJugadores, cantidadDeTesoros, dimensiones);

    try {
        this->manejoDeArchivos->eliminarDirectorioTableros();
        this->manejoDeArchivos->crearDirectorioTableros();
    } catch (...) {
        throw logic_error("No se pudo crear la directorio de tableros");
    }

    // Se inicializan los atributos 'jugadores' y 'tablero'
    this->jugadores = this->inicializarJugadores(cantidadDeJugadores);
    this->tablero = this->inicializarTablero(dimensiones, cantidadDeTesoros, esPartidaRapida);
    delete dimensiones;

    this->juegoEnCurso = true;
    this->turno = new Turno(this->obtenerJugador(1)); // Crea el primer turno, con el primer jugador
    this->destruidos = new Lista<Casillero *>();

    /* Condicion de victoria por tesoros robados */
    this->objetivoRobados = (cantidadDeTesoros * (cantidadDeJugadores / 2));

    if (inicializado) {
        this->actualizarJuego();
    }
}

Juego::~Juego() {

    this->jugadores->iniciarCursor();
    while (this->jugadores->avanzarCursor()) {
        delete this->jugadores->obtenerCursor();
    }
    delete this->jugadores;

    this->destruidos->limpiar();
    delete this->destruidos;

    delete this->tablero;
    delete this->turno;
    delete this->interacciones;

    this->manejoDeArchivos->eliminarDirectorioTableros();
    delete this->manejoDeArchivos;
}

void Juego::establecerCantidadDeJugadores(unsigned int &cantidadDeJugadores) {
    while (cantidadDeJugadores == 0) {
        try {
            cantidadDeJugadores = this->interacciones->pedirOpcion(MINIMO_JUGADORES, MAXIMO_JUGADORES,
                                                                   "Ingrese la cantidad de jugadores");
        } catch (const out_of_range &e) {
            this->interacciones->mostrarTitulo(e.what());
        }
    }
}

void Juego::establecerCantidadDeTesoros(unsigned int &cantidadDeTesoros) {
    while (cantidadDeTesoros == 0) {
        try {
            cantidadDeTesoros = this->interacciones->pedirOpcion(MINIMO_TESOROS, MAXIMO_TESOROS,
                                                                 "Ingrese la cantidad de tesoros");
        } catch (const out_of_range &e) {
            this->interacciones->mostrarTitulo(e.what());
        }
    }
}

void Juego::establecerDimensiones(Vector<unsigned int> *dimensiones) {
    while ((*dimensiones)[0] == 0) {
        try {
            this->interacciones->pedirDimensiones(dimensiones);
        } catch (const logic_error &e) {
            this->interacciones->mostrarTitulo(e.what());
        }
    }
}

void Juego::establecerDatosIniciales(unsigned int &cantidadDeJugadores, unsigned int &cantidadDeTesoros,
                                     Vector<unsigned int> *dimensiones) {
    this->validarExistencia(dimensiones);

    this->establecerCantidadDeJugadores(cantidadDeJugadores);
    this->establecerCantidadDeTesoros(cantidadDeTesoros);
    this->establecerDimensiones(dimensiones);
}

Jugador *Juego::obtenerSiguienteJugador(unsigned int idJugador) {

    if (this->jugadores->estaVacia()) {
        throw out_of_range("No hay jugadores en la partida");
    }

    this->jugadores->iniciarCursor();
    while (this->jugadores->avanzarCursor()) {
        if (this->jugadores->obtenerCursor()->obtenerId() > idJugador) {
            return this->jugadores->obtenerCursor();
        }
    }
    return this->jugadores->obtenerElemento(0);
}

unsigned int Juego::obtenerCantidadJugadores() {
    return this->jugadores->obtenerLongitud();
}

Jugador *Juego::obtenerJugador(unsigned int idJugador) {

    if (this->jugadores->estaVacia()) {
        throw out_of_range("No hay jugadores en la partida");
    }

    Jugador *jugador;
    this->jugadores->iniciarCursor();
    while (this->jugadores->avanzarCursor()) {
        jugador = this->jugadores->obtenerCursor();
        if (jugador->obtenerId() == idJugador) {
            return jugador;
        }
    }
    throw out_of_range("El jugador no existe");
}

Jugador *Juego::obtenerJugadorConMasTesoros() {

    if (this->jugadores->estaVacia()) {
        throw out_of_range("No hay jugadores en la partida");
    }

    Jugador *jugadorConMasTesoros = NULL;
    Jugador *jugador;

    this->jugadores->iniciarCursor();
    while (this->jugadores->avanzarCursor()) {
        jugador = this->jugadores->obtenerCursor();
        if (jugadorConMasTesoros == NULL ||
            (jugadorConMasTesoros->obtenerCantidadDeTesorosRobados() < jugador->obtenerCantidadDeTesorosRobados())) {
            jugadorConMasTesoros = jugador;
        }
    }
    return jugadorConMasTesoros;
}

bool Juego::obtenerJuegoEnCurso() {
    return ((this->obtenerCantidadJugadores() > 1) &&
            (this->obtenerJugadorConMasTesoros()->obtenerCantidadDeTesorosRobados() < this->objetivoRobados));
}

Lista<Jugador *> *Juego::inicializarJugadores(unsigned int cantidadDeJugadores) {

    if (cantidadDeJugadores == 0) {
        throw out_of_range("Debe haber al menos un jugador para crear");
    }

    Lista<Jugador *> *jugadores = new Lista<Jugador *>();
    Jugador *jugador;

    for (unsigned int id = 1; id <= cantidadDeJugadores; id++) {
        jugador = new Jugador(id);
        jugadores->agregarElemento(jugador);
        try {
            this->manejoDeArchivos->crearDirectorioJugador(id);
        } catch (...) {
            throw logic_error("No se pudo crear la directorio del jugador");
        }
    }

    return jugadores;
}

Tablero *
Juego::inicializarTablero(Vector<unsigned int> *dimensiones, unsigned int cantidadDeTesoros, bool esPartidaRapida) {

    this->validarExistencia(dimensiones);
    if (cantidadDeTesoros == 0) {
        throw out_of_range("Debe haber al menos un tesoro para asignar");
    }

    unsigned int ancho = (*dimensiones)[0];
    unsigned int alto = (*dimensiones)[1];
    unsigned int profundidad = (*dimensiones)[2];

    Tablero *tablero = new Tablero(ancho, alto, profundidad, true);
    inicializarTesoros(tablero, cantidadDeTesoros, ancho, alto, profundidad, esPartidaRapida);

    return tablero;
}

void Juego::inicializarTesoros(Tablero *tablero, unsigned int cantidadDeTesoros, unsigned int ancho, unsigned int alto,
                               unsigned int profundidad, bool esPartidaRapida) {

    Casillero *casillero;
    unsigned int x, y, z;
    Jugador *jugador;

    this->jugadores->iniciarCursor();
    while (this->jugadores->avanzarCursor()) {
        jugador = this->jugadores->obtenerCursor();

        srand((unsigned int) time(
                NULL)); // Comentar linea para facilitar DEBUG, los tesoros serian colocados siempre en los mismos lugares, en juegos con misma configuracion inicial
        for (unsigned int i = 1; i <= cantidadDeTesoros; i++) {
            x = rand() % ancho;
            y = rand() % alto;
            z = rand() % profundidad;
            casillero = tablero->obtenerCasillero(x, y, z);

            while (casillero->obtenerTipo() != VACIO) {
                x = rand() % ancho;
                y = rand() % alto;
                z = rand() % profundidad;
                casillero = tablero->obtenerCasillero(x, y, z);
            }
            casillero->convertirA(((esPartidaRapida) ? (TESORO) : (PROTEGIDO)), jugador,
                                  ((esPartidaRapida) ? (0) : (DURACION_PROTECCION * this->obtenerCantidadJugadores())));
            jugador->agregarCasillero(casillero);
        }
    }
}

void Juego::jugar(Jugador *jugador, unsigned int turnoActual) {

    try {
        this->agregarPirata(jugador, turnoActual);
    } catch (const logic_error &e) {
        this->interacciones->mostrarTitulo(e.what());
    }

    try {
        this->agregarBarril(jugador, turnoActual);
    } catch (const logic_error &e) {
        this->interacciones->mostrarTitulo(e.what());
    }

    try {
        this->moverTesoro(jugador, turnoActual);
    } catch (const logic_error &e) {
        this->interacciones->mostrarTitulo(e.what());
    }

    try {
        this->jugarCarta(jugador, turnoActual);
    } catch (const logic_error &e) {
        this->interacciones->mostrarTitulo(e.what());
    }
}

void Juego::mostrarInformacion(Jugador *jugador, unsigned int turnoActual, bool esTurnoDuplicado) {

    try {
        this->mostrarProgresoRobos(jugador, turnoActual);
        this->mostrarCasilleros(jugador);
    } catch (const logic_error &e) {
        this->interacciones->mostrarTitulo(e.what());
    }

    if (!esTurnoDuplicado) {
        try {
            this->sacarCarta(jugador);
        } catch (const logic_error &e) {
            this->interacciones->mostrarTitulo(e.what());
        }
    }

    this->interacciones->mostrarMenu(jugador->obtenerCasilleros(), "Casilleros ocupados");
    this->interacciones->mostrarMenu(jugador->obtenerCartas(), "Cartas en la mano");
}

void Juego::jugarTurno(Jugador *jugador, unsigned int turnoActual) {

    this->interacciones->limpiarConsola();
    this->interacciones->mostrarTitulo(turnoActual, jugador->obtenerId());

    this->mostrarCambios(jugador);

    if (jugador->sigueEnJuego()) {

        this->mostrarInformacion(jugador, turnoActual);
        this->jugar(jugador, turnoActual);
        this->interacciones->mostrarFinDeTurno();
    }
}

void Juego::avanzarTurno(unsigned int idJugador) {
    if (idJugador == 0) {
        throw logic_error("El identificador no puede ser cero");
    } else if (this->juegoEnCurso) {
        Jugador *jugadorSiguiente = this->obtenerSiguienteJugador(idJugador);
        this->turno->avanzarTurno(jugadorSiguiente);
    }
}

void Juego::actualizarJuego() {

    Jugador *jugadorActual;
    unsigned int idJugadorActual;
    unsigned int turnoActual;

    while (this->juegoEnCurso) {

        /* Se guarda el idJugador antes de actualizar jugadores para evitar
         *  errores al intentar acceder al jugador del turno actual */
        jugadorActual = this->turno->obtenerJugador();
        idJugadorActual = jugadorActual->obtenerId();
        turnoActual = this->turno->obtenerTurno();

        this->actualizarOcupados(turnoActual);
        this->actualizarDestruidos(turnoActual);
        this->jugarTurno(jugadorActual, turnoActual);

        // Avanza el turno y el jugador
        this->actualizarJugadores(jugadorActual);
        this->avanzarTurno(idJugadorActual);
        this->juegoEnCurso = this->obtenerJuegoEnCurso();
    }

    /* Mostrar mensaje de fin de juego */
    unsigned int idGanador = this->obtenerJugadorConMasTesoros()->obtenerId();
    this->interacciones->mostrarFinDeJuego(idGanador);
}

void Juego::actualizarInactivos(Jugador *jugador, unsigned int turnoActual) {

    this->validarExistencia(jugador);

    Lista<Casillero *> *robosEnProceso = new Lista<Casillero *>();
    Lista<Casillero *> *inactivos = jugador->obtenerInactivos();
    Casillero *casillero;
    unsigned int inactivoHasta;

    inactivos->iniciarCursor();
    while (inactivos->avanzarCursor()) {
        casillero = inactivos->obtenerCursor();
        inactivoHasta = casillero->obtenerInactivoHasta();

        if (inactivoHasta > turnoActual) {
            robosEnProceso->agregarElemento(casillero);
        } else if (inactivoHasta < turnoActual) {
            throw logic_error("El turno en que deja de estar inactivo no puede ser anterior al actual");
        } else {
            casillero->convertirA(PIRATA, jugador);
            this->agregarCambio(jugador, casillero, ROBOFINALIZADO);
        }
    }

    jugador->reemplazarInactivos(robosEnProceso);
}

void Juego::actualizarProtegidos(Jugador *jugador, unsigned int turnoActual) {

    this->validarExistencia(jugador);

    Lista<Casillero *> *protegidosActivos = new Lista<Casillero *>();
    Lista<Casillero *> *protegidos = jugador->obtenerProtegidos();
    Casillero *casillero;
    unsigned int protegidoHasta;

    protegidos->iniciarCursor();
    while (protegidos->avanzarCursor()) {
        casillero = protegidos->obtenerCursor();
        protegidoHasta = casillero->obtenerProtegidoHasta();

        if (protegidoHasta > turnoActual) {
            protegidosActivos->agregarElemento(casillero);
        } else if (protegidoHasta < turnoActual) {
            throw logic_error("El turno en que deja de estar protegido no puede ser anterior al actual");
        } else {
            casillero->convertirA(TESORO, jugador);
            this->agregarCambio(jugador, casillero, PROTECCIONPERDIDA);
        }
    }

    jugador->reemplazarProtegidos(protegidosActivos);
}

void Juego::actualizarOcupados(unsigned int turnoActual) {

    Jugador *jugador;
    this->jugadores->iniciarCursor();
    while (this->jugadores->avanzarCursor()) {

        jugador = this->jugadores->obtenerCursor();
        this->actualizarInactivos(jugador, turnoActual);
        this->actualizarProtegidos(jugador, turnoActual);
    }
}

void Juego::actualizarDestruidos(unsigned int turnoActual) {

    Lista<Casillero *> *destruidosActivos = new Lista<Casillero *>();
    Casillero *casillero;
    unsigned int destruidoHasta;

    this->destruidos->iniciarCursor();
    while (this->destruidos->avanzarCursor()) {
        casillero = this->destruidos->obtenerCursor();
        destruidoHasta = casillero->obtenerInactivoHasta();

        if (destruidoHasta > turnoActual) {
            destruidosActivos->agregarElemento(casillero);
        } else if (destruidoHasta < turnoActual) {
            throw logic_error("El turno en que deja de estar destruido no puede ser anterior al actual");
        } else {
            casillero->convertirA(VACIO);
        }
    }

    this->destruidos->limpiar();
    delete this->destruidos;
    this->destruidos = destruidosActivos;
}

void Juego::vaciarCasilleros(Lista<Casillero *> *casilleros) {

    if (casilleros == NULL) {
        throw logic_error("La estructura debe existir");
    }

    Casillero *casillero;

    casilleros->iniciarCursor();
    while (casilleros->avanzarCursor()) {
        casillero = casilleros->obtenerCursor();
        casillero->convertirA(VACIO);
    }
}

void Juego::agregarDestruido(Casillero *casillero) {
    if (casillero == NULL) {
        throw logic_error("El casillero debe existir");
    } else if (casillero->obtenerTipo() != DESTRUIDO) {
        throw logic_error("El casillero no es de tipo destruido");
    }
    this->destruidos->agregarElemento(casillero);
}

void Juego::validarExistencia(Jugador *jugador) {
    if (jugador == NULL) {
        throw logic_error("El jugador debe existir");
    }
}

void Juego::validarExistencia(Vector<unsigned int> *estructura, unsigned int longitudEsperada) {
    if (estructura == NULL) {
        throw logic_error("La estructura debe existir");
    } else if (estructura->obtenerLongitud() != longitudEsperada) {
        throw out_of_range("La estructura no posee la longitud esperada");
    }
}

void Juego::actualizarJugadores(Jugador *jugadorTurnoActual) {

    this->validarExistencia(jugadorTurnoActual);

    Lista<Jugador *> *jugadoresActivos = new Lista<Jugador *>();
    Jugador *jugador;

    this->jugadores->iniciarCursor();
    while (this->jugadores->avanzarCursor()) {
        jugador = this->jugadores->obtenerCursor();

        if (jugador->obtenerCantidadDeTesorosActivos() != 0) {
            jugadoresActivos->agregarElemento(jugador);

        } else if (jugador->sigueEnJuego() || (jugadorTurnoActual != jugador)) {
            jugador->dejaDeJugar();
            jugador->agregarCambio(this->interacciones->generarCambio(ELIMINACION));
            jugadoresActivos->agregarElemento(jugador);

        } else {
            this->vaciarCasilleros(jugador->obtenerCasilleros());
            delete jugador;
        }
    }

    this->jugadores->limpiar();
    delete this->jugadores;
    this->jugadores = jugadoresActivos;
}

void Juego::defenderConMercenarios(Jugador *jugador, Vector<unsigned int> *coordenadas) {
    this->validarExistencia(jugador);
    this->validarExistencia(coordenadas);
    jugador->usarMercenarios();
    jugador->agregarCambio(this->interacciones->generarCambio(MERCENARIOSPERDIDOS, coordenadas));
    this->interacciones->mostrarTitulo("Mercenarios enemigos eliminados");
}

void Juego::vaciarCasillero(Jugador *jugador, Casillero *aVaciar, Casillero *moverHacia) {
    this->validarExistencia(jugador);
    jugador->removerCasillero(aVaciar);
    aVaciar->convertirA(VACIO);
    if (moverHacia != NULL) { // En caso afirmativo, ha movido el contenido del casillero al destino
        jugador->agregarCasillero(moverHacia);
    }
}

void Juego::destruirCasillero(Jugador *jugador, Casillero *aDestruir, unsigned int destruidoHasta) {
    this->validarExistencia(jugador);
    jugador->removerCasillero(aDestruir);
    aDestruir->convertirA(DESTRUIDO, NULL, destruidoHasta);
    this->agregarDestruido(aDestruir);
}

void Juego::agregarCambio(Jugador *jugador, Casillero *casillero, unsigned int tipo) {
    this->validarExistencia(jugador);
    jugador->agregarCambio(this->interacciones->generarCambio(tipo, casillero->obtenerCoordenadas()));
}

void Juego::moverTesoro(Jugador *jugador, unsigned int turnoActual) {

    if (!(this->interacciones->preguntar("¿Desea mover un tesoro?", "Si", "No"))) {
        throw logic_error("Ha elegido no mover un tesoro");
    }

    Casillero *origen = this->obtenerTesoro(jugador, "Selecciona un tesoro a mover", true);
    Casillero *destino = this->obtenerDestino(jugador, "Ingresa el casillero hacia donde quieres mover tu tesoro");

    unsigned int tipoOrigen = origen->obtenerTipo();
    Casillero *asociadoAOrigen = (tipoOrigen == PARTIDO) ? (origen->obtenerAsociado()) : (NULL);

    Jugador *enemigo = destino->obtenerJugador();
    unsigned int tipoDestino = destino->obtenerTipo();
    Casillero *asociadoADestino = (tipoDestino == PARTIDO) ? (destino->obtenerAsociado()) : (NULL);

    unsigned int cantidadDeJugadores = this->obtenerCantidadJugadores();
    unsigned int inactivoHasta = turnoActual + DURACION_ROBO * cantidadDeJugadores;
    unsigned int protegidoHasta = turnoActual + DURACION_PROTECCION * cantidadDeJugadores;
    unsigned int destruidoHasta = turnoActual + ((rand() % MAXIMA_POTENCIA_BARRIL) + 1) * cantidadDeJugadores;

    switch (tipoDestino) {

        case VACIO:

            if (tipoOrigen == TESORO) {
                destino->convertirA(TESORO, jugador);

            } else if (tipoOrigen == PARTIDO) {
                asociadoAOrigen->convertirA(PARTIDO, jugador, destino);
                destino->convertirA(PARTIDO, jugador, asociadoAOrigen);

            } else if (tipoOrigen == PROTEGIDO) {
                destino->convertirA(PROTEGIDO, jugador, protegidoHasta);
            }

            this->vaciarCasillero(jugador, origen, destino);
            this->interacciones->mostrarTitulo("Moviste el tesoro a la nueva ubicación");
            break;

        case TESORO:

            if (enemigo->tieneMercenarios()) {
                this->defenderConMercenarios(enemigo, destino->obtenerCoordenadas());

            } else {

                enemigo->removerCasillero(destino);
                destino->convertirA(INACTIVO, jugador, inactivoHasta);
                jugador->agregarCasillero(destino);

                this->agregarCambio(enemigo, destino, TESOROPERDIDO);
                this->interacciones->mostrarTitulo("Encontraste un tesoro enemigo y comenzaste a robarlo");
            }
            break;

        case PIRATA:

            if (jugador->tieneMercenarios()) {
                this->interacciones->mostrarTitulo(
                        "Encontraste un pirata enemigo y usaste a tus mercenarios para escapar con vida");

            } else if (tipoOrigen == PROTEGIDO) {
                origen->convertirA(TESORO, jugador);
                this->interacciones->mostrarTitulo("¡Alerta! Encontraste un pirata enemigo y perdiste tu proteccion");

            } else {

                if (tipoOrigen == PARTIDO) {
                    asociadoAOrigen->convertirA(TESORO, jugador);
                }

                destino->convertirA(INACTIVO, jugador, inactivoHasta);
                this->agregarCambio(enemigo, destino, ROBOINICIADO);
                this->vaciarCasillero(jugador, origen);

                this->interacciones->mostrarTitulo("¡Alerta! Perdiste el tesoro ante un pirata enemigo");
            }
            break;

        case INACTIVO:

            this->interacciones->mostrarTitulo("¡Alerta! Avistaste a un pirata enemigo capturando y lograste escapar");
            break;

        case PROTEGIDO:

            if (enemigo->tieneMercenarios()) {
                this->defenderConMercenarios(enemigo, destino->obtenerCoordenadas());

            } else if (jugador->tieneMercenarios()) {

                jugador->usarMercenarios();
                enemigo->removerProtegido(destino);
                destino->convertirA(TESORO, enemigo);
                this->agregarCambio(enemigo, destino, PROTECCIONPERDIDA);
                this->interacciones->mostrarTitulo(
                        "Encontraste un tesoro protegido, tus mercenarios eliminaron al pirata enemigo");
            } else {

                this->interacciones->mostrarTitulo("¡Alerta! Avistaste a un pirata enemigo y lograste escapar");
            }
            break;

        case BARRIL:

            if (tipoOrigen == PARTIDO) {
                asociadoAOrigen->convertirA(TESORO, jugador);
            }

            enemigo->removerCasillero(destino);
            destino->convertirA(DESTRUIDO, NULL, destruidoHasta);
            this->vaciarCasillero(jugador, origen);
            this->agregarCambio(enemigo, destino, BARRILPERDIDO);

            this->interacciones->mostrarTitulo("¡Alerta! Tu tesoro se perdió en la explosión de un barril enemigo");
            break;

        case DESTRUIDO:

            this->interacciones->mostrarTitulo("Esta ubicación ha sido destruida, no queda nada");
            break;

        case PARTIDO:

            if (enemigo->tieneMercenarios()) {
                this->defenderConMercenarios(enemigo, destino->obtenerCoordenadas());

            } else {
                if (tipoOrigen == PARTIDO) {
                    asociadoAOrigen->convertirA(TESORO, jugador);
                }

                asociadoADestino->convertirA(TESORO, enemigo);
                this->agregarCambio(enemigo, destino, PARTIDOPERDIDO);

                enemigo->removerCasillero(destino);
                destino->convertirA(VACIO);

                this->vaciarCasillero(jugador, origen);
                this->interacciones->mostrarTitulo(
                        "¡Alerta! Has entrado en una zona bajo control enemigo y pierdes tu tesoro");
            }
            break;
    }
}

Vector<unsigned int> *Juego::obtenerCoordenadasEnemigas(Jugador *jugador) {

    Vector<unsigned int> *coordenadas = new Vector<unsigned int>(CANTIDAD_DIMENSIONES);
    unsigned int ancho = this->tablero->obtenerAncho();
    unsigned int alto = this->tablero->obtenerAlto();
    unsigned int profundidad = this->tablero->obtenerProfundidad();

    while (true) {
        try {
            this->interacciones->pedirCoordenadas(ancho, alto, profundidad, coordenadas);

        } catch (const logic_error &e) {
            this->interacciones->mostrarTitulo(e.what());
            continue;
        }

        try {
            jugador->obtenerCasillero(coordenadas);
            this->interacciones->mostrarTitulo("Las coordenadas corresponden a un casillero que ya es tuyo");

        } catch (const out_of_range &e) {
            /* Si atrapa una excepcion, el casillero no pertenece al jugador */
            return coordenadas;
        }
    }
}

void Juego::agregarPirata(Jugador *jugador, unsigned int turnoActual) {

    Casillero *destino = this->obtenerDestino(jugador, "Ingresa el casillero donde quieres colocar tu pirata");
    Jugador *enemigo = destino->obtenerJugador();
    unsigned int tipoDestino = destino->obtenerTipo();

    unsigned int inactivoHasta = turnoActual + DURACION_ROBO * obtenerCantidadJugadores();
    unsigned int destruidoHasta = turnoActual + ((rand() % MAXIMA_POTENCIA_BARRIL) + 1) * obtenerCantidadJugadores();

    if (((tipoDestino == TESORO) || (tipoDestino == PROTEGIDO) || (tipoDestino == PARTIDO) ||
         (tipoDestino == INACTIVO)) && enemigo->tieneMercenarios()) {
        this->defenderConMercenarios(enemigo, destino->obtenerCoordenadas());

    } else {

        switch (tipoDestino) {

            case VACIO:

                destino->convertirA(PIRATA, jugador);
                jugador->agregarCasillero(destino);

                this->interacciones->mostrarTitulo("Pirata desplegado y en posición");
                break;

            case TESORO:

                enemigo->removerCasillero(destino);
                this->agregarCambio(enemigo, destino, TESOROPERDIDO);

                destino->convertirA(INACTIVO, jugador, inactivoHasta);
                jugador->agregarCasillero(destino);

                this->interacciones->mostrarTurnosInactividad("Tu pirata se topó con un tesoro enemigo", inactivoHasta);
                break;

            case PIRATA:

                this->vaciarCasillero(enemigo, destino);
                this->agregarCambio(enemigo, destino, PIRATAPERDIDO);

                this->interacciones->mostrarTitulo(
                        "Tu pirata se topó con un pirata enemigo, ambos piratas fueron eliminados");
                break;

            case BARRIL:

                this->destruirCasillero(enemigo, destino, destruidoHasta);
                this->agregarCambio(enemigo, destino, BARRILPERDIDO);

                this->interacciones->mostrarTitulo("Tu pirata se topó con un barril enemigo y fue eliminado");
                break;

            case PROTEGIDO:

                this->interacciones->mostrarTitulo("Tu pirata se topó con un tesoro protegido y fue eliminado");
                break;

            case DESTRUIDO:

                this->interacciones->mostrarTitulo("Esta ubicación ha sido destruida, no queda nada");
                break;

            case INACTIVO:

                enemigo->removerCasillero(destino);
                this->agregarCambio(enemigo, destino, ROBOINTERRUMPIDO);

                destino->convertirA(INACTIVO, jugador, destino->obtenerInactivoHasta());
                jugador->agregarCasillero(destino);

                this->interacciones->mostrarTurnosInactividad("Interrumpiste una captura",
                                                              destino->obtenerInactivoHasta());
                break;

            case PARTIDO:

                Casillero *asociadoADestino = destino->obtenerAsociado();
                asociadoADestino->convertirA(TESORO, enemigo);

                enemigo->removerCasillero(destino);
                this->agregarCambio(enemigo, destino, PARTIDOPERDIDO);

                destino->convertirA(PIRATA, jugador);
                jugador->agregarCasillero(destino);

                this->interacciones->mostrarTitulo(
                        "Tu pirata encontró parte de un tesoro, sigue buscando la otra parte");
                break;
        }
    }
}

void Juego::agregarBarril(Jugador *jugador, unsigned int turnoActual) {

    Casillero *destino = this->obtenerDestino(jugador,
                                              "Ingresa el casillero donde quieres colocar tu barril explosivo");
    Jugador *enemigo = destino->obtenerJugador();

    unsigned int destruidoHasta = turnoActual + (((rand() % MAXIMA_POTENCIA_BARRIL) + 1) * obtenerCantidadJugadores());

    switch (destino->obtenerTipo()) {

        case VACIO:

            destino->convertirA(BARRIL, jugador);
            jugador->agregarCasillero(destino);

            this->interacciones->mostrarTitulo("Barril explosivo cargado y preparado");
            break;

        case TESORO:

            this->destruirCasillero(enemigo, destino, destruidoHasta);
            this->agregarCambio(enemigo, destino, TESOROPERDIDO);

            this->interacciones->mostrarTurnosInactividad("Destruiste un tesoro enemigo", destruidoHasta);
            break;

        case PIRATA:

            this->destruirCasillero(enemigo, destino, destruidoHasta);
            this->agregarCambio(enemigo, destino, PIRATAPERDIDO);

            this->interacciones->mostrarTurnosInactividad("Eliminaste a un pirata enemigo", destruidoHasta);
            break;

        case BARRIL:

            this->destruirCasillero(enemigo, destino, destruidoHasta);
            this->agregarCambio(enemigo, destino, BARRILPERDIDO);

            this->interacciones->mostrarTurnosInactividad("Destruiste un barril enemigo", destruidoHasta);
            break;

        case PROTEGIDO:

            enemigo->removerProtegido(destino);
            destino->convertirA(TESORO, enemigo);
            this->agregarCambio(enemigo, destino, PROTECCIONPERDIDA);

            this->interacciones->mostrarTitulo("Eliminaste al pirata que protegía un tesoro enemigo");
            break;

        case INACTIVO:

            this->destruirCasillero(enemigo, destino, destruidoHasta);
            this->agregarCambio(enemigo, destino, ROBOINTERRUMPIDO);

            this->interacciones->mostrarTurnosInactividad("Interrumpiste una captura", destruidoHasta);
            break;

        case DESTRUIDO:

            destino->convertirA(DESTRUIDO, NULL, destruidoHasta);
            this->interacciones->mostrarTurnosInactividad("El barril explota en una ubicación destruida",
                                                          destruidoHasta);
            break;

        case PARTIDO:

            Casillero *asociadoADestino = destino->obtenerAsociado();
            asociadoADestino->convertirA(TESORO, enemigo);

            this->destruirCasillero(enemigo, destino, destruidoHasta);
            this->agregarCambio(enemigo, destino, PARTIDOPERDIDO);

            this->interacciones->mostrarTurnosInactividad("El barril explota sobre parte de un tesoro enemigo",
                                                          destruidoHasta);
            break;
    }
}

void Juego::validarExistencia(Lista<Casillero *> *estructura) {
    if (estructura == NULL) {
        throw logic_error("La estructura debe existir");
    } else if (estructura->estaVacia()) {
        throw out_of_range("La estructura no puede estar vacía");
    }
}

void Juego::validarExistencia(Lista<unsigned int> *estructura) {
    if (estructura == NULL) {
        throw logic_error("La estructura debe existir");
    } else if (estructura->estaVacia()) {
        throw out_of_range("La estructura está vacía");
    }
}

void Juego::validarExistencia(Carta *carta) {
    if (carta == NULL) {
        throw logic_error("La carta debe existir");
    }
}

Lista<Casillero *> *Juego::filtrarCasilleros(Lista<Casillero *> *casilleros, Lista<unsigned int> *tiposAceptados) {

    this->validarExistencia(casilleros);
    this->validarExistencia(tiposAceptados);

    Lista<Casillero *> *casillerosFiltrados = new Lista<Casillero *>();
    Casillero *casillero;

    casilleros->iniciarCursor();
    while (casilleros->avanzarCursor()) {
        casillero = casilleros->obtenerCursor();

        if (tiposAceptados->contiene(casillero->obtenerTipo())) {
            casillerosFiltrados->agregarElemento(casillero);
        }
    }
    return casillerosFiltrados;
}

void Juego::descartar(Jugador *jugador, Carta *carta) {

    this->validarExistencia(jugador);
    this->validarExistencia(carta);

    if (!(this->interacciones->preguntar("¿Desea descartar una carta?", "Si", "No"))) {
        delete carta;
        throw out_of_range("Ha elegido no descartar");

    } else {

        Carta *cartaDescartada = this->interacciones->elegirMenu(jugador->obtenerCartas(),
                                                                 "Selecciona una carta de tu mano para descartar");

        if (cartaDescartada == NULL) {
            throw logic_error("Opcion invalida");
        }

        jugador->usarCarta(cartaDescartada->obtenerTipo());
        jugador->agregarCarta(carta);
        this->interacciones->mostrarObtencionCarta(carta->obtenerTipo());
    }
}

void Juego::sacarCarta(Jugador *jugador) {

    this->validarExistencia(jugador);

    srand((unsigned int) time(NULL));
    Carta *carta = new Carta((unsigned int) rand() % CANTIDAD_CARTAS);

    try {
        jugador->agregarCarta(carta);

    } catch (const out_of_range &e) {

        this->interacciones->mostrarTitulo(e.what());

        try {
            this->descartar(jugador, carta);

        } catch (const logic_error &e) {
            delete carta;
            throw out_of_range("Ha elegido no descartar");
        }
    }
}

void Juego::jugarCarta(Jugador *jugador, unsigned int turnoActual) {

    this->validarExistencia(jugador);

    if (!(this->interacciones->preguntar("¿Desea jugar una carta?", "Si", "No"))) {
        throw out_of_range("Ha elegido no jugar una carta");
    }

    Carta *cartaJugada = this->interacciones->elegirMenu(jugador->obtenerCartas(), "Selecciona una carta de tu mano");

    if (cartaJugada == NULL) {
        throw logic_error("Opcion invalida");
    }

    switch (cartaJugada->obtenerTipo()) {
        case PROTEGER:
            this->jugarProteger(jugador, turnoActual);
            break;
        case REVELAR:
            this->jugarRadar(jugador);
            break;
        case PARTIRTESORO:
            this->jugarPartirTesoro(jugador);
            break;
        case ROBARCARTA:
            this->jugarRobarCarta(jugador);
            break;
        case DUPLICARTURNO:
            this->jugarDuplicarTurno(jugador, turnoActual);
            break;
        case MERCENARIOS:
            this->jugarContratarMercenarios(jugador);
            break;
    }
}

void Juego::jugarProteger(Jugador *jugador, unsigned int turnoActual) {

    this->validarExistencia(jugador);
    Casillero *aProteger = this->obtenerTesoro(jugador, "Selecciona un tesoro para proteger");

    jugador->usarCarta(PROTEGER);

    unsigned int protegidoHasta = turnoActual + DURACION_PROTECCION * obtenerCantidadJugadores();
    aProteger->convertirA(PROTEGIDO, jugador, protegidoHasta);
    jugador->agregarProtegido(aProteger);
    this->interacciones->mostrarTitulo("Tesoro protegido con exito");
}

void Juego::jugarRadar(Jugador *jugador) {

    this->validarExistencia(jugador);
    this->interacciones->mostrarTitulo("Ingresa el casillero donde quieres colocar tu radar");
    this->exportar(jugador->obtenerCasilleros(), "Selecciona el casillero para colocar tu radar", false);

    jugador->usarCarta(REVELAR);
    this->interacciones->mostrarTitulo("Casillero exportado correctamente");
}

Casillero *Juego::obtenerTesoro(Jugador *jugador, string indicacion, bool cualquierTipo) {

    this->validarExistencia(jugador);

    Lista<unsigned int> *tiposBuscados = new Lista<unsigned int>();
    tiposBuscados->agregarElemento(TESORO);
    if (cualquierTipo) {
        tiposBuscados->agregarElemento(PROTEGIDO);
        tiposBuscados->agregarElemento(PARTIDO);
    }

    Lista<Casillero *> *casillerosFiltrados = this->filtrarCasilleros(jugador->obtenerCasilleros(), tiposBuscados);

    if (casillerosFiltrados->estaVacia()) {
        delete casillerosFiltrados;
        delete tiposBuscados;
        throw logic_error("No posees tesoros disponibles para elegir");
    }

    Casillero *aPartir = this->interacciones->elegirMenu(casillerosFiltrados, indicacion);

    while (aPartir == NULL) {
        this->interacciones->mostrarTitulo("No elegiste un tesoro válido");
        aPartir = this->interacciones->elegirMenu(casillerosFiltrados, indicacion);
    }

    delete casillerosFiltrados;
    delete tiposBuscados;
    return aPartir;
}

Casillero *Juego::obtenerDestino(Jugador *jugador, string indicacion) {

    this->validarExistencia(jugador);
    this->interacciones->mostrarTitulo(indicacion);

    Vector<unsigned int> *coordenadas = this->obtenerCoordenadasEnemigas(jugador);

    Casillero *destino = this->tablero->obtenerCasillero(coordenadas);

    delete coordenadas;
    return destino;
}

void Juego::jugarPartirTesoro(Jugador *jugador) {

    this->validarExistencia(jugador);
    Casillero *aPartir = this->obtenerTesoro(jugador, "Selecciona un tesoro para partir");
    Casillero *destino = this->obtenerDestino(jugador,
                                              "Ingresa el casillero donde quieres colocar una parte del tesoro");

    if (destino->obtenerTipo() != VACIO) {
        throw logic_error("Esa casilla está ocupada");
    }

    jugador->usarCarta(PARTIRTESORO);

    aPartir->convertirA(PARTIDO, jugador, destino);
    destino->convertirA(PARTIDO, jugador, aPartir);
    jugador->agregarCasillero(destino);

    this->interacciones->mostrarTitulo("Tesoro partido con éxito");
}

void Juego::jugarRobarCarta(Jugador *jugador) {

    this->validarExistencia(jugador);
    Jugador *enemigo = this->interacciones->elegirMenu(this->jugadores, jugador->obtenerId(), "Jugadores Activos");

    if (enemigo == NULL) {
        throw logic_error("Opción inválida");
    } else if (enemigo == jugador) {
        throw logic_error("No podes robarte cartas a vos mismo");
    } else if (enemigo->obtenerCantidadDeCartas() == 0) {
        throw logic_error("El jugador seleccionado no tiene cartas para robar");
    }

    Carta *cartaRobada = enemigo->obtenerCartaAleatoria();

    jugador->usarCarta(ROBARCARTA);
    enemigo->usarCarta(cartaRobada->obtenerTipo(), false);
    enemigo->agregarCambio(this->interacciones->generarCambio(CARTAROBADA));
    jugador->agregarCarta(cartaRobada);
    this->interacciones->mostrarRoboCarta(cartaRobada->obtenerTipo());
}

void Juego::jugarDuplicarTurno(Jugador *jugador, unsigned int turnoActual) {

    this->validarExistencia(jugador);
    jugador->usarCarta(DUPLICARTURNO);
    this->interacciones->mostrarTitulo("Doble turno activado");
    this->interacciones->mostrarTitulo(turnoActual, jugador->obtenerId());

    try {
        this->mostrarInformacion(jugador, turnoActual, true);
    } catch (const logic_error &e) {
        this->interacciones->mostrarTitulo("No posee cartas en su mano");
    }

    /* Se invocan las acciones que el Jugador puede realizar en un turno duplicado */
    agregarPirata(jugador, turnoActual);
    agregarBarril(jugador, turnoActual);
}

void Juego::jugarContratarMercenarios(Jugador *jugador) {

    this->validarExistencia(jugador);
    try {
        jugador->contratarMercenarios();
        jugador->usarCarta(MERCENARIOS);
        this->interacciones->mostrarTitulo("Mercenarios contratados");
    } catch (const logic_error &e) {
        this->interacciones->mostrarTitulo(e.what());
    }
}

void Juego::mostrarCasilleros(Jugador *jugador) {

    this->validarExistencia(jugador);
    bool quiereExportar = this->interacciones->preguntar("¿Exportar tablero?", "Si", "No");
    Lista<Casillero *> *casilleros = jugador->obtenerCasilleros();
    while (quiereExportar) {
        try {
            exportar(casilleros);
        } catch (const logic_error &e) {
            this->interacciones->mostrarTitulo(e.what());
            break;
        }
        this->interacciones->mostrarTitulo("Casillero exportado correctamente");
        quiereExportar = this->interacciones->preguntar("¿Desea exportar nuevamente?", "Si", "No");
    }
}

void Juego::exportar(Lista<Casillero *> *casilleros, string indicacion, bool ocultarEnemigos) {

    this->validarExistencia(casilleros);
    Casillero *casillero = this->interacciones->elegirMenu(casilleros, indicacion);
    if (casillero == NULL) {
        throw logic_error("No se ha elegido un casillero");
    }

    this->manejoDeArchivos->exportar(casillero->obtenerPlanoXY(), ocultarEnemigos);
    this->manejoDeArchivos->exportar(casillero->obtenerPlanoXZ(), ocultarEnemigos);
    this->manejoDeArchivos->exportar(casillero->obtenerPlanoYZ(), ocultarEnemigos);
}

void Juego::mostrarProgresoRobos(Jugador *jugador, unsigned int turnoActual) {

    this->validarExistencia(jugador);
    Lista<Casillero *> *casilleros = jugador->obtenerInactivos();
    Casillero *casillero;
    unsigned int inactivoHasta, turnosRestantes, x, y, z;

    casilleros->iniciarCursor();
    while (casilleros->avanzarCursor()) {
        casillero = casilleros->obtenerCursor();

        x = casillero->obtenerX();
        y = casillero->obtenerY();
        z = casillero->obtenerZ();
        inactivoHasta = casillero->obtenerInactivoHasta();

        if (inactivoHasta > turnoActual) {
            turnosRestantes = (inactivoHasta - turnoActual);
            this->interacciones->mostrarProgresoRobo(x, y, z, turnosRestantes);
        } else {
            throw logic_error("El turno en que finaliza el robo no puede ser anterior al turno actual");
        }
    }
}

void Juego::mostrarCambios(Jugador *jugador) {

    this->validarExistencia(jugador);
    try {
        this->interacciones->mostrarMenu(jugador->obtenerCambios());
    } catch (const logic_error &e) {
        this->interacciones->mostrarTitulo(e.what());
    }
}
