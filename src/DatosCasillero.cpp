#include "DatosCasillero.h"
#include "Jugador.h"

DatosCasillero::DatosCasillero() {
    this->tipo = 0;
    this->jugador = NULL;
    this->inactivoHasta = 0;
    this->protegidoHasta = 0;
    this->asociado = NULL;
}

DatosCasillero::~DatosCasillero() {
}

unsigned int DatosCasillero::obtenerTipo() const {
    return this->tipo;
}

Jugador *DatosCasillero::obtenerJugador() const {
    return this->jugador;
}

unsigned int DatosCasillero::obtenerInactivoHasta() const {
    return this->inactivoHasta;
}

unsigned int DatosCasillero::obtenerProtegidoHasta() const {
    return this->protegidoHasta;
}

Casillero *DatosCasillero::obtenerAsociado() const {
    return this->asociado;
}

void DatosCasillero::reemplazarTipo(unsigned int tipo) {
    this->tipo = tipo;
}

void DatosCasillero::reemplazarJugador(Jugador *jugador) {
    this->jugador = jugador;
}

void DatosCasillero::reemplazarInactivoHasta(unsigned int inactivoHasta) {
    this->inactivoHasta = inactivoHasta;
}

void DatosCasillero::reemplazarProtegidoHasta(unsigned int protegidoHasta) {
    this->protegidoHasta = protegidoHasta;
}

void DatosCasillero::reemplazarAsociado(Casillero *asociado) {
    if (asociado == NULL) {
        throw logic_error("El casillero no existe");
    }
    this->asociado = asociado;
}
