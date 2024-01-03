#include "Jugador.h"

Jugador::Jugador(unsigned int id) {
    this->casilleros = new Lista<Casillero *>;
    this->cartas = new Lista<Carta *>;
    this->descripcion = new DatosJugador(id);
    this->enJuego = true;
    this->mercenariosContratados = false;
    this->inactivos = new Lista<Casillero *>();
    this->protegidos = new Lista<Casillero *>();
}

Jugador::~Jugador() {

    // Destructor de Casilleros
    this->casilleros->limpiar();
    delete this->casilleros;

    // Destructor de Cartas
    this->cartas->iniciarCursor();
    while (this->cartas->avanzarCursor()) {
        delete this->cartas->obtenerCursor();
    }
    delete this->cartas;

    // Destructor de Robos en proceso
    this->inactivos->limpiar();
    delete this->inactivos;

    // Destructor de Tesoros Protegidos
    this->protegidos->limpiar();
    delete this->protegidos;

    // Destructor de Descripcion del Jugador
    delete this->descripcion;
}

Lista<Casillero *> *Jugador::obtenerCasilleros() const {
    return this->casilleros;
}

Lista<Carta *> *Jugador::obtenerCartas() const {
    return this->cartas;
}

unsigned int Jugador::obtenerId() const {
    return this->descripcion->obtenerId();
}

unsigned int Jugador::obtenerCantidadDeCartas() const {
    return this->cartas->obtenerLongitud();
}

unsigned int Jugador::obtenerCantidadDeTesorosActivos() const {
    return this->descripcion->obtenerCantidadDeTesorosActivos();
}

unsigned int Jugador::obtenerCantidadDeTesorosRobados() const {
    return this->descripcion->obtenerCantidadDeTesorosRobados();
}

Casillero *Jugador::obtenerCasillero(Vector<unsigned int> *coordenadas) const {

    if (coordenadas == NULL) {
        throw logic_error("Las coordenadas dadas no existen");
    } else if (coordenadas->obtenerLongitud() != CANTIDAD_DIMENSIONES) {
        throw logic_error("Las coordenadas deben tener 3 dimensiones");
    }

    unsigned int x = (*coordenadas)[0];
    unsigned int y = (*coordenadas)[1];
    unsigned int z = (*coordenadas)[2];

    Casillero *casillero;
    this->casilleros->iniciarCursor();
    while (this->casilleros->avanzarCursor()) {
        casillero = this->casilleros->obtenerCursor();
        if ((casillero->obtenerX() == x) && (casillero->obtenerY() == y) && (casillero->obtenerZ() == z)) {
            return casillero;
        }
    }
    throw out_of_range("El jugador no posee un casillero con las coordenadas indicadas");
}

void Jugador::agregarCasillero(Casillero *aAgregar) {
    if (aAgregar == NULL) {
        throw logic_error("El casillero no existe");
    } else if (aAgregar->obtenerJugador() != this) {
        throw logic_error("El casillero no está ocupado por el Jugador");
    }

    this->casilleros->agregarElemento(aAgregar);

    unsigned int tipo = aAgregar->obtenerTipo();

    if (tipo == TESORO) {
        this->descripcion->aumentarTesorosActivos();
    } else if (tipo == PROTEGIDO) {
        this->descripcion->aumentarTesorosActivos();
        this->protegidos->agregarElemento(aAgregar);
    } else if (tipo == INACTIVO) {
        this->inactivos->agregarElemento(aAgregar);
    }
}

void Jugador::removerCasillero(Casillero *aRemover) {

    if (aRemover == NULL) {
        throw logic_error("El casillero no existe");
    } else if (aRemover->obtenerJugador() != this) {
        throw logic_error("El casillero no está ocupado por el Jugador");
    }

    unsigned int tipo = aRemover->obtenerTipo();
    unsigned int posicion = 0;

    Casillero *casillero;

    this->casilleros->iniciarCursor();
    while (this->casilleros->avanzarCursor()) {
        casillero = casilleros->obtenerCursor();
        if (casillero == aRemover) {

            if (tipo == TESORO) {
                this->descripcion->disminuirTesorosActivos();
            } else if (tipo == PROTEGIDO) {
                this->removerProtegido(casillero);
                this->descripcion->disminuirTesorosActivos();
            } else if (tipo == INACTIVO) {
                this->removerInactivo(casillero);
            }

            this->casilleros->removerElemento(posicion);
            return;
        }
        posicion++;
    }
    throw out_of_range("El jugador no está siendo ocupando el casillero");
}

