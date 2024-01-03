#include "Interacciones.h"

Interacciones::Interacciones() {}

Interacciones::~Interacciones() {}

void Interacciones::limpiarBufer() {
    cin.clear();
    cin.ignore(10000, '\n');
}

void Interacciones::limpiarConsola() {
    for (unsigned int i = 0; i < ALTO_CONSOLA; ++i) {
        cout << endl;
    }
}

string Interacciones::pasarAMinuscula(string cadena) {

    string cadenaEnMinuscula;

    for (unsigned int i = 0; i < cadena.length(); ++i) {
        char caracter = cadena[i];
        if ('A' <= caracter && caracter <= 'Z') {
            caracter = 'a' + (caracter - 'A');
        }
        cadenaEnMinuscula += caracter;
    }
    return cadenaEnMinuscula;
}

void Interacciones::validarExistencia(Lista<Casillero *> *casilleros) const {
    if (casilleros == NULL || (casilleros->estaVacia())) {
        throw logic_error("La estructura esta vacia o no existe");
    }
}

void Interacciones::validarExistencia(Lista<Carta *> *cartas) const {
    if (cartas == NULL || (cartas->estaVacia())) {
        throw logic_error("La estructura esta vacia o no existe");
    }
}

void Interacciones::validarExistencia(Lista<Jugador *> *jugadores) const {
    if (jugadores == NULL || (jugadores->estaVacia())) {
        throw logic_error("La estructura esta vacia o no existe");
    }
}

void Interacciones::validarExistencia(Pila<string> *cambios) const {
    if (cambios == NULL || (cambios->estaVacia())) {
        throw logic_error("La estructura esta vacia o no existe");
    }
}

void
Interacciones::validarPosicion(unsigned int x, unsigned int y, unsigned int z, unsigned int ancho, unsigned int alto,
                               unsigned int profundidad) const {
    if (!((x < ancho) && (y < alto) && (z < profundidad))) {
        throw out_of_range("La posición no pertenece al tablero");
    }
}

void Interacciones::validarDimensiones(unsigned int ancho, unsigned int alto, unsigned int profundidad) const {

    bool validacionMinimoDimension =
            (MINIMO_DIMENSION <= ancho) && (MINIMO_DIMENSION <= alto) && (MINIMO_DIMENSION <= profundidad);
    bool validacionMaximoDimension =
            (MAXIMO_DIMENSION >= ancho) && (MAXIMO_DIMENSION >= alto) && (MAXIMO_DIMENSION >= profundidad);

    if (!validacionMinimoDimension || !validacionMaximoDimension) {
        throw out_of_range("Las dimensiones son invalidas");
    }
}

void Interacciones::validarLongitud(Vector<unsigned int> *estructura, unsigned int longitud) const {
    if (estructura == NULL) {
        throw logic_error("No existe la estructura");
    } else if (estructura->obtenerLongitud() != longitud) {
        throw out_of_range("La estructura no posee la longitud buscada");
    }
}

string Interacciones::concatenar(string primerMensaje, string segundoMensaje) {
    ostringstream cadena;
    cadena << primerMensaje << segundoMensaje;
    return cadena.str();
}

string Interacciones::generarPregunta(string pregunta, string respuestaA, string respuestaB) {
    ostringstream cadena;
    cadena << pregunta << " (" << respuestaA << "/" << respuestaB << "): ";
    cadena.str(this->agregarSangria(cadena.str()));
    cadena << "\n";
    return cadena.str();
}

string Interacciones::generarOpcion(unsigned int desde, unsigned int hasta, string pregunta) {
    ostringstream cadena;
    cadena << pregunta << " (Entre " << desde << " y " << hasta << "): ";
    cadena.str(this->agregarSangria(cadena.str()));
    cadena << "\n";
    return cadena.str();
}

