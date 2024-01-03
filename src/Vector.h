#ifndef VECTOR_H_
#define VECTOR_H_

#include <stdexcept>

using namespace std;

template<class T>
class Vector {

private:

    unsigned int longitud;
    T *elementos;

    /*
     * pre : La posición pertenece al intervalo: [0, obtenerlongitud() - 1] (@throws out_of_range).
     * post: -
     */
    void validarPosicion(unsigned int posicion) const {
        if (!(posicion < this->longitud)) {
            throw out_of_range("Posición fuera de rango");
        }
    }

public:

    /*
     * pre : La longitud dada debe ser mayor a 0 (@throws out_of_range).
     * post: Inicializa un Vector con la longitud dada y elementos en 0.
     */
    explicit Vector(unsigned int longitud) {

        if (longitud == 0) {
            throw out_of_range("La longitud debe ser mayor a 0");
        }

        this->longitud = longitud;
        this->elementos = new T[longitud];

        for (unsigned int i = 0; i < longitud; i++) {
            this->elementos[i] = 0;
        }
    }

    /*
     * pre : -
     * post: Libera los recursos asociados al Vector.
     */
    virtual ~Vector() {
        delete[] this->elementos;
    }

    /*
     * pre : La posición pertenece al intervalo: [0, obtenerlongitud() - 1] (@throws out_of_range).
     * post: Devuelve el elemento en la posición indicada.
     */
    T obtenerElemento(unsigned int posicion) const {
        this->validarPosicion(posicion);
        return this->elementos[posicion];
    }

    /*
     * pre : La posición pertenece al intervalo: [0, obtenerlongitud() - 1] (@throws out_of_range).
     * post: Devuelve el elemento en la posición indicada, utilizando el operador [].
     */
    T operator[](unsigned int posicion) const {
        this->validarPosicion(posicion);
        return obtenerElemento(posicion);
    }

    /*
     * pre : -
     * post: Devuelve la longitud del Vector.
     */
    unsigned int obtenerLongitud() const {
        return this->longitud;
    }

    /*
     * pre : La posición pertenece al intervalo: [0, obtenerlongitud() - 1] (@throws out_of_range).
     * post: Reemplaza el elemento, en la posición indicada, por el dado.
     */
    void reemplazarElemento(unsigned int posicion, T elemento) {
        this->validarPosicion(posicion);
        this->elementos[posicion] = elemento;
    }

};

#endif /* VECTOR_H_ */
