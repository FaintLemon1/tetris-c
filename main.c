
#include "myc_tetris.h"
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int main(){
    srand(time(NULL));
    inicializar_pantalla();
    pantalla_inicio();

    Pieza piezaActual;
    memcpy(piezaActual.forma,piezas[rand() %7], sizeof(piezaActual.forma));
    piezaActual.x = COLUMNAS/2 -2;
    piezaActual.y = 0;

    int juego_terminado = 0;
    int key;
    int puntos = 0;
    int c_x = (COLS - COLUMNAS)/2;
    int c_y = (LINES - FILAS) / 2;

    init_pair(1, COLOR_RED, COLOR_BLACK);
    attron(COLOR_PAIR(1));

    while (!juego_terminado)
    {
        clear();
        dibujar_tablero();
        dibujar_pieza(&piezaActual);

        key = getch();
        switch (key)    {
            case KEY_LEFT:  case 'a':   case 'A' :
                mover_pieza(&piezaActual, -1, 0);
                break;
            case KEY_RIGHT: case 'd' :  case 'D' : 
                mover_pieza(&piezaActual, 1, 0);
                break;
            case KEY_DOWN:  case 's' :  case 'S' :
                mover_pieza(&piezaActual, 0, 1);
                break;
            case 'z':
                rotar_pieza(&piezaActual);
                break;
            case KEY_F(1): // Salir del juego
                juego_terminado = 1;
                break;
        }

        if (verificar_colision(&piezaActual,0,1))
        {
            fijar_pieza(&piezaActual);
            eliminar_filas(&puntos);

            memcpy(piezaActual.forma,piezas[rand()%7],sizeof(piezaActual.forma));
            piezaActual.x = (COLUMNAS /2) -2;
            piezaActual.y = 0;

            if (verificar_colision(&piezaActual,0,0))
            {
                juego_terminado = 1;
            }
            
            
        }else{
            mover_pieza(&piezaActual,0,1);
        }
        mvprintw(0 + c_y, COLUMNAS * 2 + 2 + c_x, "Puntos: %d", puntos* 100);
        imprimir_borde(c_y - 1,c_x-1,FILAS,COLUMNAS);
        
        refresh();
        napms(200);

    }
    attroff(COLOR_PAIR(1));
    final_del_juego();

    endwin();
    return 0;
}

