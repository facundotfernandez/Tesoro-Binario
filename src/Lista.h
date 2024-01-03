#ifndef LISTA_H_
#define LISTA_H_

#include "Nodo.h"

template<class T>
class Lista {

private:

    Nodo<T> *primero;
    unsigned int longitud;

    /* Puntero a Nodo que permite recorrer la Lista sin perder referencia al primer Nodo */
    Nodo<T> *cursor;

    /*
     * pre : La posición pertenece al intervalo: [0, obtenerlongitud() - 1] (@throws out_of_range).
     * post: Verifica que la posición pertenezca al intervalo: [0, obtenerlongitud() - 1].
     */
    void validarPosicion(unsigned int posicion) {
        if (!((0 == posicion) || (posicion < this->longitud))) {
            throw out_of_range("Posición fuera de rango");
        }
    }

    /*
     * pre : La posición pertenece al intervalo: [0, obtenerlongitud() - 1] (@throws out_of_range).
     * 	     Esto se debe validar antes de invocar a obtenerNodo.
     * post: Devuelve un puntero al Nodo en la posición indicada.
     */
    Nodo<T> *obtenerNodo(unsigned int posicion) {
        Nodo<T> *actual = this->primero;
        for (unsigned int i = 0; i < posicion && actual; i++) {
            actual = actual->obtenerSiguiente();
        }
        return actual;
    }

    /*
     * pre : -
     * post: Devuelve el puntero al primer Nodo de la Lista.
     */
    Nodo<T> *obtenerPrimero() const {
        return this->primero;
    }

    /*
     * pre : -
     * post: Reemplaza el puntero al primer Nodo de la Lista.
     */
    void reemplazarPrimero(Nodo<T> *primero) {
        this->primero = primero;
    }

    /*
     * pre : -
     * post: Reemplaza el puntero al actual cursor de la Lista.
     */
    void reemplazarCursor(Nodo<T> *cursor) {
        this->cursor = cursor;
    }

public:

    /*
     * pre : -
     * post: Inicializa una Lista vacía.
     */
    Lista() {
        this->primero = NULL;
        this->longitud = 0;
        this->cursor = NULL;
    }

    /*
     * pre : -
     * post: Inicializa una Lista que tiene los mismos elementos que la Lista dada.
     *       La instancia resulta en una copia de la Lista dada.
     */
    Lista(Lista<T> &otraLista) {

        this->primero = NULL;
        this->longitud = 0;
        this->cursor = NULL;

        /* copia los elementos de otraLista */
        this->agregarElemento(otraLista);
    }

    /*
     * pre : -
     * post: Libera los recursos asociados a la Lista.
     */
    virtual ~Lista() {
        while (this->primero != NULL) {
            Nodo<T> *aBorrar = this->primero;
            this->primero = this->primero->obtenerSiguiente();
            delete aBorrar;
        }
    }

    /*
     * pre : -
     * post: Devuelve la cantidad de elementos que tiene la Lista.
     */
    unsigned int obtenerLongitud() const {
        return this->longitud;
    }

    /*
     * pre : -
     * post: Devuelve el elemento en la posición del cursor.
     */
    T obtenerCursor() const {
        T elemento;
        if (this->cursor != NULL) {
            elemento = this->cursor->obtenerDato();
        }
        return elemento;
    }

    /*
     * pre : -
     * post: Indica si la Lista tiene algún elemento.
     */
    bool estaVacia() {
        return (this->longitud == 0);
    }

    /*
     * pre : -
     * post: Agrega el elemento al final de la Lista, en la posición obtenerlongitud().
     */
    void agregarElemento(T elemento) {
        this->agregarElemento(elemento, this->longitud);
    }

    /*
     * pre : Posición pertenece al intervalo: [0, obtenerlongitud()] (@throws out_of_range).
     * post: Agrega el elemento en la posición indicada.
     */
    void agregarElemento(T elemento, unsigned int posicion) {

        try {
            this->validarPosicion(posicion);
        } catch (const out_of_range &e) {
            if (!(posicion == this->longitud)) {
                throw out_of_range("posición fuera de rango");
            }
        }

        Nodo<T> *nuevo = new Nodo<T>(elemento);

        if (posicion == 0) {
            /* Inserta el elemento al inicio de la Lista */
            nuevo->reemplazarSiguiente(this->primero);
            this->primero = nuevo;
        } else {
            Nodo<T> *anterior = this->obtenerNodo(posicion - 1);
            nuevo->reemplazarSiguiente(anterior->obtenerSiguiente());
            anterior->reemplazarSiguiente(nuevo);
        }

        this->longitud++;

    }

    /*
     * pre : -
     * post: Recorre otraLista y agrega todos sus elementos a la Lista original.
     */
    void agregarElemento(Lista<T> &otraLista) {

        otraLista.iniciarCursor();
        while (otraLista.avanzarCursor()) {
            this->agregarElemento(otraLista.obtenerCursor());
        }
    }

