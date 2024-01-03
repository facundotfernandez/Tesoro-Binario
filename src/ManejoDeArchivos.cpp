#include "ManejoDeArchivos.h"
#include "Jugador.h"

ManejoDeArchivos::ManejoDeArchivos() {
}

ManejoDeArchivos::~ManejoDeArchivos() {
}

string ManejoDeArchivos::generarRutaTablero(unsigned int idJugador, unsigned int perspectiva, unsigned int fila,
                                            unsigned int columna, unsigned int profundidad, bool enemigosOcultos) {

    ostringstream ruta;
    string plano;
    string usaRadar = (enemigosOcultos) ? ("") : ("_radar");

    switch (perspectiva) {
        case 1:
            plano = "xz";
            break;
        case 2:
            plano = "yz";
            break;
        default:
            plano = "xy";
    }

    ruta << "tableros/jugador-" << idJugador << "/" << plano << "_" << columna + 1 << "_" << fila + 1 << "_"
         << profundidad + 1 << usaRadar << ".bmp";

    return ruta.str();
}

string ManejoDeArchivos::generarRutaJugador(unsigned int idJugador) {
    ostringstream ruta;
    ruta << "tableros/jugador-" << idJugador << "/";
    return ruta.str();
}

bool ManejoDeArchivos::crearDirectorioJugador(unsigned int idJugador) {
    return mkdir((this->generarRutaJugador(idJugador).c_str()), 0777) == 0;
}

bool ManejoDeArchivos::crearDirectorioTableros() {
    ostringstream ruta;
    ruta << "tableros/";
    return mkdir(((ruta.str()).c_str()), 0777) == 0;
}

void ManejoDeArchivos::eliminarDirectorioTableros() {
    const char *ruta = "tableros";

    if (existeDirectorio(ruta)) {
        // Usa el comando "rm" para eliminar el directorio y su contenido de manera recursiva
        string command = "rm -r " + string(ruta);
        system(command.c_str());
    }
}

bool ManejoDeArchivos::existeDirectorio(const char *ruta) {
    return access(ruta, F_OK) == 0;
}