string Interacciones::generarOpcion(Casillero *casillero, unsigned int opcion) {

    ostringstream cadena;
    cadena << opcion << ") Tipo: " << this->generarNombreCasillero((casillero->obtenerTipo())) << " | Coordenadas: ("
           << (casillero->obtenerX() + 1) << ", " << (casillero->obtenerY() + 1) << ", " << (casillero->obtenerZ() + 1)
           << ")";
    cadena.str(this->centrar(cadena.str()));
    cadena << "\n";
    return cadena.str();
}

string Interacciones::generarOpcion(Carta *carta, unsigned int opcion) {

    ostringstream cadena;
    cadena << opcion << ") Tipo: " << this->generarNombreCarta((carta->obtenerTipo()));
    cadena.str(this->centrar(cadena.str()));
    cadena << "\n";
    return cadena.str();
}

string Interacciones::generarOpcion(Jugador *jugador) {
    ostringstream cadena;
    cadena << jugador->obtenerId() << ") Jugador " << jugador->obtenerId();
    cadena.str(this->centrar(cadena.str()));
    cadena << "\n";
    return cadena.str();
}

string Interacciones::centrar(string titulo, bool conGuiones) {

    unsigned int longitudMensaje = (titulo.length() + 2);

    if (longitudMensaje > ANCHO_CONSOLA) {
        throw out_of_range("La longitud del mensaje supera el límite permitido");
    }

    // Calcula la cantidad de guiones en cada lado
    unsigned int rellenoInicio = (ANCHO_CONSOLA - longitudMensaje) / 2;
    unsigned int rellenoFinal = ANCHO_CONSOLA - longitudMensaje - rellenoInicio;
    char relleno = (conGuiones) ? ('-') : (' ');

    ostringstream cadena;
    cadena << string(rellenoInicio, relleno) << " " << titulo << " " << string(rellenoFinal, relleno);
    return cadena.str();
}

string Interacciones::agregarSangria(string titulo) {

    unsigned int longitudMensaje = (titulo.length() + 2);

    if (longitudMensaje > ANCHO_CONSOLA) {
        throw out_of_range("La longitud del mensaje supera el límite permitido");
    }

    // Calcula la cantidad de guiones en cada lado
    unsigned int sangria = (ANCHO_CONSOLA - longitudMensaje) / 2;

    ostringstream cadena;
    cadena << string(sangria, ' ') << " " << titulo;
    return cadena.str();
}

string Interacciones::agregarEspaciado(string titulo, unsigned int espaciado) {
    ostringstream cadena;
    cadena << string(espaciado, '\n') << titulo << string(espaciado, '\n');
    return cadena.str();
}

string Interacciones::generarNombreCasillero(unsigned int tipoCasillero) {

    switch (tipoCasillero) {
        case VACIO:
            return "Vacio";
        case TESORO:
            return "Tesoro";
        case PIRATA:
            return "Pirata";
        case INACTIVO:
            return "Inactivo";
        case PROTEGIDO:
            return "Protegido";
        case BARRIL:
            return "Barril";
        case PARTIDO:
            return "Tesoro Partido";
        case DESTRUIDO:
            return "Destruido";
        default:
            throw out_of_range("El tipo de casillero buscado no existe");
    }
}

string Interacciones::generarNombreCarta(unsigned int tipoCarta) {

    switch (tipoCarta) {
        case PROTEGER:
            return "Proteccion";
        case REVELAR:
            return "Revelar";
        case PARTIRTESORO:
            return "Partir Tesoro";
        case ROBARCARTA:
            return "Robar Carta";
        case DUPLICARTURNO:
            return "Doble turno";
        case MERCENARIOS:
            return "Contratar mercenarios";
        default:
            throw out_of_range("El tipo de carta buscado no existe");
    }
}

void Interacciones::mostrarDimensionInvalida() {
    this->limpiarBufer();
    this->mostrarMensaje("");
    throw invalid_argument("Por favor, ingrese un NUMERO entre los valores pedidos");
}

void Interacciones::mostrarFinDeTurno() {
    this->limpiarBufer();
    this->mostrarTitulo("Presiona Enter para finalizar el turno");
    while (cin.get() != '\n') {
        // Ignora cualquier caracter que no sea Enter
    }
}

