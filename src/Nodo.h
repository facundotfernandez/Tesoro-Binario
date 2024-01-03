#ifndef NODO_H_
#define NODO_H_

#ifndef NULL
#define NULL 0
#endif /* NULL*/

#include <stdexcept>

using namespace std;

template<class T>
class Nodo {

private:

    T dato;
    Nodo<T> *siguiente;

public:

    /*
     * pre : -
     * post: Inicializa un Nodo con un dato dado y un puntero nulo al Nodo siguiente.
     */
    explicit Nodo(T dato) {
        this->dato = dato;
        this->siguiente = NULL;
    }

    /*
     * pre : -
     * post: Libera los recursos asociados al Nodo.
     */
    virtual ~Nodo() {
    }

    /*
     * pre : -
     * post: Devuelve el dato del Nodo.
     */
    T obtenerDato() const {
        return this->dato;
    }

    /*
     * pre : -
     * post: Devuelve un puntero al siguiente Nodo.
     */
    Nodo<T> *obtenerSiguiente() const {
        return this->siguiente;
    }

    /*
     * pre : -
     * post: Reemplaza el valor del dato por el dato dado.
     */
    void reemplazarDato(T dato) {
        this->dato = dato;
    }

    /*
     * pre : -
     * post: Reemplaza el siguiente Nodo por el siguiente dado.
     */
    void reemplazarSiguiente(Nodo<T> *siguiente) {
        this->siguiente = siguiente;
    }

};

#endif /* NODO_H_ */
