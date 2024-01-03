#ifndef CONSTANTES_H
#define CONSTANTES_H

#ifndef NULL
#define NULL 0
#endif /* NULL*/

// INDICADOR e ICONO solo sirven para exportar el tablero, no tienen uso en el tablero
enum TipoCasillero {
    VACIO, TESORO, PIRATA, INACTIVO, PROTEGIDO, BARRIL, PARTIDO, DESTRUIDO, INDICADOR, ICONO
};

enum TipoCarta {
    PROTEGER, REVELAR, PARTIRTESORO, ROBARCARTA, DUPLICARTURNO, MERCENARIOS
};

enum Perspectiva {
    XY, XZ, YZ
};

enum TipoCambio {
    ELIMINACION,
    CARTAROBADA,
    TESOROPERDIDO,
    PIRATAPERDIDO,
    BARRILPERDIDO,
    PROTECCIONPERDIDA,
    PARTIDOPERDIDO,
    MERCENARIOSPERDIDOS,
    ROBOINICIADO,
    ROBOFINALIZADO,
    ROBOINTERRUMPIDO
};

/* DIMENSIONES DEL TABLERO */
#define CANTIDAD_DIMENSIONES 3

/* MINIMO_DIMENSION NO PUEDE SER MENOR A 5, POR RESTRICCIONES DE PLANO.H */
#define MINIMO_DIMENSION 5
#define MAXIMO_DIMENSION 15

/* TAMAÑO DEL PLANO, DEBE SER 5x5 POR RESTRICCIONES IMPUESTAS */
#define DIMENSION_PLANO 5

/* MINIMO_JUGADORES NO PUEDE SER MENOR A 2 */
#define MINIMO_JUGADORES 2
#define MAXIMO_JUGADORES 10

/* TESOROS POR JUGADOR */
#define MINIMO_TESOROS 1
#define MAXIMO_TESOROS 5

/* CANTIDAD DE TIPOS DE CARTAS DISPONIBLES */
#define CANTIDAD_CARTAS 6

/* CANTIDAD DE RONDAS DE DURACION */
#define DURACION_ROBO 3
#define DURACION_PROTECCION 2
#define MAXIMA_POTENCIA_BARRIL 2

/* MAXIMO DE CARTAS EN MANO, POR JUGADOR */
#define MAXIMO_CARTAS_EN_MANO 3

/* CONFIGURACION INTERFAZ EN CONSOLA */
#define ANCHO_CONSOLA 100
#define ALTO_CONSOLA 50
#define ESPACIADO_SIMPLE 2
#define ESPACIADO_NOTIFICACION 3
#define ESPACIADO_TITULO 4

#endif
