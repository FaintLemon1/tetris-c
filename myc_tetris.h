//myc_tetris.h

#ifndef MYC_TETRIS_H
#define MYC_TETRIS_H

#define FILAS       20
#define COLUMNAS    10

typedef struct {
    int x, y;        // posición
    int forma[4][4]; // forma
} Pieza;

extern int Tablero[FILAS][COLUMNAS];
extern int piezas[7][4][4];


void inicializar_pantalla();
void pantalla_inicio();
void dibujar_tablero();
void dibujar_pieza(Pieza *p);
int verificar_colision(Pieza *p ,int d_x ,int d_y);
void mover_pieza(Pieza *p , int d_x, int d_y);
void rotar_pieza(Pieza *p);
void fijar_pieza(Pieza *p);
void eliminar_filas(int *puntos);
void final_del_juego();
void imprimir_borde();

#endif