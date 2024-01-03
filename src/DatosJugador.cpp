#include "DatosJugador.h"

DatosJugador::DatosJugador(unsigned int id) {
    this->id = id;
    this->cantidadDeTesorosActivos = 0;
    this->cantidadDeTesorosRobados = 0;
    this->cambios = new Pila<string>();
}

DatosJugador::~DatosJugador() {
    while (!(cambios->estaVacia())) {
        this->cambios->desapilar();
    }
    delete this->cambios;
}

unsigned int DatosJugador::obtenerId() const {
    return this->id;
}

unsigned int DatosJugador::obtenerCantidadDeTesorosActivos() const {
    return this->cantidadDeTesorosActivos;
}

unsigned int DatosJugador::obtenerCantidadDeTesorosRobados() const {
    return this->cantidadDeTesorosRobados;
}

void DatosJugador::aumentarTesorosActivos() {
    this->cantidadDeTesorosActivos++;
}

void DatosJugador::disminuirTesorosActivos() {
    this->cantidadDeTesorosActivos--;
}

void DatosJugador::aumentarTesorosRobados() {
    this->cantidadDeTesorosRobados++;
}

void DatosJugador::agregarCambio(string cambio) {
    this->cambios->apilar(cambio);
}

Pila<string> *DatosJugador::obtenerCambios() const {
    return this->cambios;
}