void Interacciones::pedirDimensiones(Vector<unsigned int> *dimensiones) {

    this->validarLongitud(dimensiones);
    unsigned int ancho = 0, alto = 0, profundidad = 0;

    this->mostrarMensaje(this->generarOpcion(MINIMO_DIMENSION, MAXIMO_DIMENSION, "Ingrese el ancho del tablero"));
    if (!(cin >> ancho)) {
        this->mostrarDimensionInvalida();
    }

    this->mostrarMensaje(this->generarOpcion(MINIMO_DIMENSION, MAXIMO_DIMENSION, "Ingrese el alto del tablero"));
    if (!(cin >> alto)) {
        this->mostrarDimensionInvalida();
    }

    this->mostrarMensaje(this->generarOpcion(MINIMO_DIMENSION, MAXIMO_DIMENSION, "Ingrese la profundidad del tablero"));
    if (!(cin >> profundidad)) {
        this->mostrarDimensionInvalida();
    }

    this->validarDimensiones(ancho, alto, profundidad);

    dimensiones->reemplazarElemento(0, ancho);
    dimensiones->reemplazarElemento(1, alto);
    dimensiones->reemplazarElemento(2, profundidad);
}

void Interacciones::pedirCoordenadas(unsigned int ancho, unsigned int alto, unsigned int profundidad,
                                     Vector<unsigned int> *coordenadas) {

    this->validarLongitud(coordenadas);

    unsigned int x, y, z;

    this->mostrarMensaje(this->generarOpcion(1, ancho, "Ingrese la coordenada X"));
    cin >> x;

    this->mostrarMensaje(this->generarOpcion(1, alto, "Ingrese la coordenada Y"));
    cin >> y;

    this->mostrarMensaje(this->generarOpcion(1, profundidad, "Ingrese la coordenada Z"));
    cin >> z;

    this->limpiarBufer();
    this->validarPosicion((x - 1), (y - 1), (z - 1), ancho, alto, profundidad);

    coordenadas->reemplazarElemento(0, (x - 1));
    coordenadas->reemplazarElemento(1, (y - 1));
    coordenadas->reemplazarElemento(2, (z - 1));
}

unsigned int Interacciones::pedirOpcion(unsigned int desde, unsigned int hasta, string pregunta) {

    unsigned int opcion;

    this->mostrarMensaje(this->generarOpcion(desde, hasta, pregunta));
    cin >> opcion;

    this->limpiarBufer();
    if (!(desde <= opcion && opcion <= hasta)) {
        throw out_of_range("La opcion elegida no es valida");
    }

    return opcion;
}

bool Interacciones::preguntar(string pregunta, string respuestaA, string respuestaB) {

    string respuesta;
    this->mostrarMensaje(this->generarPregunta(pregunta, respuestaA, respuestaB));
    cin >> respuesta;

    string respuestaEnMinuscula = this->pasarAMinuscula(respuesta);
    string respuestaAEnMinuscula = this->pasarAMinuscula(respuestaA);
    string respuestaBEnMinuscula = this->pasarAMinuscula(respuestaB);

    if (respuestaEnMinuscula == respuestaAEnMinuscula) {
        return true;
    } else if (respuestaEnMinuscula == respuestaBEnMinuscula) {
        return false;
    } else {
        throw logic_error("La respuesta ingresada no es valida");
    }
}

void Interacciones::mostrarMensaje(string mensaje) { cout << mensaje; }

string Interacciones::generarTitulo(unsigned int turno, unsigned int idJugador) {
    ostringstream cadena;
    cadena << "TESORO BINARIO ---- TURNO: " << turno << " JUGADOR " << idJugador;
    return this->agregarEspaciado(this->centrar(cadena.str(), true), ESPACIADO_TITULO);
}

string Interacciones::generarTitulo(string titulo, unsigned int espaciado, bool conGuiones) {
    return this->agregarEspaciado(this->centrar(titulo, conGuiones), espaciado);
}

