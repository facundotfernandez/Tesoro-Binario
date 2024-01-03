#include "Carta.h"

Carta::Carta(unsigned int tipo) {
    this->tipo = tipo;
}

Carta::~Carta() {
}

unsigned int Carta::obtenerTipo() const {
    return this->tipo;
}