    /*
     * pre : Posición pertenece al intervalo: [0, obtenerlongitud() - 1] (@throws out_of_range).
     *   	 La Lista no debe estar vacía (@throws out_of_range).
     * post: Devuelve el elemento en la posición indicada.
     */
    T obtenerElemento(unsigned int posicion) {
        this->validarPosicion(posicion);
        return this->obtenerNodo(posicion)->obtenerDato();

    }

    /*
     * pre : Posición pertenece al intervalo: [0, obtenerlongitud() - 1] (@throws out_of_range).
     * post: Devuelve el elemento en la posición indicada, utilizando el operador [].
     */
    T operator[](unsigned int posicion) {
        return obtenerElemento(posicion);
    }

    /*
     * pre : Posición pertenece al intervalo: [0, obtenerlongitud() - 1] (@throws out_of_range).
     * post: Reemplaza el elemento en la posición indicada por el elemento dado.
     */
    void asignarElemento(T elemento, unsigned int posicion) {
        this->validarPosicion(posicion);
        this->obtenerNodo(posicion)->reemplazarDato(elemento);
    }

    /*
     * pre : Posición pertenece al intervalo: [0, obtenerlongitud() - 1] (@throws out_of_range).
     * post: Elimina de la Lista el elemento en la posición indicada.
     */
    void removerElemento(unsigned int posicion) {

        this->validarPosicion(posicion);

        Nodo<T> *aBorrar;

        if (posicion == 0) {
            aBorrar = this->primero;
            this->primero = aBorrar->obtenerSiguiente();
        } else {
            Nodo<T> *anterior = this->obtenerNodo(posicion - 1);
            aBorrar = anterior->obtenerSiguiente();
            anterior->reemplazarSiguiente(aBorrar->obtenerSiguiente());
        }

        delete aBorrar;
        this->longitud--;
    }

    /*
     * pre : Posición pertenece al intervalo: [0, obtenerlongitud() - 1] (@throws out_of_range).
     * post: Elimina de la Lista el elemento en la posición indicada.
     */
    bool contiene(T elementoBuscado) {

        T elemento;

        this->iniciarCursor();
        while (this->avanzarCursor()) {
            elemento = this->obtenerCursor();
            if (elemento == elementoBuscado) {
                return true;
            }
        }
        return false;
    }

    /*
     * pre : El elemento existe en la Lista (@throws out_of_range).
     * post: Elimina de la Lista el elemento.
     */
    void removerPrimerAparicion(T elemento) {

        Nodo<T> *aBorrar = this->primero;
        Nodo<T> *anterior = NULL;

        this->iniciarCursor();

        while (this->avanzarCursor() && this->obtenerCursor() != elemento) {
            anterior = aBorrar;
            aBorrar = aBorrar->obtenerSiguiente();
        }

        if (!(aBorrar)) {
            throw std::out_of_range("Elemento no encontrado");
        }

        if (anterior == NULL) {
            this->primero = aBorrar->obtenerSiguiente();
        } else {
            anterior->reemplazarSiguiente(aBorrar->obtenerSiguiente());
        }

        delete aBorrar;
        this->longitud--;

        /* cualquier recorrido actual queda invalidado */
        this->iniciarCursor();
    }

    /*
     * pre : -
     * post: Inicializa el cursor de la Lista preparado para hacer un nuevo recorrido sobre sus elementos.
     */
    void iniciarCursor() {
        this->cursor = NULL;
    }

    /*
     * pre : Inicia un recorrido (invocando el método iniciarCursor())
     * 		 y desde entonces no se han agregado o removido elementos de la Lista.
     * post: Mueve el cursor y lo posiciona en el siguiente elemento del recorrido.
     *       El valor de retorno indica si el cursor quedó posicionado
     *       sobre un elemento o no (en caso de que la Lista esté vacía o
     *       no existan más elementos por recorrer).
     */
    bool avanzarCursor() {

        if (this->cursor == NULL) {
            this->cursor = this->primero;
        } else {
            this->cursor = this->cursor->obtenerSiguiente();
        }

        /* Pudo avanzar si el cursor ahora apunta a un Nodo */
        return (this->cursor != NULL);
    }

    /*
     * Pre: -
     * Post: Elimina los elementos de toda la lista, como si fuera un destructor, establece su longitud en 0 y su cursor en NULL.
     */
    void limpiar() {
        while (this->primero != NULL) {
            Nodo<T> *aBorrar = this->primero;
            this->primero = this->primero->obtenerSiguiente();
            delete aBorrar;
        }
        this->longitud = 0;
        this->cursor = NULL;
    }

};

#endif /* LISTA_H_ */
