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

extern jugador_t jugadorA, jugadorB;

static ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;

const char reloj[] = "|/-\\";
#define reloj_size 4

ca temporal_pantalla[VIDEO_FILS][VIDEO_COLS];

void screen_actualizar_reloj_global()
{
    static uint reloj_global = 0;

    reloj_global = (reloj_global + 1) % reloj_size;

    screen_pintar(reloj[reloj_global], C_BW, 49, 79);
}

void screen_actualizar_reloj_pirata(jugador_t *j, pirata_t *pirata){
    pirata->clock = (pirata->clock + 1) % reloj_size;

    if (j->index == 0) {
        screen_pintar(reloj[pirata->clock], C_BG_BLUE, 47, 9  + (pirata->id*2) );
    } else {
        screen_pintar(reloj[pirata->clock], C_BG_RED , 47, 55 + ((pirata->id%8)*2) );
    }
}

void screen_actualizar_puntaje(jugador_t *jugador){
    if (jugador->index == 0) {
        print_dec(jugador->puntuacion, 3, 30, 30, C_FG_WHITE);
    } else {
        print_dec(jugador->puntuacion, 3, 30, 33, C_FG_WHITE);
    }
}

void screen_matar_pirata(pirata_t *pirata){
    if (pirata->jugador->index == 0){
        screen_pintar(88, C_FG_MAGENTA, 47, 9  + (pirata->id*2) );
    } else {
        screen_pintar(88, C_FG_MAGENTA, 47, 55  + ((pirata->id%8)*2) );
    }
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

//TODO: terminar!!
void screen_debuggear_tarea(){
    guardar_pantalla_actual();
    screen_pintar_rect(0, C_BG_LIGHT_GREY, 42, 54, 40, 30);
    
    print("eax"    , 25, 10, C_FG_BLACK);
    print("ebx"    , 25, 12, C_FG_BLACK);
    print("ecx"    , 25, 14, C_FG_BLACK);
    print("edx"    , 25, 16, C_FG_BLACK);
    print("esi"    , 25, 18, C_FG_BLACK);
    print("edi"    , 25, 20, C_FG_BLACK);
    print("ebp"    , 25, 22, C_FG_BLACK);
    print("esp"    , 25, 24, C_FG_BLACK);
    print("eip"    , 25, 26, C_FG_BLACK);
    print("cs"     , 26, 28, C_FG_BLACK);
    print("ds"     , 26, 30, C_FG_BLACK);
    print("es"     , 26, 32, C_FG_BLACK);
    print("fs"     , 26, 34, C_FG_BLACK);
    print("gs"     , 26, 36, C_FG_BLACK);
    print("ss"     , 26, 38, C_FG_BLACK);
    print("eflags" , 26, 40, C_FG_BLACK);

    //cambiarlo para que use de la pila
    print_hex(reax(), 29, 10, 25, C_FG_WHITE);
    print_hex(rebx(), 29, 12, 25, C_FG_WHITE);
    print_hex(rebx(), 29, 14, 25, C_FG_WHITE);
    print_hex(recx(), 29, 16, 25, C_FG_WHITE);
    print_hex(redx(), 29, 18, 25, C_FG_WHITE);
    print_hex(resi(), 29, 20, 25, C_FG_WHITE);
    print_hex(redi(), 29, 22, 25, C_FG_WHITE);
    print_hex(rebp(), 29, 24, 25, C_FG_WHITE);
    print_hex(resp(), 29, 26, 25, C_FG_WHITE);
//  print_hex(reip(), 29, 28, 25, C_FG_WHITE);
    print_hex(rcs() , 29, 30, 25, C_FG_WHITE);
    print_hex(rds() , 29, 32, 25, C_FG_WHITE);
    print_hex(res() , 29, 34, 25, C_FG_WHITE);
    print_hex(rfs() , 29, 36, 25, C_FG_WHITE);
    print_hex(rgs() , 29, 38, 25, C_FG_WHITE);
    print_hex(rss() , 29, 40, 25, C_FG_WHITE);

}

void guardar_pantalla_actual(){
    uint fil, col;
    for (fil = 0; fil < VIDEO_FILS; fil++) {
        for (col = 0; col < VIDEO_COLS; col++) {
            temporal_pantalla[fil][col].a = p[fil][col].a;
            temporal_pantalla[fil][col].c = p[fil][col].c;
        }
    }
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

    screen_pintar(49, C_FG_WHITE  , 45, 55 );
    screen_pintar(88, C_FG_MAGENTA, 47, 55 );
    screen_pintar(50, C_FG_WHITE  , 45, 57 );
    screen_pintar(88, C_FG_MAGENTA, 47, 57 );
    screen_pintar(51, C_FG_WHITE  , 45, 59 );
    screen_pintar(88, C_FG_MAGENTA, 47, 59 );
    screen_pintar(52, C_FG_WHITE  , 45, 61 );
    screen_pintar(88, C_FG_MAGENTA, 47, 61 );
    screen_pintar(53, C_FG_WHITE  , 45, 63 );
    screen_pintar(88, C_FG_MAGENTA, 47, 63 );
    screen_pintar(54, C_FG_WHITE  , 45, 65 );
    screen_pintar(88, C_FG_MAGENTA, 47, 65 );
    screen_pintar(55, C_FG_WHITE  , 45, 67 );
    screen_pintar(88, C_FG_MAGENTA, 47, 67 );
    screen_pintar(56, C_FG_WHITE  , 45, 69 );
    screen_pintar(88, C_FG_MAGENTA, 47, 69 );
}