#ifndef CARTA_H_
#define CARTA_H_

class Carta {

private:

    unsigned int tipo;

public:

    /*
     * pre:
     * post: Se inicializa una Carta con el tipo dado.
     */
    explicit Carta(unsigned int tipo);

    /*
     * pre:
     * post: Se liberan los recursos asociados a la Carta.
     */
    virtual ~Carta();

    /*
     * pre:
     * post: Devuelve el tipo de la Carta.
     */
    unsigned int obtenerTipo() const;

};

#endif /* CARTA_H_ */
