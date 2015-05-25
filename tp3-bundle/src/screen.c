/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
definicion de funciones del scheduler
*/
//tablero 50x80

#include "screen.h"
#include "game.h"


extern jugador_t jugadorA, jugadorB;


static ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;

const char reloj[] = "|/-\\";
#define reloj_size 4


void screen_actualizar_reloj_global()
{
    static uint reloj_global = 0;

    reloj_global = (reloj_global + 1) % reloj_size;

    screen_pintar(reloj[reloj_global], C_BW, 49, 79);
}

void screen_pintar(uchar c, uchar color, uint fila, uint columna)
{
    p[fila][columna].c = c;
    p[fila][columna].a = color;
}

uchar screen_valor_actual(uint fila, uint columna)
{
    return p[fila][columna].c;
}

void print(const char * text, uint x, uint y, unsigned short attr) {
    int i;
    for (i = 0; text[i] != 0; i++)
     {
        screen_pintar(text[i], attr, y, x);

        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
}

void print_hex(uint numero, int size, uint x, uint y, unsigned short attr) {
    int i;
    char hexa[8];
    char letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
    for(i = 0; i < size; i++) {
        p[y][x + size - i - 1].c = hexa[i];
        p[y][x + size - i - 1].a = attr;
    }
}

void print_dec(uint numero, int size, uint x, uint y, unsigned short attr) {
    int i;
    char letras[16] = "0123456789";

    for(i = 0; i < size; i++) {
        int resto  = numero % 10;
        numero = numero / 10;
        p[y][x + size - i - 1].c = letras[resto];
        p[y][x + size - i - 1].a = attr;
    }
}

void screen_pintar_rect(uchar c, uchar color, int fila, int columna, int alto, int ancho){
    int i = fila;
    while (i < alto){
    	screen_pintar_linea_h(c, color, i, columna, ancho);
    	i++;
	}
}

void screen_pintar_linea_h(uchar c, uchar color, int fila, int columna, int ancho){
	int i = 0;
	while (i < ancho){
		screen_pintar(c, color, fila, columna);
		i++;
	}
}

void screen_inicializar(){
    screen_pintar_rect(0, C_FG_BLACK     , 0 , 0 , 50, 80);   //toda la pantalla
    screen_pintar_rect(0, C_FG_LIGHT_GREY, 0 , 0 , 44, 80);   //el_mapa
    screen_pintar_rect(0, C_FG_BLUE      , 44, 29, 50, 36); //equuipo azul
    screen_pintar_rect(0, C_FG_RED       , 44, 38, 44, 35); //equipo rojo
}


/*
 *Estas funciones quedan por si en un futuro nesecitamos abstraer las funcionalidades
 */
/*
void draw_screen(uchar color){
    int i, j;
    for(i = 0; i < 44; i++){ //filas 0-43 
        for(j = 0; j < 80; j++){ //columnas 0-79
            screen_pintar(0, C_FG_LIGHT_GREY, i / 50, i % 50);
        }
    }
}

void draw_map(uchar color){
    int i, j;
    for(i = 0; i < 44; i++){ //filas 0-43 
        for(j = 0; j < 80; j++){ //columnas 0-79
            screen_pintar(0, C_FG_LIGHT_GREY, i / 50, i % 50);
        }
    }
}

void clear_map(){
    //dibujo el area de notificaciones
    for(i = 44; i < 50; i++){ //filas 44-49 
        for(j = 0; j < 80; j++){ //columnas 80
            screen_pintar(0, C_FG_LIGHT_GREY, i / 50, i % 50);
        }
    }    
}
*/