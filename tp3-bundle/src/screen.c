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

uchar screen_valor_actual(uint fila, uint columna)
{
    return p[fila][columna].c;
}


void screen_reprintar_pantalla(){
    int x, y;
    for(y = 0; y < MAPA_ALTO; y++){
        for(x = 0; x < MAPA_ANCHO; x++){
            screen_pintar(temporal_pantalla[x][y].c, temporal_pantalla[x][y].a, x, y);            
        }
    }
}

//TODO: terminar!!
void screen_debuggear_tarea(uint eflags, uint eip, uint edi, uint esi, uint ebp, uint esp, uint ebx, uint edx, uint ecx, uint eax){
    uint ss, gs, fs, es, ds, cs;
    ss = tss_obtener_ss(sched_tarea_actual_id());
    gs = tss_obtener_gs(sched_tarea_actual_id());
    fs = tss_obtener_fs(sched_tarea_actual_id());
    es = tss_obtener_es(sched_tarea_actual_id());
    ds = tss_obtener_ds(sched_tarea_actual_id());
    cs = tss_obtener_cs(sched_tarea_actual_id());
 
    int x, y;
    for(y = 10; y < 26; y++){
        for(x = 25; x < 40; x++){
            temporal_pantalla[y][x].c = screen_valor_actual(y,x);
            temporal_pantalla[y][x].a = p[y][x].a;
        }
    }

    screen_pintar_rect(0, C_BG_LIGHT_GREY, 8, 30, 8, 1);
    screen_pintar_rect(0, C_BG_LIGHT_GREY, 10, 30, 42, 1);
    screen_pintar_rect(0, C_BG_LIGHT_GREY, 10, 30, 20, 1);
    print("eax"    , 25, 10, C_FG_RED);
    print("ecx"    , 25, 11, C_FG_RED);
    print("edx"    , 25, 12, C_FG_RED);
    print("ebx"    , 25, 13, C_FG_RED);
    print("esp"    , 25, 14, C_FG_RED);
    print("ebp"    , 25, 15, C_FG_RED);
    print("esi"    , 25, 16, C_FG_RED);
    print("edi"    , 25, 17, C_FG_RED);
    print("eip"    , 25, 18, C_FG_RED);
    print("cs"     , 26, 19, C_FG_RED);
    print("ds"     , 26, 20, C_FG_RED);
    print("es"     , 26, 21, C_FG_RED);
    print("fs"     , 26, 22, C_FG_RED);
    print("gs"     , 26, 23, C_FG_RED);
    print("ss"     , 26, 24, C_FG_RED);
    print("eflags" , 20, 25, C_FG_RED);


      //cambiarlo para que use de la pila
    print_hex(eax, 9, 30, 10, C_FG_BROWN);
    print_hex(ebx, 9, 30, 11, C_FG_BROWN);
    print_hex(ecx, 9, 30, 12, C_FG_BROWN);
    print_hex(edx, 9, 30, 13, C_FG_BROWN);
    print_hex(esi, 9, 30, 14, C_FG_BROWN);
    print_hex(edi, 9, 30, 15, C_FG_BROWN);
    print_hex(ebp, 9, 30, 16, C_FG_BROWN);
    print_hex(esp, 9, 30, 17, C_FG_BROWN);
    print_hex(eip, 9, 30, 18, C_FG_BROWN);
    print_hex(cs , 9, 30, 19, C_FG_BROWN);
    print_hex(ds , 9, 30, 20, C_FG_BROWN);
    print_hex(es , 9, 30, 21, C_FG_BROWN);
    print_hex(fs , 9, 30, 22, C_FG_BROWN);
    print_hex(gs , 9, 30, 23, C_FG_BROWN);
    print_hex(ss , 9, 30, 24, C_FG_BROWN);
    print_hex(eflags , 9, 30, 25, C_FG_BROWN);
}

void guardar_pantalla_actual(){
    uint fil, col;
    for (fil = 10; fil < 26; fil++) {
        for (col = 5; col < 40; col++) {
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