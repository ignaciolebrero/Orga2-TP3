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

void screen_actualizar_reloj_pirata(jugador_t *j, pirata_t *pirata){
    pirata->clock = (pirata->clock + 1) % reloj_size;

    if (j->index == 0) {
        screen_pintar(reloj[pirata->clock], C_BG_BLUE, 45, 10 + (pirata->id*4) );
    } else {
        screen_pintar(reloj[pirata->clock], C_BG_RED , 45, 10 + (pirata->id*4) );
    }
}

void screen_matar_pirata(uint id){
    screen_pintar(96, C_BG_RED , 47, 10 + ( id * 4 ) ); //ver cual es el caracter que va
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
    for(i = 0; i < 44; i++){ //filas 0-43 
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
    for(i = 44; i < 50; i++){ //filas 44-49 
        for(j = 0; j < 80; j++){ //columnas 80
            screen_pintar(32, C_BG_BLACK, i, j);
        }
    }    
}

void draw_red_team(){
    int i, j;
    for (i=44; i<50; i++){
        for (j=29; j<36; j++){
            screen_pintar(32, C_BG_BLUE, i, j);
        }
    }
    
    screen_pintar( 48,  C_FG_WHITE,  47,  31);
    screen_pintar( 48,  C_FG_WHITE,  47,  32);
    screen_pintar( 48,  C_FG_WHITE,  47,  33);



    screen_pintar(49, C_FG_WHITE, 45, 9 );
    screen_pintar(88, C_FG_MAGENTA, 47, 9 );
    screen_pintar(50, C_FG_WHITE, 45, 11);
    screen_pintar(88, C_FG_MAGENTA, 47, 11);
    screen_pintar(51, C_FG_WHITE, 45, 13);
    screen_pintar(88, C_FG_MAGENTA, 47, 13);
    screen_pintar(52, C_FG_WHITE, 45, 15);
    screen_pintar(88, C_FG_MAGENTA, 47, 15);
    screen_pintar(53, C_FG_WHITE, 45, 17);
    screen_pintar(88, C_FG_MAGENTA, 47, 17);
    screen_pintar(54, C_FG_WHITE, 45, 19);
    screen_pintar(88, C_FG_MAGENTA, 47, 19);
    screen_pintar(55, C_FG_WHITE, 45, 21);
    screen_pintar(88, C_FG_MAGENTA, 47, 21);
    screen_pintar(56, C_FG_WHITE, 45, 23);
    screen_pintar(88, C_FG_MAGENTA, 47, 23);
}

void draw_blue_team(){
    int i, j;
    for (i=44; i<50; i++){
        for (j=38; j<45; j++){
            screen_pintar(32, C_BG_RED, i, j);
        }
    }

    screen_pintar( 48,  C_FG_WHITE,  47,  40);
    screen_pintar( 48,  C_FG_WHITE,  47,  41);
    screen_pintar( 48,  C_FG_WHITE,  47,  42);

    screen_pintar(49, C_FG_WHITE, 45, 55 );
    screen_pintar(88, C_FG_MAGENTA, 47, 55);
    screen_pintar(50, C_FG_WHITE, 45, 57 );
    screen_pintar(88, C_FG_MAGENTA, 47, 57);
    screen_pintar(51, C_FG_WHITE, 45, 59 );
    screen_pintar(88, C_FG_MAGENTA, 47, 59);
    screen_pintar(52, C_FG_WHITE, 45, 61 );
    screen_pintar(88, C_FG_MAGENTA, 47, 61);
    screen_pintar(53, C_FG_WHITE, 45, 63 );
    screen_pintar(88, C_FG_MAGENTA, 47, 63);
    screen_pintar(54, C_FG_WHITE, 45, 65 );
    screen_pintar(88, C_FG_MAGENTA, 47, 65);
    screen_pintar(55, C_FG_WHITE, 45, 67 );
    screen_pintar(88, C_FG_MAGENTA, 47, 67);
    screen_pintar(56, C_FG_WHITE, 45, 69 );
    screen_pintar(88, C_FG_MAGENTA, 47, 69);
}