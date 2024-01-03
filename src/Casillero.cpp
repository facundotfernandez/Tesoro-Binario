#include "Plano.h"

Casillero::Casillero(unsigned int x, unsigned int y, unsigned int z) {
    this->coordenadas = new Vector<unsigned int>(3);
    this->asignarCoordenadas(x, y, z);
    this->descripcion = new DatosCasillero();
    this->planoXY = NULL;
    this->planoXZ = NULL;
    this->planoYZ = NULL;
}

Casillero::~Casillero() {
    delete this->coordenadas;
    delete this->descripcion;
    delete this->planoXY;
    delete this->planoXZ;
    delete this->planoYZ;
}

void Casillero::validarTipo(unsigned int tipo) const {

    unsigned int tipoEsperado = this->descripcion->obtenerTipo();

    if (tipoEsperado != tipo) {
        char alerta[256];
        sprintf(alerta, "Casillero invalido. Tipo esperado: %d | Tipo dado: %d", tipoEsperado, tipo);
        throw logic_error(alerta);
    }
}

void Casillero::validarExistencia(Plano *plano) const {
    if (plano == NULL) {
        throw logic_error("El plano dado no existe");
    }
}

void Casillero::validarExistencia(Casillero *casillero) const {
    if (casillero == NULL) {
        throw logic_error("El casillero dado no existe");
    }
}

void Casillero::asignarCoordenadas(unsigned int x, unsigned int y, unsigned int z) {
    this->coordenadas->reemplazarElemento(0, x);
    this->coordenadas->reemplazarElemento(1, y);
    this->coordenadas->reemplazarElemento(2, z);
}

void Casillero::asignarDescripcionCorta(unsigned int tipo, Jugador *jugador) {
    this->descripcion->reemplazarTipo(tipo);
    this->descripcion->reemplazarJugador(jugador);
}

void Casillero::asignarInactivoHasta(unsigned int inactivoHasta) {
    try {
        this->validarTipo(INACTIVO);
    } catch (const logic_error &e) {
        this->validarTipo(DESTRUIDO);
    }
    this->descripcion->reemplazarInactivoHasta(inactivoHasta);
}

void Casillero::asignarProtegidoHasta(unsigned int protegidoHasta) {
    this->validarTipo(PROTEGIDO);
    this->descripcion->reemplazarProtegidoHasta(protegidoHasta);
}

void Casillero::asignarAsociado(Casillero *asociado) {
    this->validarTipo(PARTIDO);
    this->descripcion->reemplazarAsociado(asociado);
}

unsigned int Casillero::obtenerX() const {
    return this->coordenadas->obtenerElemento(0);
}

unsigned int Casillero::obtenerY() const {
    return this->coordenadas->obtenerElemento(1);
}

unsigned int Casillero::obtenerZ() const {
    return this->coordenadas->obtenerElemento(2);
}

Vector<unsigned int> *Casillero::obtenerCoordenadas() const {
    return this->coordenadas;
}

unsigned int Casillero::obtenerTipo() const {
    return this->descripcion->obtenerTipo();
}

Jugador *Casillero::obtenerJugador() const {
    return this->descripcion->obtenerJugador();
}

unsigned int Casillero::obtenerInactivoHasta() const {
    try {
        this->validarTipo(INACTIVO);
    } catch (const logic_error &e) {
        this->validarTipo(DESTRUIDO);
    }
    return this->descripcion->obtenerInactivoHasta();
}

unsigned int Casillero::obtenerProtegidoHasta() const {
    this->validarTipo(PROTEGIDO);
    return this->descripcion->obtenerProtegidoHasta();
}

Casillero *Casillero::obtenerAsociado() const {
    this->validarTipo(PARTIDO);
    return this->descripcion->obtenerAsociado();
}

Plano *Casillero::obtenerPlanoXY() const {
    return this->planoXY;
}

Plano *Casillero::obtenerPlanoXZ() const {
    return this->planoXZ;
}

Plano *Casillero::obtenerPlanoYZ() const {
    return this->planoYZ;
}

void Casillero::asignarPlanoXY(Plano *plano) {
    if (this->planoYZ != NULL) {
        throw logic_error("No es posible reemplazar el plano previamente asignado a un casillero");
    }
    this->validarExistencia(plano);
    this->planoXY = plano;
}

void Casillero::asignarPlanoXZ(Plano *plano) {
    if (this->planoYZ != NULL) {
        throw logic_error("No es posible reemplazar el plano previamente asignado a un casillero");
    }
    this->validarExistencia(plano);
    this->planoXZ = plano;
}

void Casillero::asignarPlanoYZ(Plano *plano) {
    if (this->planoYZ != NULL) {
        throw logic_error("No es posible reemplazar el plano previamente asignado a un casillero");
    }
    this->validarExistencia(plano);
    this->planoYZ = plano;
}

void Casillero::convertirA(unsigned int tipo, Jugador *ocupante, unsigned int hastaTurno) {

    if (tipo == PARTIDO) {
        throw logic_error("No es posible convertir a tipo partido por este medio");
    } else if ((tipo == VACIO || tipo == DESTRUIDO) && ocupante != NULL) {
        throw logic_error("Un casillero vacio o destruido no puede pertenecer a un jugador en particular");
    }

    delete this->descripcion;
    this->descripcion = new DatosCasillero();

    this->asignarDescripcionCorta(tipo, ocupante);

    if (tipo == PROTEGIDO) {
        this->asignarProtegidoHasta(hastaTurno);
    } else if (tipo == INACTIVO || tipo == DESTRUIDO) {
        this->asignarInactivoHasta(hastaTurno);
    }
}

void Casillero::convertirA(unsigned int tipo, Jugador *ocupante, Casillero *asociado) {

    if (tipo != PARTIDO) {
        throw logic_error("Solo es posible convertir a tipo partido por este medio");
    }

    this->validarExistencia(asociado);

    delete this->descripcion;
    this->descripcion = new DatosCasillero();
    this->asignarDescripcionCorta(tipo, ocupante);
    this->asignarAsociado(asociado);
}
