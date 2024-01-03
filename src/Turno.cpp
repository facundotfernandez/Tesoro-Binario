#include "Turno.h"

Turno::Turno(Jugador *jugador) {
    this->turno = 1;
    this->jugador = jugador;
}

Turno::~Turno() {
}

unsigned int Turno::obtenerTurno() const {
    return this->turno;
}

Jugador *Turno::obtenerJugador() const {
    if (this->jugador == NULL) {
        throw logic_error("No se ha asignado un jugador al turno");
    }
    return this->jugador;
}

void Turno::avanzarTurno(Jugador *jugador) {
    if (jugador == NULL) {
        throw logic_error("El jugador no existe");
    }
    this->jugador = jugador;
    this->turno++;
}
