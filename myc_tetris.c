#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "myc_tetris.h"




int Tablero[FILAS][COLUMNAS] = {0};

int piezas[7][4][4] = {
    {{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // I
    {{1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // O
    {{1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // T
    {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // S
    {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // Z
    {{1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // L
    {{1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}  // J
};


void inicializar_pantalla(){//inicializa las cosas basicas de la libreria
    initscr();
    noecho();
    raw();
    keypad(stdscr, TRUE);
    timeout(10); 
    curs_set(0);
    has_colors();
    start_color();

}

void pantalla_inicio(){ //pantalla de inicio
    char *text = "PRESIONA CUALQUIER BOTON PARA EMPEZAR";
    int len = strlen(text);
    char *textt = "sal con F1";
    int lenn = strlen(textt);
    
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    attron(COLOR_PAIR(1));

    box(stdscr,'|','-');
    move((LINES/2)-1,(COLS -len)/2); // establece donde imprimira y el siguiente while imprime
    while (*(text))
    {
        addch(*text++);
        refresh();
        napms(10);
    }

    move(LINES/2,(COLS -lenn)/2); // establece donde imprimira y el siguiente while imprime
    while (*(textt))
    {
        addch(*textt++);
        refresh();
        napms(10);
    }
    box(stdscr,'|','-');

    getch();
    napms(1000);
    clear();
    attroff(COLOR_PAIR(1));

}

void dibujar_tablero(){ //dibuja el tablero 
    int c_x = (COLS - COLUMNAS)/2;
    int c_y = (LINES - FILAS) / 2;
    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            if (Tablero[i][j])
            {
                mvprintw(i + c_y,(j*2) + c_x,"[]");
            }
            else
            {
                mvprintw(i + c_y,(j*2)+ c_x,"  ");
            }
            
        }
        
    }
    
}

void dibujar_pieza(Pieza *p){  // dibuja la pieza actual
    int c_x = (COLS - COLUMNAS)/2;
    int c_y = (LINES - FILAS) / 2;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (p->forma[i][j])
            {
                mvprintw(p->y + i + c_y,(p->x + j) * 2 + c_x,"[]");
            }
        }
        
    }
}

int verificar_colision(Pieza *p ,int d_x ,int d_y){
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (p->forma[i][j])
            {
                int nueva_x = p->x + j + d_x;
                int nueva_y = p->y + i + d_y;
                if (nueva_x < 0 || nueva_x >= COLUMNAS || nueva_y >= FILAS || (nueva_y >= 0 && Tablero[nueva_y][nueva_x]))
                {
                    return 1;
                }
            }
        }
        
    }
    return 0;
}

void mover_pieza(Pieza *p , int d_x, int d_y){ 
    if (!verificar_colision(p,d_x,d_y))
    {
        p->x += d_x;
        p->y += d_y;
    }
}

void rotar_pieza(Pieza *p){
    int nueva_forma[4][4] = {0};
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            nueva_forma[j][3-i] = p->forma[i][j];
        }
    }
    Pieza pieza_temp = *p;
    memcpy(pieza_temp.forma,nueva_forma,sizeof(nueva_forma));
    if (!verificar_colision(&pieza_temp,0,0))
    {
        memcpy(p->forma,nueva_forma,sizeof(nueva_forma));
    }
}

void fijar_pieza(Pieza *p){ //fija la pieza
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (p->forma[i][j])
            {
                Tablero[p->y +i ][p->x + j] = 1;
            }
        }
    }
}

void eliminar_filas(int *puntos){

    for (int i = FILAS -1; i >= 0; i--)
    {
        int completa = 1;
        for (int j = 0; j < COLUMNAS; j++)
        {
            if (!Tablero[i][j])
            {
                completa = 0;
                break;
            }
        }

        if(completa){
            (*puntos)++;
            for (size_t k = i; k > 0; k--)
            {
                memcpy(Tablero[k],Tablero[k-1],sizeof(Tablero[k]));
            }
            memset(Tablero[0],0,sizeof(Tablero[0]));
            i++;
        }
    }
}

void final_del_juego(){   // fin de el juego
    clear();
    char *text = "GAME OVER";
    int len = strlen(text);
    char *textt = "FIN DE EL JUEGO";
    int lenn = strlen(textt);
    
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    attron(COLOR_PAIR(1));

    box(stdscr,'|','-');
    move((LINES/2)-1,(COLS -len)/2); // establece donde imprimira y el siguiente while imprime
    while (*(text))
    {
        addch(*text++);
        refresh();
        napms(10);
    }

    move(LINES/2,(COLS -lenn)/2); // establece donde imprimira y el siguiente while imprime
    while (*(textt))
    {
        addch(*textt++);
        refresh();
        napms(10);
    }
    box(stdscr,'|','-');

    getch();
    napms(1000);
    clear();
    attroff(COLOR_PAIR(1));

}

void imprimir_borde(int c_y, int c_x, int filas, int columnas) {
    int ancho = columnas * 2;

    for (int i = 0; i < ancho + 2; i++) { 
        mvaddch(c_y, c_x + i, '-');
        mvaddch(c_y + filas + 1, c_x + i, '-'); 
    }

 
    for (int i = 0; i < filas + 2; i++) { 
        mvaddch(c_y + i, c_x, '|'); 
        mvaddch(c_y + i, c_x + ancho + 1, '|'); 
    }

    // Esquinas
    mvaddch(c_y, c_x, '+');
    mvaddch(c_y, c_x + ancho + 1, '+'); 
    mvaddch(c_y + filas + 1, c_x, '+');
    mvaddch(c_y + filas + 1, c_x + ancho + 1, '+'); 
}