void ManejoDeArchivos::exportar(Plano *plano, bool ocultarEnemigos) {

    string ruta;

    int dimensionTablero = 6, dimensionCasillero = 64;

    int filaCentral = plano->obtenerFilaCentral(), columnaCentral = plano->obtenerColumnaCentral();
    int filaOrigen = plano->obtenerFilaOrigen(), columnaOrigen = plano->obtenerColumnaOrigen();

    /* Correcciones necesarias ya que el tablero se imprime desde la esquina superior izquierda */
    int correccionFila = filaOrigen + (dimensionTablero - 1) - 1;
    int correccionColumna = columnaOrigen - 1;
    int filaReal, columnaReal;

    /*
     * Genera un tablero de 6x6 celdas con tamaño de 384x384 pixeles.
     * (Incluye los cabezales, en la sexta fila y primer columna, para mostrar el número de fila/columna).
     */
    BMP tableroBmp;
    tableroBmp.SetSize(dimensionTablero * dimensionCasillero, dimensionTablero * dimensionCasillero);

    /* Indicador de fila o columna */
    RGBApixel colorIndicador = {255, 255, 255, 1};
    char indicador[4];

    /* Perspectiva (XY,XZ,YZ) del plano */
    int perspectiva = plano->obtenerPerspectiva();

    /* Imagen a mostrar en el casillero */
    BMP *imagen;
    BMP *fondo;

    /* Dimension de imagen */
    int anchoImagen, alturaImagen, anchoFondo, alturaFondo;

    /* Ubicación del pixel */
    int pixelX, pixelY;

    /* Carga las imágenes desde la carpeta "assets" */
    BMP pirata, barril, tesoro, inactivo, protegido, partido, destruido;
    BMP fondoVacio, fondoCentral, fondoEnemigo, fondoIndicador, iconoXY, iconoXZ, iconoYZ;

    tesoro.ReadFromFile("assets/tesoro.bmp");
    pirata.ReadFromFile("assets/pirata.bmp");
    inactivo.ReadFromFile("assets/inactivo.bmp");
    protegido.ReadFromFile("assets/protegido.bmp");
    barril.ReadFromFile("assets/barril.bmp");
    partido.ReadFromFile("assets/partido.bmp");
    destruido.ReadFromFile("assets/destruido.bmp");

    fondoVacio.ReadFromFile("assets/fondo_vacio.bmp");
    fondoCentral.ReadFromFile("assets/fondo_central.bmp");
    fondoEnemigo.ReadFromFile("assets/fondo_enemigo.bmp");
    fondoIndicador.ReadFromFile("assets/fondo_indicador.bmp");
    iconoXY.ReadFromFile("assets/icono_xy.bmp");
    iconoXZ.ReadFromFile("assets/icono_xz.bmp");
    iconoYZ.ReadFromFile("assets/icono_yz.bmp");

    Casillero *casillero = plano->obtenerCasillero(filaCentral, columnaCentral);
    unsigned int tipoCasillero;
    bool esCentral;
    Jugador *jugador = casillero->obtenerJugador();
    Jugador *ocupante;

    /* Copia las imagenes en todas las celdas del tablero, según corresponda*/
    for (int fila = 0; fila < dimensionTablero; fila++) {
        for (int columna = 0; columna < dimensionTablero; columna++) {

            /*
             * Accede a los casilleros, manteniendo una de las variables fija,
             * para así poder mostrar los 3 planos.
             * La correccion permite armar un 5x5 alrededor del casillero principal
             * y corre el 5x5 en caso de que no se pueda dejar el casillero principal exactamente en el centro.
             */

            if (fila == (dimensionTablero - 1) || columna == 0) {
                tipoCasillero = (fila == (dimensionTablero - 1) && columna == 0) ? (ICONO) : (INDICADOR);
                esCentral = false;
                ocupante = NULL;

            } else {
                filaReal = correccionFila - fila;
                columnaReal = correccionColumna + columna;

                esCentral = ((filaReal == filaCentral) && (columnaReal == columnaCentral));

                /* Se reemplaza el puntero a Casillero por puntero al Casillero actual */
                casillero = plano->obtenerCasillero(filaReal, columnaReal);
                ocupante = casillero->obtenerJugador();

                if (ocultarEnemigos && (ocupante != jugador) && (ocupante != NULL)) {
                    tipoCasillero = VACIO;
                } else {
                    tipoCasillero = casillero->obtenerTipo();
                }
            }

            /* Modifica el tipo de imagen a mostrar, en base al tipoCasillero del Casillero actual */
            switch (tipoCasillero) {
                case VACIO:
                    imagen = (&fondoVacio);
                    break;
                case TESORO:
                    imagen = (&tesoro);
                    break;
                case PIRATA:
                    imagen = (&pirata);
                    break;
                case INACTIVO:
                    imagen = (&inactivo);
                    break;
                case PROTEGIDO:
                    imagen = (&protegido);
                    break;
                case BARRIL:
                    imagen = (&barril);
                    break;
                case PARTIDO:
                    imagen = (&partido);
                    break;
                case DESTRUIDO:
                    imagen = (&destruido);
                    break;
                case (INDICADOR):
                    /* Fondo del cuadrante que indica el número de fila o columna */
                    imagen = &fondoIndicador;
                    break;
                case (ICONO):
                    /* Cuadrante que indica la variable de la fila y columna (X,Y o Z) según perspectiva del plano */
                    switch (perspectiva) {
                        case (XY):
                            imagen = &iconoXY;
                            break;
                        case (XZ):
                            imagen = &iconoXZ;
                            break;
                        case (YZ):
                            imagen = &iconoYZ;
                            break;
                    }
                    break;
                default:
                    throw logic_error("El tipo de casillero a mostrar no es valido");
            }

            fondo = (esCentral) ? (&fondoCentral) : (((imagen != (&fondoVacio)) && (ocupante != jugador))
                                                     ? (&fondoEnemigo) : (&fondoVacio));

            anchoImagen = imagen->TellHeight();
            alturaImagen = imagen->TellWidth();
            if ((anchoImagen != dimensionCasillero) || (alturaImagen != dimensionCasillero)) {
                throw logic_error("Las imagenes deben tener la misma dimension que el casillero");
            }
            anchoFondo = fondo->TellHeight();
            alturaFondo = fondo->TellWidth();
            if ((anchoFondo != dimensionCasillero) || (alturaFondo != dimensionCasillero)) {
                throw logic_error("Las imagenes deben tener la misma dimension que el casillero");
            }

            // Copia cada pixel de la imagen en el cuadrante correspondiente
            for (int y = 0; y < alturaImagen; ++y) {
                for (int x = 0; x < anchoImagen; ++x) {

                    pixelX = columna * dimensionCasillero + x;
                    pixelY = fila * dimensionCasillero + y;

                    if (((*imagen)(x, y)->Red == 0) && ((*imagen)(x, y)->Green == 255) &&
                        ((*imagen)(x, y)->Blue == 0)) {

                        tableroBmp(pixelX, pixelY)->Red = (*fondo)(x, y)->Red;
                        tableroBmp(pixelX, pixelY)->Green = (*fondo)(x, y)->Green;
                        tableroBmp(pixelX, pixelY)->Blue = (*fondo)(x, y)->Blue;

                    } else {
                        tableroBmp(pixelX, pixelY)->Red = (*imagen)(x, y)->Red;
                        tableroBmp(pixelX, pixelY)->Green = (*imagen)(x, y)->Green;
                        tableroBmp(pixelX, pixelY)->Blue = (*imagen)(x, y)->Blue;
                    }

                }
            }
            if (fila == (dimensionTablero - 1) && columna != 0) {

                pixelX = ((correccionColumna + columna + 1) >= 10) ? (columna * dimensionCasillero + 15) : (
                        columna * dimensionCasillero + 25);
                pixelY = fila * dimensionCasillero + 19;

                /* Imprime el número de columna en el cuadrante correspondiente */
                sprintf(indicador, "%d", (correccionColumna + columna + 1));
                PrintString(tableroBmp, indicador, pixelX, pixelY, 26, colorIndicador);

            } else if (columna == 0 && fila != (dimensionTablero - 1)) {

                pixelX = ((correccionFila - fila + 1) >= 10) ? (columna * dimensionCasillero + 15) : (
                        columna * dimensionCasillero + 25);
                pixelY = fila * dimensionCasillero + 19;

                /* Imprime el número de fila en el cuadrante correspondiente */
                sprintf(indicador, "%d", (correccionFila - fila + 1));
                PrintString(tableroBmp, indicador, pixelX, pixelY, 26, colorIndicador);
            }
        }
    }

    /* Guarda el tablero en un archivo BMP del jugador correspondiente */
    ruta = this->generarRutaTablero(jugador->obtenerId(), perspectiva, filaCentral, columnaCentral,
                                    plano->obtenerProfundidad(), ocultarEnemigos);
    tableroBmp.WriteToFile(ruta.c_str());
}
