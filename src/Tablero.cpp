#include "Tablero.h"
#include "Plano.h"

Tablero::Tablero(bool inicializado) {

    this->dimensiones = new Vector<unsigned int>(3);
    this->dimensiones->reemplazarElemento(0, MINIMO_DIMENSION);
    this->dimensiones->reemplazarElemento(1, MINIMO_DIMENSION);
    this->dimensiones->reemplazarElemento(2, MINIMO_DIMENSION);

    this->tablero = new Lista<Lista<Lista<Casillero *> *> *>();

    if (inicializado) {
        this->inicializarCasilleros();
    }
}

Tablero::Tablero(unsigned int ancho, unsigned int alto, unsigned int profundidad, bool inicializado) {

    this->validarDimensiones(ancho, alto, profundidad);

    this->dimensiones = new Vector<unsigned int>(3);
    this->dimensiones->reemplazarElemento(0, ancho);
    this->dimensiones->reemplazarElemento(1, alto);
    this->dimensiones->reemplazarElemento(2, profundidad);

    this->tablero = new Lista<Lista<Lista<Casillero *> *> *>();

    if (inicializado) {
        this->inicializarCasilleros();
    }
}

Tablero::~Tablero() {

    delete this->dimensiones;

    Lista<Lista<Casillero *> *> *columna;
    Lista<Casillero *> *fila;
    Casillero *casillero;

    tablero->iniciarCursor();

    while (tablero->avanzarCursor()) {

        columna = tablero->obtenerCursor();
        columna->iniciarCursor();

        while (columna->avanzarCursor()) {

            fila = columna->obtenerCursor();
            fila->iniciarCursor();

            while (fila->avanzarCursor()) {

                casillero = fila->obtenerCursor();

                delete casillero; // Libera cada Casillero
            }
            delete fila; // Libera cada fila
        }
        delete columna; // Libera cada columna
    }
    delete this->tablero; // Libera la estructura que contiene al Tablero
}

void Tablero::validarDimensiones(unsigned int ancho, unsigned int alto, unsigned int profundidad) const {
    bool validacionMinimoDimension =
            (MINIMO_DIMENSION <= ancho) && (MINIMO_DIMENSION <= alto) && (MINIMO_DIMENSION <= profundidad);
    bool validacionMaximoDimension =
            (MAXIMO_DIMENSION >= ancho) && (MAXIMO_DIMENSION >= alto) && (MAXIMO_DIMENSION >= profundidad);

    if (!validacionMinimoDimension || !validacionMaximoDimension) {
        throw out_of_range("Las dimensiones son invalidas");
    }
}

void Tablero::validarPosicion(unsigned int x, unsigned int y, unsigned int z) const {
    if (!((x < this->obtenerAncho()) && (y < this->obtenerAlto()) && (z < this->obtenerProfundidad()))) {
        throw out_of_range("La posición no pertenece al tablero");
    }
}

void Tablero::validarLongitud(Vector<unsigned int> *estructura, unsigned int longitud) const {
    if (estructura == NULL) {
        throw logic_error("No existe la estructura");
    } else if (estructura->obtenerLongitud() != longitud) {
        throw out_of_range("La estructura no posee la longitud buscada");
    }
}

void Tablero::inicializarCasilleros() {

    Lista<Lista<Casillero *> *> *columna;
    Lista<Casillero *> *fila;
    Casillero *casillero;

    /* Recorre el tablero para crear los punteros a Casilleros */
    for (unsigned int x = 0; x < this->obtenerAncho(); x++) {

        columna = new Lista<Lista<Casillero *> *>();
        this->tablero->agregarElemento(columna);

        for (unsigned int y = 0; y < this->obtenerAlto(); y++) {

            fila = new Lista<Casillero *>();
            columna->agregarElemento(fila);

            for (unsigned int z = 0; z < this->obtenerProfundidad(); z++) {

                casillero = new Casillero(x, y, z);
                fila->agregarElemento(casillero);
            }
        }
    }

    this->inicializarPlanos();
}

void Tablero::inicializarPlanos() {

    Lista<Lista<Casillero *> *> *columna;
    Lista<Casillero *> *fila;
    Casillero *casillero;
    Plano *planoXY;
    Plano *planoXZ;
    Plano *planoYZ;

    /* Recorre el tablero para asignarle a los Casilleros sus Planos con Casilleros vecinos */
    tablero->iniciarCursor();

    while (tablero->avanzarCursor()) {

        columna = tablero->obtenerCursor();
        columna->iniciarCursor();

        while (columna->avanzarCursor()) {

            fila = columna->obtenerCursor();
            fila->iniciarCursor();

            while (fila->avanzarCursor()) {

                casillero = fila->obtenerCursor();

                planoXY = new Plano(this, casillero, XY);
                planoXZ = new Plano(this, casillero, XZ);
                planoYZ = new Plano(this, casillero, YZ);

                casillero->asignarPlanoXY(planoXY);
                casillero->asignarPlanoXZ(planoXZ);
                casillero->asignarPlanoYZ(planoYZ);

            }
        }
    }
}

unsigned int Tablero::obtenerAncho() const {
    return this->dimensiones->obtenerElemento(0);
}

unsigned int Tablero::obtenerAlto() const {
    return this->dimensiones->obtenerElemento(1);
}

unsigned int Tablero::obtenerProfundidad() const {
    return this->dimensiones->obtenerElemento(2);
}

Casillero *Tablero::obtenerCasillero(unsigned int x, unsigned int y, unsigned int z) const {
    this->validarPosicion(x, y, z);
    return (this->tablero->obtenerElemento(x)->obtenerElemento(y)->obtenerElemento(z));
}

Casillero *Tablero::obtenerCasillero(Vector<unsigned int> *coordenadas) const {
    this->validarLongitud(coordenadas);
    return (this->obtenerCasillero(coordenadas->obtenerElemento(0), coordenadas->obtenerElemento(1),
                                   coordenadas->obtenerElemento(2)));
}