void Interacciones::mostrarFinDeMenu() {
    this->mostrarMensaje(
            this->concatenar("\n", (this->agregarEspaciado(string(ANCHO_CONSOLA, '-'), ESPACIADO_SIMPLE))));
}

Casillero *Interacciones::elegirMenu(Lista<Casillero *> *casilleros, string indicacion) {

    this->mostrarMenu(casilleros);

    unsigned int opcion;
    try {
        opcion = this->pedirOpcion(1, (casilleros->obtenerLongitud()), indicacion);
        return casilleros->obtenerElemento(opcion - 1);
    } catch (const out_of_range &e) {
        return NULL;
    }
}

Carta *Interacciones::elegirMenu(Lista<Carta *> *cartas, string indicacion) {

    this->mostrarMenu(cartas);

    unsigned int opcion;
    try {
        cout << endl;
        opcion = this->pedirOpcion(1, (cartas->obtenerLongitud()), indicacion);
        return cartas->obtenerElemento(opcion - 1);
    } catch (const out_of_range &e) {
        return NULL;
    }
}

Jugador *Interacciones::elegirMenu(Lista<Jugador *> *jugadores, unsigned int idJugador, string indicacion) {

    this->mostrarMenu(jugadores, idJugador, indicacion);

    unsigned int idPrimerJugador = jugadores->obtenerElemento(0)->obtenerId();
    unsigned int idPrimerOpcion = (idPrimerJugador == idJugador) ? (jugadores->obtenerElemento(1)->obtenerId())
                                                                 : (idPrimerJugador);
    unsigned int idUltimoJugador = jugadores->obtenerElemento(jugadores->obtenerLongitud() - 1)->obtenerId();
    unsigned int idUltimaOpcion = (idUltimoJugador == idJugador) ? (jugadores->obtenerElemento(
            jugadores->obtenerLongitud() - 2)->obtenerId()) : (idUltimoJugador);

    unsigned int opcion;
    try {
        cout << endl;
        opcion = this->pedirOpcion(idPrimerOpcion, idUltimaOpcion, indicacion);
        return jugadores->obtenerElemento(opcion - 1);
    } catch (const out_of_range &e) {
        return NULL;
    }
}

void Interacciones::mostrarTitulo(unsigned int turno, unsigned int idJugador) {
    this->mostrarMensaje(this->generarTitulo(turno, idJugador));
}

void Interacciones::mostrarTitulo(string titulo, unsigned int espaciado) {
    this->mostrarMensaje(this->generarTitulo(titulo, espaciado));
}

void Interacciones::mostrarMenu(Lista<Casillero *> *casilleros, string titulo) {

    this->validarExistencia(casilleros);

    this->mostrarTitulo(titulo);

    Casillero *casillero;
    unsigned int opcion = 0;

    casilleros->iniciarCursor();

    while (casilleros->avanzarCursor()) {
        opcion++;
        casillero = casilleros->obtenerCursor();
        this->mostrarMensaje(this->generarOpcion(casillero, opcion));
    }
    this->mostrarFinDeMenu();
}

void Interacciones::mostrarMenu(Lista<Carta *> *cartas, string titulo) {

    this->validarExistencia(cartas);

    this->mostrarTitulo(titulo);

    Carta *carta;
    unsigned int opcion = 0;

    cartas->iniciarCursor();

    while (cartas->avanzarCursor()) {
        opcion++;
        carta = cartas->obtenerCursor();
        this->mostrarMensaje(this->generarOpcion(carta, opcion));
    }
    this->mostrarFinDeMenu();
}

void Interacciones::mostrarMenu(Lista<Jugador *> *jugadores, unsigned int idJugador, string titulo) {

    this->validarExistencia(jugadores);

    this->mostrarTitulo(titulo);

    Jugador *jugador;

    jugadores->iniciarCursor();

    while (jugadores->avanzarCursor()) {
        jugador = jugadores->obtenerCursor();
        if (jugador->obtenerId() != idJugador) {
            this->mostrarMensaje(this->generarOpcion(jugador));
        }
    }
    this->mostrarFinDeMenu();
}

