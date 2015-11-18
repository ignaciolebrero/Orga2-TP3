/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
definicion de funciones del scheduler
*/
//tablero 50x80

#include "screen.h"
#include "game.h"
#include "i386.h"
#include "tss.h"
#include "sched.h"

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

void screen_actualizar_reloj_pirata(jugador_t *j, pirata_t *pirata){
    pirata->clock = (pirata->clock + 1) % reloj_size;

    if (j->index == 0) {
        screen_pintar(reloj[pirata->clock], C_BG_BLUE, 48, 9  + (pirata->id*2) );
    } else {
        screen_pintar(reloj[pirata->clock], C_BG_RED , 48, 55 + ((pirata->id%8)*2) );
    }
}

void screen_actualizar_puntaje(jugador_t *jugador){
    if (jugador->index == 0) {
        print_dec(jugador->puntuacion, 3, 31, 47, C_FG_WHITE);
    } else {
        print_dec(jugador->puntuacion, 3, 41, 47, C_FG_WHITE);
    }
}

void screen_matar_pirata(pirata_t *pirata){
    if (pirata->jugador->index == 0){
        screen_pintar(88, C_FG_MAGENTA, 48, 9  + (pirata->id*2) );
    } else {
        screen_pintar(88, C_FG_MAGENTA, 48, 55  + ((pirata->id%8)*2) );
    }
}

void screen_pintar(uchar c, uchar color, uint fila, uint columna)
{
    p[fila][columna].c = c;
    p[fila][columna].a = color;
}

unsigned char screen_valor_actual(uint y, uint x){
    return p[y][x].c;
}

unsigned char screen_attr_actual(uint y, uint x){
    return p[y][x].a;
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
    while (i > alto){
        screen_pintar_linea_h(c, color, i, columna, ancho);
        i--;
    }
}

void screen_pintar_linea_h(uchar c, uchar color, int fila, int columna, int ancho){
    int i = columna;
    while (i > ancho){
        screen_pintar(c, color, i, columna);
        i--;
    }
}

void screen_inicializar(){
    draw_map();
    draw_footer();
    draw_header();
    draw_red_team();
    draw_blue_team();
     
}

void draw_map(){
    int i, j;
    for(i = 0; i < 45; i++){ //filas 0-43 
        for(j = 0; j < 80; j++){ //columnas 0-79
            screen_pintar(32, C_BG_LIGHT_GREY, i, j);
        }
    }
}

void draw_header(){
    uint i;
    for(i = 0; i < 80; i++){
        screen_pintar(32, C_BG_BLACK, 0, i);
    }
}

void draw_footer(){
    //dibujo el area de notificaciones
    int i, j;
    for(i = 45; i < 50; i++){ //filas 44-49 
        for(j = 0; j < 80; j++){ //columnas 80
            screen_pintar(32, C_BG_BLACK, i, j);
        }
    }    
}

void draw_red_team(){
    int i, j;
    for (i=45; i<50; i++){
        for (j=29; j<36; j++){
            screen_pintar(32, C_BG_BLUE, i, j);
        }
    }
    
    screen_pintar( 48,  C_FG_WHITE,  47,  31);
    screen_pintar( 48,  C_FG_WHITE,  47,  32);
    screen_pintar( 48,  C_FG_WHITE,  47,  33);

    screen_pintar(49, C_FG_WHITE, 46, 9 );
    screen_pintar(88, C_FG_MAGENTA, 48, 9 );
    screen_pintar(50, C_FG_WHITE, 46, 11);
    screen_pintar(88, C_FG_MAGENTA, 48, 11);
    screen_pintar(51, C_FG_WHITE, 46, 13);
    screen_pintar(88, C_FG_MAGENTA, 48, 13);
    screen_pintar(52, C_FG_WHITE, 46, 15);
    screen_pintar(88, C_FG_MAGENTA, 48, 15);
    screen_pintar(53, C_FG_WHITE, 46, 17);
    screen_pintar(88, C_FG_MAGENTA, 48, 17);
    screen_pintar(54, C_FG_WHITE, 46, 19);
    screen_pintar(88, C_FG_MAGENTA, 48, 19);
    screen_pintar(55, C_FG_WHITE, 46, 21);
    screen_pintar(88, C_FG_MAGENTA, 48, 21);
    screen_pintar(56, C_FG_WHITE, 46, 23);
    screen_pintar(88, C_FG_MAGENTA, 48, 23);
}

void draw_blue_team(){
    int i, j;
    for (i=45; i<50; i++){
        for (j=38; j<45; j++){
            screen_pintar(32, C_BG_RED, i, j);
        }
    }

    screen_pintar( 48,  C_FG_WHITE,  47,  40);
    screen_pintar( 48,  C_FG_WHITE,  47,  41);
    screen_pintar( 48,  C_FG_WHITE,  47,  42);

    screen_pintar(49, C_FG_WHITE  , 46, 55 );
    screen_pintar(88, C_FG_MAGENTA, 48, 55 );
    screen_pintar(50, C_FG_WHITE  , 46, 57 );
    screen_pintar(88, C_FG_MAGENTA, 48, 57 );
    screen_pintar(51, C_FG_WHITE  , 46, 59 );
    screen_pintar(88, C_FG_MAGENTA, 48, 59 );
    screen_pintar(52, C_FG_WHITE  , 46, 61 );
    screen_pintar(88, C_FG_MAGENTA, 48, 61 );
    screen_pintar(53, C_FG_WHITE  , 46, 63 );
    screen_pintar(88, C_FG_MAGENTA, 48, 63 );
    screen_pintar(54, C_FG_WHITE  , 46, 65 );
    screen_pintar(88, C_FG_MAGENTA, 48, 65 );
    screen_pintar(55, C_FG_WHITE  , 46, 67 );
    screen_pintar(88, C_FG_MAGENTA, 48, 67 );
    screen_pintar(56, C_FG_WHITE  , 46, 69 );
    screen_pintar(88, C_FG_MAGENTA, 48, 69 );
}