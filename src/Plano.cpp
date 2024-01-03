#include "Plano.h"

Plano::Plano(Tablero *tablero, Casillero *casillero, int perspectiva) {
    this->validarDimensiones(tablero);
    switch (perspectiva) {
        case (XY):
            this->filaCentral = (int) casillero->obtenerY();
            this->columnaCentral = (int) casillero->obtenerX();
            this->profundidad = (int) casillero->obtenerZ();
            this->filaOrigen = corregir((this->filaCentral), ((int) tablero->obtenerAlto()));
            this->columnaOrigen = corregir((this->columnaCentral), ((int) tablero->obtenerAncho()));
            break;
        case (XZ):
            this->filaCentral = (int) casillero->obtenerZ();
            this->columnaCentral = (int) casillero->obtenerX();
            this->profundidad = (int) casillero->obtenerY();
            this->filaOrigen = corregir((this->filaCentral), ((int) tablero->obtenerProfundidad()));
            this->columnaOrigen = corregir((this->columnaCentral), ((int) tablero->obtenerAncho()));
            break;
        case (YZ):
            this->filaCentral = (int) casillero->obtenerZ();
            this->columnaCentral = (int) casillero->obtenerY();
            this->profundidad = (int) casillero->obtenerX();
            this->filaOrigen = corregir((this->filaCentral), ((int) tablero->obtenerProfundidad()));
            this->columnaOrigen = corregir((this->columnaCentral), ((int) tablero->obtenerAlto()));
            break;
        default:
            throw logic_error("No es posible generar el plano pedido");
    }

    this->ancho = DIMENSION_PLANO;
    this->alto = DIMENSION_PLANO;

    this->perspectiva = perspectiva;

    this->plano = new Lista<Lista<Casillero *> *>();
    this->tablero = tablero;

    this->inicializarCasilleros();
}

Plano::~Plano() {

    plano->iniciarCursor();

    while (plano->avanzarCursor()) {

        Lista<Casillero *> *columna = plano->obtenerCursor();
        columna->iniciarCursor();

        delete columna; // Libera cada columna
    }
    delete this->plano; // Libera la estructura que contiene al Plano
}

void Plano::validarDimensiones(Tablero *tablero) {

    bool validacionMinimoDimension =
            (MINIMO_DIMENSION <= tablero->obtenerAncho()) && (MINIMO_DIMENSION <= tablero->obtenerAlto()) &&
            (MINIMO_DIMENSION <= tablero->obtenerProfundidad());

    if (!validacionMinimoDimension) {
        throw out_of_range("Las dimensiones son invalidas");
    }
}

void Plano::validarPosicion(int fila, int columna) const {
    if ((columna < 0) || (fila < 0)) {
        throw out_of_range("Las coordenadas no pueden ser negativas");
    } else if (!((this->columnaOrigen <= columna) && (columna < (this->ancho + this->columnaOrigen)) &&
                 (this->filaOrigen <= fila) && (fila < (this->alto + this->filaOrigen)))) {
        throw out_of_range("La posición no pertenece al plano");
    }
}

void Plano::inicializarCasilleros() {

    Lista<Casillero *> *punteroColumna;
    Casillero *punteroFila;

    for (int columna = (this->columnaOrigen); columna < (this->columnaOrigen + DIMENSION_PLANO); columna++) {

        punteroColumna = new Lista<Casillero *>();
        this->plano->agregarElemento(punteroColumna);

        for (int fila = (this->filaOrigen); fila < (this->filaOrigen + DIMENSION_PLANO); fila++) {

            switch (this->perspectiva) {
                case (XY):
                    punteroFila = this->tablero->obtenerCasillero(columna, fila, this->profundidad);
                    break;
                case (XZ):
                    punteroFila = this->tablero->obtenerCasillero(columna, this->profundidad, fila);
                    break;
                case (YZ):
                    punteroFila = this->tablero->obtenerCasillero(this->profundidad, columna, fila);
                    break;
            }

            punteroColumna->agregarElemento(punteroFila);
        }
    }
}

int Plano::corregir(int coordenada, int limite) {
    int correccion = ((coordenada - 2) < 0) ? (0 + coordenada) : (2);
    correccion += ((coordenada + 2) > (limite - 1)) ? ((coordenada + 2) - (limite - 1)) : (0);
    return (coordenada - correccion);
}

int Plano::obtenerFilaOrigen() const {
    return this->filaOrigen;
}

int Plano::obtenerColumnaOrigen() const {
    return this->columnaOrigen;
}

int Plano::obtenerFilaCentral() const {
    return this->filaCentral;
}

int Plano::obtenerColumnaCentral() const {
    return this->columnaCentral;
}

int Plano::obtenerProfundidad() const {
    return this->profundidad;
}

int Plano::obtenerPerspectiva() const {
    return this->perspectiva;
}

Casillero *Plano::obtenerCasillero(int fila, int columna) const {
    this->validarPosicion(fila, columna);
    return (this->plano->obtenerElemento(columna - this->columnaOrigen)->obtenerElemento(fila - this->filaOrigen));
}