void Interacciones::mostrarMenu(Pila<string> *cambios, string titulo) {

    try {
        this->validarExistencia(cambios);
    } catch (const logic_error &e) {
        throw logic_error("No hay cambios registrados");
    }

    this->mostrarTitulo(titulo);
    while (!(cambios->estaVacia())) {
        this->mostrarMensaje(cambios->desapilar());
    }
    this->mostrarFinDeMenu();
}

void Interacciones::mostrarProgresoRobo(unsigned int x, unsigned int y, unsigned int z, unsigned int turnosRestantes) {
    ostringstream cadena;
    cadena << "El robo en la posición: (" << (x + 1) << "," << (y + 1) << "," << (z + 1) << ") finaliza en "
           << turnosRestantes << " turnos";
    this->mostrarTitulo(cadena.str());
    cadena.str("");
}

void Interacciones::mostrarFinDeJuego(unsigned int idJugador) {
    ostringstream cadena;
    cadena << "Juego terminado. Ha ganado el jugador " << idJugador;
    this->mostrarTitulo(cadena.str(), ESPACIADO_TITULO);
}

void Interacciones::mostrarObtencionCarta(unsigned int tipoCarta) {
    ostringstream cadena;
    cadena << "Nueva carta obtenida: " << this->generarNombreCarta(tipoCarta);
    this->mostrarTitulo(cadena.str(), ESPACIADO_NOTIFICACION);
    cadena.str("");
}

void Interacciones::mostrarRoboCarta(unsigned int tipoCarta) {
    ostringstream cadena;
    cadena << "Carta robada: " << this->generarNombreCarta(tipoCarta);
    this->mostrarTitulo(cadena.str(), ESPACIADO_NOTIFICACION);
    cadena.str("");
}

void Interacciones::mostrarTurnosInactividad(string mensaje, unsigned int hasta) {
    ostringstream cadena;
    cadena << mensaje << ", el casillero queda inactivo por " << hasta << " turnos";
    this->mostrarTitulo(cadena.str());
    cadena.str("");
}

string Interacciones::generarCambio(unsigned int tipo, Vector<unsigned int> *coordenadas) {

    ostringstream cadena;
    string titulo;

    switch (tipo) {
        case ELIMINACION:
            return this->generarTitulo("Jugador eliminado", ESPACIADO_SIMPLE, false);
        case CARTAROBADA:
            return this->generarTitulo("Carta robada", ESPACIADO_SIMPLE, false);
        case TESOROPERDIDO:
            titulo = "Tesoro perdido";
            break;
        case PIRATAPERDIDO:
            titulo = "Pirata perdido";
            break;
        case BARRILPERDIDO:
            titulo = "Barril explosivo perdido";
            break;
        case PROTECCIONPERDIDA:
            titulo = "Protección perdida";
            break;
        case PARTIDOPERDIDO:
            titulo = "Tesoro partido perdido";
            break;
        case MERCENARIOSPERDIDOS:
            titulo = "Mercenarios eliminados";
            break;
        case ROBOINICIADO:
            titulo = "Robo iniciado";
            break;
        case ROBOFINALIZADO:
            titulo = "Robo finalizado";
            break;
        case ROBOINTERRUMPIDO:
            titulo = "Robo interrumpido";
            break;
        default:
            throw out_of_range("El tipo de cambio buscado no existe");
    }

    this->validarLongitud(coordenadas);

    unsigned int x = (*coordenadas)[0];
    unsigned int y = (*coordenadas)[1];
    unsigned int z = (*coordenadas)[2];

    cadena << titulo << " en la posición (" << (x + 1) << "," << (y + 1) << "," << (z + 1) << ")";
    cadena.str(this->centrar(cadena.str()));
    cadena << "\n";
    return cadena.str();
}