void Jugador::agregarCarta(Carta *carta) {
    if (carta == NULL) {
        throw logic_error("El casillero no existe");
    } else if (this->obtenerCantidadDeCartas() >= MAXIMO_CARTAS_EN_MANO) {
        throw out_of_range("La mano está llena, debe descartar una carta antes de agregar otra");
    }
    this->cartas->agregarElemento(carta);
}

bool Jugador::usarCarta(unsigned int tipo, bool eliminar) {

    if (this->cartas->estaVacia()) {
        throw out_of_range("El jugador no tiene cartas en su mano");
    }

    unsigned int posicion = 0;
    this->cartas->iniciarCursor();

    while (this->cartas->avanzarCursor()) {
        if (this->cartas->obtenerCursor()->obtenerTipo() == tipo) {
            if (eliminar) {
                delete this->cartas->obtenerCursor();
            }
            this->cartas->removerElemento(posicion);
            return true;
        }
        posicion++;
    }
    return false;
}

Carta *Jugador::obtenerCartaAleatoria() const {

    if (this->cartas->estaVacia()) {
        throw out_of_range("El jugador no tiene cartas en su mano");
    }

    unsigned int posicionAleatoria = (rand() % (this->obtenerCantidadDeCartas()));

    for (unsigned int i = 0; i <= posicionAleatoria; ++i) {
        this->cartas->avanzarCursor();
    }

    Carta *cartaAleatoria = this->cartas->obtenerCursor();
    return cartaAleatoria;
}

void Jugador::agregarCambio(string cambio) {
    this->descripcion->agregarCambio(cambio);
    if (cambio.find("Robo finalizado") != string::npos) {
        this->descripcion->aumentarTesorosRobados();
    }
}

void Jugador::agregarProtegido(Casillero *aAgregar) {
    if (aAgregar == NULL) {
        throw logic_error("El casillero no existe");
    } else if (aAgregar->obtenerTipo() != PROTEGIDO) {
        throw logic_error("El casillero no es de tipo protegido");
    }
    this->protegidos->agregarElemento(aAgregar);
}

bool Jugador::removerProtegido(Casillero *aRemover) {
    if (aRemover == NULL) {
        throw logic_error("El casillero no existe");
    } else if (aRemover->obtenerTipo() != PROTEGIDO) {
        throw logic_error("El casillero no es de tipo protegido");
    }
    unsigned int posicion = 0;

    this->protegidos->iniciarCursor();
    while (this->protegidos->avanzarCursor()) {
        if (this->protegidos->obtenerCursor() == aRemover) {
            this->protegidos->removerElemento(posicion);
            return true;
        }
        posicion++;
    }
    this->protegidos->iniciarCursor();
    return false;

}

bool Jugador::removerInactivo(Casillero *aRemover) {

    if (aRemover == NULL) {
        throw logic_error("El casillero no existe");
    } else if (aRemover->obtenerTipo() != INACTIVO) {
        throw logic_error("El casillero no es de tipo inactivo");
    }
    unsigned int posicion = 0;
    this->inactivos->iniciarCursor();

    while (this->inactivos->avanzarCursor()) {
        if (this->inactivos->obtenerCursor() == aRemover) {
            this->inactivos->removerElemento(posicion);
            return true;
        }
        posicion++;
    }
    this->inactivos->iniciarCursor();
    return false;
}

Pila<string> *Jugador::obtenerCambios() {
    return this->descripcion->obtenerCambios();
}

void Jugador::dejaDeJugar() {
    this->enJuego = false;
}

bool Jugador::sigueEnJuego() const {
    return this->enJuego;
}

bool Jugador::tieneMercenarios() const {
    return this->mercenariosContratados;
}

void Jugador::contratarMercenarios() {
    (this->tieneMercenarios()) ? (throw logic_error("Ya tiene mercenarios contratados"))
                               : (this->mercenariosContratados = true);
}

void Jugador::usarMercenarios() {
    (this->tieneMercenarios()) ? (this->mercenariosContratados = false) : (throw logic_error(
            "No tiene mercenarios contratados"));
}

Lista<Casillero *> *Jugador::obtenerInactivos() {
    return this->inactivos;
}

Lista<Casillero *> *Jugador::obtenerProtegidos() {
    return this->protegidos;
}
